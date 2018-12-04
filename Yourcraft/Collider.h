#pragma once

#include <memory>
#include <BulletCollision/CollisionShapes/btCollisionShape.h>

//接口类——碰撞体
class Collider
{
public:
	Collider();
	~Collider();
	btCollisionShape* GetShape()const;
protected:
	btCollisionShape* mShape;
};

//碰撞体使用std::shared_ptr管理内存
using ColliderPtr = std::shared_ptr<Collider>;