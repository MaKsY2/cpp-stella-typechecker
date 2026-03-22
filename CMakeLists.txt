cmake_minimum_required(VERSION 3.16)

project(stella-typechecker
    VERSION 1.0.0
    DESCRIPTION "Stella Language Type Checker"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(APPLE)
    list(APPEND CMAKE_PREFIX_PATH /opt/homebrew/opt/antlr4-cpp-runtime /usr/local/opt/antlr4-cpp-runtime)
endif()

find_package(antlr4-runtime REQUIRED)

if(NOT DEFINED ANTLR4_JAR_LOCATION)
    set(ANTLR4_JAR_LOCATION $ENV{HOME}/Downloads/antlr-4.13.2-complete.jar)
endif()

set(ANTLR4_GENERATED_SRC_DIR ${CMAKE_CURRENT_BINARY_DIR}/antlr4-generated)

set(GRAMMAR_LEXER ${CMAKE_CURRENT_SOURCE_DIR}/src/antlr4/stellaLexer.g4)
set(GRAMMAR_PARSER ${CMAKE_CURRENT_SOURCE_DIR}/src/antlr4/stellaParser.g4)

set(ANTLR4_GENERATED_FILES
    ${ANTLR4_GENERATED_SRC_DIR}/stellaLexer.cpp
    ${ANTLR4_GENERATED_SRC_DIR}/stellaLexer.h
    ${ANTLR4_GENERATED_SRC_DIR}/stellaParser.cpp
    ${ANTLR4_GENERATED_SRC_DIR}/stellaParser.h
    ${ANTLR4_GENERATED_SRC_DIR}/stellaParserBaseVisitor.cpp
    ${ANTLR4_GENERATED_SRC_DIR}/stellaParserBaseVisitor.h
    ${ANTLR4_GENERATED_SRC_DIR}/stellaParserVisitor.cpp
    ${ANTLR4_GENERATED_SRC_DIR}/stellaParserVisitor.h
)

add_custom_command(
    OUTPUT ${ANTLR4_GENERATED_FILES}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${ANTLR4_GENERATED_SRC_DIR}
    COMMAND java -jar ${ANTLR4_JAR_LOCATION}
        -Dlanguage=Cpp
        -no-listener
        -visitor
        -o ${ANTLR4_GENERATED_SRC_DIR}
        -package stella
        ${GRAMMAR_LEXER}
        ${GRAMMAR_PARSER}
    DEPENDS ${GRAMMAR_LEXER} ${GRAMMAR_PARSER}
    COMMENT "Generating ANTLR4 C++ parser and lexer"
    VERBATIM
)

add_custom_target(generate_antlr4
    DEPENDS ${ANTLR4_GENERATED_FILES}
)

file(GLOB_RECURSE SOURCES
    "src/*.cpp"
    "src/*.cc"
    "src/*.cxx"
)

list(FILTER SOURCES EXCLUDE REGEX "src/antlr4/.*")

add_executable(${PROJECT_NAME}
    ${SOURCES}
    ${ANTLR4_GENERATED_FILES}
)

add_dependencies(${PROJECT_NAME} generate_antlr4)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${ANTLR4_GENERATED_SRC_DIR}
        ${ANTLR4_INCLUDE_DIR}
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        antlr4_static
)

set_target_properties(${PROJECT_NAME}
    PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)