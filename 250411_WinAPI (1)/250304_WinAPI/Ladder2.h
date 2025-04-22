#pragma once
#include "Ladder.h"
class Ladder2 : public Ladder
{
public:
	Ladder2();
	virtual ~Ladder2();

public:
	virtual HRESULT Init() override;
	virtual void Update(float TimeDelta) override;
	virtual void Render(ID2D1HwndRenderTarget* renderTarget) override;
	virtual void Release() override;
};

