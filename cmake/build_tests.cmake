function(add_crypto_test test_name)
    add_executable(${test_name} ${ARGN})
    target_include_directories(${test_name} PRIVATE ${PROJECT_SOURCE_DIR}/nutest)
    target_link_libraries(${test_name} PRIVATE crypto_interface)

    if(TARGET crypto_shared)
        target_link_libraries(${test_name} PRIVATE crypto_shared)
    else()
        target_link_libraries(${test_name} PRIVATE crypto_static)
    endif()

    add_test(NAME ${test_name} COMMAND ${test_name})
endfunction()
