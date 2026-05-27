#pragma once
#include "World.h"

class World;

class ImGUIManager
{
private:
    static LPDIRECT3D9              g_pD3D;
    static LPDIRECT3DDEVICE9        g_pd3dDevice;
    static bool                     g_DeviceLost;
    static UINT                     g_ResizeWidth, g_ResizeHeight;
    static D3DPRESENT_PARAMETERS    g_d3dpp;

    inline static World world;
public:
    ~ImGUIManager()
    {
        CleanupDeviceD3D();
    }
    static bool CreateDeviceD3D(HWND hWnd);
    static void CleanupDeviceD3D();
    static void ResetDevice();

    // Win32 message handler
   // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
   // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
   // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
   // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
    static LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam); //callback for processing messages sent to window

    // Initialize dx9+win32, then initialize and run main GUI thread
    static int RunUI();

    struct textures
    {
        inline static IDirect3DTexture9* Load{ nullptr };
        inline static IDirect3DTexture9* Save{ nullptr };
        inline static IDirect3DTexture9* Next{ nullptr };
        inline static IDirect3DTexture9* Add{ nullptr };
        inline static IDirect3DTexture9* Remove{ nullptr };
        inline static IDirect3DTexture9* Randomize{ nullptr };
        inline static IDirect3DTexture9* Clear{ nullptr };
        inline static IDirect3DTexture9* Grass{ nullptr };
        inline static IDirect3DTexture9* Dandelion{ nullptr };
        inline static IDirect3DTexture9* Wolf{ nullptr };
        inline static IDirect3DTexture9* Sheep{ nullptr };
        inline static IDirect3DTexture9* Toadstool{ nullptr };
        inline static IDirect3DTexture9* Free{ nullptr };

        static void loadTextures();
    };
};