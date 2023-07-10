

#include "raylib.h"

// WARNING: raygui implementation is expected to be defined before including this header
#undef RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include <string.h>     // Required for: strcpy()

#ifndef GUI_LOADWINDOWLAYOUT_H
#define GUI_LOADWINDOWLAYOUT_H

typedef struct {
    Vector2 anchor02;
    
    bool buttonLoadPressed;
    bool buttonDeletePressed;
    bool deleteWindowActive;
    bool buttonYesPressed;
    bool buttonNoPressed;

    //Rectangle layoutRecs[9];

    // Custom state variables (depend on development software)
    // NOTE: This variables should be added manually if required
    std::string projName;

} GuiLoadWindowLayoutState;

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
GuiLoadWindowLayoutState InitGuiLoadWindowLayout(const char* name, int dY);
void GuiLoadWindowLayout(GuiLoadWindowLayoutState *state, int dY);
void LWRecalculateAnchor(GuiLoadWindowLayoutState* state, int dY);

#ifdef __cplusplus
//}
#endif

#endif // GUI_LOADWINDOWLAYOUT_H

/***********************************************************************************
*
*   GUI_LOADWINDOWLAYOUT IMPLEMENTATION
*
************************************************************************************/
#if defined(GUI_LOADWINDOWLAYOUT_IMPLEMENTATION)

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
GuiLoadWindowLayoutState InitGuiLoadWindowLayout(const char* name, int dY)
{
    GuiLoadWindowLayoutState state = { 0 };

    state.anchor02 = Vector2{ (float)GetScreenWidth() / 2 - 180, 250 + (float)dY };
    
    state.buttonLoadPressed = false;
    state.buttonDeletePressed = false;
    state.deleteWindowActive = false;
    state.buttonYesPressed = false;
    state.buttonNoPressed = false;

    //state.layoutRecs[0] = Rectangle{ state.anchor02.x + 0, state.anchor02.y + 8, 360, 160 };
    //state.layoutRecs[1] = Rectangle{ state.anchor02.x + 24, state.anchor02.y + 40, 96, 24 };
    //state.layoutRecs[2] = Rectangle{ state.anchor02.x + 120, state.anchor02.y + 40, 216, 24 };
    //state.layoutRecs[3] = Rectangle{ state.anchor02.x + 48, state.anchor02.y + 112, 120, 24 };
    //state.layoutRecs[4] = Rectangle{ state.anchor02.x + 192, state.anchor02.y + 112, 120, 24 };
    //state.layoutRecs[5] = Rectangle{ state.anchor03.x + 0, state.anchor03.y + 0, 360, 168 };
    //state.layoutRecs[6] = Rectangle{ state.anchor03.x + 24, state.anchor03.y + 120, 120, 24 };
    //state.layoutRecs[7] = Rectangle{ state.anchor03.x + 192, state.anchor03.y + 120, 120, 24 };
    //state.layoutRecs[8] = Rectangle{ state.anchor03.x + 24, state.anchor03.y + 48, 264, 24 };

    // Custom variables initialization
    state.projName = name;

    return state;
}

void GuiLoadWindowLayout(GuiLoadWindowLayoutState* state, int dY)
{
    if (IsWindowResized())
    {
        state->anchor02.x = (float)GetScreenWidth() / 2 - 180;
    }

    if (state->deleteWindowActive)
    {
        state->deleteWindowActive = !GuiWindowBox(Rectangle{ state->anchor02.x + 0, state->anchor02.y - dY, 360, 168 }, "Delete existing project");
        state->buttonYesPressed = GuiButton(Rectangle{ state->anchor02.x + 24, state->anchor02.y + 120 - dY, 120, 24 }, "YES");
        state->buttonNoPressed = GuiButton(Rectangle{ state->anchor02.x + 192, state->anchor02.y + 120 - dY, 120, 24 }, "NO");
        GuiLabel(Rectangle{ state->anchor02.x + 24, state->anchor02.y + 48 - dY, 264, 24 }, "Are you sure you want to delete this project?");
    }
    else 
    {
        GuiGroupBox(Rectangle{ state->anchor02.x + 0, state->anchor02.y + 8 - dY, 360, 160 }, "Load existing");
        GuiLabel(Rectangle{ state->anchor02.x + 24, state->anchor02.y + 40 - dY, 96, 24 }, "Name:");
        GuiLabel(Rectangle{ state->anchor02.x + 120, state->anchor02.y + 40 - dY, 216, 24 }, state->projName.c_str());
        state->buttonLoadPressed = GuiButton(Rectangle{ state->anchor02.x + 48, state->anchor02.y + 112 - dY, 120, 24 }, "Load");
        state->buttonDeletePressed = GuiButton(Rectangle{ state->anchor02.x + 192, state->anchor02.y + 112 - dY, 120, 24 }, "Delete");
    }
}
void LWRecalculateAnchor(GuiLoadWindowLayoutState* state, int dY)
{
    state->anchor02 = Vector2{ 456, 250 + (float)dY };
}
#endif // GUI_LOADWINDOWLAYOUT_IMPLEMENTATION
