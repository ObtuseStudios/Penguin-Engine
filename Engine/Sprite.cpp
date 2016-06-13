//Copyright 2016, Cameron bell, All rights reserved

#include "Sprite.h"
#include <math.h>

namespace Penguin
{

	void Sprite::Draw(Window* rend, bool stick)
	{
		SetPos();

		if (Col.Alpha != 255)
		{
			SDL_SetTextureAlphaMod(Texture, Col.Alpha);
		}

		SDL_Rect temp = Source;

		if (!stick)
		{
			temp.x -= rend->Camera.position.x / rend->Camera.scale;
			temp.y -= rend->Camera.position.y / rend->Camera.scale;

			temp.x *= rend->Camera.scale;
			temp.y *= rend->Camera.scale;

			temp.w *= rend->Camera.scale;
			temp.h *= rend->Camera.scale;

			Vector roundedPos = Vector(temp.x, temp.y);

			roundedPos.x /= rend->Camera.scale;
			roundedPos.y /= rend->Camera.scale;

			temp.x = std::round(roundedPos.x) * rend->Camera.scale;
			temp.y = std::round(roundedPos.y) * rend->Camera.scale;
		}

		SDL_RenderCopyEx(rend->GetRenderer(), Texture, NULL, &temp, rotation, &centerPoint, flipSDL);
	}

	void Sprite::Load(SDL_Renderer* rend, std::string path, bool org)
	{
		//The final texture
		SDL_Texture* newTexture = NULL;

		//Load image at specified path
		SDL_Surface* loadedSurface = IMG_Load(path.c_str());
		if (loadedSurface == NULL) Debug::Fatal("Could not load image: " + path);

		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(rend, loadedSurface);

		if (newTexture == NULL) Debug::Fatal("Could not create texture from file: " + path);
		if (org) Scale = Vector(loadedSurface->w, loadedSurface->h);

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);

		Texture = newTexture;

		SDL_SetTextureColorMod(Texture, Col.Red, Col.Green, Col.Blue);
		SDL_SetTextureBlendMode(Texture, SDL_BLENDMODE_BLEND);
	}

	void Sprite::resetColourMod(Colour col)
	{
		SDL_SetTextureColorMod(Texture, col.Red, col.Green, col.Blue);
	}
}