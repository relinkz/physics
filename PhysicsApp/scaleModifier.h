#ifndef SCALEMODIFIER_H
#define SCALEMODIFIER_H

#include "GlobalData.h"


class scaleModifier
{
private:
	float modifier;
public:
	scaleModifier();
	virtual ~scaleModifier();

	void setModifier(float newMod);
	float getModifier() const;

	void checkInput();

	void increase();
	void decrease();
};

#endif

