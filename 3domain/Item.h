#pragma once
#include <iostream>
#include <fstream>
#include <raylib.h>
#include <string>
#include "Connection.h"

class Item
{
	Vector3 _position;
	Vector3 _rotation;
	Model _model;
	std::string _modelName;
	Connection _connection;
	float _scale;
	Color _tint;

	void RedoModelTransform();
public:
	Item(std::string modelName, Model model, Vector3 position = Vector3{ 0, 0, 0 }, 
		Vector3 rotation = Vector3{ 0,0,0 }, std::string conStr = "", float scale = 1.0f, Color tint = RED);
	void Draw();
	void Update(Vector3 velocity, Vector3 rotation);
	void SetTint(Color tint);
	void SetScale(float scale);
	void SetPosition(Vector3 position);
	void ResetRotation();
	void SetModel(std::string modelName, Model model);
	void AdjustScale(float adjustment);
	Model GetModel();
	RayCollision CheckRayCollision(Ray ray);
	std::string GetModelName();
	BoundingBox GetBoundingBox();
	Connection& GetConnection();
	void Write(std::ofstream& fo);
};

