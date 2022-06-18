#ifndef TEXTURE_H
#define TEXTURE_H

struct Texture {
  int width;
  int height;
  float scale;

  Texture(int width = 0, int height = 0, float scale = 1.0) {
    this->width = width;
    this->height = height;
    this->scale = scale;
  }
};

#endif
