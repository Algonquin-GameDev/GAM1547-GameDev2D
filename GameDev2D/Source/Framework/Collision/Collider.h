#pragma once

#include "CollisionFilter.h"
#include "../Math/Vector2.h"
#include <string>


namespace GameDev2D
{
	class CollisionCallback;
	class GameObject;

	class Collider
	{
	public:
		//The supported types of Colliders
		enum Shape
		{
			Point = 0,
			Line,
			Circle,
			AxisAlignedRectangle,
			OrientedRectangle
		};

		enum Type
		{
			Static = 0,
			Dynamic
		};

		Collider(GameObject* gameObject, Type type = Dynamic, CollisionCallback* callback = nullptr);
		virtual ~Collider();

		//Checks if the collider is colliding with another collider
		bool IsColliding(Collider* collider);

		//Returns the Type (Static or Dynamic) of the Collider
		Type GetType();

		//Pure virtual method that all colliders must implement
		virtual Shape GetShape() = 0;

		//Returns the GameObject that the collider is attached to
		virtual GameObject* GetGameObject();

		//Returns the CollisionCallback pointer, if one is set
		virtual CollisionCallback* GetCallback();

		//Returns the CollisionFilter
		virtual CollisionFilter& GetFilter();

		//Returns the world position of the GameObject + Offset
		virtual Vector2 GetWorldPosition();

		//Returns the world angle in radians of the GameObject
		virtual float GetWorldRadians();

		//Offset properties relative to the Transformable position
		virtual void SetOffset(const Vector2& offset);
		virtual void SetOffset(float x, float y);
		virtual Vector2 GetOffset();

		//Active properties to determine if the Collider is active or not
		virtual void SetIsActive(bool isActive);
		virtual bool IsActive();

		//Is the Collider is currently colliding with another Collider or not
		virtual bool IsColliding();

		//Returns a string indentifier detailing the type and shape of the collider
		std::string GetTypeString();

		//Returns a string indentifier detailing the type and shape of the collider
		std::string GetShapeString();

		//Returns a string indentifier detailing the type and shape of the collider
		std::string GetStringIdentifier();

	protected:
		friend class CollisionManager;
		friend class ColliderGroup;

		//Colliding properties to determine if the Collider is currently 
		//colliding with another Collider or not
		virtual void SetIsColliding(bool isColliding);

		//Resets the colliding flag
		virtual void Reset();

	private:
		//Member variables
		GameObject* m_GameObject;
		CollisionCallback* m_Callback;
		CollisionFilter m_Filter;
		Vector2 m_Offset;
		Type m_Type;
		bool m_IsActive;
		bool m_IsColliding;
	};
}