#pragma once

#include "../../Collision/CollisionEvent.h"
#include "../../Collision/ColliderGroup.h"
#include "../../Events/EventHandler.h"
#include "../../Math/Math.h"
#include "../../Math/LineSegment.h"
#include <vector>


namespace GameDev2D
{
	class Collider;
	class PointCollider;
	class CircleCollider;
	class LineCollider;
	class AxisAlignedRectangleCollider;
	class OrientedRectangleCollider;

	// The CollisionManager game service handles all collision between Colliders
	class CollisionManager : public EventHandler
	{
	public:
		CollisionManager();
		~CollisionManager();

		//The HandleEvent is used to notify the InputManager class of various GameDev2D events
		void HandleEvent(Event* event);

		void Update(double delta);
		void Draw();

		unsigned int GetNumberOfStaticColliders();
		unsigned int GetNumberOfActiveStaticColliders();

		unsigned int GetNumberOfDynamicColliders();
		unsigned int GetNumberOfActiveDynamicColliders();

	private:
		//Ensures on Collider can register and unregister Colliders
		friend Collider;

		//Called from Collider to register a Collider
		void RegisterCollider(Collider* collider);

		//Called from Collider to unregister a Collider
		void UnregisterCollider(Collider* collider);

		//Check collision methods
		bool CheckCollision(Collider* a, Collider* b, std::vector<Vector2>& pointsOfContact);
		bool CheckCollision(CircleCollider* a, CircleCollider* b, std::vector<Vector2>& pointsOfContact);
		bool CheckCollision(PointCollider* a, CircleCollider* b, std::vector<Vector2>& pointsOfContact);
		bool CheckCollision(LineCollider* a, LineCollider* b, std::vector<Vector2>& pointsOfContact);
		bool CheckCollision(LineCollider* a, CircleCollider* b, std::vector<Vector2>& pointsOfContact);
		bool CheckCollision(LineCollider* a, PointCollider* b, std::vector<Vector2>& pointsOfContact);
		bool CheckCollision(OrientedRectangleCollider* a, OrientedRectangleCollider* b, std::vector<Vector2>& pointsOfContact);
		bool CheckCollision(CircleCollider* a, OrientedRectangleCollider* b, std::vector<Vector2>& pointsOfContact);
		bool CheckCollision(PointCollider* a, OrientedRectangleCollider* b, std::vector<Vector2>& pointsOfContact);
		bool CheckCollision(LineCollider* a, OrientedRectangleCollider* b, std::vector<Vector2>& pointsOfContact);
		bool CheckCollision(OrientedRectangleCollider* a, AxisAlignedRectangleCollider* b, std::vector<Vector2>& pointsOfContact);
		bool CheckCollision(AxisAlignedRectangleCollider* a, AxisAlignedRectangleCollider* b, std::vector<Vector2>& pointsOfContact);
		bool CheckCollision(CircleCollider* a, AxisAlignedRectangleCollider* b, std::vector<Vector2>& pointsOfContact);
		bool CheckCollision(PointCollider* a, AxisAlignedRectangleCollider* b, std::vector<Vector2>& pointsOfContact);
		bool CheckCollision(LineCollider* a, AxisAlignedRectangleCollider* b, std::vector<Vector2>& pointsOfContact);

		void ResolveCollision(Collider* a, Collider* b, std::vector<Vector2>& pointsOfContact);
		void ResolveCollision(AxisAlignedRectangleCollider* a, AxisAlignedRectangleCollider* b, std::vector<Vector2>& pointsOfContact);

		bool CheckLineIntersection(const LineSegment& lineSegmentA, const LineSegment& lineSegmentB, Vector2* intersection);
		Vector2 ClosestPointOnLine(const LineSegment& lineSegment, const Vector2& point);
		bool IsPointOnLineSegment(const LineSegment& lineSegment, const Vector2& point);
		bool DoesContainPointOfContact(const Vector2& pointOfContact, std::vector<Vector2>& pointsOfContact);

		//Member variables
		ColliderGroup m_StaticColliders;
		ColliderGroup m_DynamicColliders;
		std::vector <Vector2> m_PointsOfContact;
		bool m_Lock;
	};
}