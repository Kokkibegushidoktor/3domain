

#include "raylib.h"

// WARNING: raygui implementation is expected to be defined before including this header
#undef RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include <string.h>     // Required for: strcpy()

#ifndef GUI_CREATEWINDOWLAYOUT_H
#define GUI_CREATEWINDOWLAYOUT_H

typedef struct {
    Vector2 anchor01;
    
    bool buttonCreatePressed;
    bool textboxNewNameEditMode;
    bool overwriteWindowActive;
    bool buttonOverwriteYesPressed;
    bool buttonOverwriteNoPressed;
    char textboxNewNameText[128];

    Rectangle layoutRecs[4];

    // Custom state variables (depend on development software)
    // NOTE: This variables should be added manually if required

} GuiCreateWindowLayoutState;

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
GuiCreateWindowLayoutState InitGuiCreateWindowLayout(void);
void GuiCreateWindowLayout(GuiCreateWindowLayoutState *state, int dY);

#ifdef __cplusplus
//}
#endif

#endif // GUI_CREATEWINDOWLAYOUT_H

/***********************************************************************************
*
*   GUI_CREATEWINDOWLAYOUT IMPLEMENTATION
*
************************************************************************************/
#if defined(GUI_CREATEWINDOWLAYOUT_IMPLEMENTATION)

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
GuiCreateWindowLayoutState InitGuiCreateWindowLayout(void)
{
    GuiCreateWindowLayoutState state = { 0 };

    state.anchor01 = Vector2{ (float)GetScreenWidth() / 2 - 180, 80 };
    
    state.buttonCreatePressed = false;
    state.textboxNewNameEditMode = false;
    state.overwriteWindowActive = false;
    state.buttonOverwriteYesPressed = false;
    state.buttonOverwriteNoPressed = false;
    strcpy(state.textboxNewNameText, "proj1");

    //state.layoutRecs[0] = Rectangle{ state.anchor01.x + 0, state.anchor01.y + 8, 360, 160 };
    //state.layoutRecs[1] = Rectangle{ state.anchor01.x + 120, state.anchor01.y + 112, 120, 24 };
    //state.layoutRecs[2] = Rectangle{ state.anchor01.x + 24, state.anchor01.y + 40, 96, 24 };
    //state.layoutRecs[3] = Rectangle{ state.anchor01.x + 120, state.anchor01.y + 40, 216, 24 };

    // Custom variables initialization

    return state;
}

void GuiCreateWindowLayout(GuiCreateWindowLayoutState* state, int dY)
{
    if (IsWindowResized())
    {
        state->anchor01.x = (float)GetScreenWidth() / 2 - 180;
    }

    if (state->overwriteWindowActive)
    {
        state->overwriteWindowActive = !GuiWindowBox(Rectangle{ state->anchor01.x + 0, state->anchor01.y - dY, 360, 168 }, "File already exists");
        state->buttonOverwriteYesPressed = GuiButton(Rectangle{ state->anchor01.x + 24, state->anchor01.y + 120 - dY, 120, 24 }, "Continue");
        state->buttonOverwriteNoPressed = GuiButton(Rectangle{ state->anchor01.x + 192, state->anchor01.y + 120 - dY, 120, 24 }, "Cancel");
        GuiLabel(Rectangle{ state->anchor01.x + 24, state->anchor01.y + 48 - dY, 320, 24 }, "If you continue an existing file will be overwritten.");
        GuiLabel(Rectangle{ state->anchor01.x + 24, state->anchor01.y + 80 - dY, 264, 24 }, "Continue?");
    }
    else
    {
        GuiGroupBox(Rectangle{ state->anchor01.x + 0, state->anchor01.y + 8 - dY, 360, 160 }, "Create New Project");
        state->buttonCreatePressed = GuiButton(Rectangle{ state->anchor01.x + 120, state->anchor01.y + 112 - dY, 120, 24 }, "Create");
        GuiLabel(Rectangle{ state->anchor01.x + 24, state->anchor01.y + 40 - dY, 96, 24 }, "Name:");
        if (GuiTextBox(Rectangle{ state->anchor01.x + 120, state->anchor01.y + 40 - dY, 216, 24 }, state->textboxNewNameText, 128, state->textboxNewNameEditMode)) state->textboxNewNameEditMode = !state->textboxNewNameEditMode;
    }

}

#endif // GUI_CREATEWINDOWLAYOUT_IMPLEMENTATION
