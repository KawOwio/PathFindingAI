#include "Sprite.h"


Sprite::Sprite(SDL_Renderer* _renderer, std::string _file)
{
	const char *file = _file.c_str();
	SDL_Surface* bmpFile = SDL_LoadBMP(file);
	image = SDL_CreateTextureFromSurface(_renderer, bmpFile);
	SDL_FreeSurface(bmpFile);

	
	
	position.x = 0;
	position.y = 0;
}

Sprite::~Sprite()
{
	if (image)
	{
		SDL_DestroyTexture(image);
	}
}

void Sprite::Draw(SDL_Renderer* _renderer, int _posX, int _posY, int _d)
{
	position.w = position.h = _d;
	position.x = _posX;
	position.y = _posY;

	if (image)
	{
		SDL_RenderCopy(_renderer, image, NULL, &position);
	}
}