cmake_minimum_required(VERSION 3.16)

project(SchrodingerEquationSimulation LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(SchrodingerSim
    main.cpp
    Math/Complex.cpp
    Math/ComposedFunction.cpp
    Math/ConstantFunction.cpp
    Math/CosFunction.cpp
    Math/EquationSymbols.cpp
    Math/PolynomialFunction.cpp
    Math/SineFunction.cpp
    Math/WaveFunction.cpp
    Rendering/PropagationGraph.cpp
    Rendering/Renderer.cpp
)

target_include_directories(SchrodingerSim PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

if(APPLE)
    find_library(OPENGL_FRAMEWORK OpenGL REQUIRED)
    find_library(GLUT_FRAMEWORK GLUT REQUIRED)

    target_link_libraries(SchrodingerSim PRIVATE
        ${OPENGL_FRAMEWORK}
        ${GLUT_FRAMEWORK}
    )
else()
    find_package(OpenGL REQUIRED)
    find_package(GLUT REQUIRED)

    target_include_directories(SchrodingerSim PRIVATE ${GLUT_INCLUDE_DIRS})
    target_link_libraries(SchrodingerSim PRIVATE
        OpenGL::GL
        ${GLUT_LIBRARIES}
    )
endif()
