#include "TimeModifier.h"


TimeModifier::TimeModifier()
{
	this->time = 10;
}

TimeModifier::~TimeModifier()
{
}

void TimeModifier::checkInput()
{
	if (GetAsyncKeyState(VK_U))
	{
		this->time++;
	}
	if (GetAsyncKeyState(VK_H))
	{
		this->time--;
	}
	if (this->time < 1)
	{
		this->time = 1;
	}
}

int TimeModifier::getTime()
{
	return this->time;
}

