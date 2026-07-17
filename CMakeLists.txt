cmake_minimum_required(VERSION 3.20)

project(lanchonete_poo CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_options(-Wall -Wextra)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(-g)
    if(NOT WIN32)
        add_compile_options(-fsanitize=address,undefined)
        add_link_options(-fsanitize=address,undefined)
    endif()
endif()

add_executable(lanchonete_poo
    src/main.cpp
    src/product.cpp
    src/food.cpp
    src/beverage.cpp
    src/combo.cpp
    src/customer.cpp
    src/order_item.cpp
    src/order.cpp
)

target_include_directories(lanchonete_poo PRIVATE src)

# Configuracao do Catch2 para testes (opcional - pode dar erro com paths especiais)
include(FetchContent)
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG        v3.5.2
)
FetchContent_MakeAvailable(Catch2)

add_executable(testes 
    tests/test_tp2.cpp
    src/product.cpp
    src/food.cpp
    src/beverage.cpp
    src/combo.cpp
    src/customer.cpp
    src/order_item.cpp
    src/order.cpp
)
target_include_directories(testes PRIVATE src)
target_link_libraries(testes PRIVATE Catch2::Catch2WithMain)

include(CTest)
include(Catch)
catch_discover_tests(testes)