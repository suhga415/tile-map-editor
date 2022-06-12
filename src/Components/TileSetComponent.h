#ifndef TILESETCOMPONENT_H
#define TILESETCOMPONENT_H

#include "../ECS/ECS.h"

struct TileSetComponent {
  std::string assetId;
  int tileSize;
  int tileNumX;
  int tileNumY;
  float scale; // scale for GUI image
  int width;
  int height;

  TileSetComponent (
    std::string assetId = "",
    int tileSize = 0,
    int tileNumX = 1,
    int tileNumY = 1,
    float scale = 1
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
