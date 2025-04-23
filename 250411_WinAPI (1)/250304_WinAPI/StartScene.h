#pragma once
#include "Scene.h"
#include <dwrite.h>
#pragma comment(lib, "dwrite.lib")


struct TorchParticle {
	D2D1_POINT_2F pos;
	D2D1_POINT_2F velocity;
	float initialScale;
	float scale;
	float opacity;
	float lifetime;
	float age;
	int frameIndex;
	float waveOffset;
};

struct FumeParticle {
	D2D1_POINT_2F pos;
	D2D1_POINT_2F velocity;
	float scale;
	float opacity;
	float lifetime;
	float age;
	float waveOffset;
};


class Image;
class StartScene : public Scene
{
private:
	Image* menu_title;
	Image* menu_title_char;
	Image* blackbg;

	Image* torchEffectBack;
	Image* torchEffectFront;
	Image* fumeEffect;

	Image* enter;


	FPOINT menuTitlePos;
	FPOINT menuTitleCharPos;
	FPOINT flamePos;


	FPOINT screenTitleScale;
	FPOINT screenCharScale;



	// StartScene Ŭ���� ���� �����
	std::vector<TorchParticle> torchParticles;
	std::vector<FumeParticle> fumes;
	float globalTime = 0.0f;

	int flameFrameIndex;

	float fumeSpawnTimer = 0.0f;
	const float FUME_SPAWN_INTERVAL = 0.2f; // ���� ���� ����


	float opacity;

	bool isFadingOut = false;
	float fadeTimer = 0.0f;            // ������� ��� �ð�
	const float fadeDuration = 1.0f;   // ���̵� �� �ð� (1��)


	// ��� ����
	IDWriteFactory* pDWriteFactory = nullptr;
	IDWriteTextFormat* pTextFormat = nullptr;
	ID2D1SolidColorBrush* pWhiteBrush = nullptr;


public:
	virtual HRESULT Init(ID2D1RenderTarget* renderTarget) override;
	virtual void Release() override;
	virtual void Update(float TimeDelta) override;
	virtual void LateUpdate(float TimeDelta) override;
	virtual void Render(ID2D1RenderTarget* renderTarget) override;
	bool returnIsSceneEnd();
	StartScene() {};
	virtual ~StartScene() {};


};

