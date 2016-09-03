macro(temp_name fname)
  if(${ARGC} GREATER 1) # Have to escape ARGC to correctly compare
    set(_base ${ARGV1})
  else(${ARGC} GREATER 1)
    set(_base ".cmake-tmp")
  endif(${ARGC} GREATER 1)
  set(_counter 0)
  while(EXISTS "${_base}${_counter}")
    math(EXPR _counter "${_counter} + 1")
  endwhile(EXISTS "${_base}${_counter}")
  set(${fname} "${_base}${_counter}")
endmacro(temp_name)

# Evaluate expression
# Suggestion from the Wiki: http://cmake.org/Wiki/CMake/Language_Syntax
# Unfortunately, no built-in stuff for this: http://public.kitware.com/Bug/view.php?id=4034
macro(eval expr)
  temp_name(_fname)
  file(WRITE ${_fname} "${expr}")
  include(${_fname})
  file(REMOVE ${_fname})
endmacro(eval)

# Add the project itself to the list of projects to be built
# This is done for every addon but might not have a project to add
# the addon will determine that.
function(add_marketplace_addons_projects)
  foreach(ADDON ${OPENGINE_ADDONS})
    message("Addon: ${ADDON}")
    include(${OPIFEX_MARKETPLACE}/${ADDON}/addon.cmake)
    string(REPLACE "." ";" VERSION_LIST ${ADDON})
    list(GET VERSION_LIST 0 ADDON_NAME)
    eval("ADDON_${ADDON_NAME}_PROJECT(${CMAKE_CURRENT_BINARY_DIR})")
  endforeach()
endfunction(add_marketplace_addons_projects)


macro(add_marketplace_addons APPLICATION_TARGET FOLDER ADDITIONAL)

  foreach(ADDON ${OPENGINE_ADDONS})

    include(${OPIFEX_MARKETPLACE}/${ADDON}/addon.cmake)

    string(REPLACE "." ";" VERSION_LIST ${ADDON})
    list(GET VERSION_LIST 0 ADDON_NAME)

    eval("ADDON_${ADDON_NAME}(APPLICATION_TARGET ${FOLDER})")

    SET(TEMP_RESULT "")

    eval("ADDON_${ADDON_NAME}_LINK(TEMP_RESULT)")

    eval("target_link_libraries(${APPLICATION_TARGET} ${TEMP_RESULT})")
    add_definitions(-DADDON_${ADDON_NAME})

    eval("ADDON_${ADDON_NAME}_DEFINES(TEMP_RESULT)")
    add_definitions(${TEMP_RESULT})

    SET(TEMP_RESULT "")
    eval("ADDON_${ADDON_NAME}_ASSETS(TEMP_RESULT)")
    IF(TEMP_RESULT STREQUAL "")
    ELSE()
      SET(${ADDITIONAL} "${${ADDITIONAL}}|${TEMP_RESULT}")
    ENDIF()

  endforeach()

endmacro(add_marketplace_addons)

# Add only the defines for each addon
macro(add_marketplace_assets ADDITIONAL)

    foreach(ADDON ${OPENGINE_ADDONS})

      include(${OPIFEX_MARKETPLACE}/${ADDON}/addon.cmake)

      string(REPLACE "." ";" VERSION_LIST ${ADDON})
      list(GET VERSION_LIST 0 ADDON_NAME)

      SET(TEMP_RESULT "")
      eval("ADDON_${ADDON_NAME}_ASSETS(TEMP_RESULT)")
      IF(TEMP_RESULT STREQUAL "")
      ELSE()
        SET(${ADDITIONAL} "${${ADDITIONAL}}|${TEMP_RESULT}")
      ENDIF()

    endforeach()

endmacro(add_marketplace_assets)

# Add only the defines for each addon
macro(add_marketplace_defines)

    foreach(ADDON ${OPENGINE_ADDONS})

      include(${OPIFEX_MARKETPLACE}/${ADDON}/addon.cmake)

      string(REPLACE "." ";" VERSION_LIST ${ADDON})
      list(GET VERSION_LIST 0 ADDON_NAME)

      eval("ADDON_${ADDON_NAME}(0)")

      add_definitions(-DADDON_${ADDON_NAME})

      eval("ADDON_${ADDON_NAME}_DEFINES(TEMP_RESULT)")
      add_definitions(${TEMP_RESULT})

    endforeach()

endmacro(add_marketplace_defines)
