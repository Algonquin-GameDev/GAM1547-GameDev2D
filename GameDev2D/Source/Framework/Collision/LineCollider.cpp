#include "LineCollider.h"
#include "../Core/GameObject.h"
#include "../Math/Matrix.h"


namespace GameDev2D
{
	LineCollider::LineCollider(GameObject* aGameObject, const LineSegment& aLineSegment, Type aType, CollisionCallback* aCallback) : Collider(aGameObject, aType, aCallback),
		m_LineSegment(aLineSegment)
	{

	}

	LineCollider::LineCollider(GameObject* aGameObject, const Vector2& aStart, const Vector2& aEnd, Type aType, CollisionCallback* aCallback) : Collider(aGameObject, aType, aCallback),
		m_LineSegment(aStart, aEnd)
	{

	}

	LineCollider::~LineCollider()
	{
	}

	Collider::Shape LineCollider::GetShape()
	{
		return Collider::Line;
	}

	LineSegment LineCollider::Get()
	{
		return m_LineSegment;
	}

	LineSegment LineCollider::GetRelative()
	{
		Matrix start = GetGameObject()->GetTransformMatrix() * Matrix::MakeTranslation(GetOffset() + Get().start);
		Matrix end = GetGameObject()->GetTransformMatrix() * Matrix::MakeTranslation(GetOffset() + Get().end);
		return LineSegment(start.GetTranslation(), end.GetTranslation());
	}
}