#include "Object.h"

Object::~Object()
{
	Kill();
}

Object::Object():mAlive(false), mEnabled(false)
{
	
}

void Object::Kill()
{
	mAlive = false;
}

bool Object::IsAlive()
{
	return mAlive;
}

bool Object::IsEnabled()
{
	return mEnabled;
}

void Object::Init()
{
	mAlive = true;
	mEnabled = true;
}

void Object::operator delete(void * address) noexcept
{
	static_cast<Object*>(address)->Kill();
}
