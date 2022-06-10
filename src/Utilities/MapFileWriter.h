#ifndef MAPFILEWRITER_H
#define MAPFILEWRITER_H

#include "../Logger/Logger.h"
#include "../Components/CanvasComponent.h"
#include "../Components/SelectedTileComponent.h"
#include <fstream>

class MapFileWriter {
  public:
    static void write(
      const CanvasComponent canvas,
      const SelectedTileComponent selectedTile,
      std::string filename
    ) {
      // TODO: dealing with empty tiles when load
      std::ofstream outFile(filename);
      outFile << selectedTile.assetId << " " << std::to_string(canvas.tileSize) << " " << std::to_string(canvas.tileNumX) << " " << std::to_string(canvas.tileNumY) << " " << std::to_string(canvas.scale) << std::endl;
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
