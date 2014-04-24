#include "BirdEmitter.h"


#define RAND() ((rand()%101)/100.0f)

void BirdEmitter::Update(float msec) {
	nextParticleTime -= msec;	//some time has passed!

	/*
	Enough time might have passed that more than 1 'launch' of particles is necessary...
	*/
	while(nextParticleTime <= 0) {
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
		p->colour.w -= (msec / particleLifetime);

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
				p->direction = Vector3((3*RAND() - 1.5f) * 4,3 * RAND(),(3*RAND() - 1.5f) * 4);

			} else if (p->colour.w <0.1) {
				p->direction = Vector3(2*RAND() - 1,-2,2*RAND() - 1);
			} else {
				p->direction = Vector3(2*RAND() - 1, 0.05f - RAND() / 10, 2*RAND() - 1);
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
