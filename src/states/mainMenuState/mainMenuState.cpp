#include "pch.h"
#include "mainMenuState.hpp"

//Init func
void MainMenuState::initVariables()
{
	simulation.loadSystemXml("systems/base.xml");
	this->isSettings = false;

	std::vector<std::string> v;
	for (const auto& entry : std::filesystem::directory_iterator("lang/"))
	{
		std::string tmp = entry.path().string().substr
		(
			entry.path().string().find_last_of('/') + 1, entry.path().string().size() - entry.path().string().find_last_of('/') - 5
		);
		v.push_back(tmp);
	}
	this->lang_length = v.size();
	this->lang = new char* [this->lang_length];
	for (size_t i = 0; i < this->lang_length; i++)
	{
		this->lang[i] = new char[v[i].length() + 1];
		std::copy(v[i].begin(), v[i].end(), this->lang[i]);
		this->lang[i][v[i].length()] = 0;
	}
}

void MainMenuState::initKeybinds()
{
	std::ifstream ifs("config/mainmenustate_keybinds.ini");

	if (ifs.is_open())
	{
		std::string key = "";
		std::string key2 = "";
		while (ifs >> key >> key2)
		{
			this->keybinds[key] = this->supportedKeys->at(key2);
		}
	}

	ifs.close();
}

MainMenuState::MainMenuState(StateData* state_data)
	: State(state_data)
{
	this->initVariables();
	this->initKeybinds();
}

MainMenuState::~MainMenuState()
{
	for (size_t i = 0; i < this->lang_length; i++)
	{
		delete[] this->lang[i];
	}
	delete[] this->lang;
}


void MainMenuState::updateInput(const float& dt)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("CLOSE"))))
	{
		this->endState();
	}
}

void MainMenuState::updateGUI(const float& dt)
{
	ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);

	/*главное меню*/
	ImGui::Begin(this->locale->get_c("program_title"));

	if (ImGui::Button(this->locale->get_c("simulation")))
	{
		std::cout << "call sim" << std::endl;
	}

	if (ImGui::Button(this->locale->get_c("settings")))
	{
		this->isSettings = true;
	}

	if (ImGui::Button(this->locale->get_c("quit")))
	{
		this->endState();
	}
	ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
	
	ImGui::End();
	/*конец главного меню*/

	
	/*Настройки*/
	if (this->isSettings)
	{
		static int selectedItem{ 0 };
		static bool fullscreen{ false };

		ImGui::Begin(this->locale->get_c("settings"), &this->isSettings); // создаём окно

		ImGui::Combo(this->locale->get_c("lang"), &selectedItem, this->lang, this->lang_length);

		ImGui::Checkbox(this->locale->get_c("fullscreen"), &fullscreen);

		if (ImGui::Button(this->locale->get_c("save")))
		{
			if (std::string(this->lang[selectedItem]) != this->locale->get_lang())
			{
				this->locale->loadLocaleXml(std::string("lang/") + this->lang[selectedItem] + ".xml");
			}
			if (fullscreen != this->stateData->gfxSettings->fullscreen)
			{
				sf::String title = sf::String::fromUtf8
				(
					this->stateData->locale->get_s("program_title").begin(),
					this->stateData->locale->get_s("program_title").end()
				);
				sf::ContextSettings st;
				st.antialiasingLevel = this->stateData->gfxSettings->antialiasingLevel;
				this->window->close();
				if (fullscreen)
					this->window->create
					(
						sf::VideoMode::getFullscreenModes()[0], title, sf::Style::Fullscreen, st
					);
				else
					this->window->create
					(
						this->stateData->gfxSettings->resolution, title, sf::Style::Titlebar | sf::Style::Close, st
					);
				this->stateData->gfxSettings->fullscreen = fullscreen;
			}
		}

		ImGui::End();
	}
	/*конец настроек*/

	ImGui::PopFont();
}

void MainMenuState::update(const float& dt)
{
	this->updateMousePositions();

	this->simulation.update(2*dt);

	this->updateInput(dt);
	this->updateGUI(dt);
}

void MainMenuState::render(sf::RenderTarget* target)
{
	if (!target)
	{
		target = this->window;
	}

	this->simulation.render(target);
}

