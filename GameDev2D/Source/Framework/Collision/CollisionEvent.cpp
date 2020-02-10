#include "CollisionEvent.h"
#include "Collider.h"
#include "CircleCollider.h"
#include "../Core/Transformable.h"


namespace GameDev2D
{
	CollisionEvent::CollisionEvent(Collider* aA, Collider* aB, const std::vector<Vector2>& aPointsOfContact) :
		a(aA),
		b(aB),
		pointsOfContact(aPointsOfContact),
		resolveCollision(false)
	{

	}
}