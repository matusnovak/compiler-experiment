cmake_minimum_required(VERSION 3.0)
project (CompilerExperiment)

# Check for C++11
set (CMAKE_CXX_STANDARD 11)

set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Select build type")
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "RelWithDebInfo" "MinSizeRel")

# Extra include paths for apple
if(APPLE)
  include_directories(/usr/local/include)
  link_directories(/usr/local/lib)
endif()

if(UNIX OR APPLE)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-rpath -Wl,$ORIGIN")
endif()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

FILE(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp)
FILE(GLOB HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)

# Add our library
add_library(${PROJECT_NAME}_static STATIC ${SOURCES} ${HEADERS})
set_property(TARGET ${PROJECT_NAME}_static PROPERTY FOLDER "lib") 
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Tests
enable_testing()
FILE(GLOB TESTS_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp)
FILE(GLOB TESTS_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/tests/*.h)
add_executable(Tests ${TESTS_SOURCES} ${TESTS_HEADERS})

set(TESTS Tests)

# Configure tests
foreach(tests ${TESTS})
  target_link_libraries(${tests} ${PROJECT_NAME}_static)

  if(MSVC)
    set_target_properties(${tests} PROPERTIES LINK_FLAGS "/SUBSYSTEM:CONSOLE")
  endif(MSVC)
  
  if(NOT MSVC AND NOT APPLE)
    target_link_libraries(${tests} Threads::Threads)
  endif()
  
  # Compile with static stdlib ?
  if(STATIC_STDLIB)
    target_link_libraries(${tests} -static-libgcc -static-libstdc++)
  endif(STATIC_STDLIB)
  
  add_test(NAME ${tests} COMMAND ${tests})
  set_property(TARGET ${tests} PROPERTY FOLDER "tests") 
endforeach(tests)
