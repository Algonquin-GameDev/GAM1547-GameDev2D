#pragma once

#include "Vector2.h"

namespace GameDev2D
{
	struct LineSegment
	{
		LineSegment(const Vector2& start, const Vector2& end);
		LineSegment();

		float Length();
		Vector2 Normal();
		Vector2 Midpoint();

		Vector2 start;
		Vector2 end;
	};
}