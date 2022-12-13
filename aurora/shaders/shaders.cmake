macro(a_build_shader file name)
    get_filename_component(fext "${file}" EXT)
    get_filename_component(fname "${file}" NAME_WE)
    get_filename_component(fdir "${file}" DIRECTORY)
    get_filename_component(fabs "${file}" ABSOLUTE)

    string(REPLACE "/" "_" fdir_u "${fdir}")
    if (NOT "${fdir}" STREQUAL "")
        set(fdir "${fdir}/")
    endif ()
    add_custom_command(
            OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/${fdir_u}${fname}.cpp"
            COMMAND ashaderc --encapsulate --encapsulate-var ${name} -i "${fabs}" -o "${fdir_u}${fname}.cpp"
            MAIN_DEPENDENCY ${fabs}
            DEPENDS ashaderc
            VERBATIM)
    list(APPEND GEN_SOURCES "${CMAKE_CURRENT_BINARY_DIR}/${fdir_u}${fname}.cpp")
endmacro()

a_build_shader(aurora/shaders/test.ashdr.xml aurora::shaders::test)
