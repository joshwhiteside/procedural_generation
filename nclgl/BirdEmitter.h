#pragma once

#include "ParticleEmitter.h"



class BirdEmitter: public ParticleEmitter {
public:
	virtual void Update(float msec);


	void setalive(bool b) {alive = b;}
	bool getalive()		  {return alive;}
private:
	bool alive;


};