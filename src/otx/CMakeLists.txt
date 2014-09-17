# Copyright (c) Cameron Garnham, Monetas AG, 2014

set(name otx)

set(cxx-sources
  main.cpp
)

set(MODULE_NAME otx)
if (WIN32)
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/winexe.rc.in
    ${CMAKE_CURRENT_BINARY_DIR}/module.rc
    @ONLY
  )

  add_executable(
    ${MODULE_NAME}
    ${cxx-sources}
    ${CMAKE_CURRENT_BINARY_DIR}/module.rc
  )
else()
  add_executable(${MODULE_NAME} ${cxx-sources})
endif()

if(UNIX)
  if (LOCAL_EDITLINE)
          find_package(EditlineLocal REQUIRED)
          if (EDITLINE_FOUND)
                  include_directories(${EDITLINE_INCLUDE_DIR})
                  target_link_libraries (otx ${EDITLINE_LIBRARIES})
          endif (EDITLINE_FOUND)
  else ()
          find_package(Editline REQUIRED)
          if (EDITLINE_FOUND)
                  include_directories(${EDITLINE_INCLUDE_DIR})
                  target_link_libraries (otx ${EDITLINE_LIBRARIES})
          endif (EDITLINE_FOUND)
  endif(LOCAL_EDITLINE) 
endif()

target_link_libraries(${name} otx-base ${client} ${ext} ${basket} ${core})

install(TARGETS otx DESTINATION bin)
