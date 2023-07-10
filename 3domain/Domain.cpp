#include <filesystem>
#include "Domain.h"
#define GUI_CREATEWINDOWLAYOUT_IMPLEMENTATION
#include "createWindowLayout.h"
#define GUI_LOADWINDOWLAYOUT_IMPLEMENTATION
#include "loadWindowLayout.h"

Domain::Domain()
{
    if (FileExists("assets/fonts/DejaVuLGCSans.ttf")) 
    {
        _font = LoadLCFont();
        GuiSetFont(_font);
    }
    _cws = InitGuiCreateWindowLayout();
    //FilePathList fpl = LoadDirectoryFilesEx("data/projects", ".proj", false); //try c++ filesystem method
    //for (unsigned int i = 0; i < fpl.count; i++)
    //{
    //    _lws.emplace_back(InitGuiLoadWindowLayout(GetFileName(fpl.paths[i]), 170 * i + 20));
    //}
    //UnloadDirectoryFiles(fpl);
    int i = 0;
    if (!DirectoryExists("data/projects")) return;
    for (const auto& file : std::filesystem::directory_iterator("data/projects"))
    {
        if (file.path().extension() == ".proj") 
        {
            _lws.emplace_back(InitGuiLoadWindowLayout(file.path().filename().u8string().c_str(), 170 * i + 20));
            ++i;
        }
    }

}
Font Domain::LoadLCFont()
{
    static const char* text = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ!;%:?*()_+-=.,/\\|\"'@#$^&{}[]йцукенгшщзхъфывапролджэячсмитьбю";
    int codepointCount = 0;
    int* codepoints = LoadCodepoints(text, &codepointCount);

    // Removed duplicate codepoints to generate smaller font atlas
    int codepointsNoDupsCount = 0;
    int* codepointsNoDups = CodepointRemoveDuplicates(codepoints, codepointCount, &codepointsNoDupsCount);
    UnloadCodepoints(codepoints);
    return LoadFontEx("assets/fonts/DejaVuLGCSans.ttf", 12, codepointsNoDups, codepointsNoDupsCount);
}
int* Domain::CodepointRemoveDuplicates(int* codepoints, int codepointCount, int* codepointsResultCount)
{
    int codepointsNoDupsCount = codepointCount;
    int* codepointsNoDups = (int*)calloc(codepointCount, sizeof(int));
    memcpy(codepointsNoDups, codepoints, codepointCount * sizeof(int));

    // Remove duplicates
    for (int i = 0; i < codepointsNoDupsCount; i++)
    {
        for (int j = i + 1; j < codepointsNoDupsCount; j++)
        {
            if (codepointsNoDups[i] == codepointsNoDups[j])
            {
                for (int k = j; k < codepointsNoDupsCount; k++) codepointsNoDups[k] = codepointsNoDups[k + 1];

                codepointsNoDupsCount--;
                j--;
            }
        }
    }

    // NOTE: The size of codepointsNoDups is the same as original array but
    // only required positions are filled (codepointsNoDupsCount)

    *codepointsResultCount = codepointsNoDupsCount;
    return codepointsNoDups;
}
Domain::~Domain()
{
    if (_curScene != nullptr) delete _curScene;
    UnloadFont(_font);
}
void Domain::HandleSceneCreate()
{
    _curScene = new Scene();
    _curScene->_fileName = TextFormat("%s.proj", _cws.textboxNewNameText);
}
void Domain::Update()
{
    if (_curScene == nullptr)
    {
        if (_cws.overwriteWindowActive)
        {
            if (_cws.buttonOverwriteNoPressed)
            {
                _cws.overwriteWindowActive = false;
                _cws.buttonOverwriteNoPressed = false;
            }
            else if (_cws.buttonOverwriteYesPressed)
            {
                _cws.buttonOverwriteNoPressed = false;
                _cws.overwriteWindowActive = false;
                HandleSceneCreate();
            }
        }
        else if (_cws.buttonCreatePressed)
        {
            _cws.buttonCreatePressed = false;
            if (FileExists(TextFormat("data/projects/%s.proj", _cws.textboxNewNameText)))
            {
                _cws.overwriteWindowActive = true;
            }
            else
            {
                HandleSceneCreate();
                _lws.emplace_back(InitGuiLoadWindowLayout(_curScene->_fileName.c_str(), _lws.size() * 170 + 20));
                
            }
        }
        else
        {
            for (auto ws = _lws.begin(); ws != _lws.end(); ++ws)
            {
                if (ws->buttonDeletePressed)
                {
                    ws->deleteWindowActive = true;
                    ws->buttonDeletePressed = false;
                    break;
                }
                if (ws->buttonYesPressed)
                {
                    std::remove(TextFormat("data/projects/%s", ws->projName.c_str()));
                    _lws.erase(ws);
                    size_t j = 0;
                    for (auto& state : _lws)
                    {
                        LWRecalculateAnchor(&state, 170 * j + 20);
                        ++j;
                    }
                    break;
                }
                if (ws->buttonNoPressed)
                {
                    ws->deleteWindowActive = false;
                    ws->buttonNoPressed = false;
                    break;
                }
                if (ws->buttonLoadPressed)
                {
                    _curScene = new Scene(ws->projName);
                    break;
                }
            }
        }

        _dY -= (int)(GetMouseWheelMove() * GetFrameTime() * 1000);
        if (_dY < 0) _dY = 0;
        else if (_dY > 190 * _lws.size()) _dY = 190 * _lws.size();

    }
    else
    {
        _curScene->Update();
        if (_curScene->shouldClose)
        {
            delete _curScene;
            _curScene = nullptr;
        }
    }
}

void Domain::Draw()
{
    if (_curScene == nullptr)
    {
        ClearBackground(RAYWHITE);
        GuiCreateWindowLayout(&_cws, _dY);
        for (auto& ws : _lws)
        {
            GuiLoadWindowLayout(&ws, _dY);
        }
        //DrawTexture(_font.texture, 150, 100, BLACK); //font debug
    }
    else
    {
        _curScene->Draw();
    }
}

