function(a_add_executable name)
    set(SOURCE)
    set(ASSET_DEPENDENCIES)
    set(ASSET_PATHS_RELATIVE)

    foreach (file ${ARGN})
        get_filename_component(fext "${file}" EXT)
        get_filename_component(fname "${file}" NAME_WE)
        get_filename_component(fdir "${file}" DIRECTORY)
        get_filename_component(fabs "${file}" ABSOLUTE)

        if ("${fext}" STREQUAL ".ashdr.xml")
            string(REPLACE "/" "_" fdir_u "${fdir}")
            if (NOT "${fdir}" STREQUAL "")
                set(fdir "${fdir}/")
            endif ()
            add_custom_target("${fdir_u}${fname}.ashdr.aet" COMMAND ashaderc -i "${fabs}" -o "${fdir}${fname}.ashdr.aet" SOURCES ${file})
            list(APPEND ASSET_DEPENDENCIES "${fdir_u}${fname}.ashdr.aet")
            list(APPEND ASSET_PATHS_RELATIVE "${fdir}${fname}.ashdr.aet")
        elseif ("${fext}" STREQUAL ".alvl.xml")
            string(REPLACE "/" "_" fdir_u "${fdir}")
            if (NOT "${fdir}" STREQUAL "")
                set(fdir "${fdir}/")
            endif ()
            add_custom_target("${fdir_u}${fname}.alvl.aet" COMMAND alevelc -i "${fabs}" -o "${fdir}${fname}.alvl.aet" SOURCES ${file})
            list(APPEND ASSET_DEPENDENCIES "${fdir_u}${fname}.alvl.aet")
            list(APPEND ASSET_PATHS_RELATIVE "${fdir}${fname}.alvl.aet")
        elseif ("${fext}" STREQUAL ".obj")
            string(REPLACE "/" "_" fdir_u "${fdir}")
            if (NOT "${fdir}" STREQUAL "")
                set(fdir "${fdir}/")
            endif ()
            add_custom_target("${fdir_u}${fname}${fext}.aet"
                    COMMAND ameshc -i "${fabs}.aet.meta" -o "${fdir}${fname}${fext}.aet" -m "${fabs}"
                    SOURCES ${file} ${file}.aet.meta)
            file(COPY "${fabs}" DESTINATION "${fdir}")
            list(APPEND ASSET_DEPENDENCIES "${fdir_u}${fname}${fext}.aet")
            list(APPEND ASSET_PATHS_RELATIVE "${fdir}${fname}${fext}.aet")
        elseif ("${fext}" STREQUAL ".png" OR
                "${fext}" STREQUAL ".jpg" OR
                "${fext}" STREQUAL ".jpeg")
            string(REPLACE "/" "_" fdir_u "${fdir}")
            if (NOT "${fdir}" STREQUAL "")
                set(fdir "${fdir}/")
            endif ()
            add_custom_target("${fdir_u}${fname}${fext}.aet"
                    COMMAND atexturec -i "${fabs}.aet.meta" -o "${fdir}${fname}${fext}.aet" -t "${fdir}${fname}${fext}"
                    SOURCES ${file} ${file}.aet.meta)
            file(COPY "${fabs}" DESTINATION "${fdir}")
            list(APPEND ASSET_DEPENDENCIES "${fdir_u}${fname}${fext}.aet")
            list(APPEND ASSET_PATHS_RELATIVE "${fdir}${fname}${fext}.aet")
        else ()
            list(APPEND SOURCE ${file})
        endif ()
    endforeach ()

    add_executable(${name} ${SOURCE})
    target_link_libraries(${name} PUBLIC aurora)

    add_custom_target(${name}_index COMMAND amkindex -o assets.idx.aet -i ${ASSET_PATHS_RELATIVE} DEPENDS ${ASSET_DEPENDENCIES})
    add_dependencies(${name} ${name}_index;${ASSET_DEPENDENCIES})
endfunction()
