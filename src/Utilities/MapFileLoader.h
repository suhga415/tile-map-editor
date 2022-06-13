#ifndef MAPFILELOADER_H
#define MAPFILELOADER_H

#include "../Logger/Logger.h"
#include "../Components/CanvasComponent.h"
#include "../Components/SelectedTileComponent.h"
#include "../Structs/Tile.h"
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

class MapFileLoader {
  public:
    static void load(std::string filePath, std::string &assetId, int &tileSize, int &tileNumX, int &tileNumY, float &scale, std::vector<Tile> &assignedTiles) {
      std::ifstream mapFile;
      std::string line;
      mapFile.open(filePath);
      std::getline(mapFile, line);
      std::vector<std::string> words = MapFileLoader::split(line, ' ');
      assetId = words[0];
      tileSize = std::stoi(words[1]);
      tileNumX = std::stoi(words[2]);
      tileNumY = std::stoi(words[3]);
      scale = std::stof(words[4]);
      for (int j = 0; j < tileNumY; j++) {
        std::getline(mapFile, line);
        words = MapFileLoader::split(line, ' ');
        for (int i = 0; i < tileNumX; i++) {
          std::vector<std::string> indices = MapFileLoader::split(words[i], ',');
          Tile tile(std::stoi(indices[0]), std::stoi(indices[1]));
          assignedTiles.push_back(tile);
        }
      }
      mapFile.close();
    }

  private:
    static std::vector<std::string> split(std::string str, char delimiter) {
      std::istringstream iss(str);
      std::string word;
      std::vector<std::string> result;
      while (getline(iss, word, delimiter)) {
        result.push_back(word);
      }
      return result;
    }
};

#endif
