#include "CollisionFilter.h"


namespace GameDev2D
{
	CollisionFilter::CollisionFilter(unsigned short categoryBits, unsigned short maskBits)
	{
		this->categoryBits = categoryBits;
		this->maskBits = maskBits;
	}

	CollisionFilter::CollisionFilter()
	{
		categoryBits = 0xffff;
		maskBits = 0xffff;
	}

	bool CollisionFilter::CanCollide(const CollisionFilter& filter)
	{
		bool canCollide = (maskBits & filter.categoryBits) != 0 && (categoryBits & filter.maskBits) != 0;
		return canCollide;
	}
}