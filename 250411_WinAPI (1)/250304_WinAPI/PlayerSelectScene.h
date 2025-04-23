#pragma once
#include "Scene.h"
#include "StartScene.h"
#include <dwrite.h>
#pragma comment(lib, "dwrite.lib")

class Image;


class PlayerSelectScene : public Scene
{
private:
	Image* blackbg;
	Image* charMenu;
	Image* charMenuTop;
	Image* charMenuTopBack;
	Image* charMenuBottom;
	Image* rope;
	Image* buttonZ;
	Image* buttonESC;
	Image* charDoor;
	Image* charDoorBack;
	Image* charDoorBack2;
	float doorY;


	Image* torchEffectBack;
	Image* torchEffectFront;
	Image* fumeEffect;
	Image* selectButtonLeft;
	Image* selectButtonRight;


	Image* displayCharacter;
	FPOINT displayCharacterPos;
	POINT currCharInd;

	FPOINT selectButtonLeftPos;
	FPOINT selectButtonRightPos;


	FPOINT flamePos;
	std::vector<std::string> characterKeys;

	float charSelectBuffTime;
	float currSelectBuffTime;

	
	float selectButtonOscillateTime = 0.0f;             // �ð� ������
	float selectButtonOscillateAmplitude = 2.0f;
	float selectButtonLeftOffset = 0.0f;
	float selectButtonRightOffset = 0.0f;
	float selectButtonInputDuration = 0.15f;
	float selectButtonInputTimerL = 0.0f;
	float selectButtonInputTimerR = 0.0f;
	float selectButtonInputAmplitude = 10.0f;

	bool isSwitchingCharacter = false;
	bool switchToLeft = false;
	float runAnimTimer = 0.0f;
	float runFrameTime = 0.08f;
	float switchRunSpeed = 200.0f; // �ʴ� �̵� �ȼ�

	enum class SwitchPhase { None, RunOut, RunIn };
	SwitchPhase switchPhase = SwitchPhase::None;

	int switchDirection = 0; // -1: ����, 1: ������
	bool newCharCome;

	bool currFilp;
	bool isDoorOpening = false;
	float doorStartY = 180.0f;
	float doorTargetY = 0.0f;
	float doorMoveDuration = 1.0f; // 1�� ���� �̵�
	float doorMoveTimer = 0.0f;

	uint8_t selectedNum;

	FPOINT charMenuPos;

	float sceneOpacity;


	int flameFrameIndex;

	float fumeSpawnTimer = 0.0f;
	const float FUME_SPAWN_INTERVAL = 0.2f; // ���� ���� ����


	bool isFadingOut = false;
	float fadeTimer = 0.0f;            // ������� ��� �ð�
	const float fadeDuration = 1.0f;   // ���̵� �� �ð� (1��)



	FPOINT screencharMenuScale;
	FPOINT screenTopBottomScale;


	std::vector<TorchParticle> torchParticles2;
	std::vector<FumeParticle> fumes2;
	float globalTime2 = 0.0f;

	int flameFrameIndex2;
	
	float fumeSpawnTimer2 = 0.0f;
	const float FUME_SPAWN_INTERVAL2 = 0.2f; // ���� ���� ����

	float opacity2;

	bool isFadingOut2 = false;
	float fadeTimer2 = 0.0f;            // ������� ��� �ð�
	const float fadeDuration2 = 1.0f;   // ���̵� �� �ð� (1��)




	// ��� ����
	IDWriteFactory* pDWriteFactory = nullptr;
	IDWriteTextFormat* pTextFormat = nullptr;
	ID2D1SolidColorBrush* pWhiteBrush = nullptr;

public:
	virtual HRESULT Init(ID2D1HwndRenderTarget* renderTarget) override;
	virtual void Release() override;
	virtual void Update(float TimeDelta) override;
	virtual void LateUpdate(float TimeDelta) override;
	virtual void Render(ID2D1HwndRenderTarget* renderTarget) override;

	PlayerSelectScene() {};
	virtual ~PlayerSelectScene() {};

	
};

