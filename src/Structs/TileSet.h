#ifndef TILESET_H
#define TILESET_H

struct TileSet {
  int tileSize;
  int tileNumX;
  int tileNumY;
  float scale;

  TileSet(int tileSize = 0, int tileNumX = 1, int tileNumY = 1, float scale = 1.0) {
    this->tileSize = tileSize;
    this->tileNumX = tileNumX;
    this->tileNumY = tileNumY;
    this->scale = scale;
  }
};

#endif
