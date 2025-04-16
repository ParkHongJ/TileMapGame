
#include "Tile.h"
#include "Image.h"
#include "Collider.h"
HRESULT Tile::Init()
{
	tileImage = ImageManager::GetInstance()->FindImage("CaveTile");

	tileScale = GAME_TILE_SIZE / ATLAS_TILE_SIZE;
	return S_OK;
}

void Tile::Release()
{
	if (collider)
	{
		collider->Release();
		delete collider;
	}
}

void Tile::Update(float TimeDelta)
{
}

void Tile::Render(ID2D1HwndRenderTarget* renderTarget)
{
	if (tileInfo.valid)
	{
		//collider->DebugRender(renderTarget);
		tileImage->Render(renderTarget, Pos.x, Pos.y, tileScale, tileScale, tileInfo.atlasX, tileInfo.atlasY, ATLAS_TILE_SIZE, ATLAS_TILE_SIZE);
		RenderDeco(renderTarget);
	}
}

void Tile::RenderDeco(ID2D1HwndRenderTarget* renderTarget)
{
	for (int i = 0; i < decos.size(); ++i)
	{
		DecoDirection dir = decos[i]->dir;
		
		switch (dir)
		{
		case DecoDirection::TOP:
			decos[i]->decoImage->Render(renderTarget, Pos.x, Pos.y - GAME_TILE_SIZE / 2.f, tileScale, tileScale, decos[i]->atlasX, decos[i]->atlasY, ATLAS_TILE_SIZE, ATLAS_TILE_SIZE);
			break;
		case DecoDirection::DOWN:
			decos[i]->decoImage->Render(renderTarget, Pos.x, Pos.y + GAME_TILE_SIZE / 2.f, tileScale, tileScale, decos[i]->atlasX, decos[i]->atlasY, ATLAS_TILE_SIZE, ATLAS_TILE_SIZE);
			break;
		case DecoDirection::LEFT:
			decos[i]->decoImage->Render(renderTarget, Pos.x - GAME_TILE_SIZE / 2.f, Pos.y, -tileScale, tileScale, decos[i]->atlasX, decos[i]->atlasY, ATLAS_TILE_SIZE, ATLAS_TILE_SIZE);
			break;
		case DecoDirection::RIGHT:
			decos[i]->decoImage->Render(renderTarget, Pos.x + GAME_TILE_SIZE / 2.f, Pos.y, tileScale, tileScale, decos[i]->atlasX, decos[i]->atlasY, ATLAS_TILE_SIZE, ATLAS_TILE_SIZE);
			break;
		}
	}
}

void Tile::InitTile(int atlasX, int atlasY, bool valid, FPOINT pos)
{
	tileInfo = { atlasX, atlasY, valid };
	this->Pos = pos;

	if (valid)
	{
		collider = new BoxCollider({ 0.f,0.f }, { GAME_TILE_SIZE, GAME_TILE_SIZE }, this);
	}	
}

void Tile::CreateDecoTile(DecoDirection dir, bool hasTileAbove)
{
	DecoInfo* decoInfo = new DecoInfo;

	decoInfo->atlasX = 0;
	decoInfo->atlasY = 0;
	decoInfo->dir = dir;

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

	decos.push_back(decoInfo);
}

void Tile::Destruction()
{
	if (tileType == TileType::BORDER)
		return;

	SetActive(false);
}
