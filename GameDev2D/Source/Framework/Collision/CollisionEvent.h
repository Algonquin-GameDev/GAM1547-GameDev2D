#pragma once

namespace GameDev2D
{
	class Collider;

	struct CollisionEvent
	{
		CollisionEvent(Collider* a, Collider* b, const std::vector<Vector2>& pointsOfContact);

		Collider* a;
		Collider* b;
		std::vector<Vector2> pointsOfContact;
		bool resolveCollision;
	};
}