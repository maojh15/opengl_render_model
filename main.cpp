/**
 * Created by MaoJiahao at 2024/09/21: 17:00
 */

// Dear ImGui: standalone example application for SDL2 + OpenGL
// (SDL is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan/Metal graphics context creation, etc.)
// If you are new to Dear ImGui, read documentation from the docs/ folder + read the top of imgui.cpp.
// Read online: https://github.com/ocornut/imgui/tree/master/docs

#include "OpenGlTools/CameraFlyLike.h"
#include "OpenGlTools/Model.h"
#include "OpenGlTools/ShaderProgram.h"
#include "OpenGlTools/TransformMatrix.h"

#include "glad/glad.h"

#include <chrono>
#include <iostream>

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#include <SDL.h>

#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL_opengles2.h>
#else

#include <SDL_opengl.h>

#endif

// This example can also compile and run with Emscripten! See 'Makefile.emscripten' for details.
#ifdef __EMSCRIPTEN__
#include "../libs/emscripten/emscripten_mainloop_stub.h"
#endif

const int fps = 60;

// Main code
int main(int, char **) {
    auto time1 = std::chrono::steady_clock::now();
    // Setup SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0) {
        printf("Error: %s\n", SDL_GetError());
        return -1;
    }

    // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100
    const char* glsl_version = "#version 100";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#elif defined(__APPLE__)
    // GL 3.2 Core + GLSL 150
    const char *glsl_version = "#version 150";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG); // Always required on Mac
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#endif

    // From 2.0.18: Enable native IME.
#ifdef SDL_HINT_IME_SHOW_UI
    SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");
#endif

    // Create window with graphics context
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

//    int window_width = bg_img_ptr->width;
//    int window_height = bg_img_ptr->height;
    int window_width = 1000;
    int window_height = 600;


    SDL_WindowFlags window_flags = (SDL_WindowFlags) (SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE |
                                                      SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_Window *window = SDL_CreateWindow("Hello Triangle", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                          window_width, window_height, window_flags);
    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, gl_context);
    SDL_GL_SetSwapInterval(1); // Enable vsync

    // initial GLAD
    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        std::cout << "Failed to initialize GLAD!" << std::endl;
        return -1;
    }

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void) io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    // ImGui::StyleColorsLight();
    
    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    // - Our Emscripten build process allows embedding fonts to be accessible at runtime from the "fonts/" folder. See Makefile.emscripten for details.
    ImFont *default_font = io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != NULL);
    // std::string root_dir = SDL_GetBasePath();
    // ImFont *font = io.Fonts->AddFontFromFileTTF((root_dir + "./Resources/Noteworthy.ttc").c_str(), 35.0f, nullptr);
//                                                io.Fonts->GetGlyphRangesChineseFull());
    // if (font == nullptr) {
    //     printf("cannot open font file: Noteworthy.ttc\n");
    // }
//    if (font == nullptr) {
//        printf("cannot open font file: Kaiti.ttc\n");
//    }

    // Our state
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 0.5f);
    // ImVec4 clear_color = ImVec4(0.0f, 0.0f, 0.0f, 1.00f);

    glEnable(GL_DEPTH_TEST);


    // set camera
    float camera_initial_pos[3] = {0, 0, 5}; 
    MyOpenGlTools::CameraFlyLike camera(camera_initial_pos, 0, 5);

    // creat shader program
    MyOpenGlTools::ShaderProgram shader_program("../shader_program/shader.vs", "../shader_program/shader.fs");

    float fov = M_PI / 4.0;
    MyOpenGlTools::Model model("model/小船.obj");

    // Main loop
    bool done = false;
#ifdef __EMSCRIPTEN__
    // For an Emscripten build we are disabling file-system access, so let's not attempt to do a fopen() of the imgui.ini file.
    // You may manually call LoadIniSettingsFromMemory() to load settings from your own storage.
    io.IniFilename = NULL;
    EMSCRIPTEN_MAINLOOP_BEGIN
#else

    std::cout << "Start main loop" << std::endl;
    Uint64 ms_per_frame = 1000 / fps;
    auto time2 = std::chrono::steady_clock::now();
    time1 = std::chrono::steady_clock::now();

    Uint64 camera_delta_ticks = 0;
    bool camera_enable_move = true;
    while (!done)
#endif
    {
        time2 = std::chrono::steady_clock::now();
        double delta_time = std::chrono::duration<double, std::milli>(time2 - time1).count();
        time1 = time2;
        Uint64 tick1 = SDL_GetTicks64();
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        SDL_Event event;
        auto io = ImGui::GetIO();
        io.WantCaptureKeyboard = false;
        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
                done = true;
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE &&
                event.window.windowID == SDL_GetWindowID(window))
                done = true;
            if (event.type == SDL_KEYDOWN && (event.key.keysym.sym == SDLK_LCTRL || event.key.keysym.sym == SDLK_RCTRL)) {
                camera_enable_move = !camera_enable_move;
            }
            camera.HandleSDLEvent(event, camera_enable_move);                
        }
        static Uint64 last_tick = tick1;
        Uint64 current_tick = SDL_GetTicks64();
        camera.UpdateCameraPosition(current_tick - last_tick);
        last_tick = current_tick;

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        // main window
        {

            ImGui::Begin("Hello, world!", nullptr,
                         ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground |
                         ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse |
                         ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar |
                         ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoBringToFrontOnFocus);
            ImGui::SetWindowSize(io.DisplaySize, ImGuiCond_Always);
            ImGui::SetWindowPos(ImVec2(0, 0), ImGuiCond_Always);


            // show FPS
            ImGui::SetCursorPos(ImVec2(10, 10));
            ImGui::TextColored(ImVec4(0.98, 0.98, 0.98, 1), "%f pfs", 1000 / delta_time);

            ImGui::End();
        }

        // Debug window.
        {
            ImGui::Text("Press ");
            ImGui::SameLine(); ImGui::TextColored(ImVec4(0.973,0.238,0.836,1.0), "'ctrl'");
            ImGui::SameLine(); ImGui::Text(" to enabel/disable mouse movement.");
            // ImGui::Checkbox("wire draw", &cube.wire_draw);
        }


        // Rendering
        ImGui::Render();
        // glViewport(0, 0, (int) io.DisplaySize.x, (int) io.DisplaySize.y);

        // custom opengl drawing
        int w, h;
        SDL_GL_GetDrawableSize(window, &w, &h);
        glViewport(0, 0, w, h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w,
                     clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        shader_program.SetUniformMat4("camera_trans", camera.GetCameraTransformMat().data());
        auto perp_trans_mat = MyOpenGlTools::TransformMatrix::PerspectiveFovMat(fov,
            float(w)/h, 0.1f, 100.0f);
        shader_program.SetUniformMat4("perp_trans", perp_trans_mat.data());
        model.Draw(shader_program);

        glViewport(0, 0, (int) io.DisplaySize.x, (int) io.DisplaySize.y);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        SDL_GL_SwapWindow(window);

        Uint64 tick2 = SDL_GetTicks64();
        if (tick2 - tick1 < ms_per_frame) {
            SDL_Delay(ms_per_frame - (tick2 - tick1));
        }

    }
#ifdef __EMSCRIPTEN__
    EMSCRIPTEN_MAINLOOP_END;
#endif

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
