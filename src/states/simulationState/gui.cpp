#include "pch.h"
#include "gui.h"

static int FilterImGuiLetters(ImGuiInputTextCallbackData* data)
{
	if (data->EventChar < 256 && strchr(" 0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZабвгдеёжзийклмнопрстуфхцчшщъыьэюяАБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ", (char)data->EventChar))
		return 0;
	return 1;
}

Gui::Gui(PhysSimulation* phys, const Locale* locale, SoundManager* soundManager, sf::View& camera, bool& quit)
	: camera(camera), simulation(phys), locale(locale), quit(quit), soundManager(soundManager)
{
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
	std::copy(this->simulation->getName().begin(), this->simulation->getName().end(), this->input_name);
	this->input_name[this->simulation->getName().length()] = 0;
	std::copy(this->simulation->getDesc().begin(), this->simulation->getDesc().end(), this->input_desc);
	this->input_desc[this->simulation->getDesc().length()] = 0;

	std::vector<std::string> v;
	for (const auto& entry : std::filesystem::directory_iterator("systems/"))
	{
		std::string tmp = entry.path().string().substr
		(
			entry.path().string().find_last_of('/') + 1, entry.path().string().size() - entry.path().string().find_last_of('/') - 5
		);
		v.push_back(std::move(tmp));
	}
	this->systems_length = v.size();
	this->systems = new char* [this->systems_length];
	for (size_t i = 0; i < this->systems_length; i++)
	{
		this->systems[i] = new char[v[i].length() + 1];
		std::copy(v[i].begin(), v[i].end(), this->systems[i]);
		this->systems[i][v[i].length()] = 0;
	}
}

Gui::~Gui()
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

void Gui::updateInput(sf::RenderWindow* window, std::queue<sf::Event>& events)
{
	bool isLeftPress{ false };
	while (events.size())
	{
		if (events.front().type == sf::Event::MouseButtonPressed
			&& events.front().mouseButton.button == sf::Mouse::Left
			&& !ImGui::GetIO().WantCaptureMouse)
		{
			isLeftPress = true;
		}
		events.pop();
	}
	if (this->isAdding && isLeftPress)
	{
		this->soundManager->play("click");
		if (!this->add_obj)
		{
			this->add_obj = new SpaceObj
			(
				static_cast<double>(int(rand() % 100) + 1),
				static_cast<double>(window->mapPixelToCoords(sf::Mouse::getPosition(*window)).x),
				static_cast<double>(window->mapPixelToCoords(sf::Mouse::getPosition(*window)).y),
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
			this->add_obj->setX() = static_cast<double>(window->mapPixelToCoords(sf::Mouse::getPosition(*window)).x);
			this->add_obj->setY() = static_cast<double>(window->mapPixelToCoords(sf::Mouse::getPosition(*window)).y);
		}
	}
	else if (isLeftPress) /*открытие меню изменения добавленного объекта*/
	{
		bool isFind{ false };
		/*поиск объекта, на который нажали*/
		for (size_t i = 0; i < this->simulation->getCountObj(); i++)
		{
			if (std::sqrt
			(
				std::pow(window->mapPixelToCoords(sf::Mouse::getPosition(*window)).x - this->simulation->getObjects()[i]->getX(), 2)
				+
				std::pow(window->mapPixelToCoords(sf::Mouse::getPosition(*window)).y - this->simulation->getObjects()[i]->getY(), 2)
			) <= this->simulation->getObjects()[i]->getRadius())
			{
				this->soundManager->play("click");
				this->open_edit_menu = this->simulation->getObjects()[i];
				isFind = true;
			}
		}
		if (!isFind)
			this->open_edit_menu = nullptr;
	}
}

void Gui::update()
{
	/*отрисовка меню вверху экрана*/
	this->updateMainMenuBar();

	/*отрисовка меню управления симуляцией*/
	this->updateControlSim();

	/*отрисовка меню редактирования симуляцией*/
	this->updateEditSim();

	/*отрисовка меню добавления объекта*/
	this->updateAddObj();

	if (this->add_obj) /*обновление добавляемого объекта*/
		add_obj->update();
}

void Gui::render(sf::RenderTarget* target)
{
	if (this->add_obj)
		add_obj->render(target);
}

const Gui::control_t& Gui::getControl() const
{
	return this->ctrl;
}

const float& Gui::getKoef() const
{
	return this->koef;
}

const bool& Gui::getAlwaysCenter() const
{
	return this->isAlwaysCenter;
}

void Gui::setControl(control_t ctrl)
{
	this->ctrl = ctrl;
}

void Gui::setKoef(float koef)
{
	this->koef = koef;
}

void Gui::setAlwaysCenter(bool cond)
{
	this->isAlwaysCenter = cond;
}

void Gui::updateMainMenuBar()
{
	/*отрисовка меню вверху экрана*/
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu(this->locale->get_c("system")))
		{
			if (ImGui::MenuItem(this->locale->get_c("enable_play")))
			{
				this->soundManager->play("click");
				this->enableControlSimulation = true;
			}
			if (ImGui::MenuItem(this->locale->get_c("enable_edit")))
			{
				this->soundManager->play("click");
				this->enableEditSimulation = true;
			}
			if (ImGui::MenuItem(this->locale->get_c("escape"), "Esc"))
			{
				this->soundManager->play("click");
				this->quit = true;
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

void Gui::updateControlSim()
{
	/*Контролирование процессом симуляции*/
	if (this->enableControlSimulation)
	{
		ImGui::Begin(this->locale->get_c("control_simulation"), &this->enableControlSimulation);

		ImGui::Columns(3, "control", false);

		if (ImGui::Button(this->locale->get_c("play"), ImVec2(ImGui::GetContentRegionAvail().x, 0)))
		{
			this->soundManager->play("click");
			this->ctrl = control_t::play;
		}
		ImGui::NextColumn();
		if (ImGui::Button(this->locale->get_c("pause"), ImVec2(ImGui::GetContentRegionAvail().x, 0)))
		{
			this->soundManager->play("click");
			this->ctrl = control_t::paused;
		}
		ImGui::NextColumn();
		if (ImGui::Button(this->locale->get_c("play_koef"), ImVec2(ImGui::GetContentRegionAvail().x, 0)))
		{
			this->soundManager->play("click");
			this->ctrl = control_t::play_koef;
		}
		ImGui::NextColumn();
		ImGui::Separator();
		if (ImGui::Button(this->locale->get_c("reload"), ImVec2(ImGui::GetContentRegionAvail().x, 0)))
		{
			this->soundManager->play("click");
			this->simulation->restoreInitialState();
			this->ctrl = control_t::paused;
		}
		ImGui::NextColumn();
		if (ImGui::Button(this->locale->get_c("center"), ImVec2(ImGui::GetContentRegionAvail().x, 0)))
		{
			this->soundManager->play("click");
			this->camera.setCenter(this->simulation->getMaxMassCoord());
		}
		ImGui::NextColumn();
		if (ImGui::Button(this->locale->get_c("always_center"), ImVec2(ImGui::GetContentRegionAvail().x, 0)))
		{
			this->soundManager->play("click");
			this->isAlwaysCenter = !this->isAlwaysCenter;
		}
		ImGui::Columns(1);
		ImGui::Separator();

		ImGui::SliderFloat(this->locale->get_c("koef"), &this->koef, 0.1f, 10.f, "%.3f");

		ImGui::End();
	}
}

void Gui::updateEditSim()
{
	/*отрисовка меню редактирования симуляцией*/
	if (this->enableEditSimulation)
	{
		/*присваивание переменных*/
		std::copy(this->simulation->getName().begin(), this->simulation->getName().end(), this->input_name);
		this->input_name[this->simulation->getName().length()] = 0;
		std::copy(this->simulation->getDesc().begin(), this->simulation->getDesc().end(), this->input_desc);
		this->input_desc[this->simulation->getDesc().length()] = 0;

		ImGui::Begin(this->locale->get_c("edit_simulation"), &this->enableEditSimulation);

		ImGui::Text(this->locale->get_c("info"));

		/*название системы*/
		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * 0.5f);
		if (ImGui::InputText(this->locale->get_c("name"), this->input_name, 256, ImGuiInputTextFlags_CallbackCharFilter, FilterImGuiLetters))
		{
			this->simulation->setName(this->input_name);
		}

		/*описание системы*/
		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * 0.5f);
		if (ImGui::InputText(this->locale->get_c("desc"), this->input_desc, 256))
		{
			this->simulation->setDescription(this->input_desc);
		}

		/*Кнопки создать тело и сохранить систему*/
		if (ImGui::Button(this->locale->get_c("add")))
		{
			this->soundManager->play("click");
			this->isAdding = !this->isAdding;
		}
		if (ImGui::Button(this->locale->get_c("save_system")))
		{
			this->soundManager->play("click");
			this->simulation->replaceSimtoSave();
			this->simulation->saveSystemXml();
		}

		if (ImGui::TreeNode(this->locale->get_c("load_system"))) /*загрузка системы*/
		{
			static int system_input{};
			ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * 0.4f);
			ImGui::Combo(this->locale->get_c("available_system"), &system_input, this->systems, this->systems_length);

			if (ImGui::Button(this->locale->get_c("load")))
			{
				this->soundManager->play("click");
				this->isAdding = false;
				if (this->add_obj)
				{
					delete this->add_obj;
					this->add_obj = nullptr;
				}
				this->open_edit_menu = nullptr;
				this->enableAddMenu = false;
				this->ctrl = control_t::paused;
				this->simulation->loadSystemXml(std::string("systems/") + this->systems[system_input] + ".xml");
				this->camera.setCenter(this->simulation->getMaxMassCoord());
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
				this->soundManager->play("click");
				this->isAdding = false;
				if (this->add_obj)
				{
					delete this->add_obj;
					this->add_obj = nullptr;
				}
				this->enableAddMenu = false;
				this->open_edit_menu = nullptr;
				this->ctrl = control_t::paused;
				this->simulation->createSystem(this->new_system_name, this->new_system_desc);
				this->camera.setCenter(this->simulation->getMaxMassCoord());
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
			for (size_t i = 0; i < this->simulation->getCountObj(); i++)
			{
				if (this->simulation->getObjects()[i] == this->open_edit_menu) /*открываем, если нажали на объект*/
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
					color = ImVec4(this->simulation->getObjects()[i]->getColor());
					std::fill(this->edit_name_obj, this->edit_name_obj + 256, 0);
					std::copy(this->simulation->getObjects()[i]->getName().begin(), this->simulation->getObjects()[i]->getName().end(), this->edit_name_obj);

					ImGui::Text(this->locale->get_c("characteristics")); /*характеристики*/

					ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x * 0.5f);
					if (ImGui::InputText(this->locale->get_c("object_name"), this->edit_name_obj, 256)) /*имя тела*/
					{
						this->simulation->setObjects()[i]->setName(this->edit_name_obj);
					}

					if (ImGui::ColorEdit3(this->locale->get_c("object_color"), (float*)&color))  /*цвет*/
					{
						this->simulation->setObjects()[i]->setColor(static_cast<sf::Color>(color));
					}

					if (ImGui::Combo(this->locale->get_c("object_type"), &index_type, this->type_names, static_cast<int>(SpaceObj::obj_t::count)))
					{
						this->simulation->setObjects()[i]->setType(SpaceObj::obj_t(index_type));
					}

					ImGui::InputDouble(this->locale->get_c("mass"), &this->simulation->setObjects()[i]->setMass(), 10., 50.0, "%.3f");
					ImGui::InputDouble(this->locale->get_c("x"), &this->simulation->setObjects()[i]->setX(), 1., 1.0, "%.3f");
					ImGui::InputDouble(this->locale->get_c("y"), &this->simulation->setObjects()[i]->setY(), 1., 1.0, "%.3f");
					ImGui::InputDouble(this->locale->get_c("vx"), &this->simulation->setObjects()[i]->setVx(), 0.5, 1.0, "%.3f");
					ImGui::InputDouble(this->locale->get_c("vy"), &this->simulation->setObjects()[i]->setVy(), 0.5, 1.0, "%.3f");

					if (ImGui::InputDouble(this->locale->get_c("r"), &this->simulation->setObjects()[i]->setRadius(), 1., 1.0, "%.3f"))
					{
						if (this->simulation->setObjects()[i]->setRadius() < 0.)
						{
							this->simulation->setObjects()[i]->setRadius() = 0.;
						}
					}
					ImGui::PopItemWidth();

					if (ImGui::Button(this->locale->get_c("delete")))
					{
						this->soundManager->play("click");
						this->simulation->deleteObj(i);
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

void Gui::updateAddObj()
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
			this->soundManager->play("click");
			this->simulation->addObj(*this->add_obj);
			delete this->add_obj;
			this->add_obj = nullptr;
			this->isAdding = false;
		}
		ImGui::SameLine();
		if (ImGui::Button(this->locale->get_c("del_close")))
		{
			this->soundManager->play("click");
			delete this->add_obj;
			this->add_obj = nullptr;
			this->isAdding = false;
		}

		ImGui::End();
	}
}