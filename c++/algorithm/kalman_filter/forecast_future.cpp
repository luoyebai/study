#include "../../myhead_file/myfmt.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <cmath>
#include <iostream>
#include <vector>
#define IMGUI_IMPL_OPENGL_LOADER_GLAD

int main(int argc, char *argv[]) {

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    GLFWwindow *window;
    float color;
    float *objectColor = &color;
    float *lightColor = &color;
    ImGui_ImplGlfw_InitForOpenGL(window, true); // GLFWwindow
    ImGui_ImplOpenGL3_Init("#version 330");     // 自己的版本

    bool show_demo_window = false; // 是否显示样例
    bool p_open = true;
    // 渲染循环
    while (!glfwWindowShouldClose(window)) {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        // 创建一个面板
        //{
        ImGui::Begin("Panel", &p_open, ImGuiWindowFlags_MenuBar);
        ImGui::ColorEdit3("object color",
                          (float *)&objectColor); // 颜色编辑条
        ImGui::ColorEdit3(
            "lightColor",
            (float
                 *)&lightColor); // lightColor类型为ImVec4，后续将被赋值给着色器中的变量
        ImGui::End();
        //}
        // 准备绘制
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    // 释放ImGui资源
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    return 0;
}
