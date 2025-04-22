#pragma once
#include "Ladder.h"
class Ladder3 : public Ladder
{
	GENERATE_BODY(Textures/CaveTile.png, 128, 128)

public:
	Ladder3();
	virtual ~Ladder3();

public:
	virtual HRESULT Init() override;
	virtual void Update(float TimeDelta) override;
	virtual void Render(ID2D1RenderTarget* renderTarget) override;
	virtual void Release() override;
};

