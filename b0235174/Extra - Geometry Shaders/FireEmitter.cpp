#include "FireEmitter.h"


#define RAND() ((rand()%101)/100.0f)

void FireEmitter::Update(float msec) {
	nextParticleTime -= msec;	//some time has passed!

	/*
	Enough time might have passed that more than 1 'launch' of particles is necessary...
	*/
	while(nextParticleTime <= 0 && alive) {
		nextParticleTime += particleRate;
		//Add a number of particles to the vector, obtained from the free list.
		for(int i = 0; i < numLaunchParticles; ++i) {
			particles.push_back(GetFreeParticle());
		}
	}

	//Now for the particle 'think' function. Particles are so 'lightweight' there's not
	//much point putting this as a member variable of the Particle struct...


	for(std::vector<Particle *>::iterator i = particles.begin(); i != particles.end();/*No I++ here!!! */) {
		Particle *p = (*i);

		//We're keeping the particles 'life' in the alpha channel of its colour. 
		//This means it'll also fade out as it loses energy. Kinda cool?
		float life =(msec / particleLifetime);
		p->colour.w -= life;
		p->colour.y += life * 0.5f;
		

		//If this particle has ran out of life, remove it from the 'active' list,
		//and put it on the 'free' list for reuse later.
		if(p->colour.w <= 0.0f) {
			freeList.push_back(p);
			i = particles.erase(i);
		}
		else{
			//Otherwise, this particle must be still 'alive'. Update its
			//position by multiplying its normalised direction by the
			//particle speed, and adding the result to the position. Easy!
			if(p->colour.w >0.9) {
				p->direction = Vector3((RAND()-0.5f)*6,0.0f,(RAND()-0.5f)*6);

			} else if (p->colour.w <0.1) {
				p->direction = Vector3(0.0f,-2,0.0f);
			} else {
				//p->direction = Vector3(2*RAND() - 1, 0.05f - RAND() / 10, 2*RAND() - 1);
				p->direction = Vector3((RAND()-0.5f), RAND()* 3,(RAND()-0.5f));
			}



			p->position += p->direction*(msec*particleSpeed);


			++i;	//Only update our iterator value here.
			//If we didn't do this, we'd skip over particles when removing
			//particles from the list...
		}
	}

	//If we now have more particles than we have graphics memory for, we
	//must allocate some new space for them, using ResizeArrays.
	if(particles.size() > largestSize) {
		ResizeArrays();	
	}
}



Particle* FireEmitter::GetFreeParticle()	{
	Particle * p = NULL;

	//If we have a spare particle on the freelist, pop it off!
	if(freeList.size() > 0) {
		p = freeList.back();
		freeList.pop_back();
	}
	else{ 
		//no spare particles, so we need a new one
		p = new Particle();
	}

	//Now we have to reset its values - if it was popped off the
	//free list, it'll still have the values of its 'previous life'

	//p->colour		= Vector4(RAND(),RAND(),RAND(),1.0);
	p->colour		= Vector4(1.0,0.0,0.0,1.0);
	p->direction	= initialDirection;
	p->direction.x += ((RAND()-RAND()) * particleVariance);
	p->direction.y += ((RAND()-RAND()) * particleVariance);
	p->direction.y += (-1.0f * particleVariance);
	p->direction.z += ((RAND()-RAND()) * particleVariance);

	p->direction.Normalise();	//Keep its direction normalised!
	p->position.ToZero();

	return p;	//return the new particle :-)
}