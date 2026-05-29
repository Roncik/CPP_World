#include "pch.h"
#include "ImGUIManager.h"
#include "ImGUI/custom.h"
#include "ImGUI/icons.h"
#include "FileHelpers.h"
#include "OrganismFactoryRegistry.h"

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
        case WM_CLOSE:
            exit(0);
            return 0;
        case WM_DESTROY:
            ::PostQuitMessage(0);
            return 0;
        }
        return ::DefWindowProcW(hWnd, msg, wParam, lParam);
    }
}



// Run the main window
int ImGUIManager::RunUI()
{
    // Make process DPI aware and obtain main monitor scale
    ImGui_ImplWin32_EnableDpiAwareness();
    float main_scale = ImGui_ImplWin32_GetDpiScaleForMonitor(::MonitorFromPoint(POINT{ 0, 0 }, MONITOR_DEFAULTTOPRIMARY));

    // Create application window
    WNDCLASSEXW wc = { sizeof(wc), CS_CLASSDC, ImGUIManager::WndProc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, L"World simulator", nullptr };
    ::RegisterClassExW(&wc);
    HWND hwnd = ::CreateWindowW(wc.lpszClassName, L"World simulator", WS_OVERLAPPEDWINDOW, 100, 10, (int)(1200 * main_scale), (int)(1000 * main_scale), nullptr, nullptr, wc.hInstance, nullptr);

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
    ImGUIManager::textures::loadTextures();


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

        static bool mWinOpen{ false };
        if (mWinOpen) 
            ImGui::SetNextWindowSize({ (1183 * main_scale), (960 * main_scale) });
        ImGui::SetNextWindowPos({ 0,0 }, ImGuiCond_Once);
        ImGui::SetNextWindowBgAlpha(1.0f);
		if (ImGui::Begin("mainWindow", &mWinOpen,
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoSavedSettings |
			ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoScrollbar |
			ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoNav
            ))
		{
			
            // Options bar
            ImGui::SetCursorPos(ImVec2(10, 10));
			if (ImGui::BeginChild(1, ImVec2(70, (310 * main_scale))), ImGuiWindowFlags_AlwaysAutoResize)
			{
                using tex = ImGUIManager::textures;
                constexpr int y_base{ 10 };
                constexpr int y_step{ 60 };

                //Continue
                {
                    ImGui::SetCursorPos(ImVec2(10, y_base));
                    style.Colors[ImGuiCol_Button] = ImColor(0, 0, 0);
                    if (ImGui::ImageButton("1", (ImTextureRef)tex::Next, ImVec2(43, 43)))
                    {
                        world.makeTurn();
                    }
                    if (ImGui::IsItemHovered())
                    {
                        ImGui::SetTooltip("Continue");
                    }
                }

                //Clear world
                {
                    ImGui::SetCursorPos(ImVec2(10, y_base + y_step));
                    style.Colors[ImGuiCol_Button] = ImColor(0, 0, 0);
                    if (ImGui::ImageButton("4", (ImTextureRef)tex::Clear, ImVec2(43, 43)))
                    {
                        world.clear();
                    }
                    if (ImGui::IsItemHovered())
                    {
                        ImGui::SetTooltip("Clear world");
                    }
                }

                //Randomize world
                {
                    static int worldX, worldY;
                    static int numOfOrganisms; //must be int for InputInt
                    static std::vector<char> possibleOrganisms;

                    ImGui::SetCursorPos(ImVec2(10, y_base + y_step * 2));
                    style.Colors[ImGuiCol_Button] = ImColor(0, 0, 0);
                    if (ImGui::ImageButton("5", (ImTextureRef)tex::Randomize, ImVec2(43, 43)))
                    {
                        ImGui::OpenPopup("input world parameters");
                    }
                    if (ImGui::IsItemHovered())
                    {
                        ImGui::SetTooltip("Randomize world");
                    }

                    if (ImGui::BeginPopupModal("input world parameters", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
                    {
                        ImGui::InputInt("World width", &worldX);
                        ImGui::InputInt("World height", &worldY);
                        ImGui::InputInt("Number of created organisms", &numOfOrganisms);

                        static std::string signs(6, 0);
                        ImGui::InputText("Possible organisms signs('G', 'S', 'D', 'W', 'T')", signs.data(), signs.size());



                        if (ImGui::Button("OK"))
                        {
                            possibleOrganisms.clear();
                            for (size_t i = 0; i < strlen(signs.data()); ++i)
                            {
                                char& sign = signs[i];
                                possibleOrganisms.push_back(sign);
                            }

                            world.Randomize(worldX, worldY, numOfOrganisms, possibleOrganisms);

                            ImGui::CloseCurrentPopup();
                        }

                        ImGui::SameLine();

                        if (ImGui::Button("Cancel"))
                        {
                            ImGui::CloseCurrentPopup();
                        }

                        ImGui::EndPopup();
                    }
                }

                //Export world
                {
                    ImGui::SetCursorPos(ImVec2(10, y_base + y_step * 3));
                    style.Colors[ImGuiCol_Button] = ImColor(0, 0, 0);
                    if (ImGui::ImageButton("6", (ImTextureRef)tex::Save, ImVec2(43, 43)))
                    {
                        std::string path = FileHelpers::SaveFileDialog();
                        world.writeWorld(path);
                    }
                    if (ImGui::IsItemHovered())
                    {
                        ImGui::SetTooltip("Export world");
                    }
                }

                //Import world
                {
                    ImGui::SetCursorPos(ImVec2(10, y_base + y_step * 4));
                    style.Colors[ImGuiCol_Button] = ImColor(0, 0, 0);
                    if (ImGui::ImageButton("7", (ImTextureRef)tex::Load, ImVec2(43, 43)))
                    {
                        std::string path = FileHelpers::OpenFileDialog();
                        if (path.size())
                            world.readWorld(path);
                    }
                    if (ImGui::IsItemHovered())
                    {
                        ImGui::SetTooltip("Import world");
                    }
                }

				ImGui::EndChild();
			}


            // World window
            //style.Colors[ImGuiCol_ChildBg] = 
            ImGui::SetCursorPos(ImVec2(90, 10));
            if (ImGui::BeginChild(2, ImVec2(1084, (940 * main_scale))), ImGuiWindowFlags_AlwaysAutoResize)
            {
                ImDrawList* drawList = ImGui::GetWindowDrawList();
                ImGui::SetCursorPos(ImVec2(10, 10));
                ImVec2 origin = ImGui::GetCursorScreenPos();
                ImGui::Dummy(ImVec2(0, 0));

                constexpr float cellSize = 80.0f;
                int width = world.getWorldX();
                int height = world.getWorldY();

                for (int x = 0; x <= width; ++x)
                {
                    float px = origin.x + x * cellSize;
                    drawList->AddLine(ImVec2(px, origin.y), ImVec2(px, origin.y + height * cellSize), IM_COL32(200, 200, 200, 50));
                }

                for (int y = 0; y <= height; ++y)
                {
                    float py = origin.y + y * cellSize;
                    drawList->AddLine(ImVec2(origin.x, py), ImVec2(origin.x + width * cellSize, py), IM_COL32(200, 200, 200, 50));
                }

                //const auto& organisms = world.getOrganisms();
                //for (size_t i = 0; i < organisms.size(); ++i)
                //{
                //    const auto& org = organisms[i];
                //    
                //    Position p = org->getPosition();

                //    float x = origin.x + p.getX() * cellSize;
                //    float y = origin.y + p.getY() * cellSize;

                //   // drawList->AddRectFilled(ImVec2(x, y), ImVec2(x + cellSize, y + cellSize), IM_COL32(0, 255, 0, 255));

                //    //tutaj jest duzo magicznych liczb bo ImageButton dodaje jakies niejasne marginesy
                //    ImGui::SetCursorScreenPos(ImVec2(x+0.5, y+0.5));
                //    std::string id = "organism:" + std::to_string(i);
                //    std::string popupid = "organism: " + std::to_string(i) + " popup";
                //    if (ImGui::ImageButton(id.c_str(), (ImTextureRef)org->getTexture(), ImVec2(cellSize-8.3, cellSize-6.25)))
                //    {
                //        ImGui::OpenPopup(popupid.c_str());
                //    }

                //    if (ImGui::BeginPopupModal(popupid.c_str(), nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize))
                //    {
                //        if (ImGui::Button("Remove"))
                //        {
                //            world.removeOrganism(i);
                //            ImGui::CloseCurrentPopup();
                //        }

                //        ImGui::SameLine();

                //        std::string historypopupid = "organism " + std::to_string(i) + " history";
                //        if (ImGui::Button("Show history"))
                //        {
                //            ImGui::OpenPopup(historypopupid.c_str());
                //        }

                //        if (ImGui::BeginPopupModal(historypopupid.c_str(), nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize))
                //        {
                //            ImGui::Text(org->printHistory().c_str());
                //            if (ImGui::Button("Close"))
                //                ImGui::CloseCurrentPopup();

                //            ImGui::EndPopup();
                //        }

                //        if (ImGui::Button("Cancel"))
                //        {
                //            ImGui::CloseCurrentPopup();
                //        }

                //        ImGui::EndPopup();
                //    }
                //}

                for (int pos_x = 0; pos_x < world.getWorldX(); ++pos_x)
                    for (int pos_y = 0; pos_y < world.getWorldY(); ++pos_y)
                    {
                        Position pos = Position(pos_x, pos_y);
                        
                        float x = origin.x + pos.getX() * cellSize;
                        float y = origin.y + pos.getY() * cellSize;

                        //tutaj jest duzo magicznych liczb bo ImageButton dodaje jakies niejasne marginesy
                        ImGui::SetCursorScreenPos(ImVec2(x + 0.9, y + 0.9));

                        std::string id = '(' + std::to_string(pos.getX()) + ", " + std::to_string(pos.getY()) + ')';

                        size_t i{};
                        if (world.getOrganismFromPosition(pos.getX(), pos.getY(), &i))
                        {
                            auto& org = world.getOrganisms()[i];
                            std::string popupid = id + " organism";
                            if (ImGui::ImageButton(id.c_str(), (ImTextureRef)org->getTexture(), ImVec2(cellSize - 8.7, cellSize - 6.65)))
                            {
                                ImGui::OpenPopup(popupid.c_str());
                            }

                            if (ImGui::BeginPopupModal(popupid.c_str(), nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize))
                            {
                                if (ImGui::Button("Remove"))
                                {
                                    world.removeOrganism(i);
                                    ImGui::CloseCurrentPopup();
                                }

                                ImGui::SameLine();

                                std::string historypopupid = "organism " + std::to_string(i) + " history";
                                if (ImGui::Button("Show history"))
                                {
                                    ImGui::OpenPopup(historypopupid.c_str());
                                }

                                if (ImGui::BeginPopupModal(historypopupid.c_str(), nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize))
                                {
                                    ImGui::Text(org->printHistory().c_str());
                                    if (ImGui::Button("Close"))
                                        ImGui::CloseCurrentPopup();

                                    ImGui::EndPopup();
                                }

                                if (ImGui::Button("Cancel"))
                                {
                                    ImGui::CloseCurrentPopup();
                                }

                                ImGui::EndPopup();
                            }
                        }
                        else
                        {
                            //free positon
                            std::string popupid = id + ' ';
                            if (ImGui::ImageButton(id.c_str(), (ImTextureRef)textures::Empty, ImVec2(cellSize - 8.7, cellSize - 6.65)))
                            {
                                ImGui::OpenPopup(popupid.c_str());
                            }

                            std::string addOrgPopupId = "Adding organism at: " + id;
                            if (ImGui::BeginPopupModal(popupid.c_str(), nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize))
                            {
                                if (ImGui::Button("Add Organism"))
                                {
                                    ImGui::OpenPopup(addOrgPopupId.c_str());
                                }

                                static std::string sign(2, 0);
                                if (ImGui::BeginPopupModal(addOrgPopupId.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize))
                                {
                                    ImGui::InputText("Select organism sign(one of 'G', 'S', 'D', 'W', 'T')", sign.data(), sign.size());
                                    if (ImGui::Button("OK"))
                                    {
                                        auto org = OrganismFactoryRegistry::getFactory(sign[0])->create();
                                        org->setPosition(pos);
                                        org->setSelfRecord(std::pair<int, int>(world.getTurn(), -1));
                                        
                                        org->clearHistory();
                                        std::vector<std::shared_ptr<std::pair<int, int>>> newHistory;
                                        newHistory.push_back(org->getSelfRecord());
                                        org->setHistory(newHistory);
                                        
                                        /*auto history = org->getHistory();
                                        history[0]->first = world.getTurn();
                                        history[0]->second = -1;*/


                                        world.addOrganism(org);

                                        ImGui::CloseCurrentPopup();
                                    }

                                    ImGui::EndPopup();
                                }

                                if (ImGui::Button("Cancel"))
                                {
                                    ImGui::CloseCurrentPopup();
                                }
                                ImGui::EndPopup();
                            }
                        }
                    }
                ImGui::EndChild();
            }
			ImGui::End();
		}

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

void ImGUIManager::textures::loadTextures()
{
    if (!ImGUIManager::g_pd3dDevice)
        return;

    if (D3DXCreateTextureFromFileW(ImGUIManager::g_pd3dDevice, L"imgui\\extra_icons\\file_open.png", &Load) ||
        D3DXCreateTextureFromFileW(ImGUIManager::g_pd3dDevice, L"imgui\\extra_icons\\file_export.png", &Save) ||
        D3DXCreateTextureFromFileW(ImGUIManager::g_pd3dDevice, L"imgui\\extra_icons\\continue.png", &Next) ||
        D3DXCreateTextureFromFileW(ImGUIManager::g_pd3dDevice, L"imgui\\extra_icons\\add_organism.png", &Add) ||
        D3DXCreateTextureFromFileW(ImGUIManager::g_pd3dDevice, L"imgui\\extra_icons\\remove_organism.png", &Remove) ||
        D3DXCreateTextureFromFileW(ImGUIManager::g_pd3dDevice, L"imgui\\extra_icons\\randomize_world.png", &Randomize) ||
        D3DXCreateTextureFromFileW(ImGUIManager::g_pd3dDevice, L"imgui\\extra_icons\\delete_world.png", &Clear) ||
        D3DXCreateTextureFromFileW(ImGUIManager::g_pd3dDevice, L"imgui\\extra_icons\\grass.png", &Grass) ||
        D3DXCreateTextureFromFileW(ImGUIManager::g_pd3dDevice, L"imgui\\extra_icons\\dandelion.png", &Dandelion) ||
        D3DXCreateTextureFromFileW(ImGUIManager::g_pd3dDevice, L"imgui\\extra_icons\\wolf.png", &Wolf) ||
        D3DXCreateTextureFromFileW(ImGUIManager::g_pd3dDevice, L"imgui\\extra_icons\\sheep_fix.png", &Sheep) ||
        D3DXCreateTextureFromFileW(ImGUIManager::g_pd3dDevice, L"imgui\\extra_icons\\toadstool.png", &Toadstool) ||
        D3DXCreateTextureFromFileW(ImGUIManager::g_pd3dDevice, L"imgui\\extra_icons\\square.png", &Free) ||
        D3DXCreateTextureFromFileW(ImGUIManager::g_pd3dDevice, L"imgui\\extra_icons\\empty.png", &Empty))
    {
        throw std::runtime_error("failed to load all textures");
    }
}