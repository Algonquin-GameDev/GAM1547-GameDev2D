#pragma once

#include "Collider.h"
#include "../Math/LineSegment.h"

namespace GameDev2D
{
	class LineCollider : public Collider
	{
	public:
		LineCollider(GameObject* gameObject, const LineSegment& lineSegment, Type type, CollisionCallback* callback = nullptr);
		LineCollider(GameObject* gameObject, const Vector2& start, const Vector2& end, Type type, CollisionCallback* callback = nullptr);
		~LineCollider();

		//Overridden from Collider, returns that this is a Collider::Line 
		virtual Shape GetShape() override;

		//Returns the Line Segment (start and end points of the line), NOT taking into account the position and rotation of the Transformable
		LineSegment Get();

		//Returns the Line Segment (start and end points of the line), taking into account the position and rotation of the Transformable
		LineSegment GetRelative();

	private:
		//Member variables
		LineSegment m_LineSegment;
	};
}