#include "RefZonePool.h"


//静态变量初始化
std::array<RefZonePool::RefZone, RefZonePool::MAX_PTRZONE_NUMS> RefZonePool::sPtrZones
= std::array<RefZonePool::RefZone, MAX_PTRZONE_NUMS>();
int RefZonePool::sAvailableItr = -1;

RefZonePool::RefZone * RefZonePool::Create()
{
	do
	{
		sAvailableItr++;
		sAvailableItr %= MAX_PTRZONE_NUMS;
	} while ((sPtrZones[sAvailableItr].weakcount > 0));

	return &sPtrZones[sAvailableItr];
}

void RefZonePool::Release(RefZone * zone)
{
	zone->count = 0;
	zone->weakcount = 0;
}


RefZonePool::RefZone::RefZone():weakcount(0),count(0)
{

}
