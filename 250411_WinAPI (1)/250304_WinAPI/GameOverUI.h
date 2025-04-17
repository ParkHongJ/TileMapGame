#pragma once
#include "Scene.h"

class JinScene;
class Image;
class GameOverUI : public Scene
{
private:
	JinScene* tempChar;
	Image* GameOver_journalRearImage;
	Image* GameOver_journalFrontImage;
	Image* GameOver_journalSelectBox;
	Image* GameOver_characterSticker;

	FPOINT GameOver_journalRearImagePos;
	FPOINT GameOver_journalFrontImagePos;
	FPOINT GameOver_journalSelectBoxPos;
	FPOINT GameOver_characterStickerPos;

	FPOINT selectBoxPos;
	FPOINT stickerPos;

	FPOINT imageRatio;

	int playTime;
	int currency;
	int stageIndex_Outer;
	int stageIndex_Inner;
	bool isGameOver;
	int selectBoxIndex;
	float selectBoxSpeed;

public:
	virtual HRESULT Init(ID2D1HwndRenderTarget* renderTarget) override;
	virtual void Release() override;
	virtual void Update(float TimeDelta) override;
	virtual void Render(ID2D1HwndRenderTarget* renderTarget) override;

	inline void SetDrawGameOverUI(bool setValue) { isGameOver = setValue; };
	FPOINT ResolutionRatio(Image image);

};

