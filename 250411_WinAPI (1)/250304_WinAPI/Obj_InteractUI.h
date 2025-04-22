#pragma once
#include "UI.h"

class Image;
class Obj_InterectUI : public UI
{
private:

public:
	virtual HRESULT Init() override;
	virtual void Release() override;
	virtual void Update(float TimeDelta) override;
	virtual void Render(ID2D1RenderTarget* renderTarget) override;
};

