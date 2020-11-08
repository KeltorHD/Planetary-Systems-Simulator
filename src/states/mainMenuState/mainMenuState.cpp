#include "pch.h"
#include "mainMenuState.hpp"

//Init func
void MainMenuState::initVariables()
{
	simulation.loadSystemXml("systems/base.xml");
	this->isSettings = false;

	/*языки*/
	std::vector<std::string> v;
	for (const auto& entry : std::filesystem::directory_iterator("lang/"))
	{
		std::string tmp = entry.path().string().substr
		(
			entry.path().string().find_last_of('/') + 1, entry.path().string().size() - entry.path().string().find_last_of('/') - 5
		);
		v.push_back(tmp);
	}
	this->langs_length = v.size();
	this->langs = new char* [this->langs_length];
	for (size_t i = 0; i < this->langs_length; i++)
	{
		this->langs[i] = new char[v[i].length() + 1];
		std::copy(v[i].begin(), v[i].end(), this->langs[i]);
		this->langs[i][v[i].length()] = 0;
	}

	/*режимы видео*/
	this->vmodes_length = this->stateData->gfxSettings->videoModes.size();
	this->vmodes = new char* [this->vmodes_length];
	for (size_t i = 0; i < vmodes_length; i++)
	{
		std::string tmp
		{
			std::to_string(this->stateData->gfxSettings->videoModes[i].width) +
			"*" +
			std::to_string(this->stateData->gfxSettings->videoModes[i].height)
		};
		this->vmodes[i] = new char[tmp.length() + 1];
		std::copy(tmp.begin(), tmp.end(), this->vmodes[i]);
		this->vmodes[i][tmp.length()] = 0;
	}

	this->camera.setSize
	(
		static_cast<float>(this->stateData->gfxSettings->resolution.width),
		static_cast<float>(this->stateData->gfxSettings->resolution.height)
	);
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
	for (size_t i = 0; i < this->langs_length; i++)
	{
		delete[] this->langs[i];
	}
	delete[] this->langs;

	for (size_t i = 0; i < this->vmodes_length; i++)
	{
		delete[] this->vmodes[i];
	}
	delete[] this->vmodes;
}


void MainMenuState::updateInput(const float& dt)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("RELOAD"))))
	{
		this->simulation.restoreInitialState();
	}
}

void MainMenuState::updateGUI()
{
	ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);

	/*главное меню*/
	ImGui::Begin(this->locale->get_c("program_title"));

	if (ImGui::Button(this->locale->get_c("simulation")))
	{
		this->states->push(new SimulationState(this->stateData));
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
		this->updateSettingsGUI();
	}
	/*конец настроек*/

	ImGui::PopFont();
}

void MainMenuState::updateSettingsGUI()
{
	/*текущий язык*/
	static int selected_lang{ std::distance(this->langs, std::find(this->langs, this->langs + this->langs_length, this->locale->get_lang())) };
	/*текущий видеорежим*/
	static int selected_resolution
	{
		std::distance
		(
			this->stateData->gfxSettings->videoModes.begin(),
			std::find(this->stateData->gfxSettings->videoModes.begin(), this->stateData->gfxSettings->videoModes.end(), this->stateData->gfxSettings->resolution)
		)
	};
	/*текущий режим полного экрана*/
	static bool fullscreen{ this->stateData->gfxSettings->fullscreen };

	ImGui::Begin(this->locale->get_c("settings"), &this->isSettings);

	ImGui::Combo(this->locale->get_c("lang"), &selected_lang, this->langs, this->langs_length);

	ImGui::Combo(this->locale->get_c("resolution"), &selected_resolution, this->vmodes, this->vmodes_length);

	ImGui::Checkbox(this->locale->get_c("fullscreen"), &fullscreen);

	if (ImGui::Button(this->locale->get_c("save"))) /*сохраняем изменения*/
	{
		if (std::string(this->langs[selected_lang]) != this->locale->get_lang()) /*язык*/
		{
			this->locale->loadLocaleXml(std::string("lang/") + this->langs[selected_lang] + ".xml");
			this->stateData->gfxSettings->lang = this->langs[selected_lang];
		}
		if (fullscreen != this->stateData->gfxSettings->fullscreen
			|| this->stateData->gfxSettings->videoModes[selected_resolution] != this->stateData->gfxSettings->resolution) /*полноэкранный режим или изменение разрешения*/
		{
			this->stateData->gfxSettings->resolution = this->stateData->gfxSettings->videoModes[selected_resolution];
			this->stateData->gfxSettings->fullscreen = fullscreen;

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
					this->stateData->gfxSettings->resolution, title, sf::Style::Fullscreen, st
				);
			else
				this->window->create
				(
					this->stateData->gfxSettings->resolution, title, sf::Style::Titlebar | sf::Style::Close, st
				);
		}

		this->camera.setSize /*изменяем камеру*/
		(
			static_cast<float>(this->stateData->gfxSettings->resolution.width),
			static_cast<float>(this->stateData->gfxSettings->resolution.height)
		);
		this->stateData->gfxSettings->saveXml("config/settings.xml"); /*сохраняем в файлик*/
	}

	ImGui::End();
}

void MainMenuState::update(const float& dt)
{
	this->simulation.update(2*dt);

	this->updateInput(dt);
	this->updateGUI();

	this->camera.setCenter(this->simulation.getMaxMassCoord());
}

void MainMenuState::render(sf::RenderTarget* target)
{
	if (!target)
	{
		target = this->window;
	}
	this->window->setView(this->camera);

	this->simulation.render(target);
}

