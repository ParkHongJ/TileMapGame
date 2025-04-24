#include "pch.h"
#include "Tile.h"
#include "Image.h"
#include "Collider.h"
#include "GameManager.h"
#include "CameraManager.h"
#include "IncreaseGold.h"
#include "ImageManager.h"

HRESULT Tile::Init()
{
	tileScale = GAME_TILE_SIZE / ATLAS_TILE_SIZE;

	decos.resize((int)DecoDirection::RIGHT);
	objectName = OBJECTNAME::TILE;
	interactState = INTERACTSTATE::INTERACT_UNABLE;
	
	return S_OK;
}

void Tile::Release()
{
	if (collider)
	{
		collider->Release();
		delete collider;
	}

	for (int i = 0; i < decos.size(); i++)
	{
		if (decos[i] != nullptr)
		{
			delete decos[i];
		}
	}
	decos.clear();
}

void Tile::Update(float TimeDelta)
{
}

void Tile::LateUpdate(float TimeDelta)
{
	float scale = GAME_TILE_SIZE / ATLAS_TILE_SIZE;
	float tileDrawSize = ATLAS_TILE_SIZE * scale;

	FPOINT cameraPos = Pos + CameraManager::GetInstance()->GetPos();
	const float margin = 48.f; // 여유 픽셀

	if (cameraPos.x + tileDrawSize < -margin || cameraPos.x > WINSIZE_X + margin ||
		cameraPos.y + tileDrawSize < -margin || cameraPos.y > WINSIZE_Y + margin)
	{
		bHidden = true; // 화면 바깥이면 그리지 않음
	}
	else
	{
		bHidden = false;
	}
}

void Tile::Render(ID2D1RenderTarget* renderTarget)
{
	if (tileInfo.valid)
	{
		//collider->DebugRender(renderTarget);
		FPOINT cameraPos = Pos + CameraManager::GetInstance()->GetPos();
		tileImage->Render(renderTarget, floor(cameraPos.x), floor(cameraPos.y), tileScale, tileScale, tileInfo.atlasX, tileInfo.atlasY, ATLAS_TILE_SIZE, ATLAS_TILE_SIZE);
		if (goldImage)
		{
			goldImage->Render(renderTarget, floor(cameraPos.x), floor(cameraPos.y), tileScale, tileScale, 11, 0, ATLAS_TILE_SIZE, ATLAS_TILE_SIZE);
		}
		RenderDeco(renderTarget);
	}
}

void Tile::RenderDeco(ID2D1RenderTarget* renderTarget)
{
	for (int i = 0; i < decos.size(); ++i)
	{
		if (decos[i] == nullptr)
			continue;


		FPOINT cameraPos = Pos + CameraManager::GetInstance()->GetPos();

		DecoDirection dir = decos[i]->dir;
		
		switch (dir)
		{
		case DecoDirection::TOP:
			decos[i]->decoImage->Render(renderTarget, cameraPos.x, cameraPos.y - GAME_TILE_SIZE / 2.f, tileScale, tileScale, decos[i]->atlasX, decos[i]->atlasY, ATLAS_TILE_SIZE, ATLAS_TILE_SIZE);
			break;
		case DecoDirection::DOWN:
			decos[i]->decoImage->Render(renderTarget, cameraPos.x, cameraPos.y + GAME_TILE_SIZE / 2.f, tileScale, tileScale, decos[i]->atlasX, decos[i]->atlasY, ATLAS_TILE_SIZE, ATLAS_TILE_SIZE);
			break;
		case DecoDirection::LEFT:
			decos[i]->decoImage->Render(renderTarget, cameraPos.x - GAME_TILE_SIZE / 2.f, cameraPos.y, -tileScale, tileScale, decos[i]->atlasX, decos[i]->atlasY, ATLAS_TILE_SIZE, ATLAS_TILE_SIZE);
			break;
		case DecoDirection::RIGHT:
			decos[i]->decoImage->Render(renderTarget, cameraPos.x + GAME_TILE_SIZE / 2.f, cameraPos.y, tileScale, tileScale, decos[i]->atlasX, decos[i]->atlasY, ATLAS_TILE_SIZE, ATLAS_TILE_SIZE);
			break;
		}
	}
}

void Tile::InitTile(int atlasX, int atlasY, bool valid, FPOINT pos, TileType type)
{
	tileInfo = { atlasX, atlasY, valid };
	this->Pos = pos;

	if (type != BORDER && valid)
	{
		collider = new BoxCollider({ 0.f,0.f }, { GAME_TILE_SIZE, GAME_TILE_SIZE }, CollisionMaskType::TILE, this);

		tileImage = ImageManager::GetInstance()->FindImage("CaveTile");


		if (rand() % 10 < 1)
		{
			goldImage = ImageManager::GetInstance()->FindImage("items");
		}
	}
	else
	{
		tileImage = ImageManager::GetInstance()->FindImage("Border");
	}
}

void Tile::CreateDecoTile(DecoDirection dir, bool hasTileAbove)
{
	//나무타일은 데코가 필요없다.
	if (tileInfo.atlasX >= 4)
		return;

	DecoInfo* decoInfo = new DecoInfo;

	int decoIndex = (int)dir - 1;

	if (decos[decoIndex] == nullptr)
	{
		decoInfo = new DecoInfo;

		decoInfo->atlasX = 0;
		decoInfo->atlasY = 0;
		decoInfo->dir = dir;

		decos[decoIndex] = decoInfo;
	}
	else
	{
		decoInfo = decos[decoIndex];
	}

	switch (dir)
	{
	case DecoDirection::TOP:
		decoInfo->decoImage = ImageManager::GetInstance()->FindImage("CaveDecoTop");
		break;
	case DecoDirection::DOWN:
		decoInfo->decoImage = ImageManager::GetInstance()->FindImage("CaveDecoDown");
		break;
	case DecoDirection::LEFT:
		decoInfo->decoImage = ImageManager::GetInstance()->FindImage("CaveDecoRight");

		if (hasTileAbove == true)
			decoInfo->atlasX = 2;
		break;
	case DecoDirection::RIGHT:
		decoInfo->decoImage = ImageManager::GetInstance()->FindImage("CaveDecoRight");

		if (hasTileAbove == true)
			decoInfo->atlasX = 2;
		break;
	}

}

void Tile::Destruction()
{
	if (tileType == TileType::BORDER)
		return;

	GameManager::GetInstance()->DestructionTile(Pos);

	if (goldImage)
	{
		IncreaseGold* goldItem = new IncreaseGold();
		ObjectManager::GetInstance()->AddObject(RENDER_ITEM, goldItem);
		goldItem->SetPos(Pos);
		goldItem->SetDrop(300.f, 90.f, 0.3f, { 0,400.f });
	}

	SetDestroy();
}
