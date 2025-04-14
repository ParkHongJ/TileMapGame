#pragma once
#include "../250304_WinAPI/config.h"

enum class DecoType {
    None, A, B, C, D
};

struct Deco {
    DecoType type;
    bool flipH = false;
};

struct EditorTile {
    int atlasX = -1; // Atlas �� Ÿ�� ��ǥ
    int atlasY = -1;
    bool valid = false;

    FPOINT pos;
    //std::vector<Deco> decos; // �� Ÿ�Ͽ� �ٴ� ���� ���
    //~Tile() {
    //    decos.clear();
    //}
};