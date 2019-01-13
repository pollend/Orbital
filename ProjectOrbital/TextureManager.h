#pragma once
#include <SFML\Graphics.hpp>
class TextureManager
{
private:

	static std::map<std::string, sf::Texture*> mTextures;

public:

	//loads the texture and returns a refrence
	static sf::Texture* LoadAndRetrieveTexture(std::string file);

	//unloads the texture from the map
	//WARNING: pointers linked to this texture become unstable
	static void UnLoadTexture(std::string file);

	~TextureManager(void);
};

