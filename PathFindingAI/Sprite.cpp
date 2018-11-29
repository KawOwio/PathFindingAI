#include "Sprite.h"

Sprite::Sprite(SDL_Renderer* _renderer, std::string _file)
{
	const char *file = _file.c_str();
	SDL_Surface* bmpFile = SDL_LoadBMP(file);
	image = SDL_CreateTextureFromSurface(_renderer, bmpFile);
	SDL_FreeSurface(bmpFile);

	position.w = 32;
	position.h = 32;
	
	position.x = 100;
	position.y = 100;
}

Sprite::~Sprite()
{
	if (image)
	{
		SDL_DestroyTexture(image);
	}
}

void Sprite::Draw(SDL_Renderer* _renderer)
{
	if (image)
	{
		SDL_RenderCopy(_renderer, image, NULL, &position);
	}
}