#include "CollisionManager.h"
#include "../Services.h"
#include "../../Collision/CollisionCallback.h"
#include "../../Collision/CollisionEvent.h"
#include "../../Collision/CircleCollider.h"
#include "../../Collision/PointCollider.h"
#include "../../Collision/LineCollider.h"
#include "../../Collision/AxisAlignedRectangleCollider.h"
#include "../../Collision/OrientedRectangleCollider.h"
#include "../../Core/GameObject.h"
#include "../../Math/Math.h"
#include "../../Events/UpdateEvent.h"
#include "../../Windows/Application.h"
#include "../../GameDev2D_Settings.h"


namespace GameDev2D
{
	CollisionManager::CollisionManager() : EventHandler(),
		m_DynamicColliders(Collider::Dynamic),
		m_StaticColliders(Collider::Static)
	{
		m_Lock = false;

		//Add an event listener callback for the Update event
		Services::GetApplication()->AddEventListener(this, UPDATE_EVENT);
		Services::GetApplication()->AddEventListener(this, DRAW_EVENT);
	}

	CollisionManager::~CollisionManager()
	{
		//Remove the event listener callback for the Update event
		Services::GetApplication()->RemoveEventListener(this, UPDATE_EVENT);
		Services::GetApplication()->RemoveEventListener(this, DRAW_EVENT);
	}

	void CollisionManager::HandleEvent(Event * aEvent)
	{
		if (aEvent != nullptr)
		{
			if (aEvent->GetEventCode() == UPDATE_EVENT)
			{
				UpdateEvent* updateEvent = (UpdateEvent*)aEvent;
				Update(updateEvent->GetDelta());
			}
			else if (aEvent->GetEventCode() == DRAW_EVENT)
			{
				Draw();
			}
		}
	}

	void CollisionManager::Update(double delta)
	{
		//Ensures, no colliders are removed during a collision callback
		m_Lock = true;

		//Update the collider groups
		m_DynamicColliders.PreCollisionUpdate(delta);
		m_StaticColliders.PreCollisionUpdate(delta);


#if DEBUG && DEBUG_DRAW_COLLIDERS
		//Clear the points of contact
		m_PointsOfContact.clear();

        m_DynamicColliders.Reset();
        m_StaticColliders.Reset();
#endif

        //Get the iterator to the first collider
        std::vector<Collider*>::iterator itA = m_DynamicColliders.activeColliders.begin();

		//Keep looping while there are active colliders to check
		while (itA != m_DynamicColliders.activeColliders.end())
		{
			//Cast iteratorA to a Collider*
			Collider* dynamicColliderA = *itA;

			//Check if dynamicColliderA is active
			if (dynamicColliderA->IsActive() == false)
			{
				m_DynamicColliders.inactiveColliders.push_back(dynamicColliderA);
				itA = m_DynamicColliders.activeColliders.erase(itA);
				continue;
			}

			std::vector<Collider*>::iterator itB = itA + 1;
			while (itB != m_DynamicColliders.activeColliders.end())
			{
				//Cast iteratorB to a Collider*
				Collider* dynamicColliderB = *itB;

				//Check if dynamicColliderB is active
				if (dynamicColliderB->IsActive() == false)
				{
					m_DynamicColliders.inactiveColliders.push_back(dynamicColliderB);
					itB = m_DynamicColliders.activeColliders.erase(itB);
					continue;
				}

				//Used to keep track of the point of contact between two colliders
				std::vector<Vector2> pointsOfContact;

				//Was there a collision?
				if(CheckCollision(dynamicColliderA, dynamicColliderB, pointsOfContact) == true)
				{
					//Flag both colliders for a collision
                    dynamicColliderA->SetIsColliding(true);
                    dynamicColliderB->SetIsColliding(true);

					//Add the point of contact to the vector
					for (int i = 0; i < pointsOfContact.size(); i++)
					{
						m_PointsOfContact.push_back(pointsOfContact.at(i));
					}

					//Do we need to notify a callback?
					if (dynamicColliderA->GetCallback() != nullptr || dynamicColliderB->GetCallback() != nullptr)
					{
						//Create the collision event
						CollisionEvent collisionEvent(dynamicColliderA, dynamicColliderB, pointsOfContact);

						//Is there a callback set for the first collider?
						if (dynamicColliderA->GetCallback() != nullptr)
						{
							dynamicColliderA->GetCallback()->CollisionDetected(&collisionEvent);
						}

						//Is there a callback set for the second collider?
						if (dynamicColliderB->GetCallback() != nullptr)
						{
							dynamicColliderB->GetCallback()->CollisionDetected(&collisionEvent);
						}

						//Did either callback request that the collision be resolved?
						if (collisionEvent.resolveCollision == true)
						{
							ResolveCollision(*itA, *itB, collisionEvent.pointsOfContact);
						}
					}
				}
				
				//Increment iterator B
				itB++;
			}

			//Get the iterator to the first collider
			std::vector<Collider*>::iterator itStatic = m_StaticColliders.activeColliders.begin();

			while (itStatic != m_StaticColliders.activeColliders.end())
			{
				//Cast iteratorB to a Collider*
				Collider* staticColliderB = *itStatic;

				//Check if dynamicColliderB is active
				if (staticColliderB->IsActive() == false)
				{
					m_StaticColliders.inactiveColliders.push_back(staticColliderB);
					itStatic = m_StaticColliders.activeColliders.erase(itStatic);
					continue;
				}

				//Used to keep track of the point of contact between two colliders
				std::vector<Vector2> pointsOfContact;

				//Was there a collision?
				if (CheckCollision(dynamicColliderA, staticColliderB, pointsOfContact) == true)
				{
					//Flag both colliders for a collision
                    dynamicColliderA->SetIsColliding(true);
                    staticColliderB->SetIsColliding(true);

					//Add the point of contact to the vector
					for (int i = 0; i < pointsOfContact.size(); i++)
					{
						m_PointsOfContact.push_back(pointsOfContact.at(i));
					}

					//Do we need to notify a callback?
					if (dynamicColliderA->GetCallback() != nullptr || staticColliderB->GetCallback() != nullptr)
					{
						//Create the collision event
						CollisionEvent collisionEvent(dynamicColliderA, staticColliderB, pointsOfContact);

						//Is there a callback set for the first collider?
						if (dynamicColliderA->GetCallback() != nullptr)
						{
							dynamicColliderA->GetCallback()->CollisionDetected(&collisionEvent);
						}

						//Is there a callback set for the second collider?
						if (staticColliderB->GetCallback() != nullptr)
						{
							staticColliderB->GetCallback()->CollisionDetected(&collisionEvent);
						}

						//Did either callback request that the collision be resolved?
						if (collisionEvent.resolveCollision == true)
						{
							ResolveCollision(*itA, *itStatic, collisionEvent.pointsOfContact);
						}
					}
				}

				//Increment iterator static
				itStatic++;
			}

			//Increment iterator A
			itA++;
		}

        //Update the collider groups
        m_DynamicColliders.PostCollisionUpdate(delta);
        m_StaticColliders.PostCollisionUpdate(delta);

		//Unlock colliders, they can now be unregistered
		m_Lock = false;
	}

	void CollisionManager::Draw()
	{
#if DEBUG && DEBUG_DRAW_COLLIDERS
		m_DynamicColliders.DebugDraw();
		m_StaticColliders.DebugDraw();

		//Draw the points of contact
		for (int i = 0; i < m_PointsOfContact.size(); i++)
		{
			Vector2 position = m_PointsOfContact.at(i);
			Services::GetGraphics()->DrawCircle(position, 3.0f, Vector2(0.5f, 0.5f), Color::YellowColor(), true);
		}
#endif
	}

	unsigned int CollisionManager::GetNumberOfStaticColliders()
	{
		return m_StaticColliders.Count();
	}

	unsigned int CollisionManager::GetNumberOfActiveStaticColliders()
	{
		return m_StaticColliders.activeColliders.size();
	}

	unsigned int CollisionManager::GetNumberOfDynamicColliders()
	{
		return m_DynamicColliders.Count();
	}

	unsigned int CollisionManager::GetNumberOfActiveDynamicColliders()
	{
		return m_DynamicColliders.activeColliders.size();
	}

	void CollisionManager::RegisterCollider(Collider* aCollider)
	{
		if (aCollider->GetType() == Collider::Dynamic)
		{
			m_DynamicColliders.Add(aCollider);
		}
		else if (aCollider->GetType() == Collider::Static)
		{
			m_StaticColliders.Add(aCollider);
		}
	}

	void CollisionManager::UnregisterCollider(Collider* aCollider)
	{
		//If you are crashing here, it means you are trying to unregister a collider,
		//from a collision callback function, that is not allowed.
		assert(m_Lock == false);

		if (aCollider->GetType() == Collider::Dynamic)
		{
			m_DynamicColliders.Remove(aCollider);
		}
		else if (aCollider->GetType() == Collider::Static)
		{
			m_StaticColliders.Remove(aCollider);
		}
	}

	bool CollisionManager::CheckCollision(Collider * colliderA, Collider * colliderB, std::vector<Vector2>& pointsOfContact)
	{
		bool collision = false;

		//Check the collision filters and see if these colliders are allowed to collide
		if (colliderA->GetFilter().CanCollide(colliderB->GetFilter()) == true)
		{
			//Are both colliders circles?
			if (colliderA->GetShape() == Collider::Circle && colliderB->GetShape() == Collider::Circle)
			{
				collision = CheckCollision(static_cast<CircleCollider*>(colliderA), static_cast<CircleCollider*>(colliderB), pointsOfContact);
			}
			//Is one collider a point and the other collider a circle?
			else if (colliderA->GetShape() == Collider::Point && colliderB->GetShape() == Collider::Circle)
			{
				collision = CheckCollision(static_cast<PointCollider*>(colliderA), static_cast<CircleCollider*>(colliderB), pointsOfContact);
			}
			//Is one collider a circle and the other collider a point?
			else if (colliderA->GetShape() == Collider::Circle && colliderB->GetShape() == Collider::Point)
			{
				collision = CheckCollision(static_cast<PointCollider*>(colliderB), static_cast<CircleCollider*>(colliderA), pointsOfContact);
			}
			//Are both colliders lines?
			else if (colliderA->GetShape() == Collider::Line && colliderB->GetShape() == Collider::Line)
			{
				collision = CheckCollision(static_cast<LineCollider*>(colliderA), static_cast<LineCollider*>(colliderB), pointsOfContact);
			}
			//Is one collider a line and the other collider a circle?
			else if (colliderA->GetShape() == Collider::Line && colliderB->GetShape() == Collider::Circle)
			{
				collision = CheckCollision(static_cast<LineCollider*>(colliderA), static_cast<CircleCollider*>(colliderB), pointsOfContact);
			}
			//Is one collider a circle and the other collider a line?
			else if (colliderA->GetShape() == Collider::Circle && colliderB->GetShape() == Collider::Line)
			{
				collision = CheckCollision(static_cast<LineCollider*>(colliderB), static_cast<CircleCollider*>(colliderA), pointsOfContact);
			}
			//Is one collider a line and the other collider a point?
			else if (colliderA->GetShape() == Collider::Line && colliderB->GetShape() == Collider::Point)
			{
				collision = CheckCollision(static_cast<LineCollider*>(colliderA), static_cast<PointCollider*>(colliderB), pointsOfContact);
			}
			//Is one collider a point and the other collider a line?
			else if (colliderA->GetShape() == Collider::Point && colliderB->GetShape() == Collider::Line)
			{
				collision = CheckCollision(static_cast<LineCollider*>(colliderB), static_cast<PointCollider*>(colliderA), pointsOfContact);
			}
			//Are both colliders rectangles?
			else if (colliderA->GetShape() == Collider::OrientedRectangle && colliderB->GetShape() == Collider::OrientedRectangle)
			{
				collision = CheckCollision(static_cast<OrientedRectangleCollider*>(colliderA), static_cast<OrientedRectangleCollider*>(colliderB), pointsOfContact);
			}
			//Is one collider a circle and the other collider a rectangle?
			else if (colliderA->GetShape() == Collider::Circle && colliderB->GetShape() == Collider::OrientedRectangle)
			{
				collision = CheckCollision(static_cast<CircleCollider*>(colliderA), static_cast<OrientedRectangleCollider*>(colliderB), pointsOfContact);
			}
			//Is one collider a rectangle and the other collider a circle?
			else if (colliderB->GetShape() == Collider::Circle && colliderA->GetShape() == Collider::OrientedRectangle)
			{
				collision = CheckCollision(static_cast<CircleCollider*>(colliderB), static_cast<OrientedRectangleCollider*>(colliderA), pointsOfContact);
			}
			//Is one collider a point and the other collider a rectangle?
			else if (colliderA->GetShape() == Collider::Point && colliderB->GetShape() == Collider::OrientedRectangle)
			{
				collision = CheckCollision(static_cast<PointCollider*>(colliderA), static_cast<OrientedRectangleCollider*>(colliderB), pointsOfContact);
			}
			//Is one collider a rectangle and the other collider a point?
			else if (colliderB->GetShape() == Collider::Point && colliderA->GetShape() == Collider::OrientedRectangle)
			{
				collision = CheckCollision(static_cast<PointCollider*>(colliderB), static_cast<OrientedRectangleCollider*>(colliderA), pointsOfContact);
			}
			//Is one collider a point and the other collider a rectangle?
			else if (colliderA->GetShape() == Collider::Line && colliderB->GetShape() == Collider::OrientedRectangle)
			{
				collision = CheckCollision(static_cast<LineCollider*>(colliderA), static_cast<OrientedRectangleCollider*>(colliderB), pointsOfContact);
			}
			//Is one collider a rectangle and the other collider a point?
			else if (colliderB->GetShape() == Collider::Line && colliderA->GetShape() == Collider::OrientedRectangle)
			{
				collision = CheckCollision(static_cast<LineCollider*>(colliderB), static_cast<OrientedRectangleCollider*>(colliderA), pointsOfContact);
			}
			//Is one collider an oriented rectangle and the other collider an axis-aligned rectangle?
			else if (colliderA->GetShape() == Collider::OrientedRectangle && colliderB->GetShape() == Collider::AxisAlignedRectangle)
			{
				collision = CheckCollision(static_cast<OrientedRectangleCollider*>(colliderA), static_cast<AxisAlignedRectangleCollider*>(colliderB), pointsOfContact);
			}
			//Is one collider an axis-aligned rectangle and the other collider an oriented rectangle?
			else if (colliderB->GetShape() == Collider::OrientedRectangle && colliderA->GetShape() == Collider::AxisAlignedRectangle)
			{
				collision = CheckCollision(static_cast<OrientedRectangleCollider*>(colliderB), static_cast<AxisAlignedRectangleCollider*>(colliderA), pointsOfContact);
			}
			//Are both colliders axis-aligned rectangles?
			else if (colliderA->GetShape() == Collider::AxisAlignedRectangle && colliderB->GetShape() == Collider::AxisAlignedRectangle)
			{
				collision = CheckCollision(static_cast<AxisAlignedRectangleCollider*>(colliderA), static_cast<AxisAlignedRectangleCollider*>(colliderB), pointsOfContact);
			}
			//Is one collider a circle and the other collider an axis-aligned rectangle?
			else if (colliderA->GetShape() == Collider::Circle && colliderB->GetShape() == Collider::AxisAlignedRectangle)
			{
				collision = CheckCollision(static_cast<CircleCollider*>(colliderA), static_cast<AxisAlignedRectangleCollider*>(colliderB), pointsOfContact);
			}
			//Is one collider an axis-aligned rectangle and the other collider a circle?
			else if (colliderB->GetShape() == Collider::Circle && colliderA->GetShape() == Collider::AxisAlignedRectangle)
			{
				collision = CheckCollision(static_cast<CircleCollider*>(colliderB), static_cast<AxisAlignedRectangleCollider*>(colliderA), pointsOfContact);
			}
			//Is one collider a point and the other collider an axis-aligned rectangle?
			else if (colliderA->GetShape() == Collider::Point && colliderB->GetShape() == Collider::AxisAlignedRectangle)
			{
				collision = CheckCollision(static_cast<PointCollider*>(colliderA), static_cast<AxisAlignedRectangleCollider*>(colliderB), pointsOfContact);
			}
			//Is one collider an axis-aligned rectangle and the other collider a point?
			else if (colliderB->GetShape() == Collider::Point && colliderA->GetShape() == Collider::AxisAlignedRectangle)
			{
				collision = CheckCollision(static_cast<PointCollider*>(colliderB), static_cast<AxisAlignedRectangleCollider*>(colliderA), pointsOfContact);
			}
			//Is one collider a point and the other collider an axis-aligned rectangle?
			else if (colliderA->GetShape() == Collider::Line && colliderB->GetShape() == Collider::AxisAlignedRectangle)
			{
				collision = CheckCollision(static_cast<LineCollider*>(colliderA), static_cast<AxisAlignedRectangleCollider*>(colliderB), pointsOfContact);
			}
			//Is one collider an axis-aligned rectangle and the other collider a point?
			else if (colliderB->GetShape() == Collider::Line && colliderA->GetShape() == Collider::AxisAlignedRectangle)
			{
				collision = CheckCollision(static_cast<LineCollider*>(colliderB), static_cast<AxisAlignedRectangleCollider*>(colliderA), pointsOfContact);
			}
		}

		return collision;
	}

	bool CollisionManager::CheckCollision(CircleCollider * a, CircleCollider * b, std::vector<Vector2>& pointsOfContact)
	{
		if (a->IsActive() == true && b->IsActive() == true)
		{
			//Calculate the collision specifics
			Vector2 positionA = a->GetWorldPosition();
			Vector2 positionB = b->GetWorldPosition();
			float distance = positionA.Distance(positionB);
			float radii = a->GetRadius() + b->GetRadius();
			float overlap = radii - distance;
			Vector2 normal = Vector2(positionB - positionA) / distance;
			pointsOfContact.push_back(positionB - normal * (b->GetRadius() - overlap * 0.5f));

			//If the distance squared is less than the radii squared, then there's a collision
			return distance < radii;
		}
		return false;
	}

	bool CollisionManager::CheckCollision(PointCollider* a, CircleCollider* b, std::vector<Vector2>& pointsOfContact)
	{
		if (a->IsActive() == true && b->IsActive() == true)
		{
			Vector2 positionA = a->GetWorldPosition();
			Vector2 positionB = b->GetWorldPosition();
			float distanceSquared = positionA.DistanceSquared(positionB);
			float radiusSquared = b->GetRadius() * b->GetRadius();
			pointsOfContact.push_back(positionA);

			//If the distance squared is less than the radii squared, then there's a collision
			return distanceSquared < radiusSquared;
		}
		return false;
	}

	bool CollisionManager::CheckCollision(LineCollider * a, LineCollider * b, std::vector<Vector2>& pointsOfContact)
	{
		Vector2 intersection;
		bool isIntersecting = CheckLineIntersection(a->GetRelative(), b->GetRelative(), &intersection);
		if (isIntersecting == true)
		{
			pointsOfContact.push_back(intersection);
		}
		
		return isIntersecting;
	}

	bool CollisionManager::CheckCollision(LineCollider * a, CircleCollider * b, std::vector<Vector2>& pointsOfContact)
	{
		if (a->IsActive() == true && b->IsActive() == true)
		{
			//Calculate the closest point
			Vector2 closestPoint = ClosestPointOnLine(a->GetRelative(), b->GetWorldPosition());

			//Calculate the distance between the closest point and the center of the ball
			float distance = b->GetWorldPosition().Distance(closestPoint);
			float overlap = b->GetRadius() - distance;
			Vector2 normal = Vector2(b->GetWorldPosition() - closestPoint) / distance;
			pointsOfContact.push_back(b->GetWorldPosition() - normal * (b->GetRadius() - overlap * 0.5f));

			//If the distance is less than the radius, then there's a collision
			return distance < b->GetRadius();
		}

		return false;
	}

	bool CollisionManager::CheckCollision(LineCollider * a, PointCollider* b, std::vector<Vector2>& pointsOfContact)
	{
		if (a->IsActive() == true && b->IsActive() == true)
		{
			//Calculate the closest point of the line
			Vector2 lineStart = a->GetRelative().start;
			Vector2 lineEnd = a->GetRelative().end;

			//Calculate the circle vector        
			Vector2 circleVector = b->GetWorldPosition() - lineStart;

			//Calculate the line segment vector        
			Vector2 lineVector = lineEnd - lineStart;

			//Normalize the line segment vector        
			Vector2 normalizedVector = lineVector.Normalized();

			//Calculate the dot product between the circle vector and the normalized line segment vector       
			float magnitude = normalizedVector.DotProduct(circleVector);

			//Calculate the projection using the result of the dot product and multiply it by the normalized line segment        
			Vector2 projection = normalizedVector * magnitude;

			//Calculate the closest point on the line segment, by adding the project vector to the line start vector        
			Vector2 closestPoint = lineStart + projection;

			//Ensure the closest point is actually on the line
			closestPoint = Math::Clamp(closestPoint, lineStart, lineEnd);

			//Calculate the distance between the closest point and the center of the ball
			float distance = b->GetWorldPosition().Distance(closestPoint);
			pointsOfContact.push_back(closestPoint);

			//If the distance is with a pixel
			return distance < 1.0f;
		}

		return false;
	}

	bool CollisionManager::CheckCollision(OrientedRectangleCollider * a, OrientedRectangleCollider* b, std::vector<Vector2>& pointsOfContact)
	{
		if (a->IsActive() == true && b->IsActive() == true)
		{
			LineSegment edgeSegmentsA[4];
			a->GetEdges(edgeSegmentsA);

			LineSegment edgeSegmentsB[4];
			b->GetEdges(edgeSegmentsB);

			bool isIntersecting = false;
			Vector2 pointOfContact;

			//Loop through the Segments and check for intersection
			for (int a = 0; a < 4; a++)
			{
				for (int b = 0; b < 4; b++)
				{
					//Was there collision
					if (CheckLineIntersection(edgeSegmentsA[a], edgeSegmentsB[b], &pointOfContact) == true)
					{
						if (DoesContainPointOfContact(pointOfContact, pointsOfContact) == false)
						{
							pointsOfContact.push_back(pointOfContact);
							isIntersecting = true;
						}
					}
				}
			}

			return isIntersecting;
		}
		return false;
	}

	bool CollisionManager::CheckCollision(CircleCollider * a, OrientedRectangleCollider* b, std::vector<Vector2>& pointsOfContact)
	{
		if (a->IsActive() == true && b->IsActive() == true)
		{
			Vector2 closestPoint = Vector2::Zero;
			Vector2 circlePosition = a->GetWorldPosition();
			Vector2 rectCenter = b->GetWorldPosition();
			float rectLeftEdge = rectCenter.x - b->GetWidth() * 0.5f;
			float rectRightEdge = rectCenter.x + b->GetWidth() * 0.5f;
			float rectTopEdge = rectCenter.y + b->GetHeight() * 0.5f;
			float rectBottomEdge = rectCenter.y - b->GetHeight() * 0.5f;
			float rectRadians = b->GetWorldRadians();

			//Rotate circle's center point
			float unrotatedCircleX = cosf(rectRadians) * (circlePosition.x - rectCenter.x) - sinf(rectRadians) * (circlePosition.y - rectCenter.y) + rectCenter.x;
			float unrotatedCircleY = sinf(rectRadians) * (circlePosition.x - rectCenter.x) + cosf(rectRadians) * (circlePosition.y - rectCenter.y) + rectCenter.y;

			//Find the unrotated closestPoint.x from center of unrotated circle
			if (unrotatedCircleX < rectLeftEdge) 
			{
				closestPoint.x = rectLeftEdge;
			}
			else if (unrotatedCircleX > rectRightEdge) 
			{
				closestPoint.x = rectRightEdge;
			}
			else 
			{
				closestPoint.x = unrotatedCircleX;
			}

			//Find the unrotated closestPoint.y from center of unrotated circle
			if (unrotatedCircleY < rectBottomEdge) 
			{
				closestPoint.y = rectBottomEdge;
			}
			else if (unrotatedCircleY > rectTopEdge) 
			{
				closestPoint.y = rectTopEdge;
			}
			else 
			{
				closestPoint.y = unrotatedCircleY;
			}

			//Calculate the distance between the closest point and the unrotated circle
			float distance = Vector2(unrotatedCircleX, unrotatedCircleY).Distance(closestPoint);
			float overlap = a->GetRadius() - distance;
			Vector2 normal = Vector2(b->GetWorldPosition() - a->GetWorldPosition()) / Vector2(b->GetWorldPosition()).Distance(a->GetWorldPosition());
			pointsOfContact.push_back(a->GetWorldPosition() + normal * (a->GetRadius() - overlap));
			return distance < a->GetRadius();
		}

		return false;
	}

	bool CollisionManager::CheckCollision(PointCollider * a, OrientedRectangleCollider* b, std::vector<Vector2>& pointsOfContact)
	{
		if (a->IsActive() == true && b->IsActive() == true)
		{
			Vector2 pointPosition = a->GetWorldPosition();
			Vector2 rectCenter = b->GetWorldPosition();
			float rectLeftEdge = rectCenter.x - b->GetWidth() * 0.5f;
			float rectRightEdge = rectCenter.x + b->GetWidth() * 0.5f;
			float rectTopEdge = rectCenter.y + b->GetHeight() * 0.5f;
			float rectBottomEdge = rectCenter.y - b->GetHeight() * 0.5f;
			float rectRadians = b->GetWorldRadians();

			//Rotate point
			float unrotatedPointX = cosf(rectRadians) * (pointPosition.x - rectCenter.x) - sinf(rectRadians) * (pointPosition.y - rectCenter.y) + rectCenter.x;
			float unrotatedPointY = sinf(rectRadians) * (pointPosition.x - rectCenter.x) + cosf(rectRadians) * (pointPosition.y - rectCenter.y) + rectCenter.y;

			//Set the point of collision
			pointsOfContact.push_back(pointPosition);

			return unrotatedPointX > rectLeftEdge && unrotatedPointX < rectRightEdge && unrotatedPointY > rectBottomEdge && unrotatedPointY < rectTopEdge;
		}

		return false;
	}

	bool CollisionManager::CheckCollision(LineCollider * a, OrientedRectangleCollider* b, std::vector<Vector2>& pointsOfContact)
	{
		if (a->IsActive() == true && b->IsActive() == true)
		{
			Vector2 rectCenter = b->GetWorldPosition();
			float rectLeftEdge = rectCenter.x - b->GetWidth() * 0.5f;
			float rectRightEdge = rectCenter.x + b->GetWidth() * 0.5f;
			float rectTopEdge = rectCenter.y + b->GetHeight() * 0.5f;
			float rectBottomEdge = rectCenter.y - b->GetHeight() * 0.5f;
			float rectRadians = b->GetWorldRadians();

			//Rotate the start point
			Vector2 unrotatedStart;
			unrotatedStart.x = cosf(rectRadians) * (a->GetRelative().start.x - rectCenter.x) - sinf(rectRadians) * (a->GetRelative().start.y - rectCenter.y) + rectCenter.x;
			unrotatedStart.y = sinf(rectRadians) * (a->GetRelative().start.x - rectCenter.x) + cosf(rectRadians) * (a->GetRelative().start.y - rectCenter.y) + rectCenter.y;

			//Rotate the end point
			Vector2 unrotatedEnd;
			unrotatedEnd.x = cosf(rectRadians) * (a->GetRelative().end.x - rectCenter.x) - sinf(rectRadians) * (a->GetRelative().end.y - rectCenter.y) + rectCenter.x;
			unrotatedEnd.y = sinf(rectRadians) * (a->GetRelative().end.x - rectCenter.x) + cosf(rectRadians) * (a->GetRelative().end.y - rectCenter.y) + rectCenter.y;

			//Segments array
			LineSegment segments[4] =
			{
				LineSegment(Vector2(rectLeftEdge, rectTopEdge), Vector2(rectLeftEdge, rectBottomEdge)),
				LineSegment(Vector2(rectRightEdge, rectTopEdge), Vector2(rectRightEdge, rectBottomEdge)),
				LineSegment(Vector2(rectLeftEdge, rectTopEdge), Vector2(rectRightEdge, rectTopEdge)),
				LineSegment(Vector2(rectLeftEdge, rectBottomEdge), Vector2(rectRightEdge, rectBottomEdge)),
			};

			bool isIntersecting = false;
			Vector2 pointOfContact;

			//Loop through the Segments and check for intersection
			for (int i = 0; i < 4; i++)
			{
				
				//Was there collision?
				if (CheckLineIntersection(LineSegment(unrotatedStart, unrotatedEnd), segments[i], &pointOfContact) == true)
				{
					pointsOfContact.push_back(pointOfContact);
					isIntersecting = true;
				}
			}
			return isIntersecting;
		}
		return false;
	}

	bool CollisionManager::CheckCollision(OrientedRectangleCollider* a, AxisAlignedRectangleCollider* b, std::vector<Vector2>& pointsOfContact)
	{
		if (a->IsActive() == true && b->IsActive() == true)
		{
			LineSegment edgeSegmentsA[4];
			a->GetEdges(edgeSegmentsA);

			LineSegment edgeSegmentsB[4];
			b->GetEdges(edgeSegmentsB);

			bool isIntersecting = false;
			Vector2 pointOfContact;

			//Loop through the Segments and check for intersection
			for (int a = 0; a < 4; a++)
			{
				for (int b = 0; b < 4; b++)
				{
					//Was there collision
					if (CheckLineIntersection(edgeSegmentsA[a], edgeSegmentsB[b], &pointOfContact) == true)
					{
						if (DoesContainPointOfContact(pointOfContact, pointsOfContact) == false)
						{
							pointsOfContact.push_back(pointOfContact);
							isIntersecting = true;
						}
					}
				}
			}

			return isIntersecting;
		}

		return false;
	}

	bool CollisionManager::CheckCollision(AxisAlignedRectangleCollider* a, AxisAlignedRectangleCollider* b, std::vector<Vector2>& pointsOfContact)
	{
		if (a->IsActive() == true && b->IsActive() == true)
		{
			LineSegment edgeSegmentsA[4];
			a->GetEdges(edgeSegmentsA);

			LineSegment edgeSegmentsB[4];
			b->GetEdges(edgeSegmentsB);

			bool isIntersecting = false;
			Vector2 pointOfContact;

			//Loop through the Segments and check for intersection
			for (int a = 0; a < 4; a++)
			{
				for (int b = 0; b < 4; b++)
				{
					//Was there collision
					if (CheckLineIntersection(edgeSegmentsA[a], edgeSegmentsB[b], &pointOfContact) == true)
					{
						if (DoesContainPointOfContact(pointOfContact, pointsOfContact) == false)
						{
							pointsOfContact.push_back(pointOfContact);
							isIntersecting = true;
						}
					}
				}
			}

            if (isIntersecting == true)
            {
                Vector2 positionA = a->GetWorldPosition();
                Vector2 positionB = b->GetWorldPosition();
                Vector2 minA = a->GetMin();
                Vector2 maxA = a->GetMax();
                Vector2 minB = b->GetMin();
                Vector2 maxB = b->GetMax();
                Vector2 overlappingSize = Vector2::Min(maxA, maxB) - Vector2::Max(minA, minB);
                float widthA = maxA.x - minA.x;
                float heightA = maxA.y - minA.y;
                float widthB = maxB.x - minB.x;
                float heightB = maxB.y - minB.y;

                //Minkowski sum
                float wy = (widthA + widthB) * (positionB.y - positionA.y);
                float hx = (heightA + heightB) * (positionB.x - positionA.x);

                //Determine which edge for each Collider is colliding
                if (wy > hx)
                {
                    if (wy > -hx)
                    {
                        a->SetEdgeCollision(AxisAlignedRectangleCollider::TopEdge);
                        b->SetEdgeCollision(AxisAlignedRectangleCollider::BottomEdge);
                    }
                    else
                    {
                        a->SetEdgeCollision(AxisAlignedRectangleCollider::LeftEdge);
                        b->SetEdgeCollision(AxisAlignedRectangleCollider::RightEdge);
                    }
                }
                else
                {
                    if (wy > -hx)
                    {
                        a->SetEdgeCollision(AxisAlignedRectangleCollider::RightEdge);
                        b->SetEdgeCollision(AxisAlignedRectangleCollider::LeftEdge);
                    }
                    else
                    {
                        a->SetEdgeCollision(AxisAlignedRectangleCollider::BottomEdge);
                        b->SetEdgeCollision(AxisAlignedRectangleCollider::TopEdge);
                    }

                }
            }

			return isIntersecting;
		}

		return false;
	}

	bool CollisionManager::CheckCollision(CircleCollider* a, AxisAlignedRectangleCollider* b, std::vector<Vector2>& pointsOfContact)
	{
		if (a->IsActive() == true && b->IsActive() == true)
		{
			Vector2 closestPoint = Vector2::Zero;
			Vector2 circlePosition = a->GetWorldPosition();
			Vector2 rectCenter = b->GetWorldPosition();
			float rectRadians = b->GetWorldRadians();
			float rectWidth = fabsf(cosf(rectRadians)) * b->GetWidth() + fabsf(sinf(rectRadians)) * b->GetHeight();
			float rectHeight = fabsf(cosf(rectRadians)) * b->GetHeight() + fabsf(sinf(rectRadians)) * b->GetWidth();
			float rectLeftEdge = rectCenter.x - rectWidth * 0.5f;
			float rectRightEdge = rectCenter.x + rectWidth * 0.5f;
			float rectTopEdge = rectCenter.y + rectHeight * 0.5f;
			float rectBottomEdge = rectCenter.y - rectHeight * 0.5f;

			//Find the closestPoint.x from center of the circle
			if (circlePosition.x < rectLeftEdge)
			{
				Log::Message("LeftEdge");
				closestPoint.x = rectLeftEdge;
			}
			else if (circlePosition.x > rectRightEdge)
			{
				Log::Message("RightEdge");
				closestPoint.x = rectRightEdge;
			}
			else
			{
				closestPoint.x = circlePosition.x;
			}

			//Find the closestPoint.y from center of the circle
			if (circlePosition.y < rectBottomEdge)
			{
				Log::Message("BottomEdge");
				closestPoint.y = rectBottomEdge;
			}
			else if (circlePosition.y > rectTopEdge)
			{
				Log::Message("TopEdge");
				closestPoint.y = rectTopEdge;
			}
			else
			{
				closestPoint.y = circlePosition.y;
			}

			//Calculate the distance between the closest point and the unrotated circle
			float distance = circlePosition.Distance(closestPoint);
			float overlap = a->GetRadius() - distance;
			Vector2 normal = Vector2(rectCenter - circlePosition) / Vector2(rectCenter).Distance(circlePosition);
			pointsOfContact.push_back(circlePosition + normal * (a->GetRadius() - overlap));
			bool isCollision = distance < a->GetRadius();

			if (isCollision == true)
			{

			}

    		return isCollision;
		}

		return false;
	}

	bool CollisionManager::CheckCollision(PointCollider* a, AxisAlignedRectangleCollider* b, std::vector<Vector2>& pointsOfContact)
	{
		if (a->IsActive() == true && b->IsActive() == true)
		{
			Vector2 pointPosition = a->GetWorldPosition();
			Vector2 rectCenter = b->GetWorldPosition();
			float rectRadians = b->GetWorldRadians();
			float rectWidth = fabsf(cosf(rectRadians)) * b->GetWidth() + fabsf(sinf(rectRadians)) * b->GetHeight();
			float rectHeight = fabsf(cosf(rectRadians)) * b->GetHeight() + fabsf(sinf(rectRadians)) * b->GetWidth();
			float rectLeftEdge = rectCenter.x - rectWidth * 0.5f;
			float rectRightEdge = rectCenter.x + rectWidth * 0.5f;
			float rectTopEdge = rectCenter.y + rectHeight * 0.5f;
			float rectBottomEdge = rectCenter.y - rectHeight * 0.5f;

			//Set the point of collision
			pointsOfContact.push_back(pointPosition);

			return pointPosition.x > rectLeftEdge && pointPosition.x < rectRightEdge && pointPosition.y > rectBottomEdge && pointPosition.y < rectTopEdge;
		}

		return false;
	}

	bool CollisionManager::CheckCollision(LineCollider* a, AxisAlignedRectangleCollider* b, std::vector<Vector2>& pointsOfContact)
	{
		if (a->IsActive() == true && b->IsActive() == true)
		{
			Vector2 rectCenter = b->GetWorldPosition();
			float rectRadians = b->GetWorldRadians();
			float rectWidth = fabsf(cosf(rectRadians)) * b->GetWidth() + fabsf(sinf(rectRadians)) * b->GetHeight();
			float rectHeight = fabsf(cosf(rectRadians)) * b->GetHeight() + fabsf(sinf(rectRadians)) * b->GetWidth();
			float rectLeftEdge = rectCenter.x - rectWidth * 0.5f;
			float rectRightEdge = rectCenter.x + rectWidth * 0.5f;
			float rectTopEdge = rectCenter.y + rectHeight * 0.5f;
			float rectBottomEdge = rectCenter.y - rectHeight * 0.5f;

			//Segments array
			LineSegment edgeSegments[4] =
			{
				LineSegment(Vector2(rectLeftEdge, rectTopEdge), Vector2(rectLeftEdge, rectBottomEdge)),
				LineSegment(Vector2(rectRightEdge, rectTopEdge), Vector2(rectRightEdge, rectBottomEdge)),
				LineSegment(Vector2(rectLeftEdge, rectTopEdge), Vector2(rectRightEdge, rectTopEdge)),
				LineSegment(Vector2(rectLeftEdge, rectBottomEdge), Vector2(rectRightEdge, rectBottomEdge)),
			};

			bool isIntersecting = false;
			Vector2 pointOfContact;

			//Loop through the Segments and check for intersection
			for (int i = 0; i < 4; i++)
			{

				//Was there collision?
				if (CheckLineIntersection(a->GetRelative(), edgeSegments[i], &pointOfContact) == true)
				{
					pointsOfContact.push_back(pointOfContact);
					isIntersecting = true;
				}
			}
			return isIntersecting;
		}
		return false;
	}

	void CollisionManager::ResolveCollision(Collider* colliderA, Collider* colliderB, std::vector<Vector2>& pointsOfContact)
	{
		//Are both colliders axis-aligned rectangles?
		if (colliderA->GetShape() == Collider::AxisAlignedRectangle && colliderB->GetShape() == Collider::AxisAlignedRectangle)
		{
			ResolveCollision(static_cast<AxisAlignedRectangleCollider*>(colliderA), static_cast<AxisAlignedRectangleCollider*>(colliderB), pointsOfContact);
		}
		//Not all the colliders are currently setup to resolve collisions, output an error message
		else
		{
			Log::Error(false, Log::Verbosity_Debug, "ResolveCollision not implemented for %s, and %s", colliderA->GetShapeString().c_str(), colliderB->GetShapeString().c_str()); 
		}
	}

    void CollisionManager::ResolveCollision(AxisAlignedRectangleCollider* colliderA, AxisAlignedRectangleCollider* colliderB, std::vector<Vector2>& pointsOfContact)
    {
        Vector2 positionA = colliderA->GetWorldPosition();
        Vector2 positionB = colliderB->GetWorldPosition();
        Vector2 linearVelocityA = colliderA->GetGameObject()->GetLinearVelocity();
        Vector2 linearVelocityB = colliderB->GetGameObject()->GetLinearVelocity();
        Vector2 minA = colliderA->GetMin();
        Vector2 maxA = colliderA->GetMax();
        Vector2 minB = colliderB->GetMin();
        Vector2 maxB = colliderB->GetMax();
        Vector2 overlappingSize = Vector2::Min(maxA, maxB) - Vector2::Max(minA, minB);
        float widthA = maxA.x - minA.x;
        float heightA = maxA.y - minA.y;
        float widthB = maxB.x - minB.x;
        float heightB = maxB.y - minB.y;

        //Minkowski sum
        float wy = (widthA + widthB) * (positionB.y - positionA.y);
        float hx = (heightA + heightB) * (positionB.x - positionA.x);

        //Collider A is static (only move collider B)
        if (colliderA->GetType() == Collider::Static)
        {
            if (wy > hx)
            {
                if (wy > -hx)
                {
                    //Bottom
                    linearVelocityB.y = 0.0f;
                    positionB.y += overlappingSize.y;
                }
                else
                {
                    //Right
                    linearVelocityB.x = 0.0f;
                    positionB.x -= overlappingSize.x;
                }
            }
            else
            {
                if (wy > -hx)
                {
                    //Left
                    linearVelocityB.x = 0.0f;
                    positionB.x += overlappingSize.x;
                }
                else
                {
                    //Top
                    linearVelocityB.y = 0.0f;
                    positionB.y -= overlappingSize.y;
                }
            }
        }
        //Collider B is static (only move collider A)
        else if (colliderB->GetType() == Collider::Static)
        {
            if (wy > hx)
            {
                if (wy > -hx)
                {
                    //Top
                    linearVelocityA.y = 0.0f;
                    positionA.y -= overlappingSize.y;
                }
                else
                {
                    //Left
                    linearVelocityA.x = 0.0f;
                    positionA.x += overlappingSize.x;
                }
            }
            else
            {
                if (wy > -hx)
                {
                    //Right
                    linearVelocityA.x = 0.0f;
                    positionA.x -= overlappingSize.x;
                }
                else
                {
                    //Bottom
                    linearVelocityA.y = 0.0f;
                    positionA.y += overlappingSize.y;
                }
            }
        }
        //Both colliders are dynamic (move both)
        else
        {
            if (wy > hx)
            {
                if (wy > -hx)
                {
                    //Top
                    linearVelocityA.y = 0.0f;
                    positionA.y -= overlappingSize.y * 0.5f;

					//Bottom
					linearVelocityB.y = 0.0f;
                    positionB.y += overlappingSize.y * 0.5f;
                }
                else
                {
                    //Left
                    linearVelocityA.x = 0.0f;
					positionA.x += overlappingSize.x * 0.5f;

					//Right
                    linearVelocityB.x = 0.0f;
                    positionB.x -= overlappingSize.x * 0.5f;
                }
            }
            else
            {
                if (wy > -hx)
                {
                    //Right
                    linearVelocityA.x = 0.0f;
					positionA.x -= overlappingSize.x * 0.5f;

					//Left
                    linearVelocityB.x = 0.0f;
                    positionB.x += overlappingSize.x * 0.5f;
                }
                else
                {
                    //Bottom
                    linearVelocityA.y = 0.0f;
					positionA.y += overlappingSize.y * 0.5f;

					//Top
                    linearVelocityB.y = 0.0f;
                    positionB.y -= overlappingSize.y * 0.5f;
                }
            }
        }

        //Set the position and linear velocity of collider A
        colliderA->GetGameObject()->SetLinearVelocity(linearVelocityA);
        colliderA->GetGameObject()->SetPosition(positionA);

        //Set the position and linear velocity of collider B
        colliderB->GetGameObject()->SetLinearVelocity(linearVelocityB);
        colliderB->GetGameObject()->SetPosition(positionB);
    }

	bool CollisionManager::CheckLineIntersection(const LineSegment& lineSegmentA, const LineSegment& lineSegmentB, Vector2 * intersection)
	{
		float numeratorA = ((lineSegmentB.end.x - lineSegmentB.start.x)*(lineSegmentA.start.y - lineSegmentB.start.y) - (lineSegmentB.end.y - lineSegmentB.start.y)*(lineSegmentA.start.x - lineSegmentB.start.x));
		float numeratorB = ((lineSegmentA.end.x - lineSegmentA.start.x)*(lineSegmentA.start.y - lineSegmentB.start.y) - (lineSegmentA.end.y - lineSegmentA.start.y)*(lineSegmentA.start.x - lineSegmentB.start.x));
		float denominatorA = ((lineSegmentB.end.y - lineSegmentB.start.y)*(lineSegmentA.end.x - lineSegmentA.start.x) - (lineSegmentB.end.x - lineSegmentB.start.x) * (lineSegmentA.end.y - lineSegmentA.start.y));
		float denominatorB = ((lineSegmentB.end.y - lineSegmentB.start.y)*(lineSegmentA.end.x - lineSegmentA.start.x) - (lineSegmentB.end.x - lineSegmentB.start.x)* (lineSegmentA.end.y - lineSegmentA.start.y));
		
		float uA = 0.0f;
		float uB = 0.0f;

		//if (denominatorA != 0.0f && denominatorB != 0.0f)
		//{
			uA = numeratorA / denominatorA;
			uB = numeratorB / denominatorB;

			//Lines are colliding
			if (uA >= 0.0f && uA <= 1.0f && uB >= 0.0f && uB <= 1.0f)
			{
				(*intersection).x = lineSegmentA.start.x + (uA * (lineSegmentA.end.x - lineSegmentA.start.x));
				(*intersection).y = lineSegmentA.start.y + (uA * (lineSegmentA.end.y - lineSegmentA.start.y));
				return true;
			}
		//}
		//else
		//{
		//	float lengthA = startA.DistanceSquared(endA);
		//	float lengthB = startB.DistanceSquared(endB);

		//	if (lengthA > lengthB)
		//	{
		//		bool isCollinear = IsPointOnLineSegment(startA, endA, startB) && IsPointOnLineSegment(startA, endA, endB);
		//		(*intersection) = (startB + endB) * 0.5f;
		//		return isCollinear;
		//	}
		//	else if(lengthA < lengthB)
		//	{
		//		bool isCollinear = IsPointOnLineSegment(startB, endB, startA) && IsPointOnLineSegment(startB, endB, endA);
		//		(*intersection) = (startA + endA) * 0.5f;
		//		return isCollinear;
		//	}
		//}



		return false;
	}

	Vector2 CollisionManager::ClosestPointOnLine(const LineSegment& lineSegment, const Vector2& point)
	{
		//Calculate the closest point of the line
		Vector2 lineStart = lineSegment.start;
		Vector2 lineEnd = lineSegment.end;

		//Calculate the circle vector        
		Vector2 circleVector = point - lineStart;

		//Calculate the line segment vector        
		Vector2 lineVector = lineEnd - lineStart;

		//Normalize the line segment vector        
		Vector2 normalizedVector = lineVector.Normalized();

		//Calculate the dot product between the circle vector and the normalized line segment vector       
		float magnitude = normalizedVector.DotProduct(circleVector);

		//Calculate the projection using the result of the dot product and multiply it by the normalized line segment        
		Vector2 projection = normalizedVector * magnitude;

		//Calculate the closest point on the line segment, by adding the project vector to the line start vector        
		Vector2 closestPoint = lineStart + projection;

		//Ensure the closest point is actually on the line
		closestPoint = Math::Clamp(closestPoint, lineStart, lineEnd);
		return closestPoint;
	}

	bool CollisionManager::IsPointOnLineSegment(const LineSegment& lineSegment, const Vector2& point)
	{
		Vector2 closestPoint = ClosestPointOnLine(lineSegment, point);
		return closestPoint == point;
	}
	bool CollisionManager::DoesContainPointOfContact(const Vector2 & pointOfContact, std::vector<Vector2>& pointsOfContact)
	{
		for (int i = 0; i < pointsOfContact.size(); i++)
		{
			if (pointsOfContact.at(i) == pointOfContact)
			{
				return true;
			}
		}

		return false;
	}
}