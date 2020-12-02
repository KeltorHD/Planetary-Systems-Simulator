#include "pch.h"
#include "state.hpp"

StateData::StateData()
{
	this->programSettings = nullptr;
	this->states = nullptr;
	this->supportedKeys = nullptr;
	this->window = nullptr;
	this->locale = nullptr;
	this->soundManager = nullptr;
}

State::State(StateData* state_data)
{
	this->stateData = state_data;
	this->locale = state_data->locale;
	this->soundManager = state_data->soundManager;
	this->window = state_data->window;
	this->supportedKeys = state_data->supportedKeys;
	this->states = state_data->states;
	this->quit = false;
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