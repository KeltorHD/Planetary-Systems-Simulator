#include "pch.h"
#include "program.hpp"

void Program::initVariables()
{
	this->window = nullptr;
	this->dt = 0;
}

void Program::initGraphicsSettings()
{
	this->programSettings.loadXml("config/settings.xml");
}

//Initializer func
void Program::initWindow()
{
	sf::ContextSettings st;
	st.antialiasingLevel = this->programSettings.antialiasingLevel;

	sf::String title = sf::String::fromUtf8
	(
		this->stateData.locale->get_s("program_title").begin(), 
		this->stateData.locale->get_s("program_title").end()
	);


	if (this->programSettings.fullscreen)
		this->window = new sf::RenderWindow(
			this->programSettings.resolution,
			title,
			sf::Style::Fullscreen, st);
	else
		this->window = new sf::RenderWindow(
			this->programSettings.resolution,
			title,
			sf::Style::Titlebar | sf::Style::Close, st);
	sf::Image icon;
	if (!icon.loadFromFile("icon/icon.png"))
	{
		throw "COULD NOT LOAD ICON";
	}
	this->window->setIcon(512, 512, icon.getPixelsPtr());

	ImGui::SFML::Init(*this->window);
	
	ImGui::GetIO().Fonts->AddFontFromFileTTF
	(
		"font/GoogleSans-Regular.ttf", static_cast<float>(this->stateData.programSettings->fontSize), nullptr, ImGui::GetIO().Fonts->GetGlyphRangesCyrillic()
	);
	ImGui::SFML::UpdateFontTexture();

	this->window->setFramerateLimit(this->programSettings.framerateLimit);
	this->window->setVerticalSyncEnabled(this->programSettings.verticalSync);

	this->stateData.window = this->window; /*связываем окно в program с stateData*/
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
			this->supportedKeys[key] = std::move(value);
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
	this->stateData.programSettings = &this->programSettings;
	this->stateData.supportedKeys = &this->supportedKeys;
	this->stateData.states = &this->states;
	this->stateData.locale = new Locale("lang/" + this->programSettings.lang + ".xml");

	this->stateData.soundManager = new SoundManager("sounds/", ".wav");
	this->stateData.soundManager->loadSound("backmusic");
	this->stateData.soundManager->loadSound("click");
	this->stateData.soundManager->setVolume("backmusic", this->programSettings.musicVolume);
	this->stateData.soundManager->setVolume("click", this->programSettings.soundVolume);
	this->stateData.soundManager->setRepeat("backmusic", true);
	this->stateData.soundManager->play("backmusic");
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
	this->initStateData();
	this->initWindow();
	this->initKeys();
	this->initStates();
}

Program::~Program()
{
	while (!this->states.empty())
	{
		delete this->states.top();
		this->states.pop();
	}

	delete this->stateData.locale;
	delete this->stateData.soundManager;

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
	ImGui::SFML::Update(*this->window, t);
}

void Program::updateSFMLEvents()
{
	std::queue<sf::Event>().swap(this->stateData.events); /*очищение очереди*/
	
	while (this->window->pollEvent(this->sfEvent))
	{
		ImGui::SFML::ProcessEvent(this->sfEvent);
		
		if (this->sfEvent.type == sf::Event::Closed)
			this->window->close();

		this->stateData.events.push(this->sfEvent);
	}
}

void Program::update()
{
	this->updateSFMLEvents();
	this->updateDt_ImGui();

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
		this->update();
		this->render();
	}
}
