
#include "Tile.h"
#include "Image.h"
HRESULT Tile::Init()
{
	tileImage = ImageManager::GetInstance()->FindImage("CaveTile");
	return S_OK;
}

void Tile::Release()
{
}

void Tile::Update(float TimeDelta)
{
}

void Tile::Render(ID2D1HwndRenderTarget* renderTarget)
{
	if (tileInfo.valid)
	{
		tileImage->Render(renderTarget, Pos.x, Pos.y, 1.f,1.f,0.5f,0.5f, tileInfo.atlasX, tileInfo.atlasY, GAME_TILE_SIZE, GAME_TILE_SIZE);
	}
}

void Tile::InitTile(int atlasX, int atlasY, bool valid, FPOINT pos)
{
	tileInfo = { atlasX, atlasY, valid };
	this->Pos = pos;
}
