#include <ctime>
#include <filesystem>
#include <raylib.h>
#include <raymath.h>
#include "Scene.h"
#define RLIGHTS_IMPLEMENTATION
#include "rlights.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#define GUI_EDITORLAYOUT_IMPLEMENTATION
#include "editorLayout.h"
#define GUI_LINKWINDOWLAYOUT_IMPLEMENTATION
#include "linkWindowLayout.h"
#include "ShellWrapper.h"


Scene::Scene()
{
	LoadModels();
	char buf[100];
	time_t rTime;
	struct tm* tInfo;
	time(&rTime);
	tInfo = localtime(&rTime);
	strftime(buf, 100, "proj %G-%m-%d %H-%M-%S.proj", tInfo);
	_fileName = buf;
	
	Construct();
	_items.emplace_back("cube", _models["cube"]);
}
Scene::Scene(std::string fileName)
{
	LoadModels();
	Construct();
	_fileName = fileName;
	std::ifstream inf(std::filesystem::u8path("data/projects/" + fileName), std::ios::binary);
	if (!inf.is_open())
	{
		std::cout << "ERROR: Failed to open project file\n";
		return;
	}

	Vector3 pos, rot;
	float scale;
	std::string conStr, modStr;
	Color tint;
	size_t size;
	Model model;

	while (!inf.eof())
	{
		inf.read((char*)&size, sizeof(size));
		if (inf.eof()) return;
		modStr.resize(size);
		char* tmp = new char[size + 1];
		inf.read(tmp, size);
		tmp[size] = '\0';
		modStr = tmp;
		delete[] tmp;

		inf.read((char*)&size, sizeof(size));
		if (inf.eof()) return;
		conStr.resize(size);
		tmp = new char[size + 1];
		inf.read(tmp, size);
		tmp[size] = '\0';
		conStr = tmp;
		delete[] tmp;

		inf.read((char*)&pos, sizeof(pos));
		inf.read((char*)&rot, sizeof(rot));
		inf.read((char*)&scale, sizeof(scale));
		inf.read((char*)&tint, sizeof(tint));

		try {
			model = _models.at(modStr);
		}
		catch (const std::out_of_range)
		{
			model = _models["cube"];
		}
		if (inf.eof()) break;
		_items.emplace_back(modStr, model, pos, rot, conStr, scale, tint);

	}
}
Scene::~Scene()
{
	UnloadShader(_lShader);
	lightsCount = 0;
	UnloadModels();
}
void Scene::Construct()
{
	_camera.position.z = _camera.position.y = 10;
	_camera.fovy = 60;
	_camera.projection = CAMERA_PERSPECTIVE;
	_camera.up = { 0, 1, 0 };
	//_items.emplace_back("cube", _models.at("cube"));
	_selectedModel = _models.begin();
	_itemsIt = _items.begin();
	_guiState = InitGuiEditorLayout();
	_linkWinState = InitGuiLinkWindowLayout();
}
void Scene::Write(std::string fileName)
{
	if (!DirectoryExists("data") || !DirectoryExists("data/projects")) {
		std::cout << "WARNING: Directory data not found. Creating new one\n";
		std::filesystem::create_directories("data/projects");
	}
	std::ofstream fo(std::filesystem::u8path("data/projects/" + _fileName), std::ios::binary | std::ios::trunc);
	if (!fo.is_open()) 
	{
		std::cout << "ERROR: Failed to open output file\n";
		SetInfoMsg("Failed to open output file", 15);
		return;
	}
	std::cout << "INFO: " << _fileName << " saved\n";
	SetInfoMsg("Saved", 15);
	for (auto& item : _items)
	{
		item.Write(fo);
	}
}
void Scene::Update()
{
	_deltaTime = GetFrameTime();
	_mouseDelta = GetMouseDelta();
	if (_msgTimer > 0) _msgTimer -= _deltaTime;
	float cameraPos[3] = { _camera.position.x, _camera.position.y, _camera.position.z };
	SetShaderValue(_lShader, _lShader.locs[SHADER_LOC_VECTOR_VIEW], cameraPos, SHADER_UNIFORM_VEC3);
	/*_camLight.position = _camera.position;
	UpdateLightValues(_lShader, _camLight);*/

	if (IsKeyPressed(KEY_ESCAPE))
	{
		_guiState.backConfirmActive = true;
		return;
	}

	if (_guiState.backConfirmActive)
	{
		if (_guiState.backYButtonPressed)
		{
			shouldClose = true;
			return;
		}
		else if (_guiState.backNButtonPressed)
		{
			_guiState.backConfirmActive = false;
		}
		return;
	}
	if (_linkWinState.windowNewLinkActive)
	{
		if (IsFileDropped()) {
			FilePathList fpl = LoadDroppedFiles();
			if (!_selectedItem->GetConnection().SetConnection(fpl.paths[0]))
			{
				SetInfoMsg("Coludn't create connection", 15);
			}
			else
			{
				_linkWinState.windowNewLinkActive = false;
				SetInfoMsg("Link created", 10);
			}
			UnloadDroppedFiles(fpl);
		}
		return;
	}
	else if (_linkWinState.windowBox01Active)
	{
		if (_linkWinState.buttonClosePressed) _linkWinState.windowBox01Active = false;
		else if (_linkWinState.buttonOpenPressed)
		{
			ShellWrapper::OpenFile(_selectedItem->GetConnection().GetPath());
		}
		else if (_linkWinState.buttonDeletePressed)
		{
			_linkWinState.windowBox01Active = false;
			_linkWinState.deleteConfWindowActive = true;
		}

		return;
	}
	else if (_linkWinState.deleteConfWindowActive)
	{
		if (_linkWinState.buttonDeleteNPressed) _linkWinState.deleteConfWindowActive = false;
		else if (_linkWinState.buttonDeleteYPressed)
		{
			_selectedItem->GetConnection().ReSet();
			_linkWinState.deleteConfWindowActive = false;
			SetInfoMsg("Link reset", 10);
		}

		return;
	}

	if (_guiState.backButtonPressed)
	{
		_guiState.backConfirmActive = true;
		return;
	}

	if (_guiState.saveButtonPressed)
	{
		Write(_fileName);
	}
	if (_selectedItem != nullptr && _guiState.itemColourButtonPressed)
	{
		_guiState.itemColourButtonPressed = false;
		Vector3 rgb = ConvertHSVtoRGB(_guiState.ColorPicker003Value);
		Color newColor = { rgb.x * 255, rgb.y * 255, rgb.z * 255, 255 };
		_selectedItem->SetTint(newColor);
	}
	if (_selectedItem != nullptr && _guiState.linkButtonPressed)
	{
		Connection& con = _selectedItem->GetConnection();
		if (con.IsValid())
		{
			_linkWinState.filePath = con.GetPath();

			_linkWinState.windowBox01Active = true;
		}
		else if (con.IsSet())
		{
			con.ReSet();
			SetInfoMsg("File moved or removed. Link reset", 10);
		}//add notification about connection not beign valid (file moved/deleted etc)
		else _linkWinState.windowNewLinkActive = true;
	}

	if (_handleItemEdit == IE_NONE || _forceMove) UpdateView();
	if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
	{
		if (!_cursorHidden)
		{
			DisableCursor();
			_cursorHidden = true;
		}
		_forceMove = true;
	}
	if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT))
	{
		if (_cursorHidden)
		{
			EnableCursor();
			_cursorHidden = false;
		}
		_forceMove = false;
	}

	if (IsKeyPressed(KEY_DELETE) && _selectedItem != nullptr)
	{
		_selectedItem = nullptr;
		_items.erase(_itemsIt);
	}
	if (IsKeyPressed(KEY_E) && _selectedItem == nullptr) CreateItem();
	if (!_guiState.mouseOnGui && !_cursorHidden && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) DoItemSelect();
	if (_selectedItem != nullptr && !_forceMove) DoItemEdit();
}
void Scene::Draw()
{
	ClearBackground(RAYWHITE);
	BeginMode3D(_camera);
	DrawGrid(10, 1);
	for (auto& item : _items)
	{
		item.Draw();
	}
	if (_selectedItem != nullptr)
		DrawBoundingBox(_selectedItem->GetBoundingBox(), PURPLE);
	DrawCube(_colInfo.point, 0.25f, 0.25f, 0.25f, YELLOW);
	EndMode3D();
	DrawInfo();
	GuiEditorLayout(&_guiState);
	GuiLinkWindowLayout(&_linkWinState);
}
void Scene::UpdateView()
{	
	Vector3 camForward = GetCameraDirection();
	Vector3 velocity = { IsKeyDown(KEY_W) - IsKeyDown(KEY_S), IsKeyDown(KEY_D) - IsKeyDown(KEY_A), (IsKeyDown(KEY_W) - IsKeyDown(KEY_S)) * camForward.y };
	velocity.z += (IsKeyDown(KEY_SPACE) - IsKeyDown(KEY_LEFT_SHIFT));
	velocity = Vector3Scale(velocity, _moveSpeed * _deltaTime);
	Vector3 rotation = Vector3Zero();
	if (_cursorHidden)
		rotation = { _mouseDelta.x * _deltaTime * _mouseSensitivity, _mouseDelta.y * _deltaTime * _mouseSensitivity, 0 };
	UpdateCameraPro(&_camera, velocity, rotation, 0);
}
void Scene::LoadModels()
{
	_lShader = LoadShader("assets/shaders/lighting.vs", "assets/shaders/lighting.fs");
	_lShader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(_lShader, "viewPos");
	int ambientLoc = GetShaderLocation(_lShader, "ambient");
	float v4[] = { 5.5f, 5.5f, 5.5f, 1.0f };
	SetShaderValue(_lShader, ambientLoc, v4, SHADER_UNIFORM_VEC4);
	CreateLight(LIGHT_POINT, { 0, 10, 0 }, Vector3Zero(), WHITE, _lShader);
	//_camLight = CreateLight(LIGHT_POINT, { 0, 10, 0 }, Vector3Zero(), WHITE, _lShader);
	FilePathList fpl = LoadDirectoryFilesEx("assets/models", ".obj; .iqm; .gltf; .glb; .vox; .m3d", false);
	_models["cube"] = LoadModelFromMesh(GenMeshCube(1, 1, 1));
	_models["sphere"] = LoadModelFromMesh(GenMeshSphere(1, 20, 20));
	_models["torus"] = LoadModelFromMesh(GenMeshTorus(0.5f, 1, 16, 32));
	for (int i = 0; i < fpl.count; i++)
	{
		auto mod = LoadModel(fpl.paths[i]);
		_models[GetFileNameWithoutExt(fpl.paths[i])] = mod;
	}
	for (auto& m : _models)
	{
		m.second.materials[0].shader = _lShader;
	}
	UnloadDirectoryFiles(fpl);
}
void Scene::UnloadModels()
{
	for (auto& model : _models)
	{
		UnloadModel(model.second);
	}
}
void Scene::DrawInfo()
{
	int y = 50;
	int dY = 10;
	static int infoY = GetScreenHeight() - 40;
	static Color infoColor = YELLOW;
	static Color infoBack = { 0, 0, 0, 155 };
	if (IsWindowResized()) infoY = GetScreenHeight() - 40;
	if (_msgTimer > 0) 
	{
		if (_msgTimer <= 5.0f)
		{
			infoColor.a = _msgTimer / 5.0f * 255;
			infoBack.a = _msgTimer / 5.0f * 155;
		}
		else
		{
			infoColor.a = 255;
			infoBack.a = 155;
		}
		DrawRectangle(7, infoY - 1, MeasureText(_infoMsg.c_str(), 30) + 6, 33, infoBack);
		DrawText(_infoMsg.c_str(), 10, infoY, 30, infoColor);
	}

	DrawText(TextFormat("World position: %.3f, %.3f, %.3f", _camera.position.x, _camera.position.y, _camera.position.z), 10, y+=dY, 10, BLACK);
	if (_selectedItem != nullptr)
	{
		DrawText("Press F/R/T to edit Position/Rotation/Scale. Use <- and -> to switch models.", 10, y+=dY, 10, BLACK);
		DrawText("Use WASD, Left Shift and Space to move/rotate selected item.", 10, y+=dY, 10, BLACK);
		DrawText("Use W and S to adjust scale. Press Backspace to reset scale/rotation.", 10, y+=dY, 10, BLACK);
		DrawText(TextFormat("Editor force: %.1f", _editorForce), 10, y+=dY, 10, BLACK);
		switch (_handleItemEdit)
		{
		case IE_POSITION:
			DrawText("Editing item position", 10, y+=dY, 10, BLACK);
			break;
		case IE_ROTATION:
			DrawText("Editing item rotation", 10, y+=dY, 10, BLACK);
			break;
		case IE_SCALE:
			DrawText("Editing item scale", 10, y+=dY, 10, BLACK);
			break;
		default:
			break;
		}
	}
}
void Scene::DoItemSelect()
{
	Ray mRay = { 0 };
	mRay = GetMouseRay(GetMousePosition(), _camera);
	RayCollision col = { 0 };
	col.distance = FLT_MAX;
	Item* pItem = nullptr;
	for (auto it = _items.begin(); it != _items.end(); ++it )
	{
		Item& item = *it;
		RayCollision colItem = it->CheckRayCollision(mRay);
		if (colItem.hit && colItem.distance < col.distance)
		{
			col = colItem;
			pItem = &item;
			_itemsIt = it;
		}
	}
	if (col.hit)
	{
		_colInfo = col;
		_selectedItem = pItem;
		_selectedModel = _models.find(pItem->GetModelName());
	}
	else
	{
		_selectedItem = nullptr;
		_handleItemEdit = IE_NONE;
	}
}
void Scene::DoItemEdit()
{
	if (IsKeyPressed(KEY_RIGHT))
	{
		++_selectedModel;
		if (_selectedModel == _models.end())
			_selectedModel = _models.begin();
		_selectedItem->SetModel(_selectedModel->first, _selectedModel->second);
	}
	else if (IsKeyPressed(KEY_LEFT))
	{
		--_selectedModel;
		if (_selectedModel == _models.end())
			_selectedModel = --_models.end();
		_selectedItem->SetModel(_selectedModel->first, _selectedModel->second);
	}
	if (IsKeyPressed(KEY_E))
	{
		Vector3 pos = { 0 };
		if (FindNewItemPosition(pos))
			_selectedItem->SetPosition(pos);
	}
	if (IsKeyPressed(KEY_F))
	{
		if (_handleItemEdit == IE_POSITION) _handleItemEdit = IE_NONE;
		else _handleItemEdit = IE_POSITION;
	}
	if (IsKeyPressed(KEY_R))
	{
		if (_handleItemEdit == IE_ROTATION) _handleItemEdit = IE_NONE;
		else _handleItemEdit = IE_ROTATION;
	}
	if (IsKeyPressed(KEY_T))
	{
		if (_handleItemEdit == IE_SCALE) _handleItemEdit = IE_NONE;
		else _handleItemEdit = IE_SCALE;
	}
	
	if (_handleItemEdit == IE_NONE) return;

	_editorForce += GetMouseWheelMove() / 2.0f;
	if (_editorForce < 0.5f)
		_editorForce = 0.5f;
	if (_editorForce > 100.0f)
		_editorForce = 100.0f;

	Vector3 delta;
	if (IsKeyDown(KEY_LEFT_CONTROL))
	{
		delta = { (float)(IsKeyPressed(KEY_W) - IsKeyPressed(KEY_S)), 
				  (float)(IsKeyPressed(KEY_SPACE) - IsKeyPressed(KEY_LEFT_SHIFT)), 
				  (float)(IsKeyPressed(KEY_D) - IsKeyPressed(KEY_A)) 
		};
		if (_handleItemEdit == IE_ROTATION)
			delta = Vector3Scale(delta, 15.0f);
		else
			delta = Vector3Scale(delta, 0.25f);
	}
	else 
	{
		delta = { (float)(IsKeyDown(KEY_W) - IsKeyDown(KEY_S)), (float)(IsKeyDown(KEY_SPACE) - IsKeyDown(KEY_LEFT_SHIFT)), (float)(IsKeyDown(KEY_D) - IsKeyDown(KEY_A)) };
		delta = Vector3Scale(delta, _deltaTime * _editorForce);
	}
	
	switch (_handleItemEdit)
	{
	case IE_POSITION:
		_selectedItem->Update(delta, Vector3Zero());
		break;
	case IE_ROTATION:
		if (IsKeyPressed(KEY_BACKSPACE)) _selectedItem->ResetRotation();
		_selectedItem->Update(Vector3Zero(), delta);
		break;
	case IE_SCALE:
		if (IsKeyPressed(KEY_BACKSPACE)) _selectedItem->SetScale(1.0f);
		_selectedItem->AdjustScale(delta.x);
		break;
	default:
		break;
	}
}
void Scene::CreateItem()
{
	Vector3 point = { 0 };
	if (!FindNewItemPosition(point)) return;

	_selectedItem = &_items.emplace_back("cube", _models["cube"], point);
}
bool Scene::FindNewItemPosition(Vector3& outPoint)
{
	Ray mRay = GetMouseRay(GetMousePosition(), _camera);
	RayCollision col = GetRayCollisionQuad(mRay,
		{ -1000.0f, 0.0f, -1000.0f },
		{ -1000.0f, 0.0f,  1000.0f },
		{ 1000.0f, 0.0f,  1000.0f },
		{ 1000.0f, 0.0f, -1000.0f }
	);

	for (auto& item : _items)
	{
		RayCollision colItem = item.CheckRayCollision(mRay);
		if (colItem.hit && colItem.distance < col.distance) col = colItem;
	}

	if (!col.hit) return false;

	outPoint = col.point;
	return true;
}
inline void Scene::SetInfoMsg(const char* msg, float timer)
{
	_infoMsg = msg;
	_msgTimer = timer;
}
inline Vector3 Scene::GetCameraDirection()
{
	return Vector3Normalize(Vector3Subtract(_camera.target, _camera.position));
}
