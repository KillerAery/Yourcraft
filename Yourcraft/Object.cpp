#include "Object.h"

void Object::ReduceRefCount()
{
}

Object::~Object()
{
	Dead();
}

Object::Object():mCount(0), mEnabled(false)
{
	
}

void Object::Init()
{
	mCount = 0;
	mEnabled = true;
}

void Object::Kill()
{
	mCount = 0;
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

void Object::Dead()
{
	mCount = 0;
}
