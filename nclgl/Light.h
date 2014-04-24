#pragma once

#include "Vector4.h"
#include "Vector3.h"

class Light {
public:
	Light(Vector3 position, Vector4 colour, float radius) {
		this->position = position;
		this->colour   = colour;
		this->radius   = radius;
	}
	Light(){position = Vector3(0,0,0);
			colour = Vector4(1.0f,0.0f,0.0f,1.0f);
			radius = 30000;};

	~Light(void){};

	inline Vector3 GetPosition() const			{return position;}
	inline void SetPosition(Vector3 val)		{position = val;}

	float GetRadius() const				{return radius;}
	void SetRadius(float val)			{radius = val;}

	inline Vector4 GetColour() const			{return colour;}
	inline void SetColour(Vector4 val)			{colour = val;}


protected:
	Vector3 position;
	Vector4 colour;
	float radius;
};