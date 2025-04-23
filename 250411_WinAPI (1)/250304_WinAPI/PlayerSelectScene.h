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


	FPOINT flamePos;


	bool isDoorOpening = false;
	float doorStartY = 180.0f;
	float doorTargetY = 0.0f;
	float doorMoveDuration = 1.0f; // 1�� ���� �̵�
	float doorMoveTimer = 0.0f;



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

