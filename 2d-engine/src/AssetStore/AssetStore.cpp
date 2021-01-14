#include "AssetStore.h"
#include "SDL_image.h"
#include <spdlog/spdlog.h>

AssetStore::AssetStore() {
	spdlog::info("AssetStore constructor called");
}

AssetStore::~AssetStore() {
	ClearAssets();
	spdlog::info("AssetStore destructor called");
}

void AssetStore::ClearAssets() {
	for (auto texture : textures) {
		SDL_DestroyTexture(texture.second);  // get the value of key-value pair
	}
	textures.clear();
}

void AssetStore::AddTexture(SDL_Renderer* renderer, const std::string& assetId, const std::string& filePath)
{
	SDL_Surface* surface = IMG_Load(filePath.c_str()); // convert to a C string
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	// Add the texture to the map
	textures.emplace(assetId, texture);

	spdlog::info("New texture added to the Asset Store with id = {0}", assetId);
}

SDL_Texture* AssetStore::GetTexture(const std::string& assetId) // const
{
	return textures[assetId];  // to acces with brackets the method cant be const
}