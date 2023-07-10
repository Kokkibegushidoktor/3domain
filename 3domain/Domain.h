#pragma once
#include "Scene.h"
#include "loadWindowLayout.h"
#include "createWindowLayout.h"
class Domain
{
	Font _font;
	Scene* _curScene = nullptr;
	int _dY = 0;
	GuiCreateWindowLayoutState _cws;
	std::list<GuiLoadWindowLayoutState> _lws;

	static Font LoadLCFont();
	static int* CodepointRemoveDuplicates(int* codepoints, int codepointCount, int* codepointsResultCount);
public:
	Domain();
	~Domain();
	void Update();
	void Draw();
	void HandleSceneCreate();
};

