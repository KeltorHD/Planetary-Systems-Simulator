#include "pch.h"
#include "simulationState.hpp"

//Init func
void SimulationState::initVariables()
{
	this->keyTime = 0.f;
	this->keyTimeMax = 10.f;

	this->simulation.loadDemoSystem();
	this->gui.updateTypes();

	this->camera.setSize
	(
		static_cast<float>(this->stateData->programSettings->resolution.width),
		static_cast<float>(this->stateData->programSettings->resolution.height)
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
			this->keybinds[key] = this->supportedKeys->at(std::move(key2));
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
	: State(state_data), gui(&this->simulation, this->locale, this->soundManager, this->camera, this->quit)
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

	/*обработка событий*/
	float deltaWheel{ 0 };
	size_t length{ this->stateData->events.size() };
	for (size_t i = 0; i < length; i++)
	{
		sf::Event event = this->stateData->events.front();
		this->stateData->events.pop();
		this->stateData->events.push(event);
		if (event.type == sf::Event::MouseWheelScrolled
			&& event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel)
		{
			deltaWheel = event.mouseWheelScroll.delta;
			break;
		}
	}

	/*camera scroll*/
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("CAM_SCROLL_UP")))
		|| sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("CAM_SCROLL_UP2")))
		|| (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("CTRL")))
			&& deltaWheel > 0))
	{
		this->camera.zoom(1.f - (deltaWheel ? 5 : 1) * camera_scroll);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("CAM_SCROLL_DOWN")))
		|| sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("CAM_SCROLL_DOWN2")))
		|| (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("CTRL")))
			&& deltaWheel < 0))
	{
		this->camera.zoom(1.f + (deltaWheel ? 5 : 1) * camera_scroll);
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
		/*используем dt для того, чтобы объекты двигались равномерно при разных FPS*/
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