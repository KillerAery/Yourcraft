#include "Object.h"

Object::~Object()
{
	Dead();
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

void Object::Dead()
{

}
