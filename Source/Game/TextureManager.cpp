#include "TextureManager.h"
#include <SFML\Graphics.hpp>

 std::map<std::string, sf::Texture*> TextureManager::mTextures;

sf::Texture* TextureManager::LoadAndRetrieveTexture(std::string file)
{
	sf::Texture* lTexture;
	if((TextureManager::mTextures.find(file) == TextureManager::mTextures.end()))
	{
		lTexture= new sf::Texture();
		lTexture->loadFromFile(file);
		TextureManager::mTextures.insert(std::pair<std::string,sf::Texture*>(file,lTexture));
	}

	return TextureManager::mTextures[file];

}

void TextureManager::UnLoadTexture(std::string file)
{
	if(TextureManager::mTextures.find(file) == TextureManager::mTextures.end())
	{
		
		delete TextureManager::mTextures[file];
		TextureManager::mTextures.erase(TextureManager::mTextures.find(file));
	}
}


