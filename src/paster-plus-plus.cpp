// CMakeProject1.cpp : Defines the entry point for the application.
// Paster++ 
//
// Copywrite (C) Daniel Alfredsson, daniel@alfredsson.nu, 2021

#include "paster-plus-plus.h"



//using namespace std;

namespace constants {
    const std::string PRODUCT_NAME = "Keypaster++";
    const std::string PRODUCT_VERSION = "v1.0";
    const std::string PRODUCT_INFO = "https://github.com/hacke78/Paster-plus-plus";
}

#define ProgressBarSize     0.02            // Size of the screen in percentage to use for progress bar to adopt for various screen resoultions.
#define ProgressBarFont     "Consolas"      // Font for the progress bar text
#define VK_V                0x56            // Virtual-Key code for V
#define VK_Q                0x51            // Virtual-Key code for Q

#define SS_SHIFT            1               // Shift state mapping
#define SS_CTRL             2               // "
#define SS_SHIFT_CTRL       3               // "
#define SS_ALT              4               // "
#define SS_SHIFT_ALT        5               // "
#define SS_ALT_CTRL         6               // "

#define SC_LSHIFT           42              // Scan-code for Left Shift
#define SC_CTRL             29              // Scan-code for CTRL
#define SC_ALT              56              // Scan-code for ALT

#define INPUT_LOCALE        "00000409"      // Force english keyboard layout

#define MOD_NOREPEAT 	    0x4000

void clear_message_queue() {
    MSG msg;
    
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

//################################################################################################################
//# Forces the keyboard layout to be English layout
//# This works when in a console which is the intended purpose of this utility
//################################################################################################################
void force_eng_kbd_layout(){
    LoadKeyboardLayoutA(INPUT_LOCALE, KLF_ACTIVATE);

}

//################################################################################################################
// debug print function
//################################################################################################################

void __cdecl odprintf(const char* format, ...)
{
    char    buf[4096], * p = buf;
    va_list args;
    int     n;

    va_start(args, format);
    n = _vsnprintf(p, sizeof buf - 3, format, args); // buf-3 is room for CR/LF/NUL
    va_end(args);

    p += (n < 0) ? sizeof buf - 3 : n;

    while (p > buf && isspace(p[-1]))
        *--p = '\0';

    *p++ = '\r';
    *p++ = '\n';
    *p = '\0';

    OutputDebugStringA(buf);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_DESTROY:
        {
            PostQuitMessage(0);
            return 0;
        }
        case WM_CLOSE:
        {
            DestroyWindow(hwnd);
            break;
        }
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            //FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
            EndPaint(hwnd, &ps);
        }
        
        case WM_CTLCOLORSTATIC:
        {
            HDC hdcStatic = (HDC)wParam;
	    SetBkMode(hdcStatic, TRANSPARENT);
            SetTextColor(hdcStatic, RGB(255, 255, 255));
            HBRUSH BGColorBrush = CreateSolidBrush(RGB(0, 0, 255));
            return (LRESULT)BGColorBrush;
        }
        break;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
class ProgressBar_handler {
public:
    ProgressBar_handler();
    void Update(int progress, int max);
    void Reset();
    bool Destroy() { return DestroyWindow(hwnd); }
    ~ProgressBar_handler() { Destroy(); }
private:
    HWND hwnd;
    HWND hwndTextBox;
    int ScreenWidth;
    int ScreenHeight;

    void _UpdateWindow();

};

#define WindowClassName "keypaster-plus-plus";

ProgressBar_handler::ProgressBar_handler() {
    // Get the current Windows system screen size
    // FIXME: This might not well work if user changes the screen resolution after the program has started
    ScreenWidth = GetSystemMetrics(SM_CXSCREEN);
    ScreenHeight = GetSystemMetrics(SM_CYSCREEN);
    
    // Create winod Class for the main window
    const char g_szClassName[] = "myWindowClass";
    WNDCLASSEXA wc = { 0 };
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = WindowClassName;
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = 0;
    wc.lpfnWndProc = WindowProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = GetModuleHandle(NULL);
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    //wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.hbrBackground = CreateSolidBrush(RGB(0, 0, 255));
    wc.lpszMenuName = NULL;
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    RegisterClassExA(&wc);

    // Create the main window for the progress bar
    hwnd = CreateWindowExA(WS_EX_TOPMOST | WS_EX_TOOLWINDOW, (LPCSTR) g_szClassName, (LPCSTR) "Main Window", WS_POPUP | WS_VISIBLE, 0, 0, 0, 0, NULL, NULL, wc.hInstance, NULL);
    if (hwnd) { 
        ShowWindow(hwnd, SW_SHOW);
        _UpdateWindow(); 
    }
    else {
        /*Handle Error*/
    }
    Sleep (1000);
    
    // Create the text box for the progress text
    hwndTextBox = CreateWindowExA(WS_EX_TRANSPARENT, "STATIC", "", WS_CHILD | SS_LEFT, 0, 0, ScreenWidth, (int) (ScreenHeight * ProgressBarSize), hwnd, NULL, wc.hInstance, NULL);
    HFONT hFont = CreateFontA((int) (ScreenHeight * ProgressBarSize), 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, ProgressBarFont);
    SendMessage(hwndTextBox, WM_SETFONT, WPARAM(hFont), TRUE);
    ShowWindow(hwndTextBox, SW_SHOW);
    _UpdateWindow();

}

void ProgressBar_handler::Update(int progress, int max) {
    std::string text;
    
    text = std::to_string(progress + 1) + "/" + std::to_string(max);
    SendMessage(hwndTextBox, WM_SETTEXT, 0, (LPARAM)text.c_str());
    SetWindowPos(hwnd, 0, 0, 0, (int) round(ScreenWidth * (progress + 1)/max), (int) (ScreenHeight * ProgressBarSize), SWP_NOACTIVATE | SWP_SHOWWINDOW | SWP_NOMOVE);
    _UpdateWindow();
}

void ProgressBar_handler::Reset() {
    
    SetWindowPos(hwnd, 0, 0, 0, 0, 0, SWP_NOACTIVATE | SWP_SHOWWINDOW | SWP_NOMOVE);
    _UpdateWindow();
}
void ProgressBar_handler::_UpdateWindow() {
    if (hwnd) {
        ShowWindow(hwnd, SW_SHOW);
        UpdateWindow(hwnd);
    }
    else {
        /*Handle Error*/
    }
}
class HotKey_handler {
public:
    enum { PASTE_KEYID = 1, QUIT_KEYID = 2 };
    HotKey_handler();

//private:

};


class ClipBoard_handler {
public:
    ClipBoard_handler() {
        CharPosition = 0;
        ClipBoardLength = 0;
        // Try opening the clipboard
        if (!OpenClipboard(nullptr)) {
            // error
        }
        // Get handle of clipboard object for ANSI text
        HANDLE hData = GetClipboardData(CF_TEXT);
        if (hData == NULL) {
            return;
        }

        // Lock the handle to get the actual text pointer
        char* pszText = static_cast<char*>(GlobalLock(hData));
        if (pszText == NULL) {
            return;
        }

        // Save text in a string class instance
        ClipBoardContent = std::string(pszText);
        ClipBoardLength = ClipBoardContent.length();
        
        // Replace \n\r with \n
        std::regex newlines_re("\r\n+");
        ClipBoardContent = std::regex_replace(ClipBoardContent, newlines_re, "\n");
        odprintf("Clipboard content: %s", ClipBoardContent.c_str());
        odprintf("Clipboard length: %d", ClipBoardLength);

        // Release the lock
        GlobalUnlock(hData);

        // Release the clipboard
        CloseClipboard();
    }
    
    size_t GetClipBoardSize() {
        return ClipBoardLength;
    }
    
    char GetNextChar() {
        return ClipBoardContent[CharPosition++];
    };
private:
    size_t CharPosition;
    size_t ClipBoardLength;
    std::string ClipBoardContent;

};

class kbdEmulator_handler {
public:
    kbdEmulator_handler() {}
    ~kbdEmulator_handler() {}
    void SendKey(TCHAR ch) {
        short virtKeyCode_raw = VkKeyScanW(ch);
        short virtKeyCode = virtKeyCode_raw & 0xFF; // Extract the low byte part(key)
        short shift_state = (virtKeyCode_raw & 0xFF00) >> 8; // Extract the hi byte part(shift state)
        UINT scancode = MapVirtualKeyEx(virtKeyCode, MAPVK_VK_TO_VSC, LoadKeyboardLayoutA(INPUT_LOCALE, KLF_ACTIVATE));
        Sleep(20);
        odprintf("shift state: %d\n", (int) shift_state);
        odprintf("Scancode: %d\n", scancode);
        switch (shift_state) {
            case SS_SHIFT: SendScanCode(SC_LSHIFT); break;
            case SS_CTRL: SendScanCode(SC_CTRL); break;
            case SS_ALT: SendScanCode(SC_ALT); break;
            case SS_SHIFT_CTRL: SendScanCode(SC_LSHIFT); SendScanCode(SC_CTRL); break;
            case SS_SHIFT_ALT: SendScanCode(SC_LSHIFT); SendScanCode(SC_ALT); break;
            case SS_ALT_CTRL: SendScanCode(SC_ALT); SendScanCode(SC_CTRL); break;
        }
        SendScanCode(scancode);
        Sleep(30);
        SendScanCode(scancode, KEYEVENTF_KEYUP);
        Sleep(30);
        switch (shift_state) {
            case SS_SHIFT: SendScanCode(SC_LSHIFT, KEYEVENTF_KEYUP); break;
            case SS_CTRL: SendScanCode(SC_CTRL, KEYEVENTF_KEYUP); break;
            case SS_ALT: SendScanCode(SC_ALT, KEYEVENTF_KEYUP); break;
            case SS_SHIFT_CTRL: SendScanCode(SC_LSHIFT, KEYEVENTF_KEYUP); SendScanCode(SC_CTRL, KEYEVENTF_KEYUP); break;
            case SS_SHIFT_ALT: SendScanCode(SC_LSHIFT, KEYEVENTF_KEYUP); SendScanCode(SC_ALT, KEYEVENTF_KEYUP); break;
            case SS_ALT_CTRL: SendScanCode(SC_ALT, KEYEVENTF_KEYUP); SendScanCode(SC_CTRL, KEYEVENTF_KEYUP); break;
        }
    }
    void ResetShiftKeys() {
        SendScanCode(SC_LSHIFT, KEYEVENTF_KEYUP);
        SendScanCode(SC_CTRL, KEYEVENTF_KEYUP);
        SendScanCode(SC_ALT, KEYEVENTF_KEYUP);
    }
private:
    void SendScanCode(DWORD code, int state=0) {
        INPUT inputs[1];
        inputs[0].type = INPUT_KEYBOARD;
        inputs[0].ki.wScan = (WORD) code;
        inputs[0].ki.time = 0;
        inputs[0].ki.wVk = 0;
        inputs[0].ki.dwExtraInfo = (ULONG_PTR) NULL;
        inputs[0].ki.dwFlags = KEYEVENTF_SCANCODE;
              
        if (state == KEYEVENTF_KEYUP) {
            inputs[0].ki.dwFlags |= KEYEVENTF_KEYUP;
        }
        SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));
    }
};
HotKey_handler::HotKey_handler() {
    
    ProgressBar_handler *progress_bar = new ProgressBar_handler();
        
    if (RegisterHotKey(NULL, PASTE_KEYID, MOD_CONTROL | MOD_ALT | MOD_NOREPEAT, VK_V))
    {
        odprintf("Hotkey 'ALT+CTRL+v' registered\n");
    }
    if (RegisterHotKey(NULL, QUIT_KEYID, MOD_CONTROL | MOD_ALT | MOD_NOREPEAT, VK_Q))
    {
        odprintf("Hotkey 'ALT+CTRL+q' registered\n");
    }

    MSG msg = { 0 };
    while (GetMessage(&msg, NULL, 0, 0) != 0)
    {
        if (msg.message == WM_HOTKEY)
        {
            odprintf("key recieved: %d", msg.wParam);
            if (msg.wParam == PASTE_KEYID) {
                odprintf("WM_HOTKEY paste received\n");
                ClipBoard_handler* clipboard = new ClipBoard_handler();
                kbdEmulator_handler* kbdEmulator = new kbdEmulator_handler;
                kbdEmulator->ResetShiftKeys();
                force_eng_kbd_layout();
                Sleep(500);
                for (int i = 0; i < clipboard->GetClipBoardSize(); i++) {
                    progress_bar->Update(i, (int) clipboard->GetClipBoardSize());
                    kbdEmulator->SendKey(clipboard->GetNextChar());
                    
                }

                progress_bar->Reset();
                delete clipboard;
                delete kbdEmulator;
            }
            if (msg.wParam == QUIT_KEYID) {
                odprintf("WM_HOTKEY quit received\n");
                odprintf("program quit");
                std::string MsgBoxText = constants::PRODUCT_NAME + "Terminated";
                std::string MsxBoxCaption = constants::PRODUCT_NAME + " " + constants::PRODUCT_VERSION + " by Daniel Alfredsson";
                int msgboxID = MessageBoxA(NULL, MsgBoxText.c_str(), MsxBoxCaption.c_str(), MB_OK | MB_ICONINFORMATION | MB_TOPMOST | MB_SETFOREGROUND | MB_DEFAULT_DESKTOP_ONLY);
                exit(0);

            }
        }
    }
}




int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
    odprintf("Program start");
    std::string MsgBoxText = "This utility lets you paste content from clipboard into a remote console\n\nUsage:\nCTRL+ALT+V to paste.\nCTRL+ALT+Q to quit\n\nFor more info: " + constants::PRODUCT_INFO;
    std::string MsxBoxCaption = constants::PRODUCT_NAME + " " + constants::PRODUCT_VERSION + " by Daniel Alfredsson";
    int nButtonPressed = 0;
    

    int msgboxID = MessageBoxA(NULL, MsgBoxText.c_str(), MsxBoxCaption.c_str(), MB_OK | MB_ICONINFORMATION | MB_TOPMOST | MB_SETFOREGROUND | MB_DEFAULT_DESKTOP_ONLY);
 
    clear_message_queue();
    force_eng_kbd_layout();
    HotKey_handler *hot_key = new HotKey_handler();
 
    return 0;
}
