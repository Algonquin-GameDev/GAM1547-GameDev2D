#include "OrientedRectangleCollider.h"
#include "../Core/Transformable.h"

namespace GameDev2D
{
	OrientedRectangleCollider::OrientedRectangleCollider(GameObject* gameObject, float width, float height, Type type, CollisionCallback* callback) : Collider(gameObject, type, callback),
		m_Width(width),
		m_Height(height)
	{
	}

	OrientedRectangleCollider::~OrientedRectangleCollider()
	{
	}

	Collider::Shape OrientedRectangleCollider::GetShape()
	{
		return Collider::OrientedRectangle;
	}

	void OrientedRectangleCollider::SetWidth(float width)
	{
		m_Width = width;
	}

	float OrientedRectangleCollider::GetWidth()
	{
		return m_Width;
	}

	void OrientedRectangleCollider::SetHeight(float height)
	{
		m_Height = height;
	}

	float OrientedRectangleCollider::GetHeight()
	{
		return m_Height;
	}

	void OrientedRectangleCollider::GetCorners(Vector2 corners[4])
	{
		float radians = GetWorldRadians();

		Vector2 x(cos(radians), sin(radians));
		Vector2 y(-sin(radians), cos(radians));

		x *= GetWidth() * 0.5f;
		y *= GetHeight() * 0.5f;

		corners[0] = GetWorldPosition() - x - y;
		corners[1] = GetWorldPosition() + x - y;
		corners[2] = GetWorldPosition() + x + y;
		corners[3] = GetWorldPosition() - x + y;
	}

	void OrientedRectangleCollider::GetEdges(LineSegment segments[4])
	{
		Vector2 corners[4];
		GetCorners(corners);

		segments[0] = LineSegment(corners[0], corners[1]); //Bottom
		segments[1] = LineSegment(corners[1], corners[2]); //Right
		segments[2] = LineSegment(corners[2], corners[3]); //Top
		segments[3] = LineSegment(corners[3], corners[0]); //Left
	}
}