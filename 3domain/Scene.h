#pragma once
#include <list>
#include <string>
#include <map>
#include <raylib.h>
#include "rlights.h"
#include "Item.h"
#include "editorLayout.h"
#include "linkWindowLayout.h"

enum {
	IE_NONE,
	IE_POSITION,
	IE_ROTATION,
	IE_SCALE,
};

class Scene
{
	Light _camLight;
	Shader _lShader;
	std::string _infoMsg = "default";
	float _msgTimer = 0;
	GuiEditorLayoutState _guiState;
	GuiLinkWindowLayoutState _linkWinState;
	std::map<std::string, Model> _models;
	std::map<std::string, Model>::const_iterator _selectedModel;
	std::list<Item> _items;
	std::list<Item>::const_iterator _itemsIt;
	Item* _selectedItem = nullptr;
	Camera _camera = { 0 };
	float _deltaTime;
	Vector2 _mouseDelta;
	bool _cursorHidden = false;
	bool _forceMove = false;
	int _handleItemEdit = IE_NONE;
	float _editorForce = 10.0f;
	float _moveSpeed = 20.0f;
	float _mouseSensitivity = 20.0f;
	RayCollision _colInfo;

	void SetInfoMsg(const char* msg, float timer);
	void UpdateView();
	void LoadModels();
	void UnloadModels();
	void DrawInfo();
	void DoItemSelect();
	void DoItemEdit();
	inline Vector3 GetCameraDirection();
	void CreateItem();
	bool FindNewItemPosition(Vector3& outPoint);
	void Write(std::string fileName);
	void Construct();
public:
	bool shouldClose = false;
	std::string _fileName;

	Scene();
	Scene(std::string fileName);
	~Scene();
	void Update();
	void Draw();
};

