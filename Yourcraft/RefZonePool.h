#pragma once
#include <array>
#include "Singleton.h"

//计数空间 内存池
//(单例类)
class RefZonePool : public Singleton<RefZonePool>
{
	friend class Singleton<RefZonePool>;
public:
	struct RefZone {
		int count;
		int weakcount;
		RefZone();
	};
protected:
	static const int MAX_PTRZONE_NUMS = 5000;
	std::array<RefZone, MAX_PTRZONE_NUMS> mPtrZones;
	int mAvailableItr;
	RefZonePool();
public:
	bool Init();
	~RefZonePool();
	RefZone* Create();
	void Release(RefZone* zone);
};

