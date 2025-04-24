#include "pch.h"
#include "GameOverUI.h"
#include "Image.h"
#include "PlayerStatus.h"
#include "ObjectManager.h"
#include "Character.h"
#include "ImageManager.h"
#include <iomanip>
#include <sstream>

GameOverUI::GameOverUI(ID2D1RenderTarget* renderTarget)
{
	if (renderTarget)
	{
		InitTextRenderer(renderTarget, L"Consolas", 20.0f, D2D1::ColorF(D2D1::ColorF::White));
	}
}


HRESULT GameOverUI::Init()
{
	blackBackgroundImage = ImageManager::GetInstance()->FindImage("blackBackground");

	GameOver_journalRearImage = ImageManager::GetInstance()->FindImage("GameOver_journalRear");

	GameOver_journalFrontImage = ImageManager::GetInstance()->FindImage("GameOver_journalFront");

	GameOver_journalSelectBox = ImageManager::GetInstance()->FindImage("GameOver_journalSelectBox");

	GameOver_characterSticker = ImageManager::GetInstance()->FindImage("GameOver_characterSticker");

	GameOver_journalBackImage = ImageManager::GetInstance()->FindImage("GameOver_journalBack");

	GameOver_journalBackClipImage = ImageManager::GetInstance()->FindImage("GameOver_journalClip");

	GameOver_journalRearImagePos = { WINSIZE_X / 2, WINSIZE_Y / 2 };
	GameOver_journalFrontImagePos = { GameOver_journalRearImagePos.x, GameOver_journalRearImagePos.y };
	GameOver_journalSelectBoxPos = { WINSIZE_X * 0.7f, WINSIZE_Y * 0.55f };
	//GameOver_characterStickerPos = { WINSIZE_X * 0.571f, WINSIZE_Y * 0.55f };
	GameOver_characterStickerPos = { GameOver_journalSelectBoxPos.x - (GameOver_journalSelectBoxPos.x * 0.185f),
		GameOver_journalSelectBoxPos.y };
	GameOver_journalBackImagePos = { WINSIZE_X / 2, WINSIZE_Y / 2 };
	GameOver_journalBackClipImagePos = { GameOver_journalBackImagePos.x - (GameOver_journalBackImagePos.x * 0.39f),
		GameOver_journalBackImagePos.y - (GameOver_journalBackImagePos.y * 0.1f)};

	playTimeSec = 0;
	playTimeMin = 0;

	stageIndex_Outer = 0;
	stageIndex_Inner = 0;
	isGameOver = false;
	isBackPage = false;
	selectBoxIndex = 1;
	selectBoxMovefloat = 45.0f;

	blackBackgroundImagePos = { WINSIZE_X / 2.0f, WINSIZE_Y / 2.0f };

	text_gameoverPos = { GameOver_journalFrontImagePos.x + 150.0f,
							   GameOver_journalFrontImagePos.y - 195.0f };

	text_levelPos = {text_gameoverPos.x - 17, text_gameoverPos.y + 50};

	text_moneyPos = {text_gameoverPos.x - 35, text_levelPos.y + 37};

	text_timePos = { text_gameoverPos.x - 16, text_moneyPos.y + 36 };

	int_levelPos = { text_levelPos.x + 140, text_levelPos.y + 5};

	int_moneyPos = { text_moneyPos.x + 140, text_moneyPos.y + 5};

	int_timePos = { text_timePos.x + 140, text_timePos.y };

	quickRestartPos = { 670, 260};

	characterselectPos = {quickRestartPos.x, quickRestartPos.y + 45};

	returntocampPos = {quickRestartPos.x, characterselectPos.y + 45};

	exittotitlePos = {quickRestartPos.x, returntocampPos.y + 45};

	imageRatio = ResolutionRatio(GameOver_journalRearImage);

	return S_OK;
}

void GameOverUI::Release()
{

}

void GameOverUI::Update(float TimeDelta)	
{
	if(!isGameOver)
	{
		gold = ObjectManager::GetInstance()->GetPlayer()->GetPlayerStatus()->GetGold();
		playTimeMin = ObjectManager::GetInstance()->GetPlayer()->GetPlayerStatus()->GetGameTime() / 60;
		playTimeSec = int(ObjectManager::GetInstance()->GetPlayer()->GetPlayerStatus()->GetGameTime()) % 60;
	}
		
		//playTimeSec = ObjectManager::GetInstance()->GetPlayer()->GetPlayerStatus()->GetGameTime();
		//if (playTimeSec >= 60)
		//{

		//	//playTimeMin++;
		//}

	if (KeyManager::GetInstance()->IsOnceKeyDown('P'))
		isGameOver = true;
	if(isGameOver)
	{
		if (KeyManager::GetInstance()->IsOnceKeyDown(VK_RIGHT))
			isBackPage = true;
		else if (KeyManager::GetInstance()->IsOnceKeyDown(VK_LEFT))
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
}

void GameOverUI::Render(ID2D1RenderTarget* renderTarget)
{
	//	뒷배경을 검정색으로 칠해야함
	if (isGameOver)
	{
		blackBackgroundImage->Render(renderTarget, blackBackgroundImagePos.x, blackBackgroundImagePos.y);

		if(!isBackPage)
		{
			if (GameOver_journalRearImage)
			{
				GameOver_journalRearImage->Render(
					renderTarget,
					GameOver_journalRearImagePos.x,
					GameOver_journalRearImagePos.y,
					imageRatio.x, imageRatio.y
				);
			}
	
			if (GameOver_journalFrontImage)
			{
				GameOver_journalFrontImage->Render(
					renderTarget,
					GameOver_journalFrontImagePos.x,
					GameOver_journalFrontImagePos.y,
					imageRatio.x, imageRatio.y
				);
				}
	
			if (GameOver_journalSelectBox)
			{
				GameOver_journalSelectBox->Render(
					renderTarget,
					GameOver_journalSelectBoxPos.x,
					GameOver_journalSelectBoxPos.y,
					imageRatio.x, imageRatio.y
				);
			}
	
			if (GameOver_characterSticker)
			{
				GameOver_characterSticker->Render(
					renderTarget,
					GameOver_journalSelectBoxPos.x - (GameOver_journalSelectBoxPos.x * 0.185f),
					GameOver_journalSelectBoxPos.y,
					imageRatio.x, imageRatio.y
				);
			}
				RenderText(
					renderTarget,
					L"Game Over",
					text_gameoverPos.x,
					text_gameoverPos.y,
					1.0f
				);
				RenderText(
					renderTarget,
					L"Level:",
					text_levelPos.x,
					text_levelPos.y,
					1.0f, D2D1::ColorF::Black
				);
				RenderText(
					renderTarget,
					L"1-1",
					int_levelPos.x,
					int_levelPos.y,
					1.0f
				);
				RenderText(
					renderTarget,
					L"Money:",
					text_moneyPos.x,
					text_moneyPos.y,
					1.0f, D2D1::ColorF::Black
				);
				RenderText(
					renderTarget,
					L"Time:",
					text_timePos.x,
					text_timePos.y,
					1.0f, D2D1::ColorF::Black
				);
				std::wstring goldPrint = std::to_wstring(gold);
				RenderText(
					renderTarget,
					goldPrint,
					int_moneyPos.x,
					int_moneyPos.y,
					1.0f
				);
				/*std::wstring timePrint = std::to_wstring(playTime);
				RenderText(
					renderTarget,
					timePrint,
					int_timePos.x,
					int_timePos.y,
					1.0f
				);*/
				std::wstringstream timeStream;
				timeStream << std::setw(2) << std::setfill(L'0') << playTimeMin << L":"
					<< std::setw(2) << std::setfill(L'0') << static_cast<int>(playTimeSec);
				std::wstring timeText = timeStream.str();
				RenderText(renderTarget,
					timeText,
					int_timePos.x,
					int_timePos.y,
					1.0f, D2D1::ColorF::White
				);

				RenderText(
					renderTarget,
					L"Quick Restart",
					quickRestartPos.x,
					quickRestartPos.y,
					1.0f, D2D1::ColorF::Black
				);
				RenderText(
					renderTarget,
					L"Character Select",
					characterselectPos.x,
					characterselectPos.y,
					1.0f, D2D1::ColorF::Black
				);
				RenderText(
					renderTarget,
					L"Return To Camp",
					returntocampPos.x,
					returntocampPos.y,
					1.0f, D2D1::ColorF::Black
				);
				RenderText(
					renderTarget,
					L"Exit To Title",
					exittotitlePos.x,
					exittotitlePos.y,
					1.0f, D2D1::ColorF::Black
				);
			}
		if(isBackPage)
		{
			if (GameOver_journalBackImage)
			{
				{
					GameOver_journalBackImage->Render(renderTarget,
						GameOver_journalBackImagePos.x,
						GameOver_journalBackImagePos.y,
						imageRatio.x, imageRatio.y
					);
				}
			}
	
			if (GameOver_journalBackClipImage)
			{
				GameOver_journalBackClipImage->Render(renderTarget,
					GameOver_journalBackClipImagePos.x,
					GameOver_journalBackClipImagePos.y,
					imageRatio.x, imageRatio.y
				);
			}
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