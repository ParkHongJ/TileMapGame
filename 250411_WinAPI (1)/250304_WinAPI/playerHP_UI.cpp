#include "pch.h"
#include "playerHP_UI.h"
#include "Image.h"
#include "PlayerStatus.h"

HRESULT playerHP_UI::Init()
{
	playerLifeImage = ImageManager::GetInstance()->FindImage("playerLife");
	playerLifeRunsOutImage = ImageManager::GetInstance()->FindImage("playerLifeRunsOut");

	playerStat = new PlayerStatus();
	playerHP_value = playerStat->GetPlayerMaxHP();
	//float x_pos_divide_factor = 25.0f;
	Pos = { WINSIZE_X * (1.0f / x_pos_divide_factor), WINSIZE_Y * (1.0f / 12.0f) };

	isAlive = true;

	//defaultOpacity = 0.5f;		//�⺻ ������
	//opacityDuraion = 3.0f;		//3�ʰ� �Ƿ�����
	//opacityTimer = 0.0f;		//3�� ī��Ʈ�� ����
	//isOpaque = false;			//�⺻������ ������

	return S_OK;
}

void playerHP_UI::Update(float TimeDelta)
{
	UI::Update(TimeDelta);

	playerHP_value = playerStat->GetPlayerHP();
	if (playerHP_value == 0)
		isAlive = false;
	
	//if(playerStat->PicktheHP || LostTheHP)
	if (KeyManager::GetInstance()->IsOnceKeyDown('H'))
		RequestOpaqueChange();
	
}

void playerHP_UI::Render(ID2D1RenderTarget* renderTarget)
{
	if (playerLifeImage)
	{
		playerLifeImage->Render(renderTarget, Pos.x, Pos.y, 1.0f, 1.0f, defaultOpacity);
		if (!isAlive)				//����� ��� �׳� �Ƿ�����
		{
			playerLifeImage->Render(renderTarget, Pos.x, Pos.y, 1.0f, 1.0f, 1.0f);
			playerLifeRunsOutImage->Render(renderTarget, Pos.x, Pos.y, 1.0f, 1.0f, 1.0f);
		}
	}
}

void playerHP_UI::Release()
{
}

//void playerHP_UI::SetOpaque()
//{
//	isOpaque = true;
//	defaultOpacity = 1.0f;
//	opacityTimer = 0.0f;
//}
//
//void playerHP_UI::DisableOpaque()
//{
//	isOpaque = false;
//	defaultOpacity = 0.5f;
//	opacityTimer = 0.0f;
//}