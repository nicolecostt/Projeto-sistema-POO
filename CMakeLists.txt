cmake_minimum_required(VERSION 3.20)

project(lanchonete_poo CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_options(-Wall -Wextra)

option(ENABLE_TSAN "Enable ThreadSanitizer on supported compilers" OFF)
option(BUILD_QT_GUI "Build the Qt Widgets GUI for TP3 question 6" OFF)

find_package(Threads REQUIRED)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(-g)
    if(NOT WIN32 AND NOT ENABLE_TSAN)
        add_compile_options(-fsanitize=address,undefined)
        add_link_options(-fsanitize=address,undefined)
    endif()
endif()

if(ENABLE_TSAN AND NOT WIN32)
    add_compile_options(-fsanitize=thread)
    add_link_options(-fsanitize=thread)
endif()

set(PROJECT_SOURCES
    src/product.cpp
    src/food.cpp
    src/beverage.cpp
    src/combo.cpp
    src/customer.cpp
    src/order_item.cpp
    src/order.cpp
    src/menu_catalog.cpp
    src/serialization.cpp
    src/repository.cpp
    src/lanchonete_app.cpp
)

add_executable(lanchonete_poo
    src/main.cpp
    ${PROJECT_SOURCES}
)

target_include_directories(lanchonete_poo PRIVATE src)

# Dependencia JSON usada pela serializacao do TP3.
include(FetchContent)
FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG        v3.11.3
)
FetchContent_MakeAvailable(nlohmann_json)

target_link_libraries(lanchonete_poo PRIVATE
    nlohmann_json::nlohmann_json
    Threads::Threads
)

if(BUILD_QT_GUI)
    find_package(Qt6 REQUIRED COMPONENTS Widgets)
    qt_standard_project_setup()

    qt_add_executable(gui
        src/gui_main.cpp
        src/main_window.cpp
        src/main_window.hpp
        ${PROJECT_SOURCES}
    )
    target_include_directories(gui PRIVATE src)
    target_link_libraries(gui PRIVATE
        Qt6::Widgets
        nlohmann_json::nlohmann_json
        Threads::Threads
    )
endif()

# Configuracao do Catch2 para testes automatizados.
include(FetchContent)
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG        v3.5.2
)
FetchContent_MakeAvailable(Catch2)

add_executable(testes 
    tests/test_tp2.cpp
    tests/test_tp3.cpp
    ${PROJECT_SOURCES}
)
target_include_directories(testes PRIVATE src)
target_link_libraries(testes PRIVATE
    Catch2::Catch2WithMain
    nlohmann_json::nlohmann_json
    Threads::Threads
)

include(CTest)
include(Catch)
catch_discover_tests(testes)
