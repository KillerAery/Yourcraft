#include "Object.h"


Object::Object():mCount(0), mEnabled(false)
{
	
}

Object::~Object()
{
	Dead();
}

void Object::Init()
{
	mCount = 0;
	mEnabled = true;
}


bool Object::IsAlive()
{
	return 	mCount > 0;
}

bool Object::IsEnabled()
{
	return mEnabled;
}

void Object::SetEnabled(bool enabled)
{
	mEnabled = enabled;
}

void Object::AddRefCount()
{
	mCount++;
}

void Object::ReduceRefCount()
{
	mCount--;
}

void Object::Kill()
{
	mCount = 0;
}

void Object::Dead()
{

}
