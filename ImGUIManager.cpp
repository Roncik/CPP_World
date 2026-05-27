#include "pch.h"
#include "ImGUIManager.h"
#include "ImGUI/custom.h"
#include "ImGUI/icons.h"

// 
 // Used when user tries to close the program (for handling proper threads joining)
 bool isTryingToExit = false;

// -----------------------------------------------------------------------
// ------------------------ D3DX9 + WIN32 --------------------------------
// -----------------------------------------------------------------------

//static member definitions
LPDIRECT3D9              ImGUIManager::g_pD3D = nullptr;
LPDIRECT3DDEVICE9        ImGUIManager::g_pd3dDevice = nullptr;
bool                     ImGUIManager::g_DeviceLost = false;
UINT                     ImGUIManager::g_ResizeWidth = 0, ImGUIManager::g_ResizeHeight = 0;
D3DPRESENT_PARAMETERS    ImGUIManager::g_d3dpp = {};

bool ImGUIManager::CreateDeviceD3D(HWND hWnd)
{
    if ((ImGUIManager::g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == nullptr)
        return false;

    // Create the D3DDevice
    ZeroMemory(&ImGUIManager::g_d3dpp, sizeof(ImGUIManager::g_d3dpp));
    ImGUIManager::g_d3dpp.Windowed = TRUE;
    ImGUIManager::g_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    ImGUIManager::g_d3dpp.BackBufferFormat = D3DFMT_UNKNOWN; // Need to use an explicit format with alpha if needing per-pixel alpha composition.
    ImGUIManager::g_d3dpp.EnableAutoDepthStencil = TRUE;
    ImGUIManager::g_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
    ImGUIManager::g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;           // Present with vsync
    //ImGUIManager::g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;   // Present without vsync, maximum unthrottled framerate
    if (ImGUIManager::g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &ImGUIManager::g_d3dpp, &ImGUIManager::g_pd3dDevice) < 0)
        return false;

    return true;
}

void ImGUIManager::CleanupDeviceD3D()
{
    if (ImGUIManager::g_pd3dDevice)
    { 
        ImGUIManager::g_pd3dDevice->Release();
        ImGUIManager::g_pd3dDevice = nullptr;
    }
    if (ImGUIManager::g_pD3D)
    { 
        ImGUIManager::g_pD3D->Release();
        ImGUIManager::g_pD3D = nullptr;
    }
}

void ImGUIManager::ResetDevice()
{
    ImGui_ImplDX9_InvalidateDeviceObjects();
    HRESULT hr = ImGUIManager::g_pd3dDevice->Reset(&ImGUIManager::g_d3dpp);
    if (hr == D3DERR_INVALIDCALL)
        IM_ASSERT(0);
    ImGui_ImplDX9_CreateDeviceObjects();
}

LRESULT __stdcall ImGUIManager::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;
    else
    {
        switch (msg)
        {
        case WM_SIZE:
            if (wParam == SIZE_MINIMIZED)
                return 0;
            g_ResizeWidth = (UINT)LOWORD(lParam); // Queue resize
            g_ResizeHeight = (UINT)HIWORD(lParam);
            return 0;
        case WM_SYSCOMMAND:
            if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
                return 0;
            break;
        case WM_CLOSE:  // For joining threads
            isTryingToExit = true;
            return 0;
        case WM_DESTROY:
            ::PostQuitMessage(0);
            return 0;
        }
        return ::DefWindowProcW(hWnd, msg, wParam, lParam);
    }
}

// -----------------------------------------------------------------------
// -------------------------- GUI CORE -----------------------------------
// -----------------------------------------------------------------------






// Run the main window
int ImGUIManager::RunUI()
{
    // Make process DPI aware and obtain main monitor scale
    ImGui_ImplWin32_EnableDpiAwareness();
    float main_scale = ImGui_ImplWin32_GetDpiScaleForMonitor(::MonitorFromPoint(POINT{ 0, 0 }, MONITOR_DEFAULTTOPRIMARY));

    // Create application window
    WNDCLASSEXW wc = { sizeof(wc), CS_CLASSDC, ImGUIManager::WndProc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, L"World simulator", nullptr };
    ::RegisterClassExW(&wc);
    HWND hwnd = ::CreateWindowW(wc.lpszClassName, L"World simulator", WS_OVERLAPPEDWINDOW, 100, 100, (int)(800 * main_scale), (int)(600 * main_scale), nullptr, nullptr, wc.hInstance, nullptr);

    // Initialize Direct3D
    if (!CreateDeviceD3D(hwnd))
    {
        CleanupDeviceD3D();
        ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
        return 1;
    }

    // Show the window
    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;                   
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls    
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable window/panel docking
    io.ConfigFlags |= ImGuiWindowFlags_NoMove;                // Main panel non-movable    

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup scaling
    ImGuiStyle& style = ImGui::GetStyle();
    style.ScaleAllSizes(main_scale);        // Bake a fixed style scale. (until we have a solution for dynamic style scaling, changing this requires resetting Style + calling this again)
    style.FontScaleDpi = main_scale;        // Set initial font scale. (using io.ConfigDpiScaleFonts=true makes this unnecessary. We leave both here for documentation purpose)

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX9_Init(ImGUIManager::g_pd3dDevice);

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return a nullptr. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
    // - Read 'docs/FONTS.md' for more instructions and details. If you like the default font but want it to scale better, consider using the 'ProggyVector' from the same author!
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //style.FontSizeBase = 20.0f;
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf");
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf");
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf");
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf");
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf");
    //IM_ASSERT(font != nullptr);

    // Load Icons
    ImFontConfig CustomFont;
    CustomFont.FontDataOwnedByAtlas = false;
    static const ImWchar icons_ranges[] = { 0xe000, 0xf8ff, 0 };
    ImFontConfig icons_config;
    icons_config.MergeMode = true;
    icons_config.PixelSnapH = true;
    icons_config.OversampleH = 3;
    icons_config.OversampleV = 3;
    io.IniFilename = NULL;
    io.WantCaptureMouse = true;
    io.Fonts->AddFontFromMemoryTTF(const_cast<std::uint8_t*>(Custom), sizeof(Custom), 15, &CustomFont);
    io.Fonts->AddFontFromMemoryCompressedTTF(font_awesome_data, font_awesome_size, 32.5f, &icons_config, icons_ranges);

    // Load textures for extra Icons
    IDirect3DTexture9* textureGrass{ nullptr };
    D3DXCreateTextureFromFileW(ImGUIManager::g_pd3dDevice, L"imgui\\extra_icons\\grass.png", &textureGrass);


    // Used when rendering
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);         

    // Used when user wants to skip waiting for threads to join when closing the program
    bool isForcingQuit = false;

    // Main loop
    bool done = false;
    while (!done)
    {
        // Poll and handle messages (inputs, window resize, etc.)
        // See the WndProc() function below for our to dispatch events to the Win32 backend.
        MSG msg;
        while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)        
                done = true;            
        }

        if (done)
            break;

        // Handle lost D3D9 device
        if (ImGUIManager::g_DeviceLost)
        {
            HRESULT hr = ImGUIManager::g_pd3dDevice->TestCooperativeLevel();
            if (hr == D3DERR_DEVICELOST)
            {
                ::Sleep(10);
                continue;
            }
            if (hr == D3DERR_DEVICENOTRESET)
                ResetDevice();
            ImGUIManager::g_DeviceLost = false;
        }

        // Handle window resize (we don't resize directly in the WM_SIZE handler)
        if (ImGUIManager::g_ResizeWidth != 0 && ImGUIManager::g_ResizeHeight != 0)
        {
            ImGUIManager::g_d3dpp.BackBufferWidth = ImGUIManager::g_ResizeWidth;
            ImGUIManager::g_d3dpp.BackBufferHeight = ImGUIManager::g_ResizeHeight;
            ImGUIManager::g_ResizeWidth = ImGUIManager::g_ResizeHeight = 0;
            ResetDevice();
        }

        // Start the Dear ImGui frame
        ImGui_ImplDX9_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
        ImGui::DockSpaceOverViewport();       


		ImGui::SetNextWindowPos({ 0,0 }, ImGuiCond_Once);
		ImGui::SetNextWindowSize({ (800 * main_scale), (600 * main_scale) });
		ImGui::SetNextWindowBgAlpha(1.0f);

		ImGuiStyle& style = ImGui::GetStyle();
		{
			//Styles
			style.ScrollbarRounding = 5;
			style.FrameRounding = 2;

			style.Colors[ImGuiCol_WindowBg] = ImColor(18, 18, 18);
			style.Colors[ImGuiCol_ChildBg] = ImColor(0, 0, 0);

			style.Colors[ImGuiCol_Text] = ImColor(255, 255, 255);
			style.Colors[ImGuiCol_CheckMark] = ImColor(212, 225, 245);

			style.Colors[ImGuiCol_FrameBg] = ImColor(20, 20, 20);
			style.Colors[ImGuiCol_FrameBgHovered] = ImColor(23, 23, 23);
			style.Colors[ImGuiCol_FrameBgActive] = ImColor(23, 23, 23);

			style.Colors[ImGuiCol_Header] = ImColor(23, 23, 23);
			style.Colors[ImGuiCol_HeaderActive] = ImColor(212, 225, 245);
			style.Colors[ImGuiCol_HeaderHovered] = ImColor(128, 128, 128);

			style.Colors[ImGuiCol_SliderGrabActive] = ImColor(212, 225, 245);
			style.Colors[ImGuiCol_SliderGrab] = ImColor(212, 225, 245);

			style.Colors[ImGuiCol_Button] = ImColor(0, 0, 0);
			style.Colors[ImGuiCol_ButtonActive] = ImColor(0, 0, 0);
			style.Colors[ImGuiCol_ButtonHovered] = ImColor(100, 100, 100);


			style.Colors[ImGuiCol_Border] = ImColor(0, 0, 0, 255);
		}

		if (ImGui::Begin("mainWindow", (bool*)0,
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoSavedSettings |
			ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoScrollbar |
			ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoNav))
		{
			ImGui::SetCursorPos(ImVec2(10, 10));
			if (ImGui::BeginChild(1, ImVec2(70, (600 * main_scale))))
			{
				ImGui::SetCursorPos(ImVec2(10, 10));

				style.Colors[ImGuiCol_Button] = ImColor(0, 0, 0);
				if (ImGui::Button("\n" "" ICON_FA_ARROW_RIGHT, ImVec2(50, 50)))
				{

				}
                if (ImGui::IsItemHovered())
                {
                    ImGui::SetTooltip("Continue");
                }

				style.Colors[ImGuiCol_Button] = ImColor(0, 0, 0);
				ImGui::SetCursorPos(ImVec2(10, 70));
                ImGui::ImageButton("grassicon", (ImTextureRef)textureGrass, ImVec2(50, 50));


				/*if (ImGui::Button("\n" "" ICON_FA_SHEEP, ImVec2(50, 50)))
				{

				}*/

				style.Colors[ImGuiCol_Button] = ImColor(0, 0, 0);
				ImGui::SetCursorPos(ImVec2(10, 130));
				if (ImGui::Button("\n" "" ICON_FA_KEYBOARD, ImVec2(50, 50)))
				{

				}

				style.Colors[ImGuiCol_Button] = ImColor(0, 0, 0);
				ImGui::SetCursorPos(ImVec2(10, 190));
				if (ImGui::Button("\n" "" ICON_FA_PAINT_BRUSH, ImVec2(50, 50)))
				{

				}

				/*style.Colors[ImGuiCol_Button] = ImColor(0, 0, 0);
				style.Colors[ImGuiCol_Text] = ImColor(0, 204, 255);
				style.Colors[ImGuiCol_ButtonActive] = ImColor(0, 102, 204);
				style.Colors[ImGuiCol_ButtonHovered] = ImColor(0, 102, 204);*/
				/*ImGui::SetCursorPos(ImVec2(10, 220));
				if (ImGui::Button("\n" "" ICON_FA_SHOPPING_CART, ImVec2(50, 50)))
				{
					val.tab = 2;
				}
				style.Colors[ImGuiCol_Text] = ImColor(255, 255, 255);
				style.Colors[ImGuiCol_ButtonActive] = ImColor(0, 0, 0);
				style.Colors[ImGuiCol_ButtonHovered] = ImColor(100, 100, 100);*/

				ImGui::EndChild();
			}
			//ImGui::SetCursorPos(ImVec2(90, 10));
			//if (ImGui::BeginChild(2, ImVec2(320, 380)))
			//{
			//	//ImGui::SetCursorPos(ImVec2(110, 5));
			//	//ImGui::Text(StringHelper::get_string(157).c_str());
			//	ImGui::EndChild();
			//}
			ImGui::End();
		}


    
        
        // Create the always-visible main menu bar over the main viewport
        /*if (ImGui::BeginMainMenuBar())
        {            
            if (ImGui::BeginMenu("Panels"))
            {
                if (ImGui::MenuItem("Active protection config", NULL, mwData.showActiveProtectionConfigPanel))                
                    mwData.showActiveProtectionConfigPanel = !mwData.showActiveProtectionConfigPanel;                                
                if (ImGui::MenuItem("Active protection console output", NULL, mwData.showActiveProtectionOutputPanel))
                    mwData.showActiveProtectionOutputPanel = !mwData.showActiveProtectionOutputPanel;                                
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }*/           





        // Rendering
        ImGui::EndFrame();
        ImGUIManager::g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
        ImGUIManager::g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
        ImGUIManager::g_pd3dDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
        D3DCOLOR clear_col_dx = D3DCOLOR_RGBA((int)(clear_color.x * clear_color.w * 255.0f), (int)(clear_color.y * clear_color.w * 255.0f), (int)(clear_color.z * clear_color.w * 255.0f), (int)(clear_color.w * 255.0f));
        ImGUIManager::g_pd3dDevice->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, clear_col_dx, 1.0f, 0);
        if (ImGUIManager::g_pd3dDevice->BeginScene() >= 0)
        {
            ImGui::Render();
            ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
            ImGUIManager::g_pd3dDevice->EndScene();
        }
        HRESULT result = ImGUIManager::g_pd3dDevice->Present(nullptr, nullptr, nullptr, nullptr);
        if (result == D3DERR_DEVICELOST)
            ImGUIManager::g_DeviceLost = true;
    }


    // Cleanup
    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    ::DestroyWindow(hwnd);
    ::UnregisterClassW(wc.lpszClassName, wc.hInstance);

    return 0;
}