#pragma once
#include <array>


//引用指针空间池
class RefZonePool
{
public:
	struct RefZone {
		int count;
		int weakcount;
		RefZone();
	};
protected:
	static const int MAX_PTRZONE_NUMS = 5000;
	static std::array<RefZone, MAX_PTRZONE_NUMS> sPtrZones;
	static int sAvailableItr;
	RefZonePool() = delete;
	~RefZonePool() = delete;
public:
	static RefZone* Create();
	static void Release(RefZone* zone);
};

