macro (link_boost TARGET_NAME)
    # ogre requires at least boost 1.46.0
    find_package (Boost 1.46.0 COMPONENTS
        system
        python)
    if (Boost_FOUND)
        add_custom_target (BOOST_PROJECT)
        # hack to force boost to use python2.7
        string (REGEX REPLACE "[0-9]\\.[0-9]" "2.7" Boost_PYTHON_LIBRARY ${Boost_PYTHON_LIBRARY})
        # make sure the replaced version exists
        if (NOT EXISTS ${Boost_PYTHON_LIBRARY})
            message (FATAL "Failed to downgrade Boost.Python to version 2.7. Please make sure you've built Boost.Python with python 2.7 support.")
        endif ()
        message (STATUS "Boost.Python library is: ${Boost_PYTHON_LIBRARY}")
    else (Boost_FOUND)
        message (STATUS "Boost will be downloaded automatically")

        # windows specific settings
        if (WIN32)
            set (BOOST_CONFIGURE_COMMAND "bootstrap.bat"
                "--with-python-version=2.7")
            set (BOOST_BUILD_COMMAND "b2")
        else (WIN32)
            set (BOOST_CONFIGURE_COMMAND "./bootstrap.sh"
                "--with-python-version=2.7")
            set (BOOST_BUILD_COMMAND "./b2")
        endif (WIN32)
        set (BOOST_BUILD_COMMAND ${BOOST_BUILD_COMMAND}
            "install"
            "link=shared"
            "--ignore-site-config"
            "--prefix=${EXTERNAL_DEP_DIR}"
            "--with-date_time"
            "--with-chrono"
            "--with-thread"
            "--with-atomic"
            "--with-system"
            "--with-python"
            "--with-signals"
        )

        externalproject_add (BOOST_PROJECT
            DEPENDS OGREDEPS_PROJECT
            PREFIX ${EXTERNAL_DEP_DIR}
            URL "http://sourceforge.net/projects/boost/files/boost/1.56.0/boost_1_56_0.tar.bz2"
            CONFIGURE_COMMAND ${BOOST_CONFIGURE_COMMAND}
            BUILD_COMMAND ${BOOST_BUILD_COMMAND}
            BUILD_IN_SOURCE 1
            INSTALL_COMMAND ""
        )
        target_link_libraries (${TARGET_NAME}
            boost_system
            boost_python
        )
        
        set (Boost_SYSTEM_LIBRARY boost_system)
        set (Boost_PYTHON_LIBRARY boost_python)
        set (Boost_INCLUDE_DIRS "")
    endif (Boost_FOUND)

    include_directories (${Boost_INCLUDE_DIRS})
    target_link_libraries (${TARGET_NAME}
        ${Boost_SYSTEM_LIBRARY}
        ${Boost_PYTHON_LIBRARY}
    )
    
    add_dependencies (${TARGET_NAME} BOOST_PROJECT)
endmacro (link_boost)
