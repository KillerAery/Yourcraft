#pragma once

#include <memory>
#include <BulletCollision/CollisionShapes/btCollisionShape.h>

//�ӿ��ࡪ����ײ��
class Collider
{
public:
	Collider();
	~Collider();
	btCollisionShape* GetShape()const;
protected:
	btCollisionShape* mShape;
};

//��ײ��ʹ��std::shared_ptr�����ڴ�
using ColliderPtr = std::shared_ptr<Collider>;