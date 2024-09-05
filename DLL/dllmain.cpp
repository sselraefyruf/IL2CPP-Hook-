// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <iostream>
#include <Windows.h>

#include "MinHook/MinHook.h"
#if _WIN64 
#pragma comment(lib, "MinHook/libMinHook.x64.lib")
#else
#pragma comment(lib, "MinHook/libMinHook.x86.lib")
#endif

uintptr_t GameAssembly = reinterpret_cast<uintptr_t>(GetModuleHandle(L"GameAssembly.dll"));

void CreateConsole() {
    AllocConsole();
    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);
}

void Init() {
    MH_Initialize();
    CreateConsole();
    printf("Start! \n");
}

void(__fastcall* set_recoil_o)(DWORD*, float, float, float, float,float,DWORD*);
void __stdcall set_recoil_hook(DWORD* __this, float a, float b, float c, float d,float e,DWORD* __method) {
    a = 0, b = 0, c = 0, d = 0,e = 0;
    printf("set recoil hook \n");
    return set_recoil_o(__this, a, b, c, d,e, __method);
}

void Main() {
    Init();

    //{
    //	"Address": 5719760, | 0x5746D0
    //	"Name" : "CameraShaking.RecoilSettings$$.ctor",
    //	"Signature" : "void CameraShaking_RecoilSettings___ctor (CameraShaking_RecoilSettings_o __this, float animationTime, float zAxis, float fovKick, float upKick, float sideKick, const MethodInfo* method);",
    //	"TypeSignature" : "vifffffi"
    //},

    MH_CreateHook(reinterpret_cast<LPVOID*>(GameAssembly + 0x5746D0), &set_recoil_hook, (LPVOID*)(&set_recoil_o));
    MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + 0x5746D0));
}



BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Main, 0, 0, 0);
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

