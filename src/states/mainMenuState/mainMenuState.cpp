#include "pch.h"
#include "mainMenuState.hpp"

//Init func
void MainMenuState::initVariables()
{
	PhysSimulation ps;
	ps.loadSystemXml("systems/base.xml");
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

void MainMenuState::initGUI()
{
	
}

MainMenuState::MainMenuState(StateData* state_data)
	: State(state_data)
{
	this->initVariables();
	this->initKeybinds();
	this->initGUI();
}

MainMenuState::~MainMenuState()
{
	
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
	sf::VideoMode vm{ this->stateData->gfxSettings->resolution };
	float width{ 250.f + vm.width / 20.f};
	float height{ 200.f };

	ImGui::Begin("Planetary Systems Simulator"); // создаём окно
	ImGui::SetWindowSize(sf::Vector2f(width, height));

	if (ImGui::Button("Simulation"))
	{
		std::cout << "call sim" << std::endl;
	}

	if (ImGui::Button("Quit"))
	{
		this->endState();
	}
	
	ImGui::End(); // end window
}

void MainMenuState::update(const float& dt)
{
	this->updateMousePositions();
	this->updateInput(dt);

	this->updateGUI(dt);
}

void MainMenuState::renderGUI(sf::RenderTarget& target)
{
	
}

void MainMenuState::render(sf::RenderTarget* target)
{
	if (!target)
	{
		target = this->window;
	}

	this->renderGUI(*target);

}

