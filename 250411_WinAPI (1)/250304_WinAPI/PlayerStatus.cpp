#include "pch.h"
#include "PlayerStatus.h"
#include "CameraManager.h"

PlayerStatus::PlayerStatus()
{
	memset(&info, 0, sizeof(info));
	info.playerMaxHP = 4;
	info.playerHP = 2;
	info.bombCount = 3;
	info.ropeCount = 5;
	info.deadTime = 3.f;
}

PlayerStatus::~PlayerStatus()
{

}

void PlayerStatus::Update(float TimeDelta)
{
	if (0.f >= info.playerHP)
	{
		info.deadTime -= TimeDelta;
		if (0.f >= info.deadTime)
		{
			info.IsEnd = true;
			CameraManager::GetInstance()->SetDeadCam();
		}
	}

	else
	{
		info.GameTime += TimeDelta;
	}
}

void PlayerStatus::SetGold(unsigned int gold)
{
	info.sumGold += max(0, gold - info.gold);
	info.gold = gold;
}

void PlayerStatus::PlusGold(unsigned int gold)
{
	info.sumGold += gold;
	info.gold += gold;
}
