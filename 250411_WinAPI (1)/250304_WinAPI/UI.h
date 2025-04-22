#pragma once
#include "GameObject.h"

enum class UI_Status
{
	PLAYER_UI,
	GAMEOVER_UI,
};

class Image;
class UI : public GameObject
{
private:

protected:
	FPOINT imagePos;
	FPOINT imageRatio;
	float x_pos_divide_factor;
	bool isInteract;
	float defaultOpacity;
	float opacityDuration;
	float opacityTimer;
	bool isOpaque;
	bool requestOpaqueChange;
public:
	virtual HRESULT Init() override;
	virtual void Release() override;
	virtual void Update(float TimeDelta) override;
	virtual void Render(ID2D1RenderTarget* renderTarget) override;

	void SetOpaque();
	void DisableOpaque();
	inline void RequestOpaqueChange() { requestOpaqueChange = true; };

	FPOINT ResolutionRatio();

	UI();
	virtual ~UI() {};
};

