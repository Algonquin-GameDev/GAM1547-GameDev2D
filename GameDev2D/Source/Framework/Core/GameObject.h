#pragma once

#include <GameDev2D.h>

namespace GameDev2D
{
	class SpriteBatch;

	class GameObject : public Transformable, public CollisionCallback
	{
	public:
		GameObject();
		virtual ~GameObject();

		virtual void Update(double delta);
		virtual void Draw(SpriteBatch* spriteBatch = nullptr);
		virtual void Reset();

		virtual void CollisionDetected(CollisionEvent* collisionEvent) override;

		virtual bool AlwaysUpdate();
		virtual bool AlwaysDraw();

		void SetLinearVelocity(const Vector2& linearVelocity);
		Vector2 GetLinearVelocity();

		void SetAngularVelocity(float angularVelocity); //In radians
		float GetAngularVelocity();						//In radians

		void SetIsActive(bool isActive);
		bool IsActive();

		void Activate();
		void Deactivate();

		//Methods to add Circle colliders to the transformable
		CircleCollider* AddCircleCollider(float radius, Collider::Type type, CollisionCallback* callback = nullptr);
		CircleCollider* AddCircleCollider(float radius, Collider::Type type, CollisionFilter filter, CollisionCallback* callback = nullptr);

		//Methods to add Rectangle colliders to the transformable
		OrientedRectangleCollider* AddOrientedRectangleCollider(float width, float height, Collider::Type type, CollisionCallback* callback = nullptr);
		OrientedRectangleCollider* AddOrientedRectangleCollider(float width, float height, Collider::Type type, CollisionFilter filter, CollisionCallback* callback = nullptr);

		//Methods to add Axis-a ligned Rectangle colliders to the transformable
		AxisAlignedRectangleCollider* AddAxisAlignedRectangleCollider(float width, float height, Collider::Type type, CollisionCallback* callback = nullptr);
		AxisAlignedRectangleCollider* AddAxisAlignedRectangleCollider(float width, float height, Collider::Type type, CollisionFilter filter, CollisionCallback* callback = nullptr);

		//Methods to add Point colliders to the transformable
		PointCollider* AddPointCollider(Collider::Type type, CollisionCallback* callback = nullptr);
		PointCollider* AddPointCollider(Collider::Type type, CollisionFilter filter, CollisionCallback* callback = nullptr);

		//Methods to add Line colliders to the transformable
		LineCollider* AddLineCollider(const LineSegment& lineSegment, Collider::Type type, CollisionCallback* callback = nullptr);
		LineCollider* AddLineCollider(const LineSegment& lineSegment, Collider::Type type, CollisionFilter filter, CollisionCallback* callback = nullptr);

		//Returns the numbers of colliders
		size_t GetNumberOfColliders();

		//Return collider at the specified index, can return nullptr if the index is out of bounds
		Collider* GetColliderAtIndex(int index);

		//Removes the collider at the specified index
		void RemoveColliderAtIndex(int index);

		//Removes all colliders attached to the Transformable
		void RemoveAllColliders();

	protected:
		std::vector<Collider*> m_Colliders;
		Vector2 m_LinearVelocity;
		float m_AngularVelocity;
		bool m_IsActive;
	};
}