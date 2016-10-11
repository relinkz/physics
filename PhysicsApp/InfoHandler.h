#ifndef INFOHANDLER_H
#define INFOHANDLER_H

#include "GlobalData.h"

class InfoHandler
{
private:
	size_t numberOfBodies;
	int watchingPlanet;
	bool inputCheck;

public:
	InfoHandler();
	InfoHandler(size_t numberOfBodies);
	~InfoHandler();

	void checkInput();

	void setPlanetWatched(int numberOfThePlanet);
	void setNumOfPlanets(size_t NumOfPlanets);

	int getNumOfPlanets();
	int getPlanetWatched();
};

#endif