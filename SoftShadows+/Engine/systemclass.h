////////////////////////////////////////////////////////////////////////////////
// Filename: systemclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _SYSTEMCLASS_H_
#define _SYSTEMCLASS_H_


///////////////////////////////
// PRE-PROCESSING DIRECTIVES //
///////////////////////////////
#define WIN32_LEAN_AND_MEAN


//////////////
// INCLUDES //
//////////////
#include <windows.h>


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "inputclass.h"
#include "graphicsclass.h"
#include "timerclass.h"
#include "positionclass.h"

// ImGUI INCLUDES
#include "ImGUI\imgui.h"
#include "ImGUI\imgui_impl_win32.h"
#include "ImGUI\imgui_impl_dx11.h"


////////////////////////////////////////////////////////////////////////////////
// Class name: SystemClass
////////////////////////////////////////////////////////////////////////////////
class SystemClass
{
public:
	SystemClass();
	SystemClass(const SystemClass&);
	~SystemClass();

	bool Initialize();
	void Shutdown();
	void Run();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	bool Frame();
	bool HandleInput(float);
	void InitializeWindows(int&, int&);
	void ShutdownWindows();

private:
	LPCWSTR m_applicationName;
	HINSTANCE m_hinstance;
	HWND m_hwnd;

	InputClass* m_Input;
	GraphicsClass* m_Graphics;
	TimerClass* m_Timer;
	PositionClass* m_Position;

};


/////////////////////////
// FUNCTION PROTOTYPES //
/////////////////////////
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


/////////////
// GLOBALS //
/////////////
static SystemClass* ApplicationHandle = 0;
static float Blur_Size;
static int Blur_Samples_Count;
static float Blur_Standard_Deviation;
static bool ImGui_Loaded;
static bool inside_ImGui;
static int screenWidth;
static int screenHeight;
static int hwnd_Pos_X;
static int hwnd_Pos_Y;
static int Show_Cursor_Return_Value;
static float upsamplingLowerLimit;
static float upsamplingUpperLimit;

#endif