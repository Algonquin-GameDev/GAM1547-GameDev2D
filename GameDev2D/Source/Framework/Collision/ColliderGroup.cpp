#include "ColliderGroup.h"
#include "CircleCollider.h"
#include "PointCollider.h"
#include "LineCollider.h"
#include "AxisAlignedRectangleCollider.h"
#include "OrientedRectangleCollider.h"
#include "../Services/Services.h"
#include "../GameDev2D_Settings.h"


namespace GameDev2D
{
	ColliderGroup::ColliderGroup(Collider::Type type) : 
		type(type)
	{
	}

	void ColliderGroup::PreCollisionUpdate(double delta)
	{
		//Check all the in-active colliders, and see if any became active since last frame
		std::vector<Collider*>::iterator it = inactiveColliders.begin();
		while (it != inactiveColliders.end())
		{
			if ((*it)->IsActive() == true)
			{
				activeColliders.push_back((*it));
				it = inactiveColliders.erase(it);
			}
			else
			{
				it++;
			}
		}
	}

	void ColliderGroup::PostCollisionUpdate(double delta)
	{
		//Check all the in-active colliders, and see if any became active since last frame
		std::vector<Collider*>::iterator it = activeColliders.begin();
		while (it != activeColliders.end())
		{
			if ((*it)->IsActive() == false)
			{
				inactiveColliders.push_back((*it));
				it = activeColliders.erase(it);
			}
			else
			{
				it++;
			}
		}
	}

	void ColliderGroup::DebugDraw()
	{
#if DEBUG && DEBUG_DRAW_COLLIDERS
		for (int i = 0; i < activeColliders.size(); i++)
		{
			Collider* collider = activeColliders.at(i);
			bool isColliding = collider->IsColliding();
			Color color = type == Collider::Dynamic ? (isColliding ? COLLIDER_DYNAMIC_ACTIVE_COLLISION_COLOR : COLLIDER_DYNAMIC_ACTIVE_DEFAULT_COLOR)  : (isColliding ? COLLIDER_STATIC_ACTIVE_COLLISION_COLOR : COLLIDER_STATIC_ACTIVE_DEFAULT_COLOR);
			DrawCollider(collider, color);
		}

		for (int i = 0; i < inactiveColliders.size(); i++)
		{
			Collider* collider = inactiveColliders.at(i);
			Color color = type == Collider::Dynamic ? COLLIDER_DYNAMIC_INACTIVE_COLOR : COLLIDER_STATIC_INACTIVE_COLOR;
			DrawCollider(collider, color);
		}
#endif
	}

    void ColliderGroup::Reset()
    {
        for (int i = 0; i < activeColliders.size(); i++)
        {
            activeColliders.at(i)->Reset();
        }

        for (int i = 0; i < inactiveColliders.size(); i++)
        {
            inactiveColliders.at(i)->Reset();
        }
    }

	void ColliderGroup::Add(Collider* collider)
	{
		if (collider->IsActive() == true)
		{
			activeColliders.push_back(collider);
		}
		else
		{
			inactiveColliders.push_back(collider);
		}
	}

	void ColliderGroup::Remove(Collider* collider)
	{
		//Look through the active colliders
		bool wasFound = false;
		std::vector<Collider*>::iterator it = activeColliders.begin();
		while (it != activeColliders.end())
		{
			if ((*it) == collider)
			{
				it = activeColliders.erase(it);
				wasFound = true;
				break;
			}
			else
			{
				it++;
			}
		}

		//Look through the inactive colliders only if the collider 
		//wasn't found among the active colliders
		if (wasFound == false)
		{
			std::vector<Collider*>::iterator it = inactiveColliders.begin();
			while (it != inactiveColliders.end())
			{
				if ((*it) == collider)
				{
					it = inactiveColliders.erase(it);
					break;
				}
				else
				{
					it++;
				}
			}
		}
	}

	unsigned int ColliderGroup::Count()
	{
		return ActiveCount() + InactiveCount();
	}

	unsigned int ColliderGroup::ActiveCount()
	{
		return activeColliders.size();
	}

	unsigned int ColliderGroup::InactiveCount()
	{
		return inactiveColliders.size();
	}

	void ColliderGroup::DrawCollider(Collider* collider, Color color)
	{
		//Determine the type of colliders to draw
		if (collider->GetShape() == Collider::Circle)
		{
			CircleCollider* circleCollider = static_cast<CircleCollider*>(collider);
			Vector2 position = circleCollider->GetWorldPosition();
			Services::GetGraphics()->DrawCircle(position, circleCollider->GetRadius(), Vector2(0.5f, 0.5f), color, false);
		}
		else if (collider->GetShape() == Collider::OrientedRectangle)
		{
			OrientedRectangleCollider* rectangleCollider = static_cast<OrientedRectangleCollider*>(collider);
			Vector2 position = rectangleCollider->GetWorldPosition();
			float radians = rectangleCollider->GetWorldRadians();
			Services::GetGraphics()->DrawRectangle(position, Vector2(rectangleCollider->GetWidth(), rectangleCollider->GetHeight()), Rotation::Radians(radians), Vector2(0.5f, 0.5f), color, false);
		}
		else if (collider->GetShape() == Collider::AxisAlignedRectangle)
		{
			AxisAlignedRectangleCollider* rectangleCollider = static_cast<AxisAlignedRectangleCollider*>(collider);
			Vector2 position = rectangleCollider->GetWorldPosition();
			float radians = rectangleCollider->GetWorldRadians();
			float w = fabsf(cosf(radians)) * rectangleCollider->GetWidth() + fabsf(sinf(radians)) * rectangleCollider->GetHeight();
			float h = fabsf(cosf(radians)) * rectangleCollider->GetHeight() + fabsf(sinf(radians)) * rectangleCollider->GetWidth();
			Services::GetGraphics()->DrawRectangle(position, Vector2(w, h), Rotation::Radians(0.0f), Vector2(0.5f, 0.5f), color, false);
		}
		else if (collider->GetShape() == Collider::Point)
		{
			PointCollider* pointCollider = static_cast<PointCollider*>(collider);
			Vector2 position = pointCollider->GetWorldPosition();
			Services::GetGraphics()->DrawCircle(position, 2.0f, Vector2(0.5f, 0.5f), color, true);
		}
		else if (collider->GetShape() == Collider::Line)
		{
			LineCollider* lineCollider = static_cast<LineCollider*>(collider);
			LineSegment lineSegment = lineCollider->GetRelative();
			Services::GetGraphics()->DrawLine(lineSegment.start, lineSegment.end, color);
		}
	}
}