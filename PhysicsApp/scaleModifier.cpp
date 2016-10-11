#include "scaleModifier.h"



scaleModifier::scaleModifier()
{
	this->modifier = 0.02f; //default val
}


scaleModifier::~scaleModifier()
{
}

void scaleModifier::setModifier(float newMod)
{
	this->modifier = newMod;
}

float scaleModifier::getModifier() const
{
	return this->modifier;
}

void scaleModifier::checkInput()
{
	if (GetAsyncKeyState(VK_I))
	{
		this->increase();
	}
	if (GetAsyncKeyState(VK_J))
	{
		this->decrease();
	}
	if (this->modifier <= 0.0005f)
	{
		this->modifier = 0.0005f;
	}
}

void scaleModifier::increase()
{
	this->modifier += 0.00005;
}

void scaleModifier::decrease()
{
	this->modifier -= 0.00005;
}
