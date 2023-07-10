
#include "raylib.h"

// WARNING: raygui implementation is expected to be defined before including this header
#undef RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include <string.h>     // Required for: strcpy()

#ifndef GUI_LINKWINDOWLAYOUT_H
#define GUI_LINKWINDOWLAYOUT_H

typedef struct {
    Vector2 anchor01;
    Vector2 anchor02;
    Vector2 anchor03;
    
    bool windowBox01Active;
    bool buttonOpenPressed;
    bool buttonClosePressed;
    bool buttonDeletePressed;
    bool deleteConfWindowActive;
    bool buttonDeleteYPressed;
    bool buttonDeleteNPressed;
    bool windowNewLinkActive;

    Rectangle layoutRecs[14];

    // Custom state variables (depend on development software)
    std::string filePath;
    std::string fileDate;

} GuiLinkWindowLayoutState;

#ifdef __cplusplus
//extern "C" {            // Prevents name mangling of functions
#endif

//----------------------------------------------------------------------------------
// Defines and Macros
//----------------------------------------------------------------------------------
//...

//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------
// ...

//----------------------------------------------------------------------------------
// Module Functions Declaration
//----------------------------------------------------------------------------------
GuiLinkWindowLayoutState InitGuiLinkWindowLayout(void);
void GuiLinkWindowLayout(GuiLinkWindowLayoutState *state);

#ifdef __cplusplus
//}
#endif

#endif // GUI_LINKWINDOWLAYOUT_H

/***********************************************************************************
*
*   GUI_LINKWINDOWLAYOUT IMPLEMENTATION
*
************************************************************************************/
#if defined(GUI_LINKWINDOWLAYOUT_IMPLEMENTATION)

#include "raygui.h"

//----------------------------------------------------------------------------------
// Global Variables Definition
//----------------------------------------------------------------------------------
//...

//----------------------------------------------------------------------------------
// Internal Module Functions Definition
//----------------------------------------------------------------------------------
//...

//----------------------------------------------------------------------------------
// Module Functions Definition
//----------------------------------------------------------------------------------
GuiLinkWindowLayoutState InitGuiLinkWindowLayout(void)
{
    GuiLinkWindowLayoutState state = { 0 };

    state.anchor01 = Vector2{ (float)GetScreenWidth() / 2 - 204, (float)GetScreenHeight() / 2 - 128 };
    state.anchor02 = Vector2{ (float)GetScreenWidth() / 2 - 152, (float)GetScreenHeight() / 2 - 72 };
    state.anchor03 = Vector2{ (float)GetScreenWidth() / 2 - 204, (float)GetScreenHeight() / 2 - 108 };
    
    state.windowBox01Active = false;
    state.buttonOpenPressed = false;
    state.buttonClosePressed = false;
    state.buttonDeletePressed = false;
    state.deleteConfWindowActive = false;
    state.buttonDeleteYPressed = false;
    state.buttonDeleteNPressed = false;
    state.windowNewLinkActive = false;

    state.layoutRecs[0] = Rectangle{ state.anchor01.x + 0, state.anchor01.y + 0, 408, 256 };
    state.layoutRecs[1] = Rectangle{ state.anchor01.x + 16, state.anchor01.y + 40, 120, 24 };
    state.layoutRecs[2] = Rectangle{ state.anchor01.x + 264, state.anchor01.y + 40, 120, 24 };
    state.layoutRecs[3] = Rectangle{ state.anchor01.x + 16, state.anchor01.y + 64, 376, 24 };
    state.layoutRecs[4] = Rectangle{ state.anchor01.x + 264, state.anchor01.y + 64, 120, 24 };
    state.layoutRecs[5] = Rectangle{ state.anchor01.x + 24, state.anchor01.y + 160, 120, 24 };
    state.layoutRecs[6] = Rectangle{ state.anchor01.x + 264, state.anchor01.y + 160, 120, 24 };
    state.layoutRecs[7] = Rectangle{ state.anchor01.x + 144, state.anchor01.y + 216, 120, 24 };
    state.layoutRecs[8] = Rectangle{ state.anchor02.x + 0, state.anchor02.y + 0, 304, 144 };
    state.layoutRecs[9] = Rectangle{ state.anchor02.x + 8, state.anchor02.y + 48, 272, 24 };
    state.layoutRecs[10] = Rectangle{ state.anchor02.x + 24, state.anchor02.y + 96, 120, 24 };
    state.layoutRecs[11] = Rectangle{ state.anchor02.x + 168, state.anchor02.y + 96, 120, 24 };
    state.layoutRecs[12] = Rectangle{ state.anchor03.x + 0, state.anchor03.y + -8, 408, 216 };
    state.layoutRecs[13] = Rectangle{ state.anchor03.x + 24, state.anchor03.y + 40, 336, 24 };

    // Custom variables initialization

    return state;
}

void GuiLinkWindowLayout(GuiLinkWindowLayoutState *state)
{
    if (IsWindowResized())
    {
        state->anchor01 = Vector2{ (float)GetScreenWidth() / 2 - 204, (float)GetScreenHeight() / 2 - 128 };
        state->anchor02 = Vector2{ (float)GetScreenWidth() / 2 - 152, (float)GetScreenHeight() / 2 - 72 };
        state->anchor03 = Vector2{ (float)GetScreenWidth() / 2 - 204, (float)GetScreenHeight() / 2 - 108 };

        state->layoutRecs[0] = Rectangle{ state->anchor01.x + 0, state->anchor01.y + 0, 408, 256 };
        state->layoutRecs[1] = Rectangle{ state->anchor01.x + 16, state->anchor01.y + 40, 120, 24 };
        state->layoutRecs[2] = Rectangle{ state->anchor01.x + 264, state->anchor01.y + 40, 120, 24 };
        state->layoutRecs[3] = Rectangle{ state->anchor01.x + 16, state->anchor01.y + 64, 376, 24 };
        state->layoutRecs[4] = Rectangle{ state->anchor01.x + 264, state->anchor01.y + 64, 120, 24 };
        state->layoutRecs[5] = Rectangle{ state->anchor01.x + 24, state->anchor01.y + 160, 120, 24 };
        state->layoutRecs[6] = Rectangle{ state->anchor01.x + 264, state->anchor01.y + 160, 120, 24 };
        state->layoutRecs[7] = Rectangle{ state->anchor01.x + 144, state->anchor01.y + 216, 120, 24 };
        state->layoutRecs[8] = Rectangle{ state->anchor02.x + 0, state->anchor02.y + 0, 304, 144 };
        state->layoutRecs[9] = Rectangle{ state->anchor02.x + 8, state->anchor02.y + 48, 272, 24 };
        state->layoutRecs[10] = Rectangle{ state->anchor02.x + 24, state->anchor02.y + 96, 120, 24 };
        state->layoutRecs[11] = Rectangle{ state->anchor02.x + 168, state->anchor02.y + 96, 120, 24 };
        state->layoutRecs[12] = Rectangle{ state->anchor03.x + 0, state->anchor03.y + -8, 408, 216 };
        state->layoutRecs[13] = Rectangle{ state->anchor03.x + 24, state->anchor03.y + 40, 336, 24 };
    }

    if (state->windowBox01Active)
    {
        state->windowBox01Active = !GuiWindowBox(state->layoutRecs[0], "Linked Document");
        GuiLabel(state->layoutRecs[1], "Filename:");
        //GuiLabel(state->layoutRecs[2], "Date:");
        GuiLabel(state->layoutRecs[3], state->filePath.c_str());
        //GuiLabel(state->layoutRecs[4], state->fileDate.c_str());
        state->buttonOpenPressed = GuiButton(state->layoutRecs[5], "Open"); 
        state->buttonClosePressed = GuiButton(state->layoutRecs[6], "Close"); 
        state->buttonDeletePressed = GuiButton(state->layoutRecs[7], "Delete Link"); 
    }
    if (state->deleteConfWindowActive)
    {
        state->deleteConfWindowActive = !GuiWindowBox(state->layoutRecs[8], "Delete Link");
        GuiLabel(state->layoutRecs[9], "Are you sure you want to remove existing file link?");
        state->buttonDeleteYPressed = GuiButton(state->layoutRecs[10], "YES"); 
        state->buttonDeleteNPressed = GuiButton(state->layoutRecs[11], "NO"); 
    }
    if (state->windowNewLinkActive)
    {
        state->windowNewLinkActive = !GuiWindowBox(state->layoutRecs[12], "Link Document");
        GuiLabel(state->layoutRecs[13], GuiIconText(ICON_FILE_SAVE, "Current link is unset. Drag and drop a file to link it."));
    }
}

#endif // GUI_LINKWINDOWLAYOUT_IMPLEMENTATION
