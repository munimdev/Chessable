cmake_minimum_required(VERSION 3.20)

project(
    chessable
    VERSION 0.1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CHESSABLE_WARNING_FLAGS "")
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    set(
        CHESSABLE_WARNING_FLAGS
        -Wall
        -Wextra
        -Wpedantic
        -Wconversion
        -Wshadow
    )
elseif(MSVC)
    set(CHESSABLE_WARNING_FLAGS /W4)
endif()

option(CHESSABLE_ENABLE_WARNINGS_AS_ERRORS "Treat warnings as errors" OFF)
if(CHESSABLE_ENABLE_WARNINGS_AS_ERRORS)
    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
        list(APPEND CHESSABLE_WARNING_FLAGS -Werror)
    elseif(MSVC)
        list(APPEND CHESSABLE_WARNING_FLAGS /WX)
    endif()
endif()

add_library(
    pgnlib
    src/pgnlib/PGNCommentText.cpp
    src/pgnlib/PGNException.cpp
    src/pgnlib/PGNGame.cpp
    src/pgnlib/PGNGameCollection.cpp
    src/pgnlib/PGNGameResult.cpp
    src/pgnlib/PGNMove.cpp
    src/pgnlib/PGNMoveList.cpp
    src/pgnlib/PGNParser.cpp
    src/pgnlib/PGNPiece.cpp
    src/pgnlib/PGNPly.cpp
    src/pgnlib/PGNPosition.cpp
    src/pgnlib/PGNSquare.cpp
    src/pgnlib/PGNTag.cpp
    src/pgnlib/PGNTagList.cpp
)
target_include_directories(pgnlib PUBLIC src src/pgnlib)
target_compile_options(pgnlib PRIVATE ${CHESSABLE_WARNING_FLAGS})

add_executable(
    chessable_cli
    src/main.cpp
    src/main_entry.cpp
    src/app/cli_runner.cpp
    src/app/config_parser.cpp
    src/app/menu_controller.cpp
    src/cli/menu.cpp
    src/domain/chess_types.cpp
    src/ingest/database_loader.cpp
    src/query/chess_queries.cpp
    src/query/query_service.cpp
)
target_link_libraries(chessable_cli PRIVATE pgnlib)
target_include_directories(chessable_cli PRIVATE src src/pgnlib)
target_compile_options(chessable_cli PRIVATE ${CHESSABLE_WARNING_FLAGS})

include(CTest)
if(BUILD_TESTING)
    add_executable(
        ds_smoke_tests
        tests/ds_smoke_tests.cpp
    )
    target_include_directories(ds_smoke_tests PRIVATE src src/pgnlib)
    target_compile_options(ds_smoke_tests PRIVATE ${CHESSABLE_WARNING_FLAGS})
    add_test(NAME ds_smoke_tests COMMAND ds_smoke_tests)
endif()
