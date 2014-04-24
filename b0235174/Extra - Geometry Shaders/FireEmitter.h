#pragma once

#include "ParticleEmitter.h"

class FireEmitter: public ParticleEmitter {
public:
	virtual void Update(float msec);


	void setalive(bool b) {alive = b;}
	bool getalive()		  {return alive;}
protected:
	virtual Particle* GetFreeParticle();
private:
	bool alive;


};