//#include "AstarScene.h"
//#include <queue>
//#include <cmath>
//
//float Heuristic(AstarTile* a, AstarTile* b) {
//    int dx = abs(a->idX - b->idX);
//    int dy = abs(a->idY - b->idY);
//    return (std::sqrt(2.0f) - 1.0f) * std::min(dx, dy) + std::max(dx, dy); // Octile distance
//}
//
//AstarTile* AstarScene::GetLowestCostTile() {
//    auto it = std::min_element(openList.begin(), openList.end(), [](AstarTile* a, AstarTile* b) {
//        return a->totalCost < b->totalCost;
//        });
//    AstarTile* tile = *it;
//    openList.erase(it);
//    return tile;
//}
//
//void AstarScene::TracePath() {
//    AstarTile* pathTile = destTile;
//    while (pathTile != nullptr && pathTile != startTile) {
//        pathTile->SetColor(RGB(0, 255, 0)); // Green for path
//        pathTile = pathTile->parentTile;
//    }
//}
//
//bool AstarScene::IsValidNeighbor(AstarTile* tile) {
//    return tile->GetType() != AstarTileType::Wall &&
//        std::find(closeList.begin(), closeList.end(), tile) == closeList.end();
//}
//
//void AstarScene::UpdateNeighborCosts(AstarTile* neighbor, AstarTile* currTile, float moveCost) {
//    float tentativeG = currTile->costFromStart + moveCost;
//    bool inOpen = std::find(openList.begin(), openList.end(), neighbor) != openList.end();
//
//    if (!inOpen || tentativeG < neighbor->costFromStart) {
//        neighbor->costFromStart = tentativeG;
//        neighbor->costToGoal = Heuristic(neighbor, destTile);
//        neighbor->totalCost = neighbor->costFromStart + neighbor->costToGoal;
//        neighbor->parentTile = currTile;
//
//        if (!inOpen) {
//            openList.push_back(neighbor);
//        }
//    }
//}
//
//void AstarScene::FindPath() {
//    openList.clear();
//    closeList.clear();
//
//    openList.push_back(startTile);
//
//    while (!openList.empty()) {
//        currTile = GetLowestCostTile();
//        closeList.push_back(currTile);
//
//        if (currTile == destTile) {
//            TracePath();
//            break;
//        }
//
//        AddOpenList(currTile);
//    }
//}
//
//void AstarScene::AddOpenList(AstarTile* currTile) {
//    static const int dx[8] = { -1, 1,  0, 0, -1, -1,  1,  1 };
//    static const int dy[8] = { 0, 0, -1, 1, -1,  1, -1,  1 };
//    static const float cost[8] = { 1, 1, 1, 1, 1.4142f, 1.4142f, 1.4142f, 1.4142f };
//
//    for (int i = 0; i < 8; ++i) {
//        int nx = currTile->idX + dx[i];
//        int ny = currTile->idY + dy[i];
//
//        if (nx < 0 || nx >= ASTAR_TILE_COUNT || ny < 0 || ny >= ASTAR_TILE_COUNT)
//            continue;
//
//
//        // 벽 충돌 회피 (대각선일 때 끼워 지나가는 것 방지)
//        if (i >= 4) {
//            int adjX = currTile->idX + dx[i];
//            int adjY = currTile->idY;
//            int adjX2 = currTile->idX;
//            int adjY2 = currTile->idY + dy[i];
//
//            if (adjX >= 0 && adjX < ASTAR_TILE_COUNT && adjY >= 0 && adjY < ASTAR_TILE_COUNT &&
//                map[adjY][adjX].GetType() == AstarTileType::Wall)
//                continue;
//
//            if (adjX2 >= 0 && adjX2 < ASTAR_TILE_COUNT && adjY2 >= 0 && adjY2 < ASTAR_TILE_COUNT &&
//                map[adjY2][adjX2].GetType() == AstarTileType::Wall)
//                continue;
//        }
//
//        AstarTile* neighbor = &map[ny][nx];
//
//        if (!IsValidNeighbor(neighbor)) continue;
//
//        UpdateNeighborCosts(neighbor, currTile, cost[i]);
//    }
//}
