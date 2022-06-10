#ifndef SELECTEDTIMECOMPONENT_H
#define SELECTEDTIMECOMPONENT_H

#include "../ECS/ECS.h"

struct SelectedTileComponent {
  std::string assetId;
  int rowIdx;
  int colIdx;
  int tileSize;
  int scale;

  SelectedTileComponent (
    std::string assetId = "",
    int rowIdx = 0,
    int colIdx = 0,
    int tileSize = 0,
    int scale = 1
  ) {
    this->assetId = assetId;
    this->rowIdx = rowIdx;
    this->colIdx = colIdx;
    this->tileSize = tileSize;
    this->scale = scale;
  }
};

#endif
