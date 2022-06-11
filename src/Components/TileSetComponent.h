#ifndef TILESETCOMPONENT_H
#define TILESETCOMPONENT_H

#include "../ECS/ECS.h"

struct TileSetComponent {
  std::string assetId;
  int tileSize;
  int tileNumX;
  int tileNumY;
  int width;
  int height;
  float scale; // scale for GUI image

  TileSetComponent (
    std::string assetId = "",
    int tileSize = 0,
    int tileNumX = 0,
    int tileNumY = 0,
    float scale = 0
  ) {
    this->assetId = assetId;
    this->tileSize = tileSize;
    this->tileNumX = tileNumX;
    this->tileNumY = tileNumY;
    this->scale = scale;
    this->width = tileSize * tileNumX;
    this->height = tileSize * tileNumY;
  }
};

#endif
