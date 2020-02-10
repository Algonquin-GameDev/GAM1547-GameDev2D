#pragma once

#include "Collider.h"
#include "../Graphics/Color.h"
#include <vector>

namespace GameDev2D
{
	const Color COLLIDER_STATIC_ACTIVE_DEFAULT_COLOR = Color::BlueColor();
	const Color COLLIDER_DYNAMIC_ACTIVE_DEFAULT_COLOR = Color::BlueColor();
	const Color COLLIDER_STATIC_ACTIVE_COLLISION_COLOR = Color::RedColor();
	const Color COLLIDER_DYNAMIC_ACTIVE_COLLISION_COLOR = Color::RedColor();
	const Color COLLIDER_STATIC_INACTIVE_COLOR = Color::GrayColor();
	const Color COLLIDER_DYNAMIC_INACTIVE_COLOR = Color::GrayColor();

	struct ColliderGroup
	{
		ColliderGroup(Collider::Type type);

		void PreCollisionUpdate(double delta);
        void PostCollisionUpdate(double delta);
        void DebugDraw();

        void Reset();

		void Add(Collider* collider);
		void Remove(Collider* collider);

		unsigned int Count();
		unsigned int ActiveCount();
		unsigned int InactiveCount();

		std::vector<Collider*> activeColliders;
		std::vector<Collider*> inactiveColliders;
		const Collider::Type type;

	private:
		void DrawCollider(Collider* collider, Color color);
	};
}