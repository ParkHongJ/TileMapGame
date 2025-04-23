#include "pch.h"
#include "PlayerStatus.h"

PlayerStatus::PlayerStatus()
{
	memset(&info, 0, sizeof(info));
	info.playerMaxHP = 4;
	info.playerHP = 4;
	info.bombCount = 3;
	info.ropeCount = 5;
}

PlayerStatus::~PlayerStatus()
{

}
