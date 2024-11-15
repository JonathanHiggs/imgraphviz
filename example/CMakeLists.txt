add_executable(imgraphviz_example)

target_sources(imgraphviz_example PRIVATE main.cpp)

target_link_libraries(imgraphviz_example PRIVATE imgraphviz glad::glad glfw fmt::fmt OpenGL::GL)
