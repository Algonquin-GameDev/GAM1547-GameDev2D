#include "PointCollider.h"

namespace GameDev2D
{
	PointCollider::PointCollider(GameObject* aGameObject, Type aType, CollisionCallback * aCallback) : Collider(aGameObject, aType, aCallback)
	{
	}

	PointCollider::~PointCollider()
	{
	}

	Collider::Shape PointCollider::GetShape()
	{
		return Collider::Point;
	}
}
