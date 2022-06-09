#ifndef MAPFILEWRITER_H
#define MAPFILEWRITER_H

#include "../Logger/Logger.h"
#include "../Components/CanvasComponent.h"
#include <fstream>

class MapFileWriter {
  public:
    static void write(const CanvasComponent canvas, std::string filename) {
      Logger::Info("[MapFileWriter] Writing started!");
      // TODO: check if there's any empty tile...
      std::ofstream outFile(filename);
      for (int j = 0; j < canvas.tileNumY; j++) {
        for (int i = 0; i < canvas.tileNumX; i++) {
          outFile << std::to_string(canvas.assignedTiles[j * canvas.tileNumX + i].colIdx) << std::to_string(canvas.assignedTiles[j * canvas.tileNumX + i].rowIdx) << ",";
        }
        outFile << std::endl;
      }
      outFile.close();
      Logger::Info("[MapFileWriter] Writing file comppleted!");
    }
};

#endif
