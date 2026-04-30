include_directories(.)

add_executable(ORR
    main.cc
    pch.cc 
)
target_precompile_headers(ORR PRIVATE pch.h)

