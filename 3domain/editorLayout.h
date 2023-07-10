
#include "raylib.h"

// WARNING: raygui implementation is expected to be defined before including this header
#undef RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include <string.h>     // Required for: strcpy()

#ifndef GUI_EDITORLAYOUT_H
#define GUI_EDITORLAYOUT_H

typedef struct {
    bool mouseOnGui;

    Vector2 anchor01;
    Vector2 anchor02;
    Vector2 anchor03;
    
    bool saveButtonPressed;
    bool linkButtonPressed;
    bool backButtonPressed;
    bool backConfirmActive;
    bool backYButtonPressed;
    bool backNButtonPressed;
    Vector3 ColorPicker003Value; //hsv
    bool itemColourButtonPressed;

    Rectangle layoutRecs[11];

    // Custom state variables (depend on development software)
    // NOTE: This variables should be added manually if required

} GuiEditorLayoutState;

#ifdef __cplusplus
extern "C" {            // Prevents name mangling of functions
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
GuiEditorLayoutState InitGuiEditorLayout(void);
void GuiEditorLayout(GuiEditorLayoutState *state);

#ifdef __cplusplus
}
#endif

#endif // GUI_EDITORLAYOUT_H

/***********************************************************************************
*
*   GUI_EDITORLAYOUT IMPLEMENTATION
*
************************************************************************************/
#if defined(GUI_EDITORLAYOUT_IMPLEMENTATION)

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
GuiEditorLayoutState InitGuiEditorLayout(void)
{
    GuiEditorLayoutState state = { 0 };

    state.anchor01 = Vector2{ 0, 0 };
    state.anchor02 = Vector2{ (float)GetScreenWidth() - 152, 47 };
    state.anchor03 = Vector2{ (float)GetScreenWidth() / 2 - 168, (float)GetScreenHeight() / 2 - 72};
    
    state.saveButtonPressed = false;
    state.linkButtonPressed = false;
    state.backButtonPressed = false;
    state.backConfirmActive = false;
    state.backNButtonPressed = false;
    state.backYButtonPressed = false;
    state.ColorPicker003Value = Vector3{ 0, 0, 0 };
    state.itemColourButtonPressed = false;

    state.layoutRecs[0] = Rectangle{ state.anchor01.x + 0, state.anchor01.y + 0, (float)GetScreenWidth(), 48};
    state.layoutRecs[1] = Rectangle{ state.anchor01.x + 16, state.anchor01.y + 8, 32, 32 };
    state.layoutRecs[2] = Rectangle{ state.anchor02.x + 0, state.anchor02.y + 0, 152, 480};
    state.layoutRecs[3] = Rectangle{ state.anchor02.x + 20, state.anchor02.y + 272, 96, 96 };
    state.layoutRecs[4] = Rectangle{ state.anchor02.x + 20, state.anchor02.y + 400, 120, 24 };
    state.layoutRecs[5] = Rectangle{ state.anchor01.x + 64, state.anchor01.y + 8, 32, 32 };
    state.layoutRecs[6] = Rectangle{ state.anchor01.x + 112, state.anchor01.y + 8, 32, 32 };
    state.layoutRecs[7] = Rectangle{ state.anchor03.x + 0, state.anchor03.y + 0, 336, 144 };
    state.layoutRecs[8] = Rectangle{ state.anchor03.x + 24, state.anchor03.y + 96, 120, 24 };
    state.layoutRecs[9] = Rectangle{ state.anchor03.x + 192, state.anchor03.y + 96, 120, 24 };
    state.layoutRecs[10] = Rectangle{ state.anchor03.x + 24, state.anchor03.y + 48, 288, 24 };
    // Custom variables initialization

    state.mouseOnGui = false;

    return state;
}

void GuiEditorLayout(GuiEditorLayoutState *state)
{
    Vector2 mPos = GetMousePosition();
    if (CheckCollisionPointRec(mPos, state->layoutRecs[0]) || CheckCollisionPointRec(mPos, state->layoutRecs[2]))
        state->mouseOnGui = true;
    else if (state->backConfirmActive && CheckCollisionPointRec(mPos, state->layoutRecs[7]))
        state->mouseOnGui = true;
    else state->mouseOnGui = false;

    if (IsWindowResized())
    {
        state->anchor02 = Vector2{ (float)GetScreenWidth() - 152, 47 };
        state->anchor03 = Vector2{ (float)GetScreenWidth() / 2 - 168, (float)GetScreenHeight() / 2 - 72 };
        state->layoutRecs[0] = Rectangle{ state->anchor01.x + 0, state->anchor01.y + 0, (float)GetScreenWidth(), 48 };
        state->layoutRecs[1] = Rectangle{ state->anchor01.x + 16, state->anchor01.y + 8, 32, 32 };
        state->layoutRecs[2] = Rectangle{ state->anchor02.x + 0, state->anchor02.y + 0, 152, 480 };
        state->layoutRecs[3] = Rectangle{ state->anchor02.x + 20, state->anchor02.y + 272, 96, 96 };
        state->layoutRecs[4] = Rectangle{ state->anchor02.x + 20, state->anchor02.y + 400, 120, 24 };
        state->layoutRecs[5] = Rectangle{ state->anchor01.x + 64, state->anchor01.y + 8, 32, 32 };
        state->layoutRecs[6] = Rectangle{ state->anchor01.x + 112, state->anchor01.y + 8, 32, 32 };
        state->layoutRecs[7] = Rectangle{ state->anchor03.x + 0, state->anchor03.y + 0, 336, 144 };
        state->layoutRecs[8] = Rectangle{ state->anchor03.x + 24, state->anchor03.y + 96, 120, 24 };
        state->layoutRecs[9] = Rectangle{ state->anchor03.x + 192, state->anchor03.y + 96, 120, 24 };
        state->layoutRecs[10] = Rectangle{ state->anchor03.x + 24, state->anchor03.y + 48, 288, 24 };
    }

    GuiPanel(state->layoutRecs[0], NULL);
    state->saveButtonPressed = GuiButton(state->layoutRecs[1], GuiIconText(ICON_FILE_SAVE_CLASSIC, ""));
    state->linkButtonPressed = GuiButton(state->layoutRecs[5], GuiIconText(ICON_LINK, ""));
    state->backButtonPressed = GuiButton(state->layoutRecs[6], GuiIconText(ICON_CROSS, ""));
    GuiPanel(state->layoutRecs[2], NULL);
    GuiColorPickerHSV(state->layoutRecs[3], NULL, &state->ColorPicker003Value);
    state->itemColourButtonPressed = GuiButton(state->layoutRecs[4], "Set Colour");
    if (state->backConfirmActive)
    {
        state->backConfirmActive = !GuiWindowBox(state->layoutRecs[7], "Quit");
        state->backYButtonPressed = GuiButton(state->layoutRecs[8], "Quit");
        state->backNButtonPressed = GuiButton(state->layoutRecs[9], "Cancel");
        GuiLabel(state->layoutRecs[10], "Any unsaved progress will be lost. Continue?");
    }
}

#endif // GUI_EDITORLAYOUT_IMPLEMENTATION
