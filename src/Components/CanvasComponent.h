#ifndef CANVASCOMPONENT_H
#define CANVASCOMPONENT_H

struct Tile {
  // index from the (source) tileset
  int rowIdx;
  int colIdx;
};

struct CanvasComponent {
  int locationX;
  int locationY;
  int tileSize;
  int tileNumX;
  int tileNumY;
  int scale;
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
    for (int i = 0; i < tileNumX * tileNumY; i++) {
      Tile tile = {-1, -1};
      assignedTiles.push_back(tile);
    }
  }
};

#endif
