#include "AxisAlignedRectangleCollider.h"
#include "../Core/Transformable.h"

namespace GameDev2D
{
	AxisAlignedRectangleCollider::AxisAlignedRectangleCollider(GameObject* gameObject, float width, float height, Type type, CollisionCallback* callback) : Collider(gameObject, type, callback),
		m_Width(width),
		m_Height(height),
		m_EdgeCollision(NoEdge)
	{
	}

	AxisAlignedRectangleCollider::~AxisAlignedRectangleCollider()
	{
	}

	Collider::Shape AxisAlignedRectangleCollider::GetShape()
	{
		return Collider::AxisAlignedRectangle;
	}

	void AxisAlignedRectangleCollider::SetWidth(float width)
	{
		m_Width = width;
	}

	float AxisAlignedRectangleCollider::GetWidth()
	{
		return m_Width;
	}

	void AxisAlignedRectangleCollider::SetHeight(float height)
	{
		m_Height = height;
	}

	float AxisAlignedRectangleCollider::GetHeight()
	{
		return m_Height;
	}

	Vector2 AxisAlignedRectangleCollider::GetMin()
	{
		Vector2 x(1, 0);
		Vector2 y(0, 1);

		float radians = GetWorldRadians();
		float w = fabsf(cosf(radians)) * GetWidth() + fabsf(sinf(radians)) * GetHeight();
		float h = fabsf(cosf(radians)) * GetHeight() + fabsf(sinf(radians)) * GetWidth();

		x *= w * 0.5f;
		y *= h * 0.5f;

		return Vector2(GetWorldPosition() - x - y);
	}

	Vector2 AxisAlignedRectangleCollider::GetMax()
	{
		Vector2 x(1, 0);
		Vector2 y(0, 1);

		float radians = GetWorldRadians();
		float w = fabsf(cosf(radians)) * GetWidth() + fabsf(sinf(radians)) * GetHeight();
		float h = fabsf(cosf(radians)) * GetHeight() + fabsf(sinf(radians)) * GetWidth();

		x *= w * 0.5f;
		y *= h * 0.5f;

		return Vector2(GetWorldPosition() + x + y);
	}

	unsigned char AxisAlignedRectangleCollider::GetEdgeCollision()
	{
		return m_EdgeCollision;
	}

	void AxisAlignedRectangleCollider::Reset()
	{
		m_EdgeCollision = NoEdge;
		Collider::Reset();
	}

	void AxisAlignedRectangleCollider::SetEdgeCollision(EdgeCollision edgeCollision)
	{
		m_EdgeCollision |= edgeCollision;
	}

	void AxisAlignedRectangleCollider::GetCorners(Vector2 corners[4])
	{
		Vector2 x(1,0);
		Vector2 y(0, 1);

		float radians = GetWorldRadians();
		float w = fabsf(cosf(radians)) * GetWidth() + fabsf(sinf(radians)) * GetHeight();
		float h = fabsf(cosf(radians)) * GetHeight() + fabsf(sinf(radians)) * GetWidth();

		x *= w * 0.5f;
		y *= h * 0.5f;

		corners[0] = GetWorldPosition() - x - y;
		corners[1] = GetWorldPosition() + x - y;
		corners[2] = GetWorldPosition() + x + y;
		corners[3] = GetWorldPosition() - x + y;
	}

	void AxisAlignedRectangleCollider::GetEdges(LineSegment segments[4])
	{
		Vector2 corners[4];
		GetCorners(corners);

		segments[0] = LineSegment(corners[0], corners[1]); //Bottom
		segments[1] = LineSegment(corners[1], corners[2]); //Right
		segments[2] = LineSegment(corners[2], corners[3]); //Top
		segments[3] = LineSegment(corners[3], corners[0]); //Left
	}
}