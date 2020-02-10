#include "Collider.h"
#include "../Core/GameObject.h"
#include "../Services/Services.h"
#include "../Services/CollisionManager/CollisionManager.h"
#include "../Math/Matrix.h"


namespace GameDev2D
{
	Collider::Collider(GameObject * aGameObject, Type aType, CollisionCallback * aCallback) :
		m_GameObject(aGameObject),
		m_Callback(aCallback),
		m_Filter(CollisionFilter()),
		m_Offset(Vector2::Zero),
		m_Type(aType),
		m_IsActive(true),
		m_IsColliding(false)
	{
		Services::GetCollisionManager()->RegisterCollider(this);
	}

	Collider::~Collider()
	{
		Services::GetCollisionManager()->UnregisterCollider(this);
	}

	bool Collider::IsColliding(Collider* collider)
	{
		std::vector<Vector2> pointsOfContact;
		bool isColliding = Services::GetCollisionManager()->CheckCollision(this, collider, pointsOfContact);
		return isColliding;
	}

	Collider::Type Collider::GetType()
	{
		return m_Type;
	}

	GameObject * Collider::GetGameObject()
	{
		return m_GameObject;
	}

	CollisionCallback * Collider::GetCallback()
	{
		return m_Callback;
	}

	CollisionFilter& Collider::GetFilter()
	{
		return m_Filter;
	}

	Vector2 Collider::GetWorldPosition()
	{
		Matrix result = GetGameObject()->GetTransformMatrix() * Matrix::MakeTranslation(GetOffset());
		return result.GetTranslation();
	}

	float Collider::GetWorldRadians()
	{
		return GetGameObject()->GetTransformMatrix().GetRadians();
	}

	void Collider::SetOffset(const Vector2 & aOffset)
	{
		m_Offset = aOffset;
	}

	void Collider::SetOffset(float x, float y)
	{
		m_Offset.x = x;
		m_Offset.y = y;
	}

	Vector2 Collider::GetOffset()
	{
		return m_Offset;
	}

	void Collider::SetIsActive(bool isActive)
	{
		m_IsActive = isActive;
	}

	bool Collider::IsActive()
	{
		return m_IsActive;
	}

	void Collider::SetIsColliding(bool isColliding)
	{
		m_IsColliding = isColliding;
	}

	void Collider::Reset()
	{
		m_IsColliding = false;
	}

	bool Collider::IsColliding()
	{
		return m_IsColliding;
	}

	std::string Collider::GetTypeString()
	{
		std::string type = GetType() == Dynamic ? "Dynamic" : "Static";
		return type;
	}

	std::string Collider::GetShapeString()
	{
		std::string shape;

		switch (GetShape())
		{
		case AxisAlignedRectangle:
			shape = "Axis-Aligned Rectangle Collider";
			break;

		case Circle:
			shape = "Circle Collider";
			break;

		case Line:
			shape = "Line Collider";
			break;

		case OrientedRectangle:
			shape = "Oriented Rectangle Collider";
			break;

		case Point:
			shape = "Point Collider";
			break;
		}

		return shape;
	}

	std::string Collider::GetStringIdentifier()
	{
		return std::string(GetTypeString() + " " + GetShapeString());
	}
}
