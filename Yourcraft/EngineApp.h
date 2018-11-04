#pragma once

#include <DirectXColors.h>
#include "d3dApp.h"
#include "Camera.h"
#include "ObjReader.h"
#include "Effects.h"

#include "TextruePool.h"
#include "ObjectPool.h"
#include "GameObject.h"
#include "BatchMeshRender.h"
#include "MeshRender.h"

class EngineApp :public D3DApp
{
public:
	EngineApp(HINSTANCE hInstance);
	 ~EngineApp();
	 bool Init();
	 void UpdateScene(float dt)override;
	 void DrawScene()override;
	 void OnResize();
private:
	bool InitResource();
protected:
	ComPtr<ID2D1SolidColorBrush> mColorBrush;				// 单色笔刷
	ComPtr<IDWriteFont> mFont;								// 字体
	ComPtr<IDWriteTextFormat> mTextFormat;					// 文本格式
	BasicEffect mBasicEffect;								// 对象渲染特效管理
	std::shared_ptr<Camera> mCamera;						// 摄像机
	CameraMode mCameraMode;									// 摄像机模式
	ObjReader mObjReader;									// 模型读取对象

	TextruePool mTextPool;
	ObjectPool<GameObject,500> mGameObjectPool;
	ObjectPool<BatchMeshRender,10> mBatchMeshRenderPool;
	ObjectPool<MeshRender, 100> mMeshRenderPool;

	GameObject* mWorld;
	GameObject* mGameObject[10];
	MeshRender* mMeshRender;
};

