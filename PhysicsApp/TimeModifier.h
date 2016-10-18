#pragma once
#ifndef TIMEMODIFIER_H
#define TIMEMODIFIER_H
#include "GlobalData.h"

class TimeModifier
{
private:
	int time;
public:
	TimeModifier();
	~TimeModifier();

	void checkInput();

	int getTime();
};


#endif // ! TIMEMODIFIER_H