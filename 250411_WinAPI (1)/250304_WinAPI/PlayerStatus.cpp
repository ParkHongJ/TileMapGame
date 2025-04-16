#include "PlayerStatus.h"
#include "config.h"

PlayerStatus::PlayerStatus()
{
	memset(&info, 0, sizeof(info));
	info.playerMaxHP = 4;
}

PlayerStatus::~PlayerStatus()
{

}
