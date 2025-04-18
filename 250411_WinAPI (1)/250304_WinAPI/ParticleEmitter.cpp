#include "pch.h"
#include "ParticleEmitter.h"
#include "ParticleManager.h"
#include "Particle.h"

void ParticleEmitter::Emit(FPOINT pos, float moveSpeed, float angle, float size, float lifeTime, int particleCount)
{
	for (int i = 0; i < particleCount; i++)
	{
		auto tempAngle = angle + (rand() % 120) - 60;

		int speedRatio{ max((int)moveSpeed / 2, 1) };
		auto tempMoveSpeed = moveSpeed + (rand() % speedRatio) - speedRatio/2;

		int sizeRatio{ max((int)size / 2, 1) };
		auto tempSize = size + (rand() % sizeRatio) - sizeRatio / 2;
		auto tempLifeTime = lifeTime;

		ParticleManager::GetInstance()->GetParticle(pos, tempMoveSpeed, tempAngle, tempSize, tempLifeTime);
	}
}

void ParticleEmitter::Emit(FPOINT pos, float moveSpeed, float size, float lifeTime, int particleCount)
{
	for (int i = 0; i < particleCount; i++)
	{
		auto tempAngle = (rand() % 360);

		int speedRatio{ max((int)moveSpeed / 2, 1) };
		auto tempMoveSpeed = moveSpeed + (rand() % speedRatio) - speedRatio / 2;

		int sizeRatio{ max((int)size / 2, 1) };
		auto tempSize = size + (rand() % sizeRatio) - sizeRatio / 2;

		auto tempLifeTime = lifeTime;

		ParticleManager::GetInstance()->GetParticle(pos, tempMoveSpeed, tempAngle, tempSize, tempLifeTime);
	}
}

