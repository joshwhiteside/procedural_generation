#pragma once
#include "Vector3.h"


class BoundingVolume{
public:
	BoundingVolume(Vector3* p, float r):pos(p), radius(r){};
	virtual bool IsInside() = 0;
protected:
	Vector3* pos;
	float radius;
};