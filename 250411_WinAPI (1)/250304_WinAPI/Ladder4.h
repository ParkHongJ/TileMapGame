#pragma once
#include "Ladder.h"
class Ladder4 : public Ladder
{
public:
	Ladder4();
	virtual ~Ladder4();

public:
	virtual HRESULT Init() override;
	virtual void Update(float TimeDelta) override;
	virtual void Render(ID2D1HwndRenderTarget* renderTarget) override;
	virtual void Release() override;
};

