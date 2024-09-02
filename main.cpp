#include <windows.h>
#include <gl/gl.h>

// Function prototypes
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void EnableOpenGL(HWND hwnd, HDC *hDC, HGLRC *hGLRenderCtx);
void DisableOpenGL(HWND hwnd, HDC *hDC, HGLRC *hGLRenderCtx);
void Render();

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int) {
    WNDCLASS wc;
    HWND hwnd;
    HDC hDC;
    HGLRC hGLRenderCtx;
    MSG msg;
    bool quit = 0;

    // Register window class
    wc.style = CS_OWNDC;
    wc.lpfnWndProc = WindowProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = "GLSample";
    RegisterClass(&wc);

    // Create main window
    hwnd = CreateWindowEx(WS_EX_LAYERED,
        "GLSample", "OpenGL Desktop Drawing",
        WS_VISIBLE | WS_POPUP,
        100, 100, 600, 400,
        NULL, NULL, hInstance, NULL);

    SetLayeredWindowAttributes(hwnd, RGB(0, 0, 0), 0, LWA_COLORKEY);

    // Enable OpenGL for the window
    EnableOpenGL(hwnd, &hDC, &hGLRenderCtx);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Main loop
    while (!quit) {
        // Check for messages
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            // Handle or dispatch messages
            if (msg.message == WM_QUIT) {
                quit = TRUE;
            } else {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        } else {
            // OpenGL animation code goes here

            Render();

            // glClearColor(1.0f, 1.0f, 0.0f, 1.0f);
            // glClear(GL_COLOR_BUFFER_BIT);

            // Swap buffers
            SwapBuffers(hDC);
        }
    }

    // Shutdown OpenGL
    DisableOpenGL(hwnd, &hDC, &hGLRenderCtx);

    // Destroy the window explicitly
    DestroyWindow(hwnd);

    return msg.wParam;
}

void Render() {
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_TRIANGLES);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex2f(0.0f, 1.0f);
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex2f(-1.0f, -1.0f);
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex2f(1.0f, -1.0f);
    glEnd();
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_CLOSE:
        PostQuitMessage(0);
        return 0;

    case WM_DESTROY:
        return 0;

    case WM_KEYDOWN:
        if (wParam == VK_ESCAPE) {
            PostQuitMessage(0);
        }
        return 0;

    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}

void EnableOpenGL(HWND hwnd, HDC *hDC, HGLRC *hGLRenderCtx) {
    PIXELFORMATDESCRIPTOR pfd;
    int format;

    // Get the device context (DC)
    *hDC = GetDC(hwnd);

    // Set the pixel format for the DC
    ZeroMemory(&pfd, sizeof(pfd));
    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 16;
    pfd.iLayerType = PFD_MAIN_PLANE;
    
    format = ChoosePixelFormat(*hDC, &pfd);
    SetPixelFormat(*hDC, format, &pfd);

    // Create and enable the render context (RC)
    *hGLRenderCtx = wglCreateContext(*hDC);
    wglMakeCurrent(*hDC, *hGLRenderCtx);
}

void DisableOpenGL(HWND hwnd, HDC *hDC, HGLRC *hGLRenderCtx) {
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(*hGLRenderCtx);
    ReleaseDC(hwnd, *hDC);
}