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
    int atlasX = -1; // Atlas 내 타일 좌표
    int atlasY = -1;
    bool valid = false;

    FPOINT pos;
    //std::vector<Deco> decos; // 이 타일에 붙는 데코 목록
    //~Tile() {
    //    decos.clear();
    //}
};