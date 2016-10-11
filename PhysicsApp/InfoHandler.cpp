#include "InfoHandler.h"

InfoHandler::InfoHandler()
{
	this->numberOfBodies = 0;
	this->watchingPlanet = 0;
	this->inputCheck = false;
}

InfoHandler::InfoHandler(size_t numberOfBodies)
{
	this->numberOfBodies = numberOfBodies;
	this->watchingPlanet = 0;				
	this->inputCheck = false;
}
	

InfoHandler::~InfoHandler()
{
}

void InfoHandler::checkInput()
{
	if (GetAsyncKeyState(VK_TAB) && inputCheck == false)
	{
		inputCheck = true;
		watchingPlanet += 1;

		if (watchingPlanet >= this->numberOfBodies)
		{
			watchingPlanet = 0;
		}
	}
	else if (GetAsyncKeyState(VK_TAB) == 0)
	{
		inputCheck = false;
	}
}

void InfoHandler::setPlanetWatched(int numberOfThePlanet)
{
	this->watchingPlanet = numberOfThePlanet;
}

void InfoHandler::setNumOfPlanets(size_t NumOfPlanets)
{
	this->numberOfBodies = NumOfPlanets;
}

int InfoHandler::getNumOfPlanets()
{
	return this->numberOfBodies;
}

int InfoHandler::getPlanetWatched()
{
	return this->watchingPlanet;
}
