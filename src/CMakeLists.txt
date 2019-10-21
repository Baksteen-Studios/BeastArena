## Compiler options
# Make the compiler complain, a lot
# Force colors when using ninja with clang
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(-fcolor-diagnostics)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_compile_options(-fdiagnostics-color=always)
endif()
add_compile_options(-Wall -Wextra -pedantic -Werror)

add_executable(BeastArena
    main.cpp
)
target_link_libraries(BeastArena PRIVATE BeastArenaLib)
target_compile_features(BeastArena PUBLIC cxx_std_17)