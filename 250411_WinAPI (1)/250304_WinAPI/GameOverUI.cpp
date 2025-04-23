#include "pch.h"
#include "GameOverUI.h"
#include "Image.h"

GameOverUI::GameOverUI()
{
	GameOver_journalRearImage = ImageManager::GetInstance()->FindImage("GameOver_journalRear");

	GameOver_journalFrontImage = ImageManager::GetInstance()->FindImage("GameOver_journalFront");

	GameOver_journalSelectBox = ImageManager::GetInstance()->FindImage("GameOver_journalSelectBox");

	GameOver_characterSticker = ImageManager::GetInstance()->FindImage("GameOver_characterSticker");

	GameOver_journalBackImage = ImageManager::GetInstance()->FindImage("GameOver_journalBack");

	GameOver_journalBackClipImage = ImageManager::GetInstance()->FindImage("GameOver_journalClip");
}


HRESULT GameOverUI::Init()
{

	GameOver_journalRearImagePos = { WINSIZE_X / 2, WINSIZE_Y / 2 };
	GameOver_journalFrontImagePos = { GameOver_journalRearImagePos.x, GameOver_journalRearImagePos.y };
	GameOver_journalSelectBoxPos = { WINSIZE_X * 0.7f, WINSIZE_Y * 0.55f };
	//GameOver_characterStickerPos = { WINSIZE_X * 0.571f, WINSIZE_Y * 0.55f };
	GameOver_characterStickerPos = { GameOver_journalSelectBoxPos.x - (GameOver_journalSelectBoxPos.x * 0.185f),
		GameOver_journalSelectBoxPos.y };
	GameOver_journalBackImagePos = { WINSIZE_X / 2, WINSIZE_Y / 2 };
	GameOver_journalBackClipImagePos = { GameOver_journalBackImagePos.x - (GameOver_journalBackImagePos.x * 0.39f),
		GameOver_journalBackImagePos.y - (GameOver_journalBackImagePos.y * 0.1f)};

	playTime = 0;
	currency = 0;
	stageIndex_Outer = 0;
	stageIndex_Inner = 0;
	isGameOver = true;
	isBackPage = false;
	selectBoxIndex = 1;
	selectBoxMovefloat = 45.0f;

	imageRatio = ResolutionRatio(GameOver_journalRearImage);

	return S_OK;
}

void GameOverUI::Release()
{

}

void GameOverUI::Update(float TimeDelta)
{
	if (KeyManager::GetInstance()->IsOnceKeyDown('C'))
		isBackPage = true;
	else if (KeyManager::GetInstance()->IsOnceKeyDown('X'))
		isBackPage = false;

	if (KeyManager::GetInstance()->IsOnceKeyDown(VK_UP))
	{
		if (selectBoxIndex <= 1)
		{
			selectBoxIndex = 4;
			MoveBox(GameOver_journalSelectBoxPos.y + selectBoxMovefloat * 3);
			return;
		}
		MoveBox(GameOver_journalSelectBoxPos.y - selectBoxMovefloat);
		selectBoxIndex--;
	}
	if (KeyManager::GetInstance()->IsOnceKeyDown(VK_DOWN))
	{
		if (selectBoxIndex >= 4)
		{
			selectBoxIndex = 1;
			MoveBox(GameOver_journalSelectBoxPos.y - selectBoxMovefloat * 3);
			return;
		}
		MoveBox(GameOver_journalSelectBoxPos.y + selectBoxMovefloat);
		selectBoxIndex++;
	}
}

void GameOverUI::Render(ID2D1RenderTarget* renderTarget)
{
	//	뒷배경을 검정색으로 칠해야함
	if (isGameOver)
	{
		if (GameOver_journalRearImage && !isBackPage)
			GameOver_journalRearImage->Render(
				renderTarget,
				GameOver_journalRearImagePos.x,
				GameOver_journalRearImagePos.y,
				imageRatio.x, imageRatio.y
			);
		if (GameOver_journalFrontImage && !isBackPage)
			GameOver_journalFrontImage->Render(
				renderTarget,
				GameOver_journalFrontImagePos.x,
				GameOver_journalFrontImagePos.y,
				imageRatio.x, imageRatio.y
			);
		if (GameOver_journalSelectBox && !isBackPage)
			GameOver_journalSelectBox->Render(
				renderTarget,
				GameOver_journalSelectBoxPos.x,
				GameOver_journalSelectBoxPos.y,
				imageRatio.x, imageRatio.y
			);
		if (GameOver_characterSticker && !isBackPage)
			GameOver_characterSticker->Render(
				renderTarget,
				GameOver_journalSelectBoxPos.x - (GameOver_journalSelectBoxPos.x * 0.185f),
				GameOver_journalSelectBoxPos.y,
				imageRatio.x, imageRatio.y
			);
		if (GameOver_journalBackImage && isBackPage)
		{
			GameOver_journalBackImage->Render(renderTarget,
				GameOver_journalBackImagePos.x,
				GameOver_journalBackImagePos.y,
				imageRatio.x, imageRatio.y
			);
		}
		if (GameOver_journalBackClipImage && isBackPage)
		{
			GameOver_journalBackClipImage->Render(renderTarget,
				GameOver_journalBackClipImagePos.x,
				GameOver_journalBackClipImagePos.y,
				imageRatio.x, imageRatio.y
			);
		}
	}
}

FPOINT GameOverUI::ResolutionRatio(Image* image)
{
	FPOINT ratio = { 0.0f, 0.0f };
	ratio.x = WINSIZE_X / static_cast<float>(image->GetWidth());
	ratio.y = WINSIZE_Y / static_cast<float>(image->GetHeight());

	return ratio;
}

void GameOverUI::MoveBox(float destY)
{
	GameOver_journalSelectBoxPos.y = destY;
}