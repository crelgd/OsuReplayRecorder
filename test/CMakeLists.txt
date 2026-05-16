include_directories(
    .
    ${CMAKE_SOURCE_DIR}/src 
)

add_executable(GetComaSeparatedFile 
    getcomaseparated.cc 
)

add_executable(MinParserWithoutCS
    minparser.cc
)

add_executable(OsuReadGeneral
    osuReadGeneral.cc
)

target_link_libraries(MinParserWithoutCS format)
target_link_libraries(GetComaSeparatedFile format)
target_link_libraries(OsuReadGeneral format)

