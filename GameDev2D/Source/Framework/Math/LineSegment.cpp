#include "LineSegment.h"

namespace GameDev2D
{
	LineSegment::LineSegment(const Vector2& start, const Vector2& end) :
		start(start),
		end(end)
	{

	}

	LineSegment::LineSegment() :
		start(0.0f, 0.0f),
		end(0.0f, 0.0f)
	{

	}

	float LineSegment::Length()
	{
		return start.Distance(end);
	}

	Vector2 LineSegment::Normal()
	{
		Vector2 normal = start - end;
		return normal.PerpendicularCounterClockwise().Normalized();
	}

	Vector2 LineSegment::Midpoint()
	{
		return (start + end) * 0.5f;
	}
}