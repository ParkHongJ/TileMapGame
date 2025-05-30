#pragma once
#include "Singleton.h"

class Particle;

class ParticleManager : public Singleton<ParticleManager>
{
public:
	void Init();
	void Update(float TimeDelta);
	void Render(ID2D1RenderTarget* rt);
	Particle* GetParticle(string imageStr, FPOINT pos, float angle, float size, float lifeTime, int atlasX, int atlasY);
	void Release();

	ParticleManager() {};
	~ParticleManager() = default;
private:
	list<Particle*> activeParticles;
	list<Particle*> particlePool;
};

