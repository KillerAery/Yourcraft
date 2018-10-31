#include "RefZonePool.h"


RefZonePool::RefZonePool():mAvailableItr(-1), mPtrZones()
{
}

bool RefZonePool::Init()
{
	return true;
}

RefZonePool::~RefZonePool()
{
}

RefZonePool::RefZone * RefZonePool::Create()
{
	do
	{
		mAvailableItr++;
		mAvailableItr %= MAX_PTRZONE_NUMS;
	} while ((mPtrZones[mAvailableItr].weakcount > 0));

	return &mPtrZones[mAvailableItr];
}

void RefZonePool::Release(RefZone * zone)
{
	zone->count = 0;
	zone->weakcount = 0;
}


RefZonePool::RefZone::RefZone():weakcount(0),count(0)
{

}
