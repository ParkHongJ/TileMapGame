#include "GameOverUI.h"
#include "Image.h"
#include "JinScene.h"

HRESULT GameOverUI::Init(ID2D1HwndRenderTarget* renderTarget)
{
	tempChar = new JinScene();
	tempChar->Init(renderTarget);

	ImageManager::GetInstance()->AddImage("GameOver_journalRear", L"Textures/UI/Journal/journal_back.png", renderTarget);
	GameOver_journalRearImage = ImageManager::GetInstance()->FindImage("GameOver_journalRear");
	
	ImageManager::GetInstance()->AddImage("GameOver_journalFront", L"Textures/UI/Journal/journal_top_gameover.png", renderTarget);
	GameOver_journalFrontImage = ImageManager::GetInstance()->FindImage("GameOver_journalFront");

	ImageManager::GetInstance()->AddImage("GameOver_journalSelectBox", L"Textures/UI/Journal/journal_select.png", renderTarget);
	GameOver_journalSelectBox = ImageManager::GetInstance()->FindImage("GameOver_journalSelectBox");

	ImageManager::GetInstance()->AddImage("GameOver_characterSticker", L"Textures/UI/Journal/journal_stickers/yellow.png", renderTarget);
	GameOver_characterSticker = ImageManager::GetInstance()->FindImage("GameOver_characterSticker");

	GameOver_journalRearImagePos = { WINSIZE_X / 2, WINSIZE_Y / 2 };
	GameOver_journalFrontImagePos = { GameOver_journalRearImagePos.x, GameOver_journalRearImagePos.y };
	GameOver_journalSelectBoxPos = { WINSIZE_X * 0.7f, WINSIZE_Y * 0.55f };
	GameOver_characterStickerPos = { WINSIZE_X * 0.571f, WINSIZE_Y * 0.55f };

	playTime = 0;
	currency = 0;
	stageIndex_Outer = 0;
	stageIndex_Inner = 0;
	isGameOver = true;
	selectBoxIndex = 1;
	selectBoxSpeed = 40.0f;

	imageRatio = ResolutionRatio(*GameOver_journalRearImage);

	return S_OK;
}

void GameOverUI::Release()
{

}

void GameOverUI::Update(float TimeDelta)
{

}

void GameOverUI::Render(ID2D1HwndRenderTarget* renderTarget)
{
	//	뒷배경을 검정색으로 칠해야함
	if (isGameOver)
	{
		if (GameOver_journalRearImage)
			GameOver_journalRearImage->Render(
				renderTarget,
				GameOver_journalRearImagePos.x,
				GameOver_journalRearImagePos.y,
				imageRatio.x, imageRatio.y
			);
		if (GameOver_journalFrontImage)
			GameOver_journalFrontImage->Render(
				renderTarget,
				GameOver_journalFrontImagePos.x,
				GameOver_journalFrontImagePos.y,
				imageRatio.x, imageRatio.y
			);
		if (GameOver_journalSelectBox)
			GameOver_journalSelectBox->Render(
				renderTarget,
				GameOver_journalSelectBoxPos.x,
				GameOver_journalSelectBoxPos.y,
				imageRatio.x, imageRatio.y
			);
		if (GameOver_characterSticker)
			GameOver_characterSticker->Render(
				renderTarget,
				GameOver_characterStickerPos.x,
				GameOver_characterStickerPos.y,
				imageRatio.x, imageRatio.y
			);
	}
}

FPOINT GameOverUI::ResolutionRatio(Image image)
{
	FPOINT ratio = { 0.0f, 0.0f };
	ratio.x = WINSIZE_X / static_cast<float>(image.GetWidth());
	ratio.y = WINSIZE_Y / static_cast<float>(image.GetHeight());

	return ratio;
}
