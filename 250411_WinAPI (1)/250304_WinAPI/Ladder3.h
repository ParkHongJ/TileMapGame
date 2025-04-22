#pragma once
#include "Ladder.h"
class Ladder3 : public Ladder
{
public:
	Ladder3();
	virtual ~Ladder3();

public:
	virtual HRESULT Init() override;
	virtual void Update(float TimeDelta) override;
	virtual void Render(ID2D1HwndRenderTarget* renderTarget) override;
	virtual void Release() override;
};

