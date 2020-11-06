#include "pch.h"
#include "mainMenuState.hpp"

//Init func
void MainMenuState::initVariables()
{
	simulation.loadSystemXml("systems/base.xml");
	this->isSettings = false;
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
	
}


void MainMenuState::updateInput(const float& dt)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("CLOSE"))))
	{
		this->endState();
	}
}

namespace ImGui
{
	static auto vector_getter = [](void* vec, int idx, const char** out_text)
	{
		auto& vector = *static_cast<std::vector<std::string>*>(vec);
		if (idx < 0 || idx >= static_cast<int>(vector.size())) { return false; }
		*out_text = vector.at(idx).c_str();
		return true;
	};

	bool Combo(const char* label, int* currIndex, std::vector<std::string>& values)
	{
		if (values.empty()) { return false; }
		return Combo(label, currIndex, vector_getter,
			static_cast<void*>(&values), values.size());
	}

	bool ListBox(const char* label, int* currIndex, std::vector<std::string>& values)
	{
		if (values.empty()) { return false; }
		return ListBox(label, currIndex, vector_getter,
			static_cast<void*>(&values), values.size());
	}

}

void MainMenuState::updateGUI(const float& dt)
{
	sf::VideoMode vm{ this->stateData->gfxSettings->resolution };
	float width{ 250.f + vm.width / 20.f};
	float height{ 200.f };

	ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);

	ImGui::Begin(this->locale->get_c("program_title")); // создаём окно
	ImGui::SetWindowSize(sf::Vector2f(width, height));

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
	
	ImGui::End(); // end window

	//if (isSettings)
	//{
	//	int selectedItem{ 0 };
	//	std::vector<std::string> lang;
	//	for (const auto& entry : std::filesystem::directory_iterator("lang/"))
	//		lang.push_back(entry.path().string());

	//	

	//	ImGui::BeginPopup("Settings"); // создаём окно


	//	ImGui::Combo("lang", &selectedItem, lang);


	//	ImGui::EndPopup();
	//}

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

