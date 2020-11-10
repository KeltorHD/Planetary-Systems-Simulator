#include "pch.h"
#include "simulationState.hpp"

//Init func
void SimulationState::initVariables()
{
	this->simulation.loadDemoSystem();
	this->ctrl = control_t::paused;
	this->enableControlSimulation = true;
	this->enableEditSimulation = true;
	this->koef = 0.1f;
	this->input_name = new char[256]{0};
	std::copy(this->simulation.getName().begin(), this->simulation.getName().end(), this->input_name);

	this->camera.setSize
	(
		static_cast<float>(this->stateData->gfxSettings->resolution.width),
		static_cast<float>(this->stateData->gfxSettings->resolution.height)
	);
	this->camera.setCenter(this->simulation.getMaxMassCoord());
}

void SimulationState::initKeybinds()
{
	std::ifstream ifs("config/simulationstate_keybinds.ini");

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

SimulationState::SimulationState(StateData* state_data)
	: State(state_data)
{
	this->initVariables();
	this->initKeybinds();
}

SimulationState::~SimulationState()
{
	delete this->input_name;
}


void SimulationState::updateInput(const float& dt)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("RELOAD"))))
	{
		this->simulation.restoreInitialState();
		this->ctrl = control_t::paused;
	}

	/*camera move update*/
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("CAM_U"))))
	{
		this->camera.move(0, -camera_move * dt);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("CAM_D"))))
	{
		this->camera.move(0, camera_move * dt);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("CAM_L"))))
	{
		this->camera.move(-camera_move * dt, 0);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("CAM_R"))))
	{
		this->camera.move(camera_move * dt, 0);
	}

	/*camera set pos at max mass object*/
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("CAM_CENTER"))))
	{
		this->camera.setCenter(this->simulation.getMaxMassCoord());
	}

	/*camera scroll*/
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("CAM_SCROLL_UP")))
		|| sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("CAM_SCROLL_UP2"))))
	{
		this->camera.zoom(1.f - camera_scroll);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("CAM_SCROLL_DOWN")))
		|| sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("CAM_SCROLL_DOWN2"))))
	{
		this->camera.zoom(1.f + camera_scroll);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("CLOSE"))))
	{
		this->endState();
	}
}

void SimulationState::updateGUI()
{
	ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[ImGui::GetIO().Fonts->Fonts.size() - 1]);

	/*отрисовка меню вверху экрана*/
	this->updateMainMenuBar();

	/*отрисовка меню управлени€ симул€цией*/
	this->updateControlSim();

	/*отрисовка меню редактировани€ симул€цией*/
	this->updateEditSim();

	ImGui::PopFont();
}

void SimulationState::updateMainMenuBar()
{
	/*отрисовка меню вверху экрана*/
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu(this->locale->get_c("system")))
		{
			if (ImGui::MenuItem(this->locale->get_c("enable_play")))
			{
				this->enableControlSimulation = true;
			}
			if (ImGui::MenuItem(this->locale->get_c("enable_edit")))
			{
				this->enableEditSimulation = true;
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu(this->locale->get_c("help")))
		{
			ImGui::MenuItem(this->locale->get_c("move_up"));
			ImGui::MenuItem(this->locale->get_c("move_down"));
			ImGui::MenuItem(this->locale->get_c("move_left"));
			ImGui::MenuItem(this->locale->get_c("move_right"));
			ImGui::MenuItem(this->locale->get_c("move_scroll_up"));
			ImGui::MenuItem(this->locale->get_c("move_scroll_down"));
			ImGui::MenuItem(this->locale->get_c("center_mass"));
			ImGui::MenuItem(this->locale->get_c("reload_help"));
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}

void SimulationState::updateControlSim()
{
	/* онтролирование процессом симул€ции*/
	if (this->enableControlSimulation)
	{
		ImGui::Begin(this->locale->get_c("control_simulation"), &this->enableControlSimulation);

		ImGui::Columns(2, "col", false);
		
		if (ImGui::Button(this->locale->get_c("play")))
		{
			this->ctrl = control_t::play;
		}
		ImGui::NextColumn();
		if (ImGui::Button(this->locale->get_c("reload")))
		{
			this->simulation.restoreInitialState();
			this->ctrl = control_t::paused;
		}
		ImGui::NextColumn();
		ImGui::Separator();
		if (ImGui::Button(this->locale->get_c("pause")))
		{
			this->ctrl = control_t::paused;
		}
		ImGui::NextColumn();
		if (ImGui::Button(this->locale->get_c("center")))
		{
			this->camera.setCenter(this->simulation.getMaxMassCoord());
		}
		ImGui::Separator();
		ImGui::Columns(1);
		if (ImGui::Button(this->locale->get_c("play_koef")))
		{
			this->ctrl = control_t::play_koef;
		}
		ImGui::SliderFloat(this->locale->get_c("koef"), &this->koef, 0.1f, 10.f);

		ImGui::End();
	}
}

void SimulationState::updateEditSim()
{
	/*отрисовка меню редактировани€ симул€цией*/
	if (this->enableEditSimulation)
	{
		ImGui::Begin(this->locale->get_c("edit_simulation"), &this->enableEditSimulation);

		ImGui::Text(this->locale->get_c("info"));

		ImGui::InputText(this->locale->get_c("name"), this->input_name, 256);

		ImGui::End();
	}
}

void SimulationState::update(const float& dt)
{
	if (this->ctrl != control_t::paused)
	{
		this->simulation.update((this->ctrl == control_t::play_koef ? this->koef : 1.f) * 2 * dt);
	}

	this->updateInput(dt);
	this->updateGUI();
}

void SimulationState::render(sf::RenderTarget* target)
{
	if (!target)
	{
		target = this->window;
	}
	this->window->setView(this->camera);

	this->simulation.render(target);
}