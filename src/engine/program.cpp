#include "pch.h"
#include "program.hpp"

void Program::initVariables()
{
	this->window = nullptr;
	this->dt = 0;
}

void Program::initGraphicsSettings()
{
	this->gfxSettings.loadFromFile("config/graphics.ini");
}

//Initializer func
void Program::initWindow()
{
	sf::ContextSettings st;
	st.antialiasingLevel = this->gfxSettings.antialiasingLevel;

	if (this->gfxSettings.fullscreen)
		this->window = new sf::RenderWindow(
			this->gfxSettings.resolution,
			this->gfxSettings.title,
			sf::Style::Fullscreen, st);
	else
		this->window = new sf::RenderWindow(
			this->gfxSettings.resolution,
			this->gfxSettings.title,
			sf::Style::Titlebar | sf::Style::Close, st);

	ImGui::SFML::Init(*this->window);

	this->window->setFramerateLimit(this->gfxSettings.framerateLimit);
	this->window->setVerticalSyncEnabled(this->gfxSettings.verticalSync);
}

void Program::initKeys()
{
	std::ifstream ifs("config/supported_keys.ini");

	if (ifs.is_open())
	{
		std::string key = "";
		int value = 0;
		while (ifs >> key >> value)
		{
			this->supportedKeys[key] = value;
		}
	}

	ifs.close();

	//Debug remove later
	for (auto i : this->supportedKeys)
	{
		std::cout << i.first << " " << i.second << "\n";
	}
}

void Program::initStateData()
{
	this->stateData.window = this->window;
	this->stateData.gfxSettings = &this->gfxSettings;
	this->stateData.supportedKeys = &this->supportedKeys;
	this->stateData.states = &this->states;
}

void Program::initStates()
{
	this->states.push(new MainMenuState(&this->stateData));
}


//Constructor/Destructor


Program::Program()
{
	this->initVariables();
	this->initGraphicsSettings();
	this->initWindow();
	this->initKeys();
	this->initStateData();
	this->initStates();
}

Program::~Program()
{
	while (!this->states.empty())
	{
		delete this->states.top();
		this->states.pop();
	}

	delete this->window;
}


//func

void Program::endApplication()
{
	//remove later
	std::cout << "Ending Application!\n";
}

void Program::updateDt_ImGui()
{
	sf::Time t{ this->dtClock.restart() };
	this->dt = t.asSeconds();
	//std::cout << this->dt << std::endl;
	ImGui::SFML::Update(*this->window, t);
}

void Program::updateSFMLEvents()
{
	while (this->window->pollEvent(this->sfEvent))
	{
		ImGui::SFML::ProcessEvent(this->sfEvent);

		if (this->sfEvent.type == sf::Event::Closed)
			this->window->close();
	}
}

void Program::update()
{
	this->updateSFMLEvents();

	if (!this->states.empty())
	{
		if (this->window->hasFocus())
		{
			this->states.top()->update(this->dt);

			if (this->states.top()->getQuit())
			{
				this->states.top()->endState();
				delete this->states.top();
				this->states.pop();
			}
		}
	}
	else //App end
	{
		this->endApplication();
		this->window->close();
	}

}

void Program::render()
{
	this->window->clear();

	//render items
	if (!this->states.empty())
	{
		this->states.top()->render();
	}

	ImGui::SFML::Render(*this->window); /*render gui*/

	this->window->display();
}

void Program::run()
{
	while (this->window->isOpen())
	{
		this->updateDt_ImGui();
		this->update();
		this->render();
	}
}
