include_directories(
    .
    ${CMAKE_SOURCE_DIR}/include
)

add_executable(orr
    main.cc
    base.cc
    osr.cc
    osu.cc
    pch.cc 
    
    visual/visual_tools.cc 
    visual/visual.cc 
)

target_compile_definitions(orr PRIVATE)
target_precompile_headers(orr PRIVATE pch.h)
target_link_libraries(orr 
    ${LIB_DIR}/liblzma.lib
    ${LIB_DIR}/spdlog.lib
    ${LIB_DIR}/SDL2.lib
    ${LIB_DIR}/SDL2main.lib
    ${LIB_DIR}/glew32.lib
    OpenGL32.lib
)

