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

void ParticleManager::Render(ID2D1HwndRenderTarget* rt)
{
	for (auto& particle : activeParticles)
	{
		particle->Render(rt);
	}
}

void ParticleManager::GetParticle(FPOINT pos, float moveSpeed, float angle, float size, float lifeTime)
{
	if (particlePool.empty())
	{
		Particle* particle = new Particle;
		particle->Init(pos, moveSpeed, angle, size, lifeTime);
		activeParticles.push_back(particle);
	}
	else
	{
		Particle* particle = particlePool.back();
		particlePool.pop_back();

		particle->Init(pos, moveSpeed, angle, size, lifeTime);
		activeParticles.push_back(particle);
	}
}

void ParticleManager::Release()
{
	activeParticles.clear();
	particlePool.clear();
}
