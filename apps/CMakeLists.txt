# Parent CMake for all applications

# Start with empty list or your default apps
set(AVAILABLE_APPS
    # Default apps that should always be included
)

# If specific apps were selected, append them to AVAILABLE_APPS
if(DEFINED APPS_TO_BUILD AND NOT "${APPS_TO_BUILD}" STREQUAL "ALL")
    # Clear the list first if you want ONLY the selected apps
    set(AVAILABLE_APPS "")

    # Add each selected app to the list
    foreach(APP ${APPS_TO_BUILD})
        list(APPEND AVAILABLE_APPS ${APP})
    endforeach()

    message(STATUS "Building selected apps: ${AVAILABLE_APPS}")
else()
    # For "build all" case, add all available apps
    list(APPEND AVAILABLE_APPS
        app1
        lib_vtuzki_test
        template_app
        # Add any other apps here
    )
    message(STATUS "Building all available apps: ${AVAILABLE_APPS}")
endif()

# Helper function to check if an app should be built
function(should_build_app APP_NAME RESULT_VAR)
    # If RE_LEARN_SELECTED_APPS is empty, build all apps
    if("${RE_LEARN_SELECTED_APPS}" STREQUAL "")
        set(${RESULT_VAR} TRUE PARENT_SCOPE)
        return()
    endif()

    # First, try to interpret the selection as a semicolon-separated list
    set(SELECTED_LIST "${RE_LEARN_SELECTED_APPS}")

    # If it contains commas, convert to a list
    if("${RE_LEARN_SELECTED_APPS}" MATCHES ",")
        string(REPLACE "," ";" SELECTED_LIST "${RE_LEARN_SELECTED_APPS}")
    endif()

    # Check if the app is in the list
    list(FIND SELECTED_LIST "${APP_NAME}" APP_INDEX)
    if(APP_INDEX GREATER -1)
        set(${RESULT_VAR} TRUE PARENT_SCOPE)
    else()
        set(${RESULT_VAR} FALSE PARENT_SCOPE)
    endif()
endfunction()

# Process each available app
foreach(APP ${AVAILABLE_APPS})
    should_build_app(${APP} SHOULD_BUILD)
    if(SHOULD_BUILD)
        message(STATUS "Adding app: ${APP}")
        add_subdirectory(${APP})
    else()
        message(STATUS "Skipping app: ${APP}")
    endif()
endforeach()

# To use the template app:
# 1. Copy the 'template_app' folder to a new name, e.g., 'my_new_app'
# 2. Rename all occurrences of 'template_app' to 'my_new_app' in files
# 3. Add 'my_new_app' to the AVAILABLE_APPS list above
# 4. Customize the app code as needed