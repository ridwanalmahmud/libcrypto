option(BUILD_SHARED_LIBS "Build shared libraries" ON)
option(BUILD_STATIC_LIBS "Build static libraries" ON)

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_C_COMPILER_ID MATCHES "Clang")
    add_compile_options(-Wall -Wextra -pedantic -pipe)
    # add_compile_options("$<$<CONFIG:Debug>:-g3;-Og;-fsanitize=address,undefined>")
    add_compile_options("$<$<CONFIG:Debug>:-g3;-Og>")
    add_compile_options("$<$<CONFIG:Release>:-O3;-D_FORTIFY_SOURCE=2;-fstack-protector-strong;-fPIE>")
    add_link_options("$<$<CONFIG:Release>:-pie;-Wl,-z,now,-z,relro>")
endif()

if(CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64x")
    add_compile_options("$<$<CONFIG:Release>:-maes;-msse4.1;-mpclmul>")
endif()

file(GLOB_RECURSE LIB_SOURCES "src/*.c")

# interface target for includes
add_library(crypto_interface INTERFACE)
target_include_directories(crypto_interface INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/crypto>
    $<INSTALL_INTERFACE:include>
)

# shared library
if(BUILD_SHARED_LIBS)
    add_library(crypto_shared SHARED ${LIB_SOURCES})
    target_link_libraries(crypto_shared PUBLIC crypto_interface m)
    set_target_properties(crypto_shared PROPERTIES 
        OUTPUT_NAME "crypto" 
        VERSION ${PROJECT_VERSION} 
        SOVERSION 1
    )
endif()

# static library
if(BUILD_STATIC_LIBS)
    add_library(crypto_static STATIC ${LIB_SOURCES})
    target_link_libraries(crypto_static PUBLIC crypto_interface m)
    set_target_properties(crypto_static PROPERTIES OUTPUT_NAME "crypto")
    if(WIN32)
        set_target_properties(crypto_static PROPERTIES OUTPUT_NAME "crypto_s")
    endif()
endif()
