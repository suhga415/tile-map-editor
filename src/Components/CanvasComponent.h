#ifndef CANVASCOMPONENT_H
#define CANVASCOMPONENT_H

#include "../Logger/Logger.h"
#include "../Structs/Tile.h"

struct CanvasComponent {
  int locationX;
  int locationY;
  int tileSize;
  int tileNumX;
  int tileNumY;
  int scale; // (canvas tile size) / (tileset tile size)
  std::vector<Tile> assignedTiles; // a 2d tile-index data

  CanvasComponent(
    int locationX = 0,
    int locationY = 0,
    int tileSize = 0,
    int tileNumX = 1,
    int tileNumY = 1,
    int scale = 1
  ) {
    this->locationX = locationX;
    this->locationY = locationY;
    this->tileSize = tileSize;
    this->tileNumX = tileNumX;
    this->tileNumY = tileNumY;
    this->scale = scale;
    initialize();
  }

  void initialize() {
    assignedTiles.clear();
    for (int i = 0; i < tileNumX * tileNumY; i++) {
      Tile tile = {-1, -1};
      assignedTiles.push_back(tile);
    }
  }

  void reassignTilesByNumChange(int prevNumX, int prevNumY) {
    std::vector<Tile> newTiles;
    for (int j = 0; j < tileNumY; j++) {
      for (int i = 0; i < tileNumX; i++) {
        if (i >= prevNumX || j >= prevNumY) {
          Tile tile = {-1, -1};
          newTiles.push_back(tile);
        } else {
          newTiles.push_back(assignedTiles[j * prevNumX + i]);
        }
      }
    }
    assignedTiles = newTiles;
  }

  // TODO: more options for canvs resizing

};

#endif
