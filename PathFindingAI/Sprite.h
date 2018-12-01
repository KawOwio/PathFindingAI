#ifndef _SPRITE_H_
#define _SPRITE_H_

#include <string>

#define SDL_MAIN_HANDLED
#include <SDL.h>

class Sprite
{
public:
	Sprite(SDL_Renderer* _renderer, std::string _file);
	~Sprite();

	virtual void Draw(SDL_Renderer* _renderer, int _posX, int _posY);

private:
	SDL_Texture* image;
	SDL_Rect texture;
	SDL_Rect position;
};

#endif