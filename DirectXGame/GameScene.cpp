#include "GameScene.h"
#include <string>	
#include <json.hpp>
#include <fstream>
#include <cassert>
#include <vector>

using namespace KamataEngine;

void GameScene::Initialize() { 
	camera_.Initialize(); 

	// レベルエディタを格納するための構造体
	// オブジェクト 1個分のデータ
	struct ObjectData {
		std::string type;
		std::string name;

		// "transform"
		struct Transform {
			Vector3 translation;
			Vector3 rotation;
			Vector3 scaling;
		};

		Transform transform;

		// "file_name
		std::string file_name;
	};

	// レベルデータ
	struct LevelData {
		// "name"
		std::string name;

		// "objects"
		std::vector<ObjectData> objects;
	};

	// jsonファイルの デシリアライズ化
	// jsonファイルのパス名
	const std::string fullpath = std::string("Resources/levels/") + "scene.json";

	// ファイルストリーム
	std::ifstream file;

	// ファイルを開く
	file.open(fullpath);
	// ファイルオープン失敗チェック
	if (file.fail()) {
		assert(0);
	}

	nlohmann::json deserialized;

	// ファイルから読み込み、メモリへ格納
	file >> deserialized;

	// 正しいレベルデータファイルかチェック
	assert(deserialized.is_object());

	assert(deserialized.contains("name"));
	assert(deserialized["name"].is_string());

	// レベルデータを構造体に格納していく
	LevelData* levelData = new LevelData();

	// "name"を文字列として取得
	levelData->name = deserialized["name"].get<std::string>();
	assert(levelData->name == "scene");

	// "object"の全オブジェクトを走査
	for (nlohmann::json& object : deserialized["objects"]) {

		// オブジェクト 1つ分の妥当性のチェック
		assert(object.contains("type"));

		if (object["type"].get<std::string>() == "MESH") {

			// 1個分の要素の準備
			levelData->objects.emplace_back(ObjectData{});
			ObjectData& objectData = levelData->objects.back();

			objectData.type = object["type"].get<std::string>();
			objectData.name = object["name"].get<std::string>();

			// トランスフォームのパラメータ読み込み
			nlohmann::json& transform = object["transform"];
			// 平衡移動 "translation"
			objectData.transform.translation.x = (float)transform["translation"][0];
			objectData.transform.translation.y = (float)transform["translation"][2];
			objectData.transform.translation.z = (float)transform["translation"][1];
			// 回転角 "rotation"
			objectData.transform.rotation.x = -(float)transform["rotation"][0];
			objectData.transform.rotation.y = -(float)transform["rotation"][2];
			objectData.transform.rotation.z = -(float)transform["rotation"][1];
			// 拡大縮小 "scaling
			objectData.transform.scaling.x = (float)transform["scaling"][0];
			objectData.transform.scaling.y = (float)transform["scaling"][2];
			objectData.transform.scaling.z = (float)transform["scaling"][1];

			// "file_name
			if (object.contains("file_name")) {
				objectData.file_name = object["file_name"].get<std::string>();
			}
		}
	}
	// レベルデータからオブジェクトを生成、配置
	// モデルファイル名 objectData.file_name にあれば入っている ⇒ file_name を元に、持てるデータを特定する
	for (auto& objectData : levelData->objects) {
		// モデルファイル名 objectData.file_name にあれば入っている ⇒ file_name を元に、持てるデータを特定する
		Model* model = Model::CreateFromOBJ(objectData.file_name, true);

		WorldTransformEx* worldTransform = new WorldTransformEx();	
		worldTransform->Initialize();

		// 位置の設定 objectData.transform.translationに入っている。
		worldTransform->translation_ = objectData.transform.translation;

		// 回転の設定 objectData.transform.rotationに入っている。
		worldTransform->rotation_ = objectData.transform.rotation;

		// 拡大縮小 objectData.transform.scalingに入っている。
		worldTransform->scale_ = objectData.transform.scaling;

		jsonObj_.push_back({worldTransform, model});

	}

}

void GameScene::Update() {


	for (auto& jsonObj : jsonObj_) {
		jsonObj.worldTransform->UpdateMatrix();
	}
}

void GameScene::Draw() {

	Model::PreDraw();

	for (auto& jsonObj : jsonObj_) {
		jsonObj.model->Draw(*jsonObj.worldTransform, camera_);
	}

	Model::PostDraw();
}

void GameScene::Finalize() {
	for (auto& jsonObj : jsonObj_) {
		delete jsonObj.model;
		delete jsonObj.worldTransform;
	}
	jsonObj_.clear();
}