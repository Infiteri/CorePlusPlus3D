#include "Input.h"
#include "Logger.h"

#include "Math/Vectors.h"

#include <unordered_map>

namespace Core
{
    struct InputMouseState
    {
        Vector2 position;
        Vector2 last;
        Vector2 delta;
    };

    static InputMouseState mouse_state;
    static std::unordered_map<Keys, bool> keys;
    static std::unordered_map<Keys, bool> justKeys;
    static std::unordered_map<Keys, bool> lastKeys;
    static std::unordered_map<Buttons, bool> buttons;

    void Input::Init()
    {
        for (auto &keyState : keys)
        {
            lastKeys[keyState.first] = keyState.second;
        }
    }

    void Input::Shutdown()
    {
    }

    bool Input::GetKey(Keys key)
    {
        return keys[key];
    }

    bool Input::GetKeyJustNow(Keys key)
    {
        return justKeys[key];
    }

    bool Input::GetButton(Buttons button)
    {
        return buttons[button];
    }

    int Input::GetMouseX()
    {
        return mouse_state.position.x;
    }

    int Input::GetMouseY()
    {
        return mouse_state.position.y;
    }

    int Input::GetMouseLastX()
    {
        return mouse_state.last.x;
    }

    int Input::GetMouseLastY()
    {
        return mouse_state.last.y;
    }

    int Input::GetMouseDeltaX()
    {
        return mouse_state.delta.x;
    }

    int Input::GetMouseDeltaY()
    {
        return mouse_state.delta.y;
    }

    int Input::GetMouseMovementDirectionX()
    {
        if (mouse_state.delta.x == 0)
            return 0;

        return mouse_state.delta.x < 0 ? -1 : 1;
    }

    int Input::GetMouseMovementDirectionY()
    {
        if (mouse_state.delta.y == 0)
            return 0;

        return mouse_state.delta.y < 0 ? -1 : 1;
    }
    void InputUpdateKey(Keys key, bool p)
    {
        lastKeys[key] = keys[key];
        keys[key] = p;
        justKeys[key] = (keys[key] && !lastKeys[key]);
    }

    void InputUpdateButton(Buttons button, bool p)
    {
        buttons[button] = p;
    }

    void InputUpdateMouse(int x, int y)
    {
        mouse_state.delta.Set(x - mouse_state.last.x, y - mouse_state.last.y);
        mouse_state.position.Set(x, y);
        mouse_state.position.Set(x, y);
    }
}