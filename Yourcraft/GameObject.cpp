#include "GameObject.h"
#include <algorithm>
#include <cassert>


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
	mComponentInfor.clear();
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


void GameObject::AddComponentInfor(int type, int componentIndex){
	mComponentInfor.emplace_back(type, componentIndex);
}

void GameObject::RemoveComponentInfor(int type, int componentIndex)
{
	auto itr = std::find_if(mComponentInfor.begin(), mComponentInfor.end(), 
		[type, componentIndex](const ComponentInfor& ci) {return type == ci.type && componentIndex ==ci.index; });

	if (itr != mComponentInfor.end())
		mComponentInfor.erase(itr);
	//如果删除失败，则报错
	else
		assert("Remove Unsuccess!");
}

int GameObject::FindComponetIndex(int type)
{
	for(auto& infor : mComponentInfor)
	{
		if (infor.type == type)
			return infor.index;
	}
	//失败的时候返还-1
	return -1;
}
