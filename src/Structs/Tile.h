#ifndef TILE_H
#define TILE_H

struct Tile {
  // index from the (source) tileset
  int rowIdx; // x-dir index
  int colIdx; // y-dir index

  Tile(int colIdx, int rowIdx) {
    this->colIdx = colIdx;
    this->rowIdx = rowIdx;
  }
};

#endif
