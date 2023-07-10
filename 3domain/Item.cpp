#include <raylib.h>
#include <raymath.h>
#include "Item.h"


Item::Item(std::string modelName, Model model, Vector3 position, Vector3 rotation, std::string conStr, float scale, Color tint) : 
	_modelName(modelName), _model(model), _position(position), _rotation(rotation), _connection(conStr), _scale(scale), _tint(tint)
{
	RedoModelTransform();
}
void Item::Draw()
{
	for (int i = 0; i < _model.meshCount; i++)
	{
		Color color = _model.materials[_model.meshMaterial[i]].maps[MATERIAL_MAP_DIFFUSE].color;

		Color colorTint = WHITE;
		colorTint.r = (unsigned char)((((float)color.r / 255.0f) * ((float)_tint.r / 255.0f)) * 255.0f);
		colorTint.g = (unsigned char)((((float)color.g / 255.0f) * ((float)_tint.g / 255.0f)) * 255.0f);
		colorTint.b = (unsigned char)((((float)color.b / 255.0f) * ((float)_tint.b / 255.0f)) * 255.0f);
		colorTint.a = (unsigned char)((((float)color.a / 255.0f) * ((float)_tint.a / 255.0f)) * 255.0f);

		_model.materials[_model.meshMaterial[i]].maps[MATERIAL_MAP_DIFFUSE].color = colorTint;
		DrawMesh(_model.meshes[i], _model.materials[_model.meshMaterial[i]], _model.transform);
		_model.materials[_model.meshMaterial[i]].maps[MATERIAL_MAP_DIFFUSE].color = color;
	}
}
void Item::Update(Vector3 velocity, Vector3 rotation)
{
	_position = Vector3Add(_position, velocity);
	_rotation = Vector3Add(_rotation, rotation);
	RedoModelTransform();
}
void Item::SetTint(Color tint)
{
	_tint = tint;
}
void Item::SetScale(float scale)
{
	_scale = scale < 0.1f ? 0.1f : scale;
	RedoModelTransform();
}
void Item::SetModel(std::string modelName, Model model)
{
	_modelName = modelName;
	_model = model;
	RedoModelTransform();
}
void Item::SetPosition(Vector3 position)
{
	_position = position;
	RedoModelTransform();
}
void Item::ResetRotation()
{
	_rotation = Vector3Zero();
	RedoModelTransform();
}
Model Item::GetModel()
{
	return _model;
}
std::string Item::GetModelName()
{
	return _modelName;
}
void Item::AdjustScale(float adjustment)
{
	_scale += adjustment;
	if (_scale < 0.1f) _scale = 0.1f;
	RedoModelTransform();
}
BoundingBox Item::GetBoundingBox()
{
	BoundingBox bBox = GetModelBoundingBox(_model);
	bBox.min = Vector3Add(_position, Vector3Scale(bBox.min, _scale));
	bBox.max = Vector3Add(_position, Vector3Scale(bBox.max, _scale));
	return bBox;
}
Connection& Item::GetConnection()
{
	return _connection;
}
void Item::RedoModelTransform()
{
	Matrix mScale = MatrixScale(_scale, _scale, _scale);
	Matrix mRotation = MatrixRotateXYZ(Vector3Scale(_rotation, DEG2RAD));
	Matrix mTranslate = MatrixTranslate(_position.x, _position.y, _position.z);
	_model.transform = MatrixMultiply(MatrixMultiply(mScale, mRotation), mTranslate);
}
RayCollision Item::CheckRayCollision(Ray ray)
{
	RayCollision col = GetRayCollisionBox(ray, GetBoundingBox());
	RayCollision meshCol = { 0 };
	if (col.hit)
	{
		for (int i = 0; i < _model.meshCount; ++i)
		{
			meshCol = GetRayCollisionMesh(ray, _model.meshes[i], _model.transform);
			
			if (meshCol.hit)
			{
				break;
			}
		}
	}
	return meshCol;
}
void Item::Write(std::ofstream& fo)
{	
	size_t size;
	size = _modelName.length();
	fo.write((char*)&size, sizeof(size));
	fo.write(_modelName.c_str(), size);
	size = _connection.GetPath().length();
	fo.write((char*)&size, sizeof(size));
	fo.write(_connection.GetPath().c_str(), size);
	fo.write((char*)&_position, sizeof(_position));
	fo.write((char*)&_rotation, sizeof(_rotation));
	fo.write((char*)&_scale, sizeof(_scale));
	fo.write((char*)&_tint, sizeof(_tint));
}