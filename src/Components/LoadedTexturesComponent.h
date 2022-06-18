#ifndef LOADEDTEXTURESCOMPONENT_H
#define LOADEDTEXTURESCOMPONENT_H

#include "../Structs/Texture.h"

struct LoadedTexturesComponent {
  std::map<std::string, Texture*> textures;

  LoadedTexturesComponent() = default;

};

#endif
