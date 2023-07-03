// cpp---std_and_stl
#include <cstdio>
#include <memory>
#include <mutex>
#include <string>
#include <thread>

// imgui---head
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

// opencv
#include "opencv4/opencv2/opencv.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>

// 判断OpenGL头文件
#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GL/glew.h>
// #include <GLES3/gl3.h>
#include <GLFW/glfw3.h> // Will drag system OpenGL headers

// [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to
// maximize ease of testing and compatibility with old VS compilers. To link
// with VS2010-era libraries, VS2015+ requires linking with
// legacy_stdio_definitions.lib, which we do using this pragma. Your own project
// should not be affected, as you are likely to link with a newer binary of GLFW
// that is adequate for your version of Visual Studio.
// 这并不重要但是提供了兼容
#if defined(_MSC_VER) && (_MSC_VER >= 1900) &&                                 \
    !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

// This example can also compile and run with Emscripten! See
// 'Makefile.emscripten' for details.
// 同样不重要
#ifdef __EMSCRIPTEN__
#include "../libs/emscripten/emscripten_mainloop_stub.h"
#endif

// 返回错误信息
static void glfw_error_callback(int error, const char *description) {
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

static void CreateTexture1(const cv::Mat &image, GLuint &textureID) {

    // 设置长宽

    int width = image.cols;

    int height = image.rows;

    int channel = image.channels();

    // 获取图像指针

    unsigned int pixellength = width * height * channel;

    const GLubyte *pixels = image.data;

    // memcpy(pixels, image.data, pixellength);

    // 使用GL指令生成贴图，获取贴图ID

    glBindTexture(GL_TEXTURE_2D, textureID);

    // 必须一个RGB 一个BGR（opencv的mat类的颜色通道是BGR）
    // 否则会出现颜色偏差
    // 字节对其用1bit
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    // 可能导致内存泄漏 纹理id不要重复申请
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR,
                 GL_UNSIGNED_BYTE, pixels);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_BGR,
                    GL_UNSIGNED_BYTE, pixels);
    // 纹理放大缩小使用线性插值

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);
    //    delete[] pixels;
    // glDrawArrays(GL_TRIANGLES, 0, 3); // 绘制

    return;
}

// Main code
int main(int, char **) {

    // 检查有没有错误
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

// Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100
    const char *glsl_version = "#version 100";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
    // GL 3.2 + GLSL 150
    const char *glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Required on Mac
#else
    // GL 3.0 + GLSL 130
    const char *glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+
    // only glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // 3.0+ only
#endif

    //*从这里开始
    // 创建上下文窗口
    // 这里创建的是最大的那个窗口
    GLFWwindow *window = glfwCreateWindow(1280, 720, "我是大窗口", NULL, NULL);

    if (window == NULL)
        return 1;

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |=
        ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |=
        ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls

    // gui风格---light或dark
    ImGui::StyleColorsDark();
    // ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    //*字体可选
    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can
    // also load multiple fonts and use ImGui::PushFont()/PopFont() to select
    // them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you
    // need to select the font among multiple.
    // - If the file cannot be loaded, the function will return NULL. Please
    // handle those errors in your application (e.g. use an assertion, or
    // display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and
    // stored into a texture when calling
    // ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame
    // below will call.
    // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use
    // Freetype for higher quality font rendering.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string
    // literal you need to write a double backslash \\ !
    // - Our Emscripten build process allows embedding fonts to be accessible at
    // runtime from the "fonts/" folder. See Makefile.emscripten for details.
    // io.Fonts->AddFontDefault();
    // io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
    // io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    // io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    // io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    // ImFont* font =
    // io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f,
    // NULL, io.Fonts->GetGlyphRangesJapanese()); IM_ASSERT(font != NULL);

    // 字体
    ImFont *font1 = io.Fonts->AddFontFromFileTTF(
        "../fonts/msyh.ttc", 20.0f, NULL,
        io.Fonts->GetGlyphRangesChineseSimplifiedCommon());
    IM_ASSERT(font1 != NULL);

    // Our state
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // Main loop
#ifdef __EMSCRIPTEN__
    // For an Emscripten build we are disabling file-system access, so let's
    // not attempt to do a fopen() of the imgui.ini file. You may manually
    // call LoadIniSettingsFromMemory() to load settings from your own
    // storage.
    io.IniFilename = NULL;
    EMSCRIPTEN_MAINLOOP_BEGIN
#else
    cv::VideoCapture cap(0);
    std::mutex image_lock;
    cv::Mat image = cv::imread("../docs/test.png");
    cv::Mat src = cv::imread("../docs/test.png");
    GLuint textureID;
    std::mutex image_mutex;
    // 在循环外进行 不然会爆炸
    glGenTextures(1, &textureID);
    std::thread([&cap, &image, &image_lock] {
        while (1) {
            std::lock_guard<std::mutex> lock(image_lock);
            cap.read(image);
        }
    }).detach();

    while (!glfwWindowShouldClose(window))
#endif
    {
        cv::resize(image, image, cv::Size(640, 720));
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard
        // flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input
        // data to your main application, or clear/overwrite your copy of
        // the mouse data.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard
        // input data to your main application, or clear/overwrite your copy
        // of the keyboard data. Generally you may always pass all inputs to
        // dear imgui, and hide them from your application based on those
        // two flags.
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // 最大的那个窗口
        //  1. Show the big demo window (Most of the sample code is in
        //  ImGui::ShowDemoWindow()! You can browse its code to learn more
        //  about Dear ImGui!).
        if (show_demo_window)
            ImGui::ShowDebugLogWindow(&show_demo_window); // 各种debug信息
        // 这里是imgui自带的一些窗口
        // ImGui::ShowUserGuide(); //指导手册
        // ImGui::ShowDemoWindow(&show_demo_window);//一个demo
        // ImGui::ShowStackToolWindow(&show_demo_window);//stack信息
        // ImGui::ShowMetricsWindow(&show_demo_window); //字体详细
        // ImGui::ShowAboutWindow(&show_demo_window); // 窗口信息
        // ImGui::ShowFontSelector("字体选择器");//选择字体
        // ImGuiStyle imgui_style;//传入下面这个函数的变量
        // ImGui::ShowStyleEditor(&imgui_style); //风格

        // 一些常规的窗口
        //  2. Show a simple window that we create ourselves. We use a
        //  Begin/End pair to create a named window.
        // 除了static都会被释放
        {

            static float f = 0.0f;
            static int counter = 0;
            ImGui::Begin("这里是小窗口"); // Create a window called "Hello,
                                          // world!" std::lock_guard<std::mutex>
                                          // lock(image_lock);

            // 字体给第一种
            ImGui::PushFont(font1);
            ImGui::Text("这里是一段描述性的文字."); // Display some text
                                                    // (you can use
            // a format strings too)
            ImGui::Checkbox("我是被控制的中窗口",
                            &show_demo_window); // Edit bools storing our window
                                                // open/close state
            ImGui::Checkbox("更小更小的窗口", &show_another_window);
            // ImGui::Image(bg_tex_id, ImGui::GetContentRegionAvail());

            ImGui::SliderFloat(
                "float", &f, 0.0f,
                1.0f); // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit3(
                "背景色",
                (float *)&clear_color); // Edit 3 floats representing a color

            if (ImGui::Button("按键"))
                // Buttons return true when clicked (most widgets
                // return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("次数 = %d", counter);

            ImGui::Text("平均刷新率 %.3f ms/frame (%.1f FPS)",
                        1000.0f / io.Framerate, io.Framerate);
            CreateTexture1(image, textureID);
            ImTextureID image_id = (GLuint *)textureID;
            ImGui::Image(image_id, ImGui::GetContentRegionAvail());
            ImGui::PopFont();
            ImGui::End();
        }

        // 3. Show another simple window.
        if (show_another_window) {
            ImGui::Begin(
                "小小窗口",
                &show_another_window); // Pass a pointer to our bool variable
                                       // (the window will have a closing button
                                       // that will clear the bool when clicked)
            ImGui::Text("爱来自小小窗口");

            if (ImGui::Button("别关！！！"))
                show_another_window = false;
            ImGui::End();
        }
        // Rendering
        ImGui::Render();
        int display_w, display_h;
        // 得到大窗口size
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        // 给大窗口换色
        glClearColor(clear_color.x * clear_color.w,
                     clear_color.y * clear_color.w,
                     clear_color.z * clear_color.w, clear_color.w);

        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }
#ifdef __EMSCRIPTEN__
    EMSCRIPTEN_MAINLOOP_END;
#endif

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
