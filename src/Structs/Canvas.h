#ifndef CANVAS_H
#ifndef CANVAS_H

struct Tile {
  // index from the (source) tileset
  int rowIdx;
  int colIdx;
};

struct Canvas {
  int x;
  int y;
  int w;
  int h;
  int tileSize;
  int tileNumX;
  int tileNumY;
  int scale; // (canvas tile size) / (tileset tile size)
  std::vector<Tile> assignedTiles; // a 2d tile-index data

  Canvas(
    int x = 0,
    int y = 0,
    int tileSize = 0,
    int tileNumX = 1,
    int tileNumY = 1,
    int scale = 1
  ) {
    this->x = x;
    this->y = y;
    this->tileSize = tileSize;
    this->tileNumX = tileNumX;
    this->tileNumY = tileNumY;
    this->scale = scale;
    this->w = tileSize * tileNumX * scale;
    this->h = tileSize * tileNumY * scale;

    for (int i = 0; i < tileNumX * tileNumY; i++) {
      Tile tile = {-1, -1};
      assignedTiles.push_back(tile);
    }
  }
};

#endif
