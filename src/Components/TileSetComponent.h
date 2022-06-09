#ifndef TILESETCOMPONENT_H
#define TILESETCOMPONENT_H

#include "../ECS/ECS.h"

struct TileSetComponent {
  int tileSize;
  int tileNumX;
  int tileNumY;
  float scale;

  TileSetComponent (
    int tileSize = 0,
    int tileNumX = 0,
    int tileNumY = 0,
    float scale = 0
  ) {
    this->tileSize = tileSize;
    this->tileNumX = tileNumX;
    this->tileNumY = tileNumY;
    this->scale = scale;
  }
};

#endif
