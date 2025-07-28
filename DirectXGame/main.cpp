#include <Windows.h>
#include <KamataEngine.h>
#include "KamataEngine.h"
#include "GameScene.h"

using namespace KamataEngine;
// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {
	KamataEngine::Initialize(L"LE3C_20_ヒラノ_タクヤ_TL1");
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	// ゲームシーンのインスタンス生成
	GameScene* gameScene = new GameScene();
	// ゲームシーンの初期化
	gameScene->Initialize();

	while (true) {

		if (KamataEngine::Update()) {
			break;
		}
		// ゲームシーンの更新

		gameScene->Update();

		dxCommon->PreDraw();

		// ゲームシーンの描画
		gameScene->Draw();

		dxCommon->PostDraw();
	}

	// ゲームシーンの終了処理
	gameScene->Finalize();
	delete gameScene;
	// nullptrの代入
	gameScene = nullptr;

	KamataEngine::Finalize();
	return 0;
}