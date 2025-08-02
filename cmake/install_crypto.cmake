option(INSTALL_SYSTEM_WIDE "Install system-wide" ON)

if(INSTALL_SYSTEM_WIDE)
    include(GNUInstallDirs)

    # install library targets
    install(TARGETS crypto_interface crypto_shared crypto_static
        EXPORT crypto-targets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    )

    # install headers
    install(DIRECTORY crypto/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/crypto)

    # install config files
    install(EXPORT crypto-targets 
        FILE crypto-config.cmake 
        NAMESPACE crypto:: 
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/crypto
    )

    # generate pkg-config file
    configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/cmake/crypto.pc.in
        ${CMAKE_CURRENT_BINARY_DIR}/crypto.pc
        @ONLY
    )
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/crypto.pc
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig
    )
endif()
