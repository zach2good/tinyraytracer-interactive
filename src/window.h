#pragma once

#include <glad/glad.h>

#include <SDL.h>

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"

#include "world.h"
#include "texture.h"

class Window
{
public:
    Window(const char* title, int width, int height)
    : title(title), width(width), height(height), _done(false)
    {
        glsl_version = "#version 150";

        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            printf("SDL Error");
        }

#if __APPLE__
        // GL 3.2 Core + GLSL 150
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG); // Always required on Mac
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
#else
        // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#endif

        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
        SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

        SDL_DisplayMode current;
        SDL_GetCurrentDisplayMode(0, &current);

        window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);

        gl_context = SDL_GL_CreateContext(window);

        SDL_GL_SetSwapInterval(0);

        if (!gladLoadGL()) {
            printf("GLAD Error");
        }

        glEnable(GL_TEXTURE_2D);

        // Print Info
        printf("GL_VERSION: %s \n", glGetString(GL_VERSION));
        printf("GL_VENDOR: %s \n", glGetString(GL_VENDOR));
        printf("GL_SHADING_LANGUAGE_VERSION: %s \n", glGetString(GL_SHADING_LANGUAGE_VERSION));

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        ImGui::StyleColorsDark();

        ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
        ImGui_ImplOpenGL3_Init(glsl_version);

        ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    }

    ~Window()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplSDL2_Shutdown();
        ImGui::DestroyContext();

        SDL_GL_DeleteContext(gl_context);
        SDL_DestroyWindow(window);

        SDL_Quit();
    }

    bool done()
    {
        return _done;
    }

    void handleEvents()
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
                _done = true;
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
                _done = true;
        }
    }

    void clear()
    {
        glViewport(0, 0, width, height);
        glClearColor(0.2, 0.2, 0.2, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void render(World& world, Texture& texture)
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame(window);
        ImGui::NewFrame();

        ImGui::SetNextWindowPos(ImVec2(0, 0));
        {
            ImGui::Begin("Menu", nullptr, ImGuiWindowFlags_AlwaysAutoResize|ImGuiWindowFlags_NoSavedSettings);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate); ImGui::NewLine();

            ImGui::SliderInt("Internal Scale", &world.scale, 1, 20);
            ImGui::Text("Internal Resolution: %dx%d", 32*world.scale, 24*world.scale); ImGui::NewLine();

            ImGui::SliderFloat("FOV", &world.fov, -10, 10); ImGui::NewLine();

            ImGui::SliderInt3("Camera Position", world.camera, -10, 10); ImGui::NewLine();

            int counter = 0;
            for(auto& light : world.lights)
            {
                ImGui::PushID(counter++);
                ImGui::SliderFloat("Light X", &light.position.x, -30, 30);
                ImGui::SliderFloat("Light Y", &light.position.y, -30, 30);
                ImGui::SliderFloat("Light Z", &light.position.z, -30, 30);
                ImGui::PopID();
            }

            for(auto& sphere : world.spheres)
            {
                ImGui::PushID(counter++);
                ImGui::SliderFloat("Sphere X", &sphere.center.x, -30, 30);
                ImGui::SliderFloat("Sphere Y", &sphere.center.y, -30, 30);
                ImGui::SliderFloat("Sphere Z", &sphere.center.z, -30, 30);
                ImGui::SliderFloat("Sphere Radius", &sphere.radius, -10, 10);
                ImGui::PopID();
            }
            ImGui::End();
        }

        ImGui::SetNextWindowPos(ImVec2(360, 0));
        {
            ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_AlwaysAutoResize|ImGuiWindowFlags_NoSavedSettings);
            ImGui::Image((ImTextureID)texture.get_id(), ImVec2(800, 600));
            ImGui::End();
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void flip()
    {
        SDL_GL_SwapWindow(window);
    }

private:
    const char* title;
    int width;
    int height;
    bool _done;

    const char *glsl_version;
    SDL_Window *window;
    SDL_GLContext gl_context;
};
