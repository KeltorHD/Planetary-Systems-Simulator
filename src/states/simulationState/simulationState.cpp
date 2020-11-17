#include "pch.h"
#include "simulationState.hpp"

//Init func
void SimulationState::initVariables()
{
	this->keyTime = 0.f;
	this->keyTimeMax = 10.f;

	this->simulation.loadDemoSystem();

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

const bool SimulationState::getKeyTime()
{
	if (this->keyTime >= keyTimeMax)
	{
		this->keyTime = 0.f;
		return true;
	}
	return false;
}

SimulationState::SimulationState(StateData* state_data)
	: State(state_data), gui(&this->simulation, this->locale, this->camera, this->quit)
{
	this->initVariables();
	this->initKeybinds();
}

SimulationState::~SimulationState()
{
	
}


void SimulationState::updateInput(const float& dt)
{
	/*уравление симуляцией*/
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("RELOAD"))) && this->getKeyTime())
	{
		this->simulation.restoreInitialState();
		this->gui.setControl(Gui::control_t::paused);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("SPACE"))) && this->getKeyTime())
	{
		this->gui.setControl(this->gui.getControl() == Gui::control_t::paused ? Gui::control_t::play : Gui::control_t::paused);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("CAM_CENTER"))) && this->getKeyTime()) /*camera set pos at max mass object*/
	{
		this->camera.setCenter(this->simulation.getMaxMassCoord());
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("ALWAYS_CENTER"))) && this->getKeyTime())
	{
		this->gui.setAlwaysCenter(!this->gui.getAlwaysCenter());
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

	/*добавление объекта, открытие меню изменения добавленного объекта*/
	this->gui.updateInput(this->window, this->stateData->events);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("CLOSE"))))
	{
		this->endState();
	}
}

void SimulationState::updateGUI()
{
	ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[ImGui::GetIO().Fonts->Fonts.size() - 1]);

	this->gui.update();

	ImGui::PopFont();
}

void SimulationState::updateKeyTime(const float& dt)
{
	if (this->keyTime < this->keyTimeMax)
		this->keyTime += 50.f * dt;
}

void SimulationState::update(const float& dt)
{
	this->updateKeyTime(dt);
	if (this->gui.getControl() != Gui::control_t::paused)
	{
		this->simulation.update((this->gui.getControl() == Gui::control_t::play_koef ? this->gui.getKoef() : 1.f) * 2.f * dt);
	}

	this->updateInput(dt);
	this->updateGUI();

	if (this->gui.getAlwaysCenter())
	{
		this->camera.setCenter(this->simulation.getMaxMassCoord());
	}
}

void SimulationState::render(sf::RenderTarget* target)
{
	if (!target)
	{
		target = this->window;
	}
	this->window->setView(this->camera);

	this->simulation.render(target);
	
	this->gui.render(target);
}