#include "./AssetStore.h"

AssetStore::AssetStore() {}

AssetStore::~AssetStore() {
	clearAssets();
}

void AssetStore::clearAssets() {
	for (auto texture: textures) {
		SDL_DestroyTexture(texture.second);
	}
	textures.clear();
}

void AssetStore::addTexture(SDL_Renderer* renderer, const std::string& textureId, const std::string& filePath) {
	SDL_Surface* surface = IMG_Load(filePath.c_str());
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);
	textures.emplace(textureId, texture);
}

void AssetStore::addTileTextures(SDL_Renderer* renderer, const std::string& textureId, const std::string& filePath, int tileNumX, int tileNumY) {
	SDL_Surface* surface = IMG_Load(filePath.c_str());
	SDL_Rect rect = {
		100, 50, 100, 100
	};
	SDL_GetClipRect(surface, &rect);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);
	textures.emplace(textureId, texture);
}

void AssetStore::addFont(const std::string& fontId, const std::string& filePath, const int fontSize) {
	// fonts.emplace(fontId, ___);
}

void AssetStore::addSound(const std::string& soundId, const std::string& filePath) {
	// sounds.emplace(soundId, ___);
}

SDL_Texture* AssetStore::getTexture(const std::string& textureId) {
	return textures[textureId];
}

TTF_Font* AssetStore::getFont(const std::string& fontId) {
	return fonts[fontId];
}

Mix_Music* AssetStore::getSound(const std::string& soundId) {
	return sounds[soundId];
}
