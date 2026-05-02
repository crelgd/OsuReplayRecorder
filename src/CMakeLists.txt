include_directories(
    .
    ../include
)

add_executable(ORR
    main.cc
    osr.cc
    pch.cc 
)

target_compile_definitions(ORR PRIVATE LZMA_API_STATIC)
target_precompile_headers(ORR PRIVATE pch.h)
target_link_libraries(ORR ${LIB_DIR}/lzma.lib)
