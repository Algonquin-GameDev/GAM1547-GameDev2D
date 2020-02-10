#pragma once

#include "Collider.h"

namespace GameDev2D
{
	class CircleCollider : public Collider
	{
	public:
		CircleCollider(GameObject* gameObject, float radius, Type type, CollisionCallback* callback = nullptr);
		~CircleCollider();

		//Overridden from Collider, returns that this is a Collider::Circle 
		virtual Shape GetShape() override;

		//Radius properties of the Collider
		void SetRadius(float radius);
		float GetRadius();

	private:
		//Member variable
		float m_Radius;
	};
}