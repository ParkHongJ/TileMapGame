#pragma once
#include "UI.h"

class Image;
class GameOverUI : public UI
{
private:
	Image* blackBackgroundImage;
	Image* GameOver_journalRearImage;
	Image* GameOver_journalFrontImage;
	Image* GameOver_journalSelectBox;
	Image* GameOver_characterSticker;
	Image* GameOver_journalBackImage;
	Image* GameOver_journalBackClipImage;

	FPOINT GameOver_journalRearImagePos;
	FPOINT GameOver_journalFrontImagePos;
	FPOINT GameOver_journalSelectBoxPos;
	FPOINT GameOver_characterStickerPos;
	FPOINT GameOver_journalBackImagePos;
	FPOINT GameOver_journalBackClipImagePos;

	FPOINT blackBackgroundImagePos;

	FPOINT selectBoxPos;
	FPOINT stickerPos;

	FPOINT imageRatio;

	FPOINT text_gameoverPos;
	FPOINT text_levelPos;
	FPOINT text_moneyPos;
	FPOINT text_timePos;

	FPOINT int_levelPos;
	FPOINT int_moneyPos;
	FPOINT int_timePos;

	FPOINT quickRestartPos;
	FPOINT characterselectPos;
	FPOINT returntocampPos;
	FPOINT exittotitlePos;

	int playTimeSec;
	int playTimeMin;
	int gold;
	int stageIndex_Outer;
	int stageIndex_Inner;
	bool isGameOver;
	bool isBackPage;
	int selectBoxIndex;
	float selectBoxMovefloat;

public:
	virtual HRESULT Init() override;
	virtual void Release() override;
	virtual void Update(float TimeDelta) override;
	virtual void Render(ID2D1RenderTarget* renderTarget) override;

	inline void SetDrawGameOverUI(bool setValue) { isGameOver = setValue; };
	FPOINT ResolutionRatio(Image* image);
	void MoveBox(float destY);

	inline int GetSelectBoxIndex() const { return selectBoxIndex; };

	GameOverUI() {};
	GameOverUI(ID2D1RenderTarget* renderTarget);
	virtual ~GameOverUI() {};
};

