include_directories(
    .
    ../include
)

add_executable(orr
    main.cc
    osr.cc
    pch.cc 
)

target_compile_definitions(orr PRIVATE)
target_precompile_headers(orr PRIVATE pch.h)
target_link_libraries(orr ${LIB_DIR}/liblzma.lib)
