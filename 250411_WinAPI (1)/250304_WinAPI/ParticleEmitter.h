#pragma once
#include "config.h"

class ParticleEmitter
{
public:
	void Emit(FPOINT pos, float moveSpeed, float angle, float size, float lifeTime, int particleCount);
	void Emit(FPOINT pos, float moveSpeed, float size, float lifeTime, int particleCount);
};

