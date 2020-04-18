if (EXISTS "${CMAKE_BINARY_DIR}/conanbuildinfo.cmake")
    include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    include(${CMAKE_BINARY_DIR}/conan_paths.cmake)
    conan_basic_setup()
endif ()

# todo: fixme
# Since our compiler is apple-clang
add_compile_options(-Werror=all)

if (ALIGNED_ENABLE_TESTS)
    enable_testing()
endif ()
