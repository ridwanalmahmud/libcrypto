# add each test as separate executable
file(GLOB TEST_SOURCES *.c)

foreach(test_source ${TEST_SOURCES})
    get_filename_component(test_name ${test_source} NAME_WE)
    add_crypto_test(${test_name} ${test_source})
endforeach()
