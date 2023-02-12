#include <Windows.h>
#include <napi.h>

#include <iostream>
#include <sstream>
#include <string>
#include <thread>

// Convert vkeyCode to string that matches these browser values
// https://developer.mozilla.org/en-US/docs/Web/API/KeyboardEvent/key/Key_Values
std::string ConvertKeyCodeToString(int key_stroke) {
    if ((key_stroke == 1) || (key_stroke == 2)) {
        return "";  // ignore mouse clicks
    }

    std::stringstream output;

    switch (key_stroke) {
        case VK_MENU:
        case VK_LMENU:
        case VK_RMENU:
            output << "Alt";
            break;
        case VK_LWIN:
        case VK_RWIN:
            output << "Meta";
            break;
        case VK_BACK:
            output << "Backspace";
            break;
        case VK_RETURN:
            output << "Enter";
            break;
        case VK_SPACE:
            output << "Spacebar";
            break;
        case VK_TAB:
            output << "Tab";
            break;
        case VK_SHIFT:
        case VK_LSHIFT:
        case VK_RSHIFT:
            output << "Shift";
            break;
        case VK_CONTROL:
        case VK_LCONTROL:
        case VK_RCONTROL:
            output << "Control";
            break;
        case VK_ESCAPE:
            output << "Escape";
            break;
        case VK_END:
            output << "End";
            break;
        case VK_HOME:
            output << "Home";
            break;
        case VK_LEFT:
            output << "ArrowLeft";
            break;
        case VK_UP:
            output << "ArrowUp";
            break;
        case VK_RIGHT:
            output << "ArrowRight";
            break;
        case VK_DOWN:
            output << "ArrowDown";
            break;
        case VK_CAPITAL:
            output << "CapsLock";
            break;
        case VK_PRIOR:
            output << "PageUp";
            break;
        case VK_NEXT:
            output << "PageDown";
            break;
        case VK_DELETE:
            output << "Delete";
            break;
        case VK_INSERT:
            output << "Insert";
            break;
        case VK_SNAPSHOT:
            output << "PrintScreen";
            break;
        case 190:
        case 110:
            output << ".";
            break;
        case 189:
        case 109:
            output << "-";
            break;
        default:
            if (key_stroke >= VK_F1 && key_stroke <= VK_F20) {
                output << "F" << (key_stroke - VK_F1 + 1);
            } else {
                // map virtual key according to keyboard layout
                char key = MapVirtualKeyExA(key_stroke, MAPVK_VK_TO_CHAR, GetKeyboardLayout(0));
                output << char(key);
            }
    }

    return output.str();
}


bool isKeyPressed(int key) {
    return ((GetAsyncKeyState(key) & 0x8000) != 0);
}

void OnKeyPressed(const Napi::CallbackInfo &info) {
    Napi::Env env = info.Env();
    Napi::Function cb = info[0].As<Napi::Function>();

    GetAsyncKeyState(VK_LBUTTON);
    GetAsyncKeyState(VK_RBUTTON);

    while (true)
    {
        //Check the mouse left button is pressed or not
        if (GetAsyncKeyState(VK_LBUTTON) & 1)
        {
            cb.Call(env.Global(), {Napi::String::New(env, "Left")});
        }
        //Check the mouse right button is pressed or not
        if (GetAsyncKeyState(VK_RBUTTON) & 1)
        {
            cb.Call(env.Global(), {Napi::String::New(env, "Right")});
        }

        for (int i = 8; i <= 190; i++) {
            if (isKeyPressed(i)) {
                cb.Call(env.Global(), {Napi::String::New(env, ConvertKeyCodeToString(i))});
            }
        }
    }
}

// Declare JS functions and map them to native functions
Napi::Object Init(Napi::Env env, Napi::Object exports) {
    exports.Set(Napi::String::New(env, "onKeyPressed"), Napi::Function::New(env, OnKeyPressed));
    return exports;
}

NODE_API_MODULE(keylogger, Init)
