#include "GameObject.h"
#include <algorithm>
#include <cassert>


GameObject::GameObject(): 
mName("GameObject"),
mTag("Default"),
mLayer(0)
{

}


void GameObject::Init()
{
	Transform::Init();
	mName = "GameObject";
	mTag = "Default";
	mLayer = 0;
	mComponentsInfor.clear();
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

void GameObject::SetLayer(int layer)
{
	mLayer = layer;
}

int GameObject::GetLayer()
{
	return mLayer;
}


void GameObject::AddComponentInfor(int type, int componentIndex){
	mComponentsInfor.emplace_back(type, componentIndex);
}

void GameObject::RemoveComponentInfor(int type)
{
	auto itr = std::find_if(mComponentsInfor.begin(), mComponentsInfor.end(), 
		[type](const ComponentInfor& ci) {return type == ci.type;});

	if (itr != mComponentsInfor.end())
		mComponentsInfor.erase(itr);
	//如果删除失败，则报错
	else
		assert("Remove Unsuccess!");
}

int GameObject::FindComponetIndex(int type)
{
	for(auto& infor : mComponentsInfor)
	{
		if (infor.type == type)
			return infor.index;
	}
	//失败的时候返还-1
	return -1;
}
