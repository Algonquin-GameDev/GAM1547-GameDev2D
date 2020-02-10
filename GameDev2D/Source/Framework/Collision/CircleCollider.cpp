#include "CircleCollider.h"


namespace GameDev2D
{
	CircleCollider::CircleCollider(GameObject* aGameObject, float aRadius, Type aType, CollisionCallback * aCallback) : Collider(aGameObject, aType, aCallback),
		m_Radius(aRadius)
	{
	}

	CircleCollider::~CircleCollider()
	{
	}

	Collider::Shape  CircleCollider::GetShape()
	{
		return Collider::Circle;
	}

	void CircleCollider::SetRadius(float radius)
	{
		m_Radius = radius;
	}

	float CircleCollider::GetRadius()
	{
		return m_Radius;
	}
}