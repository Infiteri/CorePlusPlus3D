#include "Input.h"
#include "Logger.h"

#include <unordered_map>

namespace Core
{
    struct InputMouseState
    {
        int x = 0;
        int y = 0;

        int lastX = 0;
        int lastY = 0;

        int deltaX = 0;
        int deltaY = 0;
    };

    static InputMouseState mouse_state;
    static std::unordered_map<Keys, bool> keys;
    static std::unordered_map<Buttons, bool> buttons;

    void Input::Init()
    {
    }

    void Input::Shutdown()
    {
    }

    bool Input::GetKey(Keys key)
    {
        return keys[key];
    }

    bool Input::GetButton(Buttons button)
    {
        return buttons[button];
    }

    int Input::GetMouseX()
    {
        return mouse_state.x;
    }

    int Input::GetMouseY()
    {
        return mouse_state.y;
    }

    int Input::GetMouseLastX()
    {
        return mouse_state.lastX;
    }

    int Input::GetMouseLastY()
    {
        return mouse_state.y;
    }

    int Input::GetMouseDeltaX()
    {
        return mouse_state.deltaX;
    }

    int Input::GetMouseDeltaY()
    {
        return mouse_state.deltaY;
    }

    int Input::GetMouseMovementDirectionX()
    {
        if (mouse_state.deltaX == 0)
            return 0;

        return mouse_state.deltaX < 0 ? -1 : 1;
    }

    int Input::GetMouseMovementDirectionY()
    {
        if (mouse_state.deltaY == 0)
            return 0;

        return mouse_state.deltaY < 0 ? -1 : 1;
    }

    void InputUpdateKey(Keys key, bool p)
    {
        keys[key] = p;
    }

    void InputUpdateButton(Buttons button, bool p)
    {
        buttons[button] = p;
    }

    void InputUpdateMouse(int x, int y)
    {
        mouse_state.deltaX = x - mouse_state.lastX;
        mouse_state.deltaY = y - mouse_state.lastY;

        mouse_state.x = x;
        mouse_state.y = y;

        mouse_state.lastX = mouse_state.x;
        mouse_state.lastY = mouse_state.y;
    }
}