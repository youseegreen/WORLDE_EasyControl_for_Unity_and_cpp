#pragma once

//コメントアウト：dll作成モード、コメント：debugモードmain関数必要
//それに合わせて、プロジェクトのプロパティの構成の種類をexeとdllに切り替える必要あり

//#define DEBUG_MODE 

#ifdef DEBUG_MODE
#define UNITY_INTERFACE_API  
#define UNITY_INTERFACE_EXPORT  
#else
#define UNITY_INTERFACE_API __stdcall
#define UNITY_INTERFACE_EXPORT __declspec(dllexport)
#endif

//出力する関数
#ifndef DEBUG_MODE
extern "C"
{
#endif

// API for Unity
	UNITY_INTERFACE_EXPORT bool UNITY_INTERFACE_API OpenController();
	UNITY_INTERFACE_EXPORT bool UNITY_INTERFACE_API CloseController();
	UNITY_INTERFACE_EXPORT bool UNITY_INTERFACE_API GetSliderValue(float* out_array, int first_idx = -1, int last_idx = -1);
	UNITY_INTERFACE_EXPORT bool UNITY_INTERFACE_API GetKnobValue(float* out_array, int first_idx = -1, int last_idx = -1);
	UNITY_INTERFACE_EXPORT bool UNITY_INTERFACE_API GetButtonState(int* out_array, int first_idx = -1, int last_idx = -1);
	UNITY_INTERFACE_EXPORT bool UNITY_INTERFACE_API GetSystemButtonState(int* out_array, int first_idx = -1, int last_idx = -1);
	UNITY_INTERFACE_EXPORT bool UNITY_INTERFACE_API GetSystemButtonStateWithName(int* out_array, const char* name);
	UNITY_INTERFACE_EXPORT bool UNITY_INTERFACE_API SetButtonOnInterval(int msec);

#ifndef DEBUG_MODE
}
#endif