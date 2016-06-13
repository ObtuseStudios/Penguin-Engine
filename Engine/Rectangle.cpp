//Copyright 2016, Cameron bell, All rights reserved

#include "Rectangle.h"
#include "Debug.h"

namespace Penguin
{
	void Rectangle::Draw(Window* rend, bool stick)
	{
		SetPos();

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

		SDL_SetRenderDrawColor(rend->GetRenderer() , Col.Red, Col.Green, Col.Blue, Col.Alpha);
		SDL_RenderFillRect(rend->GetRenderer(), &temp);
	}

	void Rectangle::DrawOutline(Window* rend, bool stick)
	{
		SetPos();

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

		SDL_SetRenderDrawColor(rend->GetRenderer() , Col.Red, Col.Green, Col.Blue, Col.Alpha);
		SDL_RenderDrawRect(rend->GetRenderer(), &temp);
	}
}