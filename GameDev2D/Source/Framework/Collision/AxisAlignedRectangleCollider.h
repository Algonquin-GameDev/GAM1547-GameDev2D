#pragma once

#include "Collider.h"
#include "../Math/LineSegment.h"

namespace GameDev2D
{
	class AxisAlignedRectangleCollider : public Collider
	{
	public:
		AxisAlignedRectangleCollider(GameObject* gameObject, float width, float height, Type type, CollisionCallback* callback = nullptr);

		~AxisAlignedRectangleCollider();

		//Overridden from Collider, returns that this is a Collider::Rectangle 
		virtual Shape GetShape() override;

		void SetWidth(float width);
		float GetWidth();

		void SetHeight(float height);
		float GetHeight();

		Vector2 GetMin();
		Vector2 GetMax();

		enum EdgeCollision
		{
			UnknownEdge = -1,
			NoEdge = 0,
			LeftEdge = 1,
			RightEdge = 2,
			TopEdge = 4,
			BottomEdge = 8
		};

		unsigned char GetEdgeCollision();

	private:
		friend class CollisionManager;

		virtual void Reset() override;

		void SetEdgeCollision(EdgeCollision edgeCollision);

		void GetCorners(Vector2 corners[4]);
		void GetEdges(LineSegment segments[4]);

		//Members variables
		float m_Width;
		float m_Height;
		unsigned char m_EdgeCollision;
	};
}