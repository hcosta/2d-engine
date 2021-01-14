#pragma once
#include <map>
#include <SDL.h>
#include <string>

class AssetStore {
private:
	std::map<std::string, SDL_Texture*> textures;
	// Create a map for fonts
	// Create a map for audio
public:
	AssetStore();
	~AssetStore();

	void ClearAssets();
	void AddTexture(SDL_Renderer* renderer, const std::string& assetId, const std::string& filePath);
	SDL_Texture* GetTexture(const std::string& assetId);
};