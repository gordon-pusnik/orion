#include "stdafx.h"
#include <GL/glew.h>
#include "GameBase.h"
#include "GLFont.h"
#include <io.h>
#include <fcntl.h>
// GLOBALS

HDC				hDC = NULL;				// Private GDI Device Context
HGLRC			hRC = NULL;				// Permanent Rendering Context
HWND			hWnd = NULL;			// Holds Our Window Handle
HINSTANCE		hInstance;				// Holds The Instance Of The Application

bool			g_KeyBuffer[256];		// Array Used For key buffering.
bool			g_WindowActive = true;	// Window Active Flag Set To true By Default

bool			g_Fullscreen = false;

bool			g_Done = false;			
unsigned char	g_ucKey = 0;			// Normal keyboard key, A-Z, etc.
int				g_iSpecKey = 0;			// Special keyboard key, F1, etc. 
int				g_iMouseButton = 0;		
int				g_iMouseState = 0;
int				g_iMouseX = 0;
int				g_iMouseY = 0;
int				g_iMouseDragX = 0;
int				g_iMouseDragY = 0;
int				g_iMouseMoveX = 0;
int				g_iMouseMoveY = 0;
int				g_iMouseModifiers = 0;
unsigned long	g_ulTimer = 0;
int				g_iInitDone = 0;
int				g_BitsPerPixel = 32;
int				g_Width = 1280;
int				g_Height = 768;
int				g_Argc = 2;
char*			g_Argv[2];
GameBase*		g_Base = NULL;

// PROTOTYPES.

LRESULT	CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
bool createGLContext(char* title, int width, int height, int bits, bool fullscreen);
GLvoid killGLContext(GLvoid);
GLboolean (APIENTRY *fnwglSwapIntervalEXT)(GLint interval);
int       (APIENTRY *fnwglGetSwapIntervalEXT)(void);

int checkPixelFormatsAvailable(void);
void init(int argc, char** argv, int w, int h);
void cleanup(void);
void reshape(int w, int h);
void display(void);
void loop(void);
void keyboard(unsigned char key, int x, int y);
void keyboardExtendedKeys(int key, int x, int y);
void mouse(int button, int state, int x, int y);
void mouseDrag(int x, int y);
void mouseMove(int x, int y);
void swapBuffers(void);
void setStdOutToNewConsole(void);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	MSG	msg;										// Windows Message Structure

	// Get the current device pixel depth.
	DEVMODE dmScreenSettings;										// Device Mode
	memset(&dmScreenSettings, 0 , sizeof ( dmScreenSettings));	// Makes Sure Memory's Cleared
	::EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &dmScreenSettings);
//	g_BitsPerPixel = dmScreenSettings.dmBitsPerPel;					// Selected Bits Per Pixel
//	g_Width = dmScreenSettings.dmPelsWidth;
//	g_Height = dmScreenSettings.dmPelsHeight;
		
	if (g_BitsPerPixel == 24) { // We don't allow 24 bit.
		g_BitsPerPixel = 16;
	}

	if (! createGLContext("Orion", g_Width, g_Height, g_BitsPerPixel, g_Fullscreen)) {
		return 0;									// Quit If Window Was Not Created
	}

	GLFont::setWGLContext(hDC);

	setStdOutToNewConsole();

	init(g_Argc, g_Argv, g_Width, g_Height);

	while (! g_Done) {
		if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) { // Is There A Message Waiting?
			if (msg.message == WM_QUIT) {		   		  // Have We Received A Quit Message?
				g_Done = true;
			}
			else {										// If Not, Deal With Window Messages
				::TranslateMessage(&msg);				// Translate The Message
				::DispatchMessage(&msg);				// Dispatch The Message
			}
		}
		else {											// If There Are No Messages
			if (g_WindowActive) {						// Program Active?
				loop();
			}
		}
	}

	// Shutdown
	cleanup();											// cleanup
	return ((int) msg.wParam);							// Exit The Program
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) {									// Check For Windows Messages
		case WM_ACTIVATE:							// Watch For Window Activate Message
		{
			if (! HIWORD(wParam)) {				 	// Check Minimization State
				g_WindowActive = true;				// Program Is Active
			}
			else {
				g_WindowActive = false;				// Program Is No Longer Active
			}

			return 0;								
		}
		case WM_SYSCOMMAND:							// Intercept System Commands
		{
			switch (wParam) {						// Check System Calls
				case SC_SCREENSAVE:					// Screensaver Trying To Start?
				case SC_MONITORPOWER:				// Monitor Trying To Enter Powersave?
				return 0;							// Prevent From Happening
			}
			break;									
		}
		case WM_CLOSE:								// Did We Receive A Close Message?
		{
			::PostQuitMessage(0);					// Send A Quit Message
			return 0;								
		}
		case WM_KEYDOWN:							// Is A Key Being Held Down?
		{
			g_KeyBuffer[wParam] = true;				// If So, Mark It As true
			if (g_KeyBuffer[VK_F1]) {
				keyboardExtendedKeys(VK_F1, 0, 0);
			}
			else if (g_KeyBuffer[VK_F2]) {
				keyboardExtendedKeys(VK_F2, 0, 0);
			}
			else if (g_KeyBuffer[VK_F3]) {
				keyboardExtendedKeys(VK_F3, 0, 0);
			}
			else if (g_KeyBuffer[VK_RIGHT]) {
				keyboardExtendedKeys(VK_RIGHT, 0, 0);
			}
			else if (g_KeyBuffer[VK_LEFT]) {
				keyboardExtendedKeys(VK_LEFT, 0, 0);
			}
			else if (g_KeyBuffer[VK_SHIFT]) {
				g_iMouseModifiers = 1;
			}
			else if (g_KeyBuffer[VK_CONTROL]) {
				g_iMouseModifiers = 2;
			}

			g_Base->keyDown((unsigned char) wParam);

			return 0;								
		}
		case WM_KEYUP:								// Has A Key Been Released?
		{
			g_KeyBuffer[wParam] = false;			// If So, Mark It As FALSE

			g_Base->keyUp((unsigned char) wParam);

			return 0;								
		}
		case WM_CHAR:
		{
			keyboard((unsigned char) wParam, 0, 0);
			return 0;
		}
		case WM_LBUTTONDOWN:
		{
			mouse(0, 0, LOWORD(lParam), HIWORD(lParam));
			return 0;
		}
		case WM_LBUTTONUP:
		{
			mouse(0, 1, LOWORD(lParam), HIWORD(lParam));
			return 0;
		}
		case WM_RBUTTONDOWN:
		{
			mouse(2, 0, LOWORD(lParam), HIWORD(lParam));
			return 0;
		}
		case WM_RBUTTONUP:
		{
			mouse(2, 1, LOWORD(lParam), HIWORD(lParam));
			return 0;
		}
		case WM_MOUSEMOVE:
		{
			if (wParam & MK_LBUTTON) {
				mouseDrag(LOWORD(lParam), HIWORD(lParam));
			}
			else {
				mouseMove(LOWORD(lParam), HIWORD(lParam));
			}

			return 0;
		}
		case WM_SIZE:								// Resize The OpenGL Window
		{
			reshape(LOWORD(lParam), HIWORD(lParam));  // LoWord=Width, HiWord=Height
			return 0;								
		}
	}

	// Pass All Unhandled Messages To DefWindowProc
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

 
bool createGLContext(char* title, int width, int height, int bits, bool fullscreen)
{
	GLuint		PixelFormat;			// Holds The Results After Searching For A Match
	WNDCLASS	wc;						// Windows Class Structure
	DWORD		dwExStyle;				// Window Extended Style
	DWORD		dwStyle;				// Window Style
	RECT		WindowRect;				// Grabs Rectangle Upper Left / Lower Right Values
	WindowRect.left = (long) 0;			// Set Left Value To 0
	WindowRect.right = (long) width;	// Set Right Value To Requested Width
	WindowRect.top = (long) 0;			// Set Top Value To 0
	WindowRect.bottom= (long) height;	// Set Bottom Value To Requested Height

	g_Fullscreen = fullscreen;			// Set The Global Fullscreen Flag

	hInstance			= ::GetModuleHandle(NULL);				// Grab An Instance For Our Window
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw On Size, And Own DC For Window.
	wc.lpfnWndProc		= (WNDPROC) WndProc;					// WndProc Handles Messages
	wc.cbClsExtra		= 0;									// No Extra Window Data
	wc.cbWndExtra		= 0;									// No Extra Window Data
	wc.hInstance		= hInstance;							// Set The Instance
	wc.hIcon			= ::LoadIcon(NULL, IDI_WINLOGO);		// Load The Default Icon
	wc.hCursor			= ::LoadCursor(NULL, IDC_ARROW);		// Load The Arrow Pointer
	wc.hbrBackground	= NULL;									// No Background Required For GL
	wc.lpszMenuName		= NULL;									// We Don't Want A Menu
	wc.lpszClassName	= "OpenGL";								// Set The Class Name

	if (! ::RegisterClass(&wc))	{								// Attempt To Register The Window Class
		::MessageBox(NULL, "Failed To Register The Window Class.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}
	
	if (g_Fullscreen)	{										// Attempt Fullscreen Mode?
		DEVMODE dmScreenSettings;								// Device Mode
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));	// Makes Sure Memory's Cleared
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);		// Size Of The Devmode Structure
		dmScreenSettings.dmPelsWidth = width;					// Selected Screen Width
		dmScreenSettings.dmPelsHeight = height;					// Selected Screen Height
		dmScreenSettings.dmBitsPerPel = bits;					// Selected Bits Per Pixel
		dmScreenSettings.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;

		// Try To Set Selected Mode And Get Results.  NOTE: CDS_FULLSCREEN Gets Rid Of Start Bar.
		if (::ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL) {
			::MessageBox ( NULL, "The Requested Fullscreen Mode Is Not Supported By\nYour Video Card.", "ERROR", MB_OK | MB_ICONEXCLAMATION );
			return false;
		}
	}

	if (g_Fullscreen)	{										// Are We Still Fullscreen Mode?
		dwExStyle = WS_EX_APPWINDOW;							// Window Extended Style
		dwStyle = WS_POPUP;										// Windows Style
		::ShowCursor(true);										// Hide Mouse Pointer
	}
	else {
		dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// Window Extended Style
		dwStyle = WS_OVERLAPPEDWINDOW;							// Windows Style
	}

	::AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle); // Adjust Window To True Requested Size

	if (! g_Fullscreen) {
		int pos_x = 0;
		int pos_y = 0;
	
		// Create The Window
		if ( ! ( hWnd = CreateWindowEx (dwExStyle,							// Extended Style For The Window
										"OpenGL",							// Class Name
										title,								// Window Title
										dwStyle |							// Defined Window Style
										WS_CLIPSIBLINGS |					// Required Window Style
										WS_CLIPCHILDREN,					// Required Window Style
										pos_x, pos_y,						// Window Position
										WindowRect.right-WindowRect.left,	// Calculate Window Width
										WindowRect.bottom-WindowRect.top,	// Calculate Window Height
										NULL,								// No Parent Window
										NULL,								// No Menu
										hInstance,							// Instance
										NULL)))								// Dont Pass Anything To WM_CREATE
		{
			killGLContext();	// Reset The Display
			::MessageBox ( NULL, "Window Creation Error.", "ERROR", MB_OK | MB_ICONEXCLAMATION );
			return false;
		}
	}
	else {
		// Create The Window
		if ( ! ( hWnd = CreateWindowEx (dwExStyle,							// Extended Style For The Window
										"OpenGL",							// Class Name
										title,								// Window Title
										dwStyle |							// Defined Window Style
										WS_CLIPSIBLINGS |					// Required Window Style
										WS_CLIPCHILDREN,					// Required Window Style
										0, 0,								// Window Position
										WindowRect.right-WindowRect.left,	// Calculate Window Width
										WindowRect.bottom-WindowRect.top,	// Calculate Window Height
										NULL,								// No Parent Window
										NULL,								// No Menu
										hInstance,							// Instance
										NULL)))								// Dont Pass Anything To WM_CREATE
		{
			killGLContext();	// Reset The Display
			::MessageBox(NULL, "Window Creation Error.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
			return false;
		}
	}	

	static	PIXELFORMATDESCRIPTOR pfd =				// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Must Support Window
		PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request An RGBA Format
		bits,										// Select Our Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits Ignored
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		0,											// 16Bit Z-Buffer (Depth Buffer)  
		0,											// No Stencil Buffer
		0,											// No Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};
	
	if (! (hDC = ::GetDC(hWnd))) {					// Did We Get A Device Context?
		killGLContext();							// Reset The Display
		::MessageBox(NULL, "Can't Create A GL Device Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}

	if (checkPixelFormatsAvailable() == -1) {
		killGLContext();
		return false;
	}	

	if (! (PixelFormat = ::ChoosePixelFormat(hDC, &pfd))) {	// Did Windows Find A Matching Pixel Format?
		killGLContext();								// Reset The Display
		::MessageBox (NULL, "Can't Find A Suitable PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}

	if (! ::SetPixelFormat(hDC, PixelFormat, &pfd))	{	// Are We Able To Set The Pixel Format?
		killGLContext();								// Reset The Display
		::MessageBox(NULL, "Can't Set The PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}

	if (! (hRC = ::wglCreateContext(hDC))) {	// Are We Able To Get A Rendering Context?
		killGLContext();								// Reset The Display
		::MessageBox(NULL, "Can't Create A GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}

	if (! ::wglMakeCurrent(hDC, hRC)) {		// Try To Activate The Rendering Context
		killGLContext();								// Reset The Display
		::MessageBox(NULL, "Can't Activate The GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}

	/******** Trying to lock to vsync ********/
	fnwglSwapIntervalEXT        = (unsigned char (__stdcall *)(int)) wglGetProcAddress("wglSwapIntervalEXT");
	//fnwglGetSwapIntervalEXT     = (void *) wglGetProcAddress("wglGetSwapIntervalEXT");

	if (fnwglSwapIntervalEXT) {
		fnwglSwapIntervalEXT(1);
	}
	/*****************************************/

	::ShowWindow(hWnd, SW_SHOW);					// Show The Window
	::SetForegroundWindow(hWnd);					// Slightly Higher Priority
	
//	if (! g_Fullscreen && Preferences::getSingletonPtr()->getAlwaysInFront()) {
//		::SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_SHOWWINDOW | SWP_NOSIZE);
//	}
	
	::SetFocus(hWnd);								// Sets Keyboard Focus To The Window

	return true;									// Success
}

GLvoid killGLContext(GLvoid)							// Properly Kill The Window
{
	if (g_Fullscreen) {									// Are We In Fullscreen Mode?
		::ChangeDisplaySettings(NULL, 0);				// If So Switch Back To The Desktop
	}

	if (hRC) {											// Do We Have A Rendering Context?
		if (! ::wglMakeCurrent ( NULL, NULL)) {		// Are We Able To Release The DC And RC Contexts?
			::MessageBox(NULL, "Release Of DC And RC Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		}

		if ( ! ::wglDeleteContext(hRC))	{			// Are We Able To Delete The RC?
			::MessageBox(NULL, "Release Rendering Context Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		}
		hRC = NULL;										// Set RC To NULL
	}

	if (hDC && ! ::ReleaseDC(hWnd, hDC)) {				// Are We Able To Release The DC
		::MessageBox(NULL, "Release Device Context Failed.", "SHUTDOWN ERROR" , MB_OK | MB_ICONINFORMATION);
		hDC = NULL;										// Set DC To NULL
	}

	if (hWnd && ! ::DestroyWindow(hWnd)) {				// Are We Able To Destroy The Window?
		::MessageBox(NULL, "Could Not Release hWnd.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		hWnd = NULL;									// Set hWnd To NULL
	}

	if (! ::UnregisterClass("OpenGL", hInstance)) {		// Are We Able To Unregister Class
		::MessageBox(NULL, "Could Not Unregister Class.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		hInstance = NULL;								// Set hInstance To NULL
	}
}

void loop(void) 
{
	g_iInitDone = 1;
	
	// Handle Input.
	if (g_ucKey != 0 || g_iSpecKey != 0) {
//		g_Base->processKeyPress(g_ucKey, g_iSpecKey);
	}	
	g_ucKey = 0;
	g_iSpecKey = 0;

	static int button = 0;
	static int state = 0;
	static int x = 0;
	static int y = 0;
	bool processed = false;

	if (g_iMouseX != 0 || g_iMouseY != 0) {
//		g_pBaseImpl->processMouse(g_iMouseButton, g_iMouseState, g_iMouseX, g_iMouseY);

		button = g_iMouseButton;
		state = g_iMouseState;
		x = g_iMouseX;
		y = g_iMouseY;
		processed = true;
	}
	
	g_iMouseButton = 0;
	g_iMouseState = 0;
	g_iMouseX = 0;
	g_iMouseY = 0;
	
//	if ((x != 0 || y != 0) && state == MOUSE_BUTTON_DOWN && ! processed &&
//		 g_iMouseDragX == 0 && g_iMouseDragY == 0) {
//		g_pBaseImpl->processMouseHeld(button, state, x, y);
//	}

	processed = false;

	if (g_iMouseDragX != 0 || g_iMouseDragY != 0) {
//		g_pBaseImpl->processMouseDrag(g_iMouseModifiers, g_iMouseDragX, g_iMouseDragY);
	}	
	g_iMouseDragX = 0;
	g_iMouseDragY = 0;

	if (g_iMouseMoveX != 0 || g_iMouseMoveY != 0) {
//		g_pBaseImpl->processMouseMove(g_iMouseModifiers, g_iMouseMoveX, g_iMouseMoveY);
	}	
	g_iMouseMoveX = 0;
	g_iMouseMoveY = 0;
	g_iMouseModifiers = 0;

	_int64 freq;
	::QueryPerformanceFrequency((LARGE_INTEGER*) &freq);
	int div = (int) (freq / 1000000.0);

	_int64 time;
	::QueryPerformanceCounter((LARGE_INTEGER*) &time);
	g_ulTimer = (unsigned long) (( time / div ) & 0x000fffff);

	g_ulTimer = ::timeGetTime();

	// Update machine state.    
	g_Base->run(g_ulTimer);

	if (g_Base->m_bGameOver) {
		cleanup();
		exit(0);
	}

	// post redisplay message which calls our display function.
	display();

	return;
}

void keyboard(unsigned char key, int x, int y)
{
	g_ucKey = key;

	return;
}

void keyboardExtendedKeys(int key, int x, int y)
{
	g_iSpecKey = key;

	return;
}

void mouse(int button, int state, int x, int y)
{
	g_iMouseButton = button;
	g_iMouseState = state;
	g_iMouseX = x;
	g_iMouseY = y;

	if (state) {
		g_Base->mouseUp(x, g_Height - y);
	}
	else {
		g_Base->mouseDown(x, g_Height - y);
	}

	return;
}

void mouseDrag(int x, int y)
{
	g_iMouseDragX = x;
	g_iMouseDragY = y;

	g_Base->mouseDrag(x, g_Height - y);

	return;
}

void mouseMove(int x, int y)
{
	g_iMouseMoveX = x;
	g_iMouseMoveY = y;

	return;	
}

void init(int argc, char** argv, int w, int h)
{
	g_Base = new GameBase;
	g_Base->init(w, h);

	return;
}

void cleanup(void)
{
	delete g_Base;

	killGLContext();

	return;
}

void reshape(int w, int h)
{
	if (g_Base) 
		g_Base->reshape(w, h);

	return;
}

void display(void)
{
	if (g_iInitDone) {
		// Draw scene on back buffer.
		g_Base->display();

		swapBuffers();
	}

	return;
}

void swapBuffers(void)
{
	// Swap buffers.
	::SwapBuffers(hDC);				// Swap Buffers (Double Buffering)
}

int checkPixelFormatsAvailable(void)
{
	PIXELFORMATDESCRIPTOR pfd;
	int iRet = 1;
	bool gen_accel;
	bool gen_format;
	bool support_opengl;
	bool double_buffered;
	bool draw_to_window;

	for (int i = 1; iRet != 0; i++) {
		gen_accel = false;
		gen_format = false;
		support_opengl = false;
		double_buffered = false;
		draw_to_window = false;

		iRet = ::DescribePixelFormat(hDC, i, sizeof(pfd), &pfd);

		char buffer[30];
		sprintf(buffer, "\n\nPFD: %d\n", i);
		::OutputDebugString(buffer);

		if (pfd.dwFlags & PFD_GENERIC_ACCELERATED) {
			gen_accel = true;

			::OutputDebugString("Generic accelerated\n");
		}

		if (pfd.dwFlags & PFD_GENERIC_FORMAT) {
			gen_format = true;

			::OutputDebugString("Generic format\n");
		}

		if (pfd.dwFlags & PFD_SUPPORT_OPENGL) {
			support_opengl = true;

			::OutputDebugString("OpenGL\n");
		}

		if (pfd.dwFlags & PFD_DOUBLEBUFFER) {
			double_buffered = true;

			::OutputDebugString("Double buffered\n");
		}

		if (pfd.dwFlags & PFD_DRAW_TO_WINDOW) {
			draw_to_window = true;

			::OutputDebugString("Draw to window\n");
		}


		if (pfd.iPixelType & PFD_TYPE_RGBA) {
			::OutputDebugString("Type rgba\n");
		}

		if (! gen_format && ! gen_accel && support_opengl && double_buffered && draw_to_window) {
			// Hardware accelerated icd;
			return iRet;
		}
	}

	// No hardware acceleration available.
	::MessageBox(hWnd, "Accelerated OpenGL not available", "Error", 0);
	return -1;
}

void setStdOutToNewConsole()
{
    // allocate a console for this app
    AllocConsole();

    // redirect unbuffered STDOUT to the console
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    int fileDescriptor = _open_osfhandle((intptr_t)consoleHandle, _O_TEXT);
    FILE *fp = _fdopen( fileDescriptor, "w" );
    *stdout = *fp;
    setvbuf( stdout, NULL, _IONBF, 0 );

    // give the console window a nicer title
    SetConsoleTitle("Debug Output");

    // give the console window a bigger buffer size
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if ( GetConsoleScreenBufferInfo(consoleHandle, &csbi) )
    {
        COORD bufferSize;
        bufferSize.X = csbi.dwSize.X;
        bufferSize.Y = 9999;
        SetConsoleScreenBufferSize(consoleHandle, bufferSize);
    }
}