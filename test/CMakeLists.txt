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

target_link_libraries(MinParserWithoutCS format)
target_link_libraries(GetComaSeparatedFile format)