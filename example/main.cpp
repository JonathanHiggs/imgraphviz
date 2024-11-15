#include <imgraphviz/imgraphviz_demo.h>

#include <fmt/ostream.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>


int main()
{
    if (!glfwInit())
    {
        char const * message;
        auto code = glfwGetError(&message);

        fmt::print("Failed to initialize GLFW: {} {}\n", code, message);
        return 1;
    }

    // glfwSetErrorCallback(ErrorCallback);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    auto * window = glfwCreateWindow(2560, 1440, "ImGraphviz Example", nullptr, nullptr);
    if (!window)
    {
        fmt::print("Failed to create window\n");
        return 1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        fmt::print("Failed to initialize GLAD\n");
        return 1;
    }

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glfwSwapInterval(1);
    glViewport(0, 0, width, height);
    glClearColor(0.08235f, 0.08235f, 0.08235f, 1.0f);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    auto & io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    auto const baseFontSize = 22.0f;
    auto const iconFontSize = baseFontSize * 2.0f / 3.0f;

    auto const segoeuiPath = "C:\\Windows\\Fonts\\segoeui.ttf";
    io.Fonts->AddFontFromFileTTF(segoeuiPath, baseFontSize);
    io.Fonts->Build();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 460");

    auto * context = ImGraphviz::CreateContext();

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGraphviz::ShowGraphNodeDemoWindow();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    ImGraphviz::DestroyContext();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
