#pragma once
#include "Singleton.h"
#include "config.h"

#define MAX_KEY_COUNT	256
using namespace std;

class KeyManager : public Singleton<KeyManager>
{
private:
	bitset<MAX_KEY_COUNT> keyUp;
	bitset<MAX_KEY_COUNT> keyDown;

public:
	HRESULT Init();
	void Release();

	bool IsOnceKeyDown(int key); // 해당 키가 처음 눌렸는지 판단
	bool IsOnceKeyUp(int key);   // 해당 키가 눌렸다가 처음 떼어졌는지 판단
	bool IsStayKeyDown(int key); // 해당 키가 계속 눌려 있는 상태인지

	void SetKeyDown(int key, bool state) { this->keyDown.set(key, state); }
	void SetKeyUp(int key, bool state) { this->keyUp.set(key, state); }

};

