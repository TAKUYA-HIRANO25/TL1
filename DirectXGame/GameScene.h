#pragma once
#include "WorldTransformEx.h"
#include "KamataEngine.h"
struct JsonObj {
	WorldTransformEx* worldTransform = nullptr;
	KamataEngine::Model* model = nullptr;
};


class GameScene {
public:
	void Initialize();

	void Update();

	void Draw();

	void Finalize();

private:
	KamataEngine::Camera camera_;	

	std::vector<JsonObj> jsonObj_;

	KamataEngine::DirectXCommon* dxCommon_ = nullptr;

	KamataEngine::Input* input = nullptr;

	KamataEngine::DebugCamera* debugCamera_ = nullptr;

	bool isDebugCameraActive_ = false;
};
