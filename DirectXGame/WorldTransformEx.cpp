#include "WorldTransformEx.h"

using namespace KamataEngine;
using namespace KamataEngine::MathUtility;

void WorldTransformEx::UpdateMatrix() {
	// World変換行列を計算し、matWorld_に格納
	matWorld_ = MakeAffineMatrix();
	// 定数バッファへ転送
	TransferMatrix();
}

// アフィン変換行列
KamataEngine::Matrix4x4 WorldTransformEx::MakeAffineMatrix() {
	// Scale Matrix
	Matrix4x4 matScale = MakeScaleMatrix(scale_);

	// Roation Matrix
	Matrix4x4 matRotX = MakeRotateXMatrix(rotation_.x);
	Matrix4x4 matRotY = MakeRotateYMatrix(rotation_.y);
	Matrix4x4 matRotZ = MakeRotateZMatrix(rotation_.z);
	Matrix4x4 matRot = matRotZ * matRotX * matRotY;

	// Translate Matrix
	Matrix4x4 matTrans = MakeTranslateMatrix(translation_);

	// World Matrix
	Matrix4x4 matWorld = matScale * matRot * matTrans;

	return matWorld;
}
