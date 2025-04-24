#include "pch.h"
#include "PlayerStatus.h"
#include "CameraManager.h"

PlayerStatus::PlayerStatus()
{
	memset(&info, 0, sizeof(info));
	info.playerMaxHP = 4;
	info.playerHP = 4;
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
			CameraManager::GetInstance()->SetDeadCam();
		}
	}
}
