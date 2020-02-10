#pragma once

#include "Collider.h"
#include "../Math/LineSegment.h"


namespace GameDev2D
{
	class OrientedRectangleCollider : public Collider
	{
	public:
		OrientedRectangleCollider(GameObject* gameObject, float width, float height, Type type, CollisionCallback* callback = nullptr);
		~OrientedRectangleCollider();

		//Overridden from Collider, returns that this is a Collider::Rectangle 
		virtual Shape GetShape() override;

		void SetWidth(float width);
		float GetWidth();

		void SetHeight(float height);
		float GetHeight();

	private:
		friend class CollisionManager;

		void GetCorners(Vector2 corners[4]);
		void GetEdges(LineSegment segments[4]);

		//Members variables
		float m_Width;
		float m_Height;
	};
}