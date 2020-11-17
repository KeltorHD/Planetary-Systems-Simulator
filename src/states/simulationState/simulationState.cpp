#include "pch.h"
#include "simulationState.hpp"


static int FilterImGuiLetters(ImGuiInputTextCallbackData* data)
{
	if (data->EventChar < 256 && strchr(" 0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZабвгдеёжзийклмнопрстуфхцчшщъыьэюяАБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ", (char)data->EventChar))
		return 0;
	return 1;
}

//Init func
void SimulationState::initVariables()
{
	this->simulation.loadDemoSystem();
	this->ctrl = control_t::paused;
	this->enableControlSimulation = true;
	this->enableEditSimulation = true;
	this->isAlwaysCenter = false;
	this->isAdding = false;
	this->add_obj = nullptr;
	this->open_edit_menu = nullptr;
	this->enableAddMenu = false;
	this->koef = 0.1f;
	this->new_system_name = new char[256]{ 0 };
	this->new_system_desc = new char[256]{ 0 };
	this->input_name = new char[256]{ 0 };
	this->input_desc = new char[256]{ 0 };
	this->edit_name_obj = new char[256]{ 0 };
	this->type_names = new char* [size_t(SpaceObj::obj_t::count)];

	for (size_t i = 0; i < size_t(SpaceObj::obj_t::count); i++)
	{
		std::string tmp = this->locale->get_s(SpaceObj::objToString(SpaceObj::obj_t(i)));
		this->type_names[i] = new char[tmp.length() + 1];
		std::copy(tmp.begin(), tmp.end(), this->type_names[i]);
		this->type_names[i][tmp.length()] = 0;
	}
	std::copy(this->simulation.getName().begin(), this->simulation.getName().end(), this->input_name);
	this->input_name[this->simulation.getName().length()] = 0;
	std::copy(this->simulation.getDesc().begin(), this->simulation.getDesc().end(), this->input_desc);
	this->input_desc[this->simulation.getDesc().length()] = 0;

	std::vector<std::string> v;
	for (const auto& entry : std::filesystem::directory_iterator("systems/"))
	{
		std::string tmp = entry.path().string().substr
		(
			entry.path().string().find_last_of('/') + 1, entry.path().string().size() - entry.path().string().find_last_of('/') - 5
		);
		v.push_back(tmp);
	}
	this->systems_length = v.size();
	this->systems = new char* [this->systems_length];
	for (size_t i = 0; i < this->systems_length; i++)
	{
		this->systems[i] = new char[v[i].length() + 1];
		std::copy(v[i].begin(), v[i].end(), this->systems[i]);
		this->systems[i][v[i].length()] = 0;
	}

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
	: State(state_data)
{
	this->keyTime = 0.f;
	this->keyTimeMax = 10.f;
	this->initVariables();
	this->initKeybinds();
}

SimulationState::~SimulationState()
{
	for (size_t i = 0; i < size_t(SpaceObj::obj_t::count); i++)
	{
		delete[] this->type_names[i];
	}
	delete[] this->new_system_name;
	delete[] this->new_system_desc;
	delete[] this->type_names;
	delete[] this->input_name;
	delete[] this->input_desc;
	delete[] this->edit_name_obj;
	if (this->add_obj)
		delete this->add_obj;
}


void SimulationState::updateInput(const float& dt)
{
	/*уравление симуляцией*/
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("RELOAD"))) && this->getKeyTime())
	{
		this->simulation.restoreInitialState();
		this->ctrl = control_t::paused;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("SPACE"))) && this->getKeyTime())
	{
		this->ctrl = this->ctrl == control_t::paused ? control_t::play : control_t::paused;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("CAM_CENTER"))) && this->getKeyTime()) /*camera set pos at max mass object*/
	{
		this->camera.setCenter(this->simulation.getMaxMassCoord());
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("ALWAYS_CENTER"))) && this->getKeyTime())
	{
		this->isAlwaysCenter = !this->isAlwaysCenter;
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

	/*adding obj*/
	bool isLeftPress{ false };
	while (this->stateData->events.size())
	{
		if (this->stateData->events.front().type == sf::Event::MouseButtonPressed
			&& this->stateData->events.front().mouseButton.button == sf::Mouse::Left
			&& !ImGui::GetIO().WantCaptureMouse)
		{
			isLeftPress = true;
		}
		this->stateData->events.pop();
	}
	if (this->isAdding && isLeftPress)
	{
		if (!this->add_obj)
		{
			this->add_obj = new SpaceObj
			(
				static_cast<double>((rand() % 100) + 1),
				static_cast<double>(this->window->mapPixelToCoords(sf::Mouse::getPosition(*this->window)).x),
				static_cast<double>(this->window->mapPixelToCoords(sf::Mouse::getPosition(*this->window)).y),
				static_cast<double>(rand() % 10),
				static_cast<double>(rand() % 10),
				"DEFAULT",
				static_cast<double>(rand() % 10),
				sf::Color(rand() % 255, rand() % 255, rand() % 255),
				SpaceObj::obj_t(rand() % int(SpaceObj::obj_t::count))
			);
			this->enableAddMenu = true;
		}
		else
		{
			this->add_obj->setX() = static_cast<double>(this->window->mapPixelToCoords(sf::Mouse::getPosition(*this->window)).x);
			this->add_obj->setY() = static_cast<double>(this->window->mapPixelToCoords(sf::Mouse::getPosition(*this->window)).y);
		}
	}
	else if (isLeftPress)
	{
		bool isFind{ false };
		/*поиск объекта, на который нажали*/
		for (size_t i = 0; i < this->simulation.getCountObj(); i++)
		{
			if (std::sqrt
			(
				std::pow(this->window->mapPixelToCoords(sf::Mouse::getPosition(*this->window)).x - this->simulation.getObjects()[i]->getX(), 2)
				+
				std::pow(this->window->mapPixelToCoords(sf::Mouse::getPosition(*this->window)).y - this->simulation.getObjects()[i]->getY(), 2)
			) <= this->simulation.getObjects()[i]->getRadius())
			{
				this->open_edit_menu = this->simulation.getObjects()[i];
				isFind = true;
			}
		}
		if (!isFind)
			this->open_edit_menu = nullptr;
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

	/*отрисовка меню управления симуляцией*/
	this->updateControlSim();

	/*отрисовка меню редактирования симуляцией*/
	this->updateEditSim();

	/*отрисовка меню добавления объекта*/
	this->updateAddObj();

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
			if (ImGui::MenuItem(this->locale->get_c("escape"), "Esc"))
			{
				this->endState();
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu(this->locale->get_c("help")))
		{
			ImGui::MenuItem(this->locale->get_c("move_up"), "W");
			ImGui::MenuItem(this->locale->get_c("move_down"), "S");
			ImGui::MenuItem(this->locale->get_c("move_left"), "A");
			ImGui::MenuItem(this->locale->get_c("move_right"), "D");
			ImGui::MenuItem(this->locale->get_c("move_scroll_up"), "+");
			ImGui::MenuItem(this->locale->get_c("move_scroll_down"), "-");
			ImGui::MenuItem(this->locale->get_c("center_mass"), "C");
			ImGui::MenuItem(this->locale->get_c("reload_help"), "R");
			ImGui::MenuItem(this->locale->get_c("play_pause_help"), "Space");
			ImGui::MenuItem(this->locale->get_c("always_center_help"), "X");
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}

void SimulationState::updateControlSim()
{
	/*Контролирование процессом симуляции*/
	if (this->enableControlSimulation)
	{
		ImGui::Begin(this->locale->get_c("control_simulation"), &this->enableControlSimulation);

		ImGui::Columns(3, "control", false);

		if (ImGui::Button(this->locale->get_c("play"), ImVec2(ImGui::GetContentRegionAvail().x, 0)))
		{
			this->ctrl = control_t::play;
		}
		ImGui::NextColumn();
		if (ImGui::Button(this->locale->get_c("pause"), ImVec2(ImGui::GetContentRegionAvail().x, 0)))
		{
			this->ctrl = control_t::paused;
		}
		ImGui::NextColumn();
		if (ImGui::Button(this->locale->get_c("play_koef"), ImVec2(ImGui::GetContentRegionAvail().x, 0)))
		{
			this->ctrl = control_t::play_koef;
		}
		ImGui::NextColumn();
		ImGui::Separator();
		if (ImGui::Button(this->locale->get_c("reload"), ImVec2(ImGui::GetContentRegionAvail().x, 0)))
		{
			this->simulation.restoreInitialState();
			this->ctrl = control_t::paused;
		}
		ImGui::NextColumn();
		if (ImGui::Button(this->locale->get_c("center"), ImVec2(ImGui::GetContentRegionAvail().x, 0)))
		{
			this->camera.setCenter(this->simulation.getMaxMassCoord());
		}
		ImGui::NextColumn();
		if (ImGui::Button(this->locale->get_c("always_center"), ImVec2(ImGui::GetContentRegionAvail().x, 0)))
		{
			this->isAlwaysCenter = !this->isAlwaysCenter;
		}
		ImGui::Columns(1);
		ImGui::Separator();

		ImGui::SliderFloat(this->locale->get_c("koef"), &this->koef, 0.1f, 10.f, "%.3f");

		ImGui::End();
	}
}

void SimulationState::updateEditSim()
{
	/*отрисовка меню редактирования симуляцией*/
	if (this->enableEditSimulation)
	{
		/*присваивание переменных*/
		std::copy(this->simulation.getName().begin(), this->simulation.getName().end(), this->input_name);
		this->input_name[this->simulation.getName().length()] = 0;
		std::copy(this->simulation.getDesc().begin(), this->simulation.getDesc().end(), this->input_desc);
		this->input_desc[this->simulation.getDesc().length()] = 0;

		ImGui::Begin(this->locale->get_c("edit_simulation"), &this->enableEditSimulation);

		ImGui::Text(this->locale->get_c("info"));

		/*название системы*/
		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * 0.5f);
		if (ImGui::InputText(this->locale->get_c("name"), this->input_name, 256, ImGuiInputTextFlags_CallbackCharFilter, FilterImGuiLetters))
		{
			this->simulation.setName(this->input_name);
		}

		/*описание системы*/
		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * 0.5f);
		if (ImGui::InputText(this->locale->get_c("desc"), this->input_desc, 256))
		{
			this->simulation.setDescription(this->input_desc);
		}

		/*Кнопки создать тело и сохранить систему*/
		if (ImGui::Button(this->locale->get_c("add")))
		{
			this->isAdding = !this->isAdding;
		}
		if (ImGui::Button(this->locale->get_c("save_system")))
		{
			this->simulation.replaceSimtoSave();
			this->simulation.saveSystemXml();
		}

		if (ImGui::TreeNode(this->locale->get_c("load_system"))) /*загрузка системы*/
		{
			static int system_input{};
			ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * 0.4f);
			ImGui::Combo(this->locale->get_c("available_system"), &system_input, this->systems, this->systems_length);

			if (ImGui::Button(this->locale->get_c("load")))
			{
				this->isAdding = false;
				if (this->add_obj)
				{
					delete this->add_obj;
					this->add_obj = nullptr;
				}
				this->open_edit_menu = nullptr;
				this->enableAddMenu = false;
				this->ctrl = control_t::paused;
				this->simulation.loadSystemXml(std::string("systems/") + this->systems[system_input] + ".xml");
				this->camera.setCenter(this->simulation.getMaxMassCoord());
			}
			
			ImGui::TreePop();
		}
		if (ImGui::TreeNode(this->locale->get_c("new_system"))) /*создание системы*/
		{
			/*название системы*/
			ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * 0.5f);
			ImGui::InputText(this->locale->get_c("name"), this->new_system_name, 256, ImGuiInputTextFlags_CallbackCharFilter, FilterImGuiLetters);

			/*описание системы*/
			ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * 0.5f);
			ImGui::InputText(this->locale->get_c("desc"), this->new_system_desc, 256);

			if (ImGui::Button(this->locale->get_c("new")) && std::strlen(this->new_system_name))
			{
				this->isAdding = false;
				if (this->add_obj)
				{
					delete this->add_obj;
					this->add_obj = nullptr;
				}
				this->enableAddMenu = false;
				this->open_edit_menu = nullptr;
				this->ctrl = control_t::paused;
				this->simulation.createSystem(this->new_system_name, this->new_system_desc);
				this->camera.setCenter(this->simulation.getMaxMassCoord());
			}

			ImGui::TreePop();
		}

		/*Свойста тел:*/
		if (this->open_edit_menu) /*открываем, если нажали на объект*/
		{
			ImGui::SetNextItemOpen(true, ImGuiCond_Always);
		}
		if (ImGui::TreeNode(this->locale->get_c("objects")))
		{
			//this->open_edit_menu = nullptr;
			static ImVec4 color{};
			static int index_type{};
			for (size_t i = 0; i < this->simulation.getCountObj(); i++)
			{
				if (this->simulation.getObjects()[i] == this->open_edit_menu) /*открываем, если нажали на объект*/
				{
					ImGui::SetNextItemOpen(true, ImGuiCond_Always);
				}
				else if (this->open_edit_menu)
				{
					ImGui::SetNextItemOpen(false, ImGuiCond_Always);
				}
				if (ImGui::TreeNode(std::string(this->locale->get_s("object") + " " + std::to_string(i + 1)).c_str()))
				{
					this->open_edit_menu = nullptr;
					/*присваивание переменных*/
					bool isUpdate{ false };
					color = ImVec4(this->simulation.getObjects()[i]->getColor());
					std::fill(this->edit_name_obj, this->edit_name_obj + 256, 0);
					std::copy(this->simulation.getObjects()[i]->getName().begin(), this->simulation.getObjects()[i]->getName().end(), this->edit_name_obj);

					ImGui::Text(this->locale->get_c("characteristics")); /*характеристики*/

					ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x * 0.5f);
					if (ImGui::InputText(this->locale->get_c("object_name"), this->edit_name_obj, 256)) /*имя тела*/
					{
						this->simulation.setObjects()[i]->setName(this->edit_name_obj);
						isUpdate = true;
					}

					if (ImGui::ColorEdit3(this->locale->get_c("object_color"), (float*)&color))  /*цвет*/
					{ 
						isUpdate = true; 
						this->simulation.setObjects()[i]->setColor(static_cast<sf::Color>(color));
					}

					if (ImGui::Combo(this->locale->get_c("object_type"), &index_type, this->type_names, static_cast<int>(SpaceObj::obj_t::count)))
					{
						isUpdate = true;
						this->simulation.setObjects()[i]->setType(SpaceObj::obj_t(index_type));
					}

					ImGui::InputDouble(this->locale->get_c("mass"), &this->simulation.setObjects()[i]->setMass(), 10., 50.0, "%.3f");
					ImGui::InputDouble(this->locale->get_c("x"), &this->simulation.setObjects()[i]->setX(), 1., 1.0, "%.3f");
					ImGui::InputDouble(this->locale->get_c("y"), &this->simulation.setObjects()[i]->setY(), 1., 1.0, "%.3f");
					ImGui::InputDouble(this->locale->get_c("vx"), &this->simulation.setObjects()[i]->setVx(), 0.5, 1.0, "%.3f");
					ImGui::InputDouble(this->locale->get_c("vy"), &this->simulation.setObjects()[i]->setVy(), 0.5, 1.0, "%.3f");

					if (ImGui::InputDouble(this->locale->get_c("r"), &this->simulation.setObjects()[i]->setRadius(), 1., 1.0, "%.3f"))
					{ 
						if (this->simulation.setObjects()[i]->setRadius() < 0.)
						{
							this->simulation.setObjects()[i]->setRadius() = 0.;
						}
					}
					ImGui::PopItemWidth();

					if (ImGui::Button(this->locale->get_c("delete")))
					{
						this->simulation.deleteObj(i);
						this->open_edit_menu = nullptr;
					}

					ImGui::TreePop();
				}
			}
			ImGui::TreePop();
		}

		ImGui::End();
	}
}

void SimulationState::updateAddObj()
{
	/*начало меню добавления планеты*/
	if (this->isAdding && this->add_obj && ImGui::Begin(this->locale->get_c("adding_object")))
	{
		static ImVec4 color{};
		static int index_type{};

		/*присваивание переменных*/
		bool isUpdate{ false };
		color = ImVec4(this->add_obj->getColor());
		std::fill(this->edit_name_obj, this->edit_name_obj + 256, 0);
		std::copy(this->add_obj->getName().begin(), add_obj->getName().end(), this->edit_name_obj);

		ImGui::Text(this->locale->get_c("characteristics")); /*характеристики*/

		ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x * 0.5f);
		if (ImGui::InputText(this->locale->get_c("object_name"), this->edit_name_obj, 256)) /*имя тела*/
		{
			this->add_obj->setName(this->edit_name_obj);
			isUpdate = true;
		}

		if (ImGui::ColorEdit3(this->locale->get_c("object_color"), (float*)&color))  /*цвет*/
		{
			isUpdate = true;
			this->add_obj->setColor(static_cast<sf::Color>(color));
		}

		if (ImGui::Combo(this->locale->get_c("object_type"), &index_type, this->type_names, static_cast<int>(SpaceObj::obj_t::count)))
		{
			isUpdate = true;
			this->add_obj->setType(SpaceObj::obj_t(index_type));
		}

		ImGui::InputDouble(this->locale->get_c("mass"), &this->add_obj->setMass(), 10., 50.0, "%.3f");
		ImGui::InputDouble(this->locale->get_c("x"), &this->add_obj->setX(), 1., 1.0, "%.3f");
		ImGui::InputDouble(this->locale->get_c("y"), &this->add_obj->setY(), 1., 1.0, "%.3f");
		ImGui::InputDouble(this->locale->get_c("vx"), &this->add_obj->setVx(), 1., 1.0, "%.3f");
		ImGui::InputDouble(this->locale->get_c("vy"), &this->add_obj->setVy(), 1., 1.0, "%.3f");
		ImGui::PopItemWidth();

		if (ImGui::InputDouble(this->locale->get_c("r"), &this->add_obj->setRadius(), 1., 1.0, "%.3f"))
		{
			if (this->add_obj->setRadius() < 0.)
			{
				this->add_obj->setRadius() = 0.;
			}
		}

		if (ImGui::Button(this->locale->get_c("save_add")))
		{
			this->simulation.addObj(*this->add_obj);
			delete this->add_obj;
			this->add_obj = nullptr;
			this->isAdding = false;
		}
		ImGui::SameLine();
		if (ImGui::Button(this->locale->get_c("del_close")))
		{
			delete this->add_obj;
			this->add_obj = nullptr;
			this->isAdding = false;
		}

		ImGui::End();
	}
}

void SimulationState::updateKeyTime(const float& dt)
{
	if (this->keyTime < this->keyTimeMax)
		this->keyTime += 50.f * dt;
}

void SimulationState::update(const float& dt)
{
	this->updateKeyTime(dt);
	if (this->ctrl != control_t::paused)
	{
		this->simulation.update((this->ctrl == control_t::play_koef ? this->koef : 1.f) * 2.f * dt);
	}

	this->updateInput(dt);
	this->updateGUI();

	if (this->add_obj)
		add_obj->update();

	if (this->isAlwaysCenter)
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

	if (this->add_obj)
		add_obj->render(target);
}