#pragma once
class PlayerStatus
{
	struct PlayerStatusInfo
	{
		unsigned int playerHP;
		unsigned int playerMaxHP;
		unsigned int bombCount;
		//unsigned int ropeCount;
		unsigned int gold;
	}typedef PLAYERSTATUSDESC;
public:
	PlayerStatus();
	virtual ~PlayerStatus();

public:
	inline const unsigned int GetPlayerHP() const { return info.playerHP; };
	inline void SetPlayerHP(unsigned int hp) 
	{
		hp < info.playerMaxHP ? info.playerHP = hp : info.playerHP = info.playerMaxHP;
	};
	
	inline void PlusPlayerHP(unsigned int hp) 
	{
		info.playerHP + hp < info.playerMaxHP ? info.playerHP += hp : info.playerHP = info.playerMaxHP;
	};

	inline const unsigned int GetPlayerMaxHP() const { return info.playerMaxHP; };
	inline void SetPlayerMaxHP(unsigned int hp) { info.playerMaxHP = hp; };

	inline const unsigned int GetBombCount() const { return info.bombCount; };
	inline void SetBombCount(unsigned int cnt) { info.bombCount = cnt; };

	//inline const unsigned int GetRopeCount() const { return ropeCount; };
	//inline void SetRopeCount(unsigned int cnt) { ropeCount = cnt; };

	inline const unsigned int GetGold() const { return info.gold; };
	inline void SetGold(unsigned int gold) { info.gold = gold; };

private:
	PLAYERSTATUSDESC info;

};

