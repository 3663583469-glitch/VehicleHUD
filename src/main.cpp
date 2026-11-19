#include <SDL.h>
#include <SDL_opengl.h>
#include "imgui.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_opengl3.h"
#include "Simulator.h"
#include <cstdio>

static const char* getDirection(float angle) {
    static const char* directions[] = {"N", "NE", "E", "SE", "S", "SW", "W", "NW"};
    const int index = static_cast<int>((angle + 22.5f) / 45.0f) % 8;
    return directions[index];
}

int main(int, char**) {
    SDL_SetMainReady();
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::fprintf(stderr, "SDL init failed: %s\n", SDL_GetError());
        return 1;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    SDL_Window* window = SDL_CreateWindow(
        "Vehicle HUD - Simulation", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        650, 430, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI
    );
    if (!window) {
        std::fprintf(stderr, "Window creation failed: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    SDL_SetWindowAlwaysOnTop(window, SDL_TRUE);

    SDL_GLContext glContext = SDL_GL_CreateContext(window);
    if (!glContext) {
        std::fprintf(stderr, "OpenGL context failed: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    SDL_GL_MakeCurrent(window, glContext);
    SDL_GL_SetSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 7.0f;
    style.FrameRounding = 4.0f;
    style.ItemSpacing = ImVec2(10.0f, 8.0f);

    ImGui_ImplSDL2_InitForOpenGL(window, glContext);
    ImGui_ImplOpenGL3_Init("#version 130");

    Simulator simulator;
    bool running = true;
    bool alwaysOnTop = true;
    Uint64 lastCounter = SDL_GetPerformanceCounter();

    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT) running = false;
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) running = false;
        }

        const Uint64 now = SDL_GetPerformanceCounter();
        float dt = static_cast<float>(now - lastCounter) / static_cast<float>(SDL_GetPerformanceFrequency());
        lastCounter = now;
        if (dt > 0.1f) dt = 0.1f;
        simulator.update(dt);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        ImGui::SetNextWindowSize(ImVec2(610, 380), ImGuiCond_FirstUseEver);
        ImGui::Begin("VEHICLE TACTICAL HUD");
        ImGui::TextDisabled("SIMULATION DATA  |  ESC TO EXIT");
        ImGui::Separator();
        ImGui::Checkbox("Keep window on top", &alwaysOnTop);
        SDL_SetWindowAlwaysOnTop(window, alwaysOnTop ? SDL_TRUE : SDL_FALSE);
        ImGui::Spacing();

        if (ImGui::BeginTable("vehicles", 4, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable)) {
            ImGui::TableSetupColumn("Vehicle", ImGuiTableColumnFlags_WidthStretch, 1.5f);
            ImGui::TableSetupColumn("Distance", ImGuiTableColumnFlags_WidthFixed, 100.0f);
            ImGui::TableSetupColumn("Bearing", ImGuiTableColumnFlags_WidthFixed, 120.0f);
            ImGui::TableSetupColumn("Heading", ImGuiTableColumnFlags_WidthFixed, 100.0f);
            ImGui::TableHeadersRow();
            for (const auto& v : simulator.getVehicles()) {
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0); ImGui::TextUnformatted(v.name.c_str());
                ImGui::TableSetColumnIndex(1); ImGui::Text("%.0f m", v.distance);
                ImGui::TableSetColumnIndex(2); ImGui::Text("%s  %.0f°", getDirection(v.bearing), v.bearing);
                ImGui::TableSetColumnIndex(3); ImGui::Text("%.0f°", v.heading);
            }
            ImGui::EndTable();
        }
        ImGui::Spacing();
        ImGui::TextDisabled("Coordinates and vehicle movement are generated locally for demonstration.");
        ImGui::End();

        ImGui::Render();
        int drawableW = 0, drawableH = 0;
        SDL_GL_GetDrawableSize(window, &drawableW, &drawableH);
        glViewport(0, 0, drawableW, drawableH);
        glClearColor(0.055f, 0.075f, 0.09f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
