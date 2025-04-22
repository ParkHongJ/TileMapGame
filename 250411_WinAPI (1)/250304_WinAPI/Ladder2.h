#pragma once
#include "Ladder.h"
class Ladder2 : public Ladder
{
	GENERATE_BODY(Textures/CaveTile.png, 128, 128)

public:
	Ladder2();
	virtual ~Ladder2();

public:
	virtual HRESULT Init() override;
	virtual void Update(float TimeDelta) override;
	virtual void Render(ID2D1RenderTarget* renderTarget) override;
	virtual void Release() override;
};

