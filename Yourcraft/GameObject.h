#pragma once
#include <string>
#include <list>
#include "Transform.h"


class GameObject : public Transform
{
public:
	GameObject();
	~GameObject();
	void Init();
	void Update();

	void SetName(const std::string& name);
	const std::string& GetName();

	void SetTag(const std::string& tag);
	const std::string& GetTag();

	void AddComponentInfor(int type,int componentIndex);
	void RemoveComponentInfor(int type, int componentIndex);
	int FindComponetIndex(int type);
private:
	struct ComponentInfor{
		int type;
		int index;
		ComponentInfor(int t, int i) :type(t), index(i){}
	};
protected:
	std::string mName;
	std::string mTag;
	std::list<ComponentInfor> mComponentInfor;
};

