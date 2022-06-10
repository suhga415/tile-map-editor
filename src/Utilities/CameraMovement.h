#ifndef CAMERAMOVEMENT_H
#define CAMERAMOVEMENT_H

class CameraMovement {
  public:
    CameraMovement() = default;

    static void updateCamera(int dragDistX, int dragDistY, SDL_Rect& camera, SDL_Rect& canvas) {
      camera.x += dragDistX;
      camera.y += dragDistY;
      // update camera x-position
      if (camera.w >= canvas.w) { // the canvas is smaller than camera
        if (camera.x >= 0) {
          camera.x = 0;
        } else if (camera.x <= canvas.w - camera.w) {
          camera.x = canvas.w - camera.w;
        }
      } else { // the canvas is bigger than camera
        if (camera.x <= 0) {
          camera.x = 0;
        } else if (camera.x + camera.w >= canvas.x + canvas.w) {
          camera.x = canvas.x + canvas.w - camera.w;
        }
      }
      // update camera y-position
      if (camera.h >= canvas.h) {
        if (camera.y >= 0) {
          camera.y = 0;
        } else if (camera.y <= canvas.h - camera.h) {
          camera.y = canvas.h - camera.h;
        }
      } else {
        if (camera.y <= 0) {
          camera.y = 0;
        } else if (camera.y + camera.h >= canvas.y + canvas.h) {
          camera.y = canvas.y + canvas.h - camera.h;
        }
      }
    }
};

#endif
