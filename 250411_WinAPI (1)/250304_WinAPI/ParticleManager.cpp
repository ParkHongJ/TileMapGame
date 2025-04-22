#include "pch.h"
#include "ParticleManager.h"
#include "Particle.h"

void ParticleManager::Init()
{
}

void ParticleManager::Update(float TimeDelta)
{
	// 수명이 끝난 파티클은 풀로
	for (auto iter = activeParticles.begin(); iter != activeParticles.end();)
	{
		(*iter)->Update(TimeDelta);

		if ((*iter)->IsEnd())
		{
			particlePool.push_back(*iter);
			(particlePool.back())->Release();
			iter = activeParticles.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}

void ParticleManager::Render(ID2D1RenderTarget* rt)
{
	for (auto& particle : activeParticles)
	{
		particle->Render(rt);
	}
}

Particle* ParticleManager::GetParticle(string imageStr, FPOINT pos, float angle, float size, float lifeTime, int atlasX, int atlasY)
{
	if (particlePool.empty())
	{
		Particle* particle = new Particle;
		particle->Init(imageStr, pos, angle, size, lifeTime, atlasX, atlasY);
		activeParticles.push_back(particle);

		return particle;
	}
	else
	{
		Particle* particle = particlePool.back();
		particlePool.pop_back();

		particle->Init(imageStr, pos, angle, size, lifeTime, atlasX, atlasY);
		activeParticles.push_back(particle);

		return particle;
	}
}

void ParticleManager::Release()
{
	activeParticles.clear();
	particlePool.clear();
}
