# Use the simplified setup function
setup_app(lib_vtuzki_test
    SOURCES
        s_array_test.c
    EXTERNAL_LIBS
        vtuzkicc
)

# Setup tests if enabled
if(RE_LEARN_BUILD_TESTS)
    setup_app_tests(lib_vtuzki_test)
endif()