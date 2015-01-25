# Create vcproj.user file for Visual Studio to set debug working directory
# correctly.
# (Thanks David Avedissian)
function(create_vcproj_userfile TARGETNAME)
  if(MSVC)
    configure_file(
      ${CMAKE_SOURCE_DIR}/cmake/templates/VisualStudioUserFile.vcproj.user.in
      ${CMAKE_CURRENT_BINARY_DIR}/${TARGETNAME}.vcproj.user
      @ONLY)
    configure_file(
      ${CMAKE_SOURCE_DIR}/cmake/templates/VisualStudioUserFile.vcxproj.user.in
      ${CMAKE_CURRENT_BINARY_DIR}/${TARGETNAME}.vcxproj.user
      @ONLY)
  endif()
endfunction()
