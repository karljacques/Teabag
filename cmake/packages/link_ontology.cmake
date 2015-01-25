###############################################################################
# Input variables
#   CMAKE_COMMON_ARGS:list
#     - a list of cmake arguments to pass to the cmake command when configuring
###############################################################################

macro (link_ontology TARGET_NAME)
    if (EXISTS "${EXTERNAL_DEP_DIR}/src/ontology_PROJECT-stamp/ontology_PROJECT-done")
        add_custom_target (ontology_PROJECT)
    else ()
        externalproject_add (ontology_PROJECT
            PREFIX ${EXTERNAL_DEP_DIR}
            GIT_REPOSITORY "git://github.com/TheComet93/ontology.git"
            GIT_TAG "master"
            CMAKE_ARGS ${COMMON_CMAKE_FLAGS}
        )
    endif ()
    target_link_libraries (${TARGET_NAME} ontology)
    
    add_dependencies (${TARGET_NAME} ontology_PROJECT)
endmacro (link_ontology)