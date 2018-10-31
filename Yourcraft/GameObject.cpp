#include "GameObject.h"



GameObject::GameObject(): 
mName("GameObject"),
mTag("Default")
{

}


void GameObject::Init()
{
	Transform::Init();
	mName = "GameObject";
	mTag = "Default";
}


GameObject::~GameObject()
{
}



void GameObject::Update()
{
	Transform::Update();
}

void GameObject::SetName(const std::string& name)
{
	mName = name;
}

const std::string& GameObject::GetName()
{
	return mName;
}

void GameObject::SetTag(const std::string& tag)
{
	mTag = tag;
}

const std::string& GameObject::GetTag()
{
	return mTag;
}
