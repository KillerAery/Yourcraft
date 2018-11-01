#ifndef GAMEAPP_H
#define GAMEAPP_H

#include <DirectXColors.h>
#include "d3dApp.h"
#include "Camera.h"
#include "TestObject.h"
#include "ObjReader.h"
#include "Collision.h"

class GameApp : public D3DApp
{
public:
	// 摄像机模式
	enum class CameraMode { FirstPerson, ThirdPerson, Free };
	
public:
	GameApp(HINSTANCE hInstance);
	~GameApp();

	bool Init();
	void OnResize();
	void UpdateScene(float dt);
	void DrawScene();

private:
	bool InitResource();
	
protected:
	
	ComPtr<ID2D1SolidColorBrush> mColorBrush;				// 单色笔刷
	ComPtr<IDWriteFont> mFont;								// 字体
	ComPtr<IDWriteTextFormat> mTextFormat;					// 文本格式

	
	//TestObject mSphere;										// 球
	//TestObject mCube;										// 立方体
	//TestObject mCylinder;									// 圆柱体
	//TestObject mHouse;										// 房屋
	//DirectX::BoundingSphere mBoundingSphere;				// 球的包围盒

	std::wstring mPickedObjStr;								// 已经拾取的对象名

	BasicEffect mBasicEffect;								// 对象渲染特效管理

	std::shared_ptr<Camera> mCamera;						// 摄像机
	CameraMode mCameraMode;									// 摄像机模式

	ObjReader mObjReader;									// 模型读取对象
};


#endif