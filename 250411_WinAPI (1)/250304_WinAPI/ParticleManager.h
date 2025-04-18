#pragma once
#include "Singleton.h"

class Particle;

class ParticleManager : public Singleton<ParticleManager>
{
public:
	void Init();
	void Update(float TimeDelta);
	void Render(ID2D1HwndRenderTarget* rt);
	void GetParticle(string imageStr, FPOINT pos, float angle, float size, float lifeTime);
	void Release();

	ParticleManager() {};
	~ParticleManager() = default;
private:
	list<Particle*> activeParticles;
	list<Particle*> particlePool;
};

