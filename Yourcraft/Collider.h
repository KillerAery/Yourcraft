#pragma once

#include <memory>
#include <BulletCollision/CollisionShapes/btCollisionShape.h>

//�ӿ��ࡪ����ײ��
class Collider
{
public:
	Collider();
	btCollisionShape* GetShape()const;
protected:
	~Collider();
	btCollisionShape* mShape;
};

//��ײ��ʹ��std::shared_ptr�����ڴ�
using ColliderPtr = std::shared_ptr<Collider>;