#pragma once


namespace GameDev2D
{
	struct CollisionFilter
	{
		CollisionFilter(unsigned short categoryBits, unsigned short maskBits);
		CollisionFilter();

		//Returns wether another CollisionFilter can collide with it
		bool CanCollide(const CollisionFilter& filter);

		//The collision filter bits
		unsigned short categoryBits;
		unsigned short maskBits;
	};
}