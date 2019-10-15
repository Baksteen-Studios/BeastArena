file(GLOB_RECURSE BEASTARENA_SRC_FILES RELATIVE ${CMAKE_CURRENT_LIST_DIR} *.c[p]*)
list(REMOVE_ITEM BEASTARENA_SRC_FILES "main.cpp")

## Compiler options
# Make the compiler complain, a lot
# Force colors when using ninja with clang
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(-fcolor-diagnostics)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_compile_options(-fdiagnostics-color=always)
endif()
# Turn on the address sanitizer for safety checks
option(BEASTARENA_SANITIZER "Turn on the address sanitizer" OFF)
if(BEASTARENA_SANITIZER)
    add_compile_options(-fsanitize=address -fno-omit-frame-pointer)
    add_link_options(-fsanitize=address)
endif()
add_compile_options(-Wall -Wextra -pedantic -Werror)

add_executable(BeastArena
    main.cpp
    ${BEASTARENA_SRC_FILES}
)
target_link_libraries(BeastArena PRIVATE BrickEngine)
target_include_directories(BeastArena PUBLIC ${BEASTARENA_INCLUDE_DIRECTORY})
add_custom_command(TARGET BeastArena PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_SOURCE_DIR}/assets/ $<TARGET_FILE_DIR:BeastArena>/assets)
target_compile_features(BeastArena PUBLIC cxx_std_17)

# Export source file list so that it can be used for test compilation
set(SRC_FILES ${SRC_FILES} PARENT_SCOPE)