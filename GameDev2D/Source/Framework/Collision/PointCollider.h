#pragma once

#include "Collider.h"

namespace GameDev2D
{
	class PointCollider : public Collider
	{
	public:
		PointCollider(GameObject* gameObject, Type type, CollisionCallback* callback = nullptr);
		~PointCollider();

		//Overridden from Collider, returns that this is a Collider::Point 
		virtual Shape GetShape() override;
	};
}