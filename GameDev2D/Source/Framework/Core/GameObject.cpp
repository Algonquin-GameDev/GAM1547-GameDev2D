#include "GameObject.h"


namespace GameDev2D
{
	GameObject::GameObject() : Transformable(),
		m_LinearVelocity(Vector2::Zero),
		m_AngularVelocity(0.0f),
		m_IsActive(true)
	{

	}

	GameObject::~GameObject()
	{
		RemoveAllColliders();
	}

	void GameObject::Update(double delta)
	{
		Vector2 linearDisplacement = m_LinearVelocity * delta;
		Translate(linearDisplacement);

		float angularDisplacement = m_AngularVelocity * delta;
		RotateRadians(angularDisplacement);
	}

	void GameObject::Draw(SpriteBatch* spriteBatch)
	{
	}

	void GameObject::Reset()
	{
	}

	void GameObject::CollisionDetected(CollisionEvent* collisionEvent)
	{
		//Override if you want to handle Collision Events
	}

	bool GameObject::AlwaysUpdate()
	{
		return false;
	}

	bool GameObject::AlwaysDraw()
	{
		return false;
	}

	void GameObject::SetLinearVelocity(const Vector2& linearVelocity)
	{
		m_LinearVelocity = linearVelocity;
	}

	Vector2 GameObject::GetLinearVelocity()
	{
		return m_LinearVelocity;
	}

	void GameObject::SetAngularVelocity(float angularVelocity)
	{
		m_AngularVelocity = angularVelocity;
	}

	float GameObject::GetAngularVelocity()
	{
		return m_AngularVelocity;
	}

	void GameObject::SetIsActive(bool isActive)
	{
		m_IsActive = isActive;

		//Ensure the colliders are active at the same time as the Tile
		for (int i = 0; i < GetNumberOfColliders(); i++)
		{
			GetColliderAtIndex(i)->SetIsActive(m_IsActive);
		}
	}

	bool GameObject::IsActive()
	{
		return m_IsActive;
	}

	void GameObject::Activate()
	{
		SetIsActive(true);
	}

	void GameObject::Deactivate()
	{
		SetIsActive(false);
	}

	CircleCollider* GameObject::AddCircleCollider(float radius, Collider::Type type)
	{
		CircleCollider* collider = new CircleCollider(this, radius, type, this);
		m_Colliders.push_back(collider);
		return collider;
	}

	CircleCollider* GameObject::AddCircleCollider(float radius, Collider::Type type, CollisionFilter filter)
	{
		CircleCollider* collider = new CircleCollider(this, radius, type, this);
		collider->GetFilter() = filter;
		m_Colliders.push_back(collider);
		return collider;
	}

	OrientedRectangleCollider* GameObject::AddOrientedRectangleCollider(float width, float height, Collider::Type type)
	{
		OrientedRectangleCollider* collider = new OrientedRectangleCollider(this, width, height, type, this);
		m_Colliders.push_back(collider);
		return collider;
	}

	OrientedRectangleCollider* GameObject::AddOrientedRectangleCollider(float width, float height, Collider::Type type, CollisionFilter filter)
	{
		OrientedRectangleCollider* collider = new OrientedRectangleCollider(this, width, height, type, this);
		collider->GetFilter() = filter;
		m_Colliders.push_back(collider);
		return collider;
	}

	AxisAlignedRectangleCollider* GameObject::AddAxisAlignedRectangleCollider(float width, float height, Collider::Type type)
	{
		AxisAlignedRectangleCollider* collider = new AxisAlignedRectangleCollider(this, width, height, type, this);
		m_Colliders.push_back(collider);
		return collider;
	}

	AxisAlignedRectangleCollider* GameObject::AddAxisAlignedRectangleCollider(float width, float height, Collider::Type type, CollisionFilter filter)
	{
		AxisAlignedRectangleCollider* collider = new AxisAlignedRectangleCollider(this, width, height, type, this);
		collider->GetFilter() = filter;
		m_Colliders.push_back(collider);
		return collider;
	}

	PointCollider* GameObject::AddPointCollider(Collider::Type type)
	{
		PointCollider* collider = new PointCollider(this, type, this);
		m_Colliders.push_back(collider);
		return collider;
	}

	PointCollider* GameObject::AddPointCollider(Collider::Type type, CollisionFilter filter)
	{
		PointCollider* collider = new PointCollider(this, type, this);
		collider->GetFilter() = filter;
		m_Colliders.push_back(collider);
		return collider;
	}

	LineCollider* GameObject::AddLineCollider(const LineSegment& lineSegment, Collider::Type type)
	{
		LineCollider* collider = new LineCollider(this, lineSegment, type, this);
		m_Colliders.push_back(collider);
		return collider;
	}

	LineCollider* GameObject::AddLineCollider(const LineSegment& lineSegment, Collider::Type type, CollisionFilter filter)
	{
		LineCollider* collider = new LineCollider(this, lineSegment, type, this);
		collider->GetFilter() = filter;
		m_Colliders.push_back(collider);
		return collider;
	}

	size_t GameObject::GetNumberOfColliders()
	{
		return m_Colliders.size();
	}

	Collider* GameObject::GetColliderAtIndex(int index)
	{
		if (index >= 0 && index < GetNumberOfColliders())
		{
			return m_Colliders.at(index);
		}
		return nullptr;
	}

	void GameObject::RemoveColliderAtIndex(int index)
	{
		if (index >= 0 && index < GetNumberOfColliders())
		{
			m_Colliders.erase(m_Colliders.begin() + index);
		}
	}

	void GameObject::RemoveAllColliders()
	{
		for (int i = 0; i < m_Colliders.size(); i++)
		{
			if (m_Colliders.at(i) != nullptr)
			{
				delete m_Colliders.at(i);
				m_Colliders.at(i) = nullptr;
			}
		}
		m_Colliders.clear();
	}
}