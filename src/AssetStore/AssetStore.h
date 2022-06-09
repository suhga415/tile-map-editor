#ifndef ASSETSTORE_H
#define ASSETSTORE_H

#include <map>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

class AssetStore {
	private:
		std::map<std::string, SDL_Texture*> textures;
		std::map<std::string, TTF_Font*> fonts;
		std::map<std::string, Mix_Music*> sounds;

	public:
		AssetStore();
		~AssetStore();

		void clearAssets();

		void addTexture(SDL_Renderer* renderer, const std::string& textureId, const std::string& filePath);
		void addTileTextures(SDL_Renderer* renderer, const std::string& textureId, const std::string& filePath, int tileNumX, int tileNumY);
		void addFont(const std::string& fontId, const std::string& filePath, const int fontSize);
		void addSound(const std::string& soundId, const std::string& filePath);

		SDL_Texture* getTexture(const std::string& textureId);
		TTF_Font* getFont(const std::string& fontId);
		Mix_Music* getSound(const std::string& soundId);
};

#endif
