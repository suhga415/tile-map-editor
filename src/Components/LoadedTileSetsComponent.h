#ifndef LOADEDTILESETSCOMPONENT_H
#define LOADEDTILESETSCOMPONENT_H

#include "../Structs/TileSet.h"

struct LoadedTileSetsComponent {
  std::map<std::string, TileSet*> tileSets;

  LoadedTileSetsComponent() = default;

};

#endif
