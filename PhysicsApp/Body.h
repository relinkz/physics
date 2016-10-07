#ifndef BODY_H
#define BODY_H
#include "Model.h"
#include "GlobalData.h"


class Body
{
private:
	std::string name;
	Vector3 velocity;
	Vector3 position;
	long float mass;
	float size;
	Model* bodyModel;
	ID3D11ShaderResourceView *SRV;
public:
	Body();
	Body(Model* src, const Vector3 &pos);
	virtual ~Body();

	void update();
	void applyForce(Vector3 &force);

	std::string getName() const;
	Vector3 getVelocity() const;
	Vector3 getPosition() const;
	float getMass() const;
	float getSize() const;
	Model* getModelAdress();
	ID3D11ShaderResourceView* getSRV();

	void setName(const std::string& newName);
	void setVelocity(const Vector3& newVelocity);
	void setPosition(const Vector3& newPosition);
	void setMass(const float& newMass);
	void setSize(const float& newSize);
	void setModelAdress(Model* newAdress);
	void setSRV(ID3D11ShaderResourceView *newSRV);

};

#endif

