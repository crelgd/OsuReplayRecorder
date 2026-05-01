include_directories(.)

add_executable(ORR
    main.cc
    osr.cc
    pch.cc 
)

add_executable(uleb128
    uleb128.cc
    osr.cc
)

target_precompile_headers(ORR PRIVATE pch.h)
target_precompile_headers(uleb128 PRIVATE pch.h)
