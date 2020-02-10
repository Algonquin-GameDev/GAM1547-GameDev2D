#pragma once


namespace GameDev2D
{
	class CollisionEvent;

	class CollisionCallback
	{
	public:
		virtual ~CollisionCallback();

		//Implement this method in derived class to properly recieve Collision callbacks
		virtual void CollisionDetected(CollisionEvent* collisionEvent) = 0;
	};
}