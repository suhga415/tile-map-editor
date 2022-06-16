#ifndef SELECTEDTIMECOMPONENT_H
#define SELECTEDTIMECOMPONENT_H

#include "../ECS/ECS.h"

struct SelectedTileComponent {
  std::string assetId;
  int rowIdx; // x-dir index of selected tile
  int colIdx; // y-dir index of selected tile
  int tileSize; // original size

  SelectedTileComponent (
    std::string assetId = "",
    int rowIdx = -1,
    int colIdx = -1,
    int tileSize = 0
  ) {
    this->assetId = assetId;
    this->rowIdx = rowIdx;
    this->colIdx = colIdx;
    this->tileSize = tileSize;
  }
};

#endif
