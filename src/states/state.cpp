#include "pch.h"
#include "state.hpp"

StateData::StateData()
{
	this->gfxSettings = nullptr;
	this->states = nullptr;
	this->supportedKeys = nullptr;
	this->window = nullptr;
	this->locale = nullptr;
}

State::State(StateData* state_data)
{
	this->stateData = state_data;
	this->locale = state_data->locale;
	this->window = state_data->window;
	this->supportedKeys = state_data->supportedKeys;
	this->states = state_data->states;
	this->quit = false;
	this->paused = false;
}

State::~State()
{

}

//accessors
const bool& State::getQuit() const
{
	return this->quit;
}

//func
void State::endState()
{
	this->quit = true;
}

void State::pauseState()
{
	this->paused = true;
}

void State::unpauseState()
{
	this->paused = false;
}

void State::updateMousePositions(sf::View* view)
{
	this->mousePosScreen = sf::Mouse::getPosition();
	this->mousePosWindow = sf::Mouse::getPosition(*this->window);

	if (view)
		this->window->setView(*view);
	this->mousePosView = this->window->mapPixelToCoords(sf::Mouse::getPosition(*this->window));

	this->window->setView(this->window->getDefaultView());
}