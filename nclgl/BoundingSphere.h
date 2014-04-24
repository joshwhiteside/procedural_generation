#pragma once
#include "BoundingVolume.h"

class BoundingSphere : BoundingVolume {
public:
	BoundingSphere(Vector3* p, float r):BoundingVolume(p,r){};
	virtual bool IsInside(const Vector3 check, const Vector3 normal);


protected:


};