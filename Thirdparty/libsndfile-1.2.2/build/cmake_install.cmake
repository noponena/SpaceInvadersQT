# Install script for directory: C:/Users/aaron/Downloads/libsndfile-1.2.2/libsndfile-1.2.2

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/libsndfile")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set path to fallback-tool for dependency-resolution.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "C:/Qt/Tools/mingw1310_64/bin/objdump.exe")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "C:/Users/aaron/Downloads/libsndfile-1.2.2/libsndfile-1.2.2/build/sndfile-info.exe")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/sndfile-info.exe" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/sndfile-info.exe")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "C:/Qt/Tools/mingw1310_64/bin/strip.exe" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/sndfile-info.exe")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "C:/Users/aaron/Downloads/libsndfile-1.2.2/libsndfile-1.2.2/build/sndfile-play.exe")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/sndfile-play.exe" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/sndfile-play.exe")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "C:/Qt/Tools/mingw1310_64/bin/strip.exe" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/sndfile-play.exe")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "C:/Users/aaron/Downloads/libsndfile-1.2.2/libsndfile-1.2.2/build/sndfile-convert.exe")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/sndfile-convert.exe" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/sndfile-convert.exe")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "C:/Qt/Tools/mingw1310_64/bin/strip.exe" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/sndfile-convert.exe")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "C:/Users/aaron/Downloads/libsndfile-1.2.2/libsndfile-1.2.2/build/sndfile-cmp.exe")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/sndfile-cmp.exe" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/sndfile-cmp.exe")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "C:/Qt/Tools/mingw1310_64/bin/strip.exe" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/sndfile-cmp.exe")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "C:/Users/aaron/Downloads/libsndfile-1.2.2/libsndfile-1.2.2/build/sndfile-metadata-set.exe")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/sndfile-metadata-set.exe" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/sndfile-metadata-set.exe")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "C:/Qt/Tools/mingw1310_64/bin/strip.exe" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/sndfile-metadata-set.exe")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "C:/Users/aaron/Downloads/libsndfile-1.2.2/libsndfile-1.2.2/build/sndfile-metadata-get.exe")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/sndfile-metadata-get.exe" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/sndfile-metadata-get.exe")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "C:/Qt/Tools/mingw1310_64/bin/strip.exe" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/sndfile-metadata-get.exe")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "C:/Users/aaron/Downloads/libsndfile-1.2.2/libsndfile-1.2.2/build/sndfile-interleave.exe")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/sndfile-interleave.exe" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/sndfile-interleave.exe")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "C:/Qt/Tools/mingw1310_64/bin/strip.exe" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/sndfile-interleave.exe")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "C:/Users/aaron/Downloads/libsndfile-1.2.2/libsndfile-1.2.2/build/sndfile-deinterleave.exe")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/sndfile-deinterleave.exe" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/sndfile-deinterleave.exe")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "C:/Qt/Tools/mingw1310_64/bin/strip.exe" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/sndfile-deinterleave.exe")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "C:/Users/aaron/Downloads/libsndfile-1.2.2/libsndfile-1.2.2/build/sndfile-concat.exe")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/sndfile-concat.exe" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/sndfile-concat.exe")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "C:/Qt/Tools/mingw1310_64/bin/strip.exe" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/sndfile-concat.exe")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "C:/Users/aaron/Downloads/libsndfile-1.2.2/libsndfile-1.2.2/build/sndfile-salvage.exe")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/sndfile-salvage.exe" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/sndfile-salvage.exe")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "C:/Qt/Tools/mingw1310_64/bin/strip.exe" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/sndfile-salvage.exe")
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "C:/Users/aaron/Downloads/libsndfile-1.2.2/libsndfile-1.2.2/build/libsndfile.a")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES
    "C:/Users/aaron/Downloads/libsndfile-1.2.2/libsndfile-1.2.2/include/sndfile.h"
    "C:/Users/aaron/Downloads/libsndfile-1.2.2/libsndfile-1.2.2/include/sndfile.hh"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/SndFile/SndFileTargets.cmake")
    file(DIFFERENT _cmake_export_file_changed FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/SndFile/SndFileTargets.cmake"
         "C:/Users/aaron/Downloads/libsndfile-1.2.2/libsndfile-1.2.2/build/CMakeFiles/Export/5c71f72976042dd672d3a20ad1898c82/SndFileTargets.cmake")
    if(_cmake_export_file_changed)
      file(GLOB _cmake_old_config_files "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/SndFile/SndFileTargets-*.cmake")
      if(_cmake_old_config_files)
        string(REPLACE ";" ", " _cmake_old_config_files_text "${_cmake_old_config_files}")
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/SndFile/SndFileTargets.cmake\" will be replaced.  Removing files [${_cmake_old_config_files_text}].")
        unset(_cmake_old_config_files_text)
        file(REMOVE ${_cmake_old_config_files})
      endif()
      unset(_cmake_old_config_files)
    endif()
    unset(_cmake_export_file_changed)
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/SndFile" TYPE FILE FILES "C:/Users/aaron/Downloads/libsndfile-1.2.2/libsndfile-1.2.2/build/CMakeFiles/Export/5c71f72976042dd672d3a20ad1898c82/SndFileTargets.cmake")
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^()$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/SndFile" TYPE FILE FILES "C:/Users/aaron/Downloads/libsndfile-1.2.2/libsndfile-1.2.2/build/CMakeFiles/Export/5c71f72976042dd672d3a20ad1898c82/SndFileTargets-noconfig.cmake")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/SndFile" TYPE FILE RENAME "SndFileConfig.cmake" FILES "C:/Users/aaron/Downloads/libsndfile-1.2.2/libsndfile-1.2.2/build/SndFileConfig2.cmake")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/SndFile" TYPE FILE FILES "C:/Users/aaron/Downloads/libsndfile-1.2.2/libsndfile-1.2.2/build/SndFileConfigVersion.cmake")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/man/man1" TYPE FILE FILES
    "C:/Users/aaron/Downloads/libsndfile-1.2.2/libsndfile-1.2.2/man/sndfile-info.1"
    "C:/Users/aaron/Downloads/libsndfile-1.2.2/libsndfile-1.2.2/man/sndfile-play.1"
    "C:/Users/aaron/Downloads/libsndfile-1.2.2/libsndfile-1.2.2/man/sndfile-convert.1"
    "C:/Users/aaron/Downloads/libsndfile-1.2.2/libsndfile-1.2.2/man/sndfile-cmp.1"
    "C:/Users/aaron/Downloads/libsndfile-1.2.2/libsndfile-1.2.2/man/sndfile-metadata-get.1"
    "C:/Users/aaron/Downloads/libsndfile-1.2.2/libsndfile-1.2.2/man/sndfile-concat.1"
    "C:/Users/aaron/Downloads/libsndfile-1.2.2/libsndfile-1.2.2/man/sndfile-interleave.1"
    "C:/Users/aaron/Downloads/libsndfile-1.2.2/libsndfile-1.2.2/man/sndfile-salvage.1"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/man/man1" TYPE FILE RENAME "sndfile-metadata-set.1" FILES "C:/Users/aaron/Downloads/libsndfile-1.2.2/libsndfile-1.2.2/man/sndfile-metadata-get.1")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/man/man1" TYPE FILE RENAME "sndfile-deinterleave.1" FILES "C:/Users/aaron/Downloads/libsndfile-1.2.2/libsndfile-1.2.2/man/sndfile-interleave.1")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/doc/libsndfile" TYPE FILE FILES
    "C:/Users/aaron/Downloads/libsndfile-1.2.2/libsndfile-1.2.2/docs/index.md"
    "C:/Users/aaron/Downloads/libsndfile-1.2.2/libsndfile-1.2.2/docs/libsndfile.jpg"
    "C:/Users/aaron/Downloads/libsndfile-1.2.2/libsndfile-1.2.2/docs/libsndfile.css"
    "C:/Users/aaron/Downloads/libsndfile-1.2.2/libsndfile-1.2.2/docs/print.css"
    "C:/Users/aaron/Downloads/libsndfile-1.2.2/libsndfile-1.2.2/docs/api.md"
    "C:/Users/aaron/Downloads/libsndfile-1.2.2/libsndfile-1.2.2/docs/command.md"
    "C:/Users/aaron/Downloads/libsndfile-1.2.2/libsndfile-1.2.2/docs/bugs.md"
    "C:/Users/aaron/Downloads/libsndfile-1.2.2/libsndfile-1.2.2/docs/formats.md"
    "C:/Users/aaron/Downloads/libsndfile-1.2.2/libsndfile-1.2.2/docs/sndfile_info.md"
    "C:/Users/aaron/Downloads/libsndfile-1.2.2/libsndfile-1.2.2/docs/new_file_type_howto.md"
    "C:/Users/aaron/Downloads/libsndfile-1.2.2/libsndfile-1.2.2/docs/win32.md"
    "C:/Users/aaron/Downloads/libsndfile-1.2.2/libsndfile-1.2.2/docs/FAQ.md"
    "C:/Users/aaron/Downloads/libsndfile-1.2.2/libsndfile-1.2.2/docs/lists.md"
    "C:/Users/aaron/Downloads/libsndfile-1.2.2/libsndfile-1.2.2/docs/embedded_files.md"
    "C:/Users/aaron/Downloads/libsndfile-1.2.2/libsndfile-1.2.2/docs/octave.md"
    "C:/Users/aaron/Downloads/libsndfile-1.2.2/libsndfile-1.2.2/docs/tutorial.md"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/pkgconfig" TYPE FILE FILES "C:/Users/aaron/Downloads/libsndfile-1.2.2/libsndfile-1.2.2/build/sndfile.pc")
endif()

if(CMAKE_INSTALL_COMPONENT)
  if(CMAKE_INSTALL_COMPONENT MATCHES "^[a-zA-Z0-9_.+-]+$")
    set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
  else()
    string(MD5 CMAKE_INST_COMP_HASH "${CMAKE_INSTALL_COMPONENT}")
    set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INST_COMP_HASH}.txt")
    unset(CMAKE_INST_COMP_HASH)
  endif()
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
  file(WRITE "C:/Users/aaron/Downloads/libsndfile-1.2.2/libsndfile-1.2.2/build/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
endif()
