macro (link_python TARGET_NAME)
    message (STATUS "Looking for Python...")
    find_package (PythonLibs 2.7 REQUIRED)
    # hack to force python 2.7
    string (REGEX REPLACE "[0-9]\\.[0-9]" "2.7" PYTHON_LIBRARIES
        ${PYTHON_LIBRARIES})
    string (REGEX REPLACE "[0-9]\\.[0-9]" "2.7" PYTHON_INCLUDE_DIRS
        ${PYTHON_INCLUDE_DIRS})
    # make sure the replaced version exists
    if (NOT EXISTS ${PYTHON_LIBRARIES} OR NOT EXISTS ${PYTHON_INCLUDE_DIRS})
            message (FATAL "Could NOT Find Python 2.7. Please download and install Python 2.7.")
    endif ()
    message (STATUS "Found Python library: ${PYTHON_LIBRARIES}")
    message (STATUS "Found Python headers: ${PYTHON_INCLUDE_DIRS}")
    include_directories (${PYTHON_INCLUDE_DIRS})
    target_link_libraries (${TARGET_NAME} ${PYTHON_LIBRARIES})
endmacro (link_python)