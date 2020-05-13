##################################################################################
#src CMAKELISTS
##################################################################################
# macro
MACRO(HEADER_DIRECTORIES return_list)
    FILE(GLOB_RECURSE new_list *.h)
    SET(dir_list "")
    FOREACH(file_path ${new_list})
        GET_FILENAME_COMPONENT(dir_path ${file_path} PATH)
        SET(dir_list ${dir_list} ${dir_path})
    ENDFOREACH()
    LIST(REMOVE_DUPLICATES dir_list)
    SET(${return_list} ${dir_list})
ENDMACRO()
HEADER_DIRECTORIES(header_dir_list)
#foreach(th ${header_dir_list})
#message(${th})
#endforeach(th ${header_dir_list})

# basics
cmake_minimum_required(VERSION 3.14)
project(Email_Server LANGUAGES CXX)

# C++ version
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_INCLUDE_CURRENT_DIR ON) # include build directory where the qt generated header exists
set(CMAKE_AUTOMOC ON)
SET(CMAKE_AUTOUIC ON)

# C compile
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D__VTableFILE__='\"$(subst ${CMAKE_SOURCE_DIR}/,,$(abspath $<))\"'")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D__ROOTPATH__='\"${CMAKE_SOURCE_DIR}/temp/\"'")

#### files  ##############################################################################
# src files
set(Qt5_DIR /usr/local/Cellar/qt/5.13.1/cmake/Qt5) # qt

set(MYSQL_DIR /usr/local/opt/mysql-client) # mysql
set(MYSQL_INCLUDE_DIR ${MYSQL_DIR}/include)
set(MYSQL_LIB_DIR ${MYSQL_DIR}/lib)

set(MYSQLPP_DIR /usr/local/Cellar/mysql++/3.2.5_1) # mysql++
set(MYSQLPP_INCLUDE_DIR ${MYSQLPP_DIR}/include)
set(MYSQLPP_LIB_DIR ${MYSQLPP_DIR}/lib)

file(GLOB cpp_files
        ${PROJECT_SOURCE_DIR}/src/main.cpp
        ${PROJECT_SOURCE_DIR}/src/*/*.cpp)
file(GLOB src_files
        ${PROJECT_SOURCE_DIR}/src/*/*.cpp)
file(GLOB ui_files
        ${PROJECT_SOURCE_DIR}/src/*/*.ui)
file(GLOB header_files
        ${PROJECT_SOURCE_DIR}/src/*/*.h)
list(FILTER header_files EXCLUDE REGEX ".*/include/.*.h$")
file(GLOB mysqlpp_files
        ${MYSQLPP_LIB_DIR}/*.a
        ${MYSQLPP_LIB_DIR}/*.dylib)
file(GLOB mysql_files
        ${MYSQL_LIB_DIR}/*.a
        ${MYSQL_LIB_DIR}/*.dylib)

# qt
find_package(Qt5 REQUIRED COMPONENTS Widgets Core Gui)
qt5_wrap_ui(header_wrapped_files ${ui_files}) # ui files are converted to header files at build time like XML in WPF
qt5_wrap_cpp(cpp_moc_files ${header_files}) # because header files are where Q_Object is defined


#### executables #############################################################################
# executables
add_executable(${PROJECT_NAME}  # ui files are not necessary here
        ${cpp_files} # xx.cpp
        ${header_wrapped_files} # ui_xx.h
        ${cpp_moc_files} # moc_xx.cpp
        )

# include
target_include_directories(${PROJECT_NAME} PUBLIC # PROJECT_NAME is case-sensitive
        ${MYSQLPP_INCLUDE_DIR}
        ${MYSQL_INCLUDE_DIR}
        ${header_dir_list}
        )

# links
target_link_libraries(${PROJECT_NAME} PUBLIC # PROJECT_NAME is case-sensitive
        Qt5::Core
        Qt5::Widgets
        Qt5::Gui
        ${mysql_files}
        ${mysqlpp_files}
        )

# all source file
add_library(email SHARED ${src_files})


##################################################################################
#TEST CMAKELISTS
##################################################################################

set(CTEST_FLAGS "")
add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND} ${CTEST_FLAGS} --verbose)


#### files  ##############################################################################
file(GLOB test_srcs_temp ${PROJECT_SOURCE_DIR}/test/*/*test.cpp)

set(test_srcs "")
foreach(test_src_temp ${test_srcs_temp} )
    string(REPLACE "//" "/" test_src ${test_src_temp})
    list(APPEND test_srcs ${test_src})
endforeach(test_src_temp ${test_srcs_temp})

# --[ Gmock
set(GMOCK_DIR  "${PROJECT_SOURCE_DIR}/third_party/gmock")
file(GLOB gmock_srcs  ${GMOCK_DIR}/*.cc)
include_directories(SYSTEM ${GMOCK_DIR})
add_library(gtest EXCLUDE_FROM_ALL ${gmock_srcs})
target_link_libraries(gtest ${CMAKE_THREAD_LIBS_INIT})

# --[ Memcheck
find_program(MEMORYCHECK_COMMAND valgrind REQUIRED)
# Note you can add '--gen-suppressions=all' to MEMORYCHECK_COMMAND_OPTIONS
# if you want valgrind to print out the syntax to use to suppress a particular
# memory leak
set(MEMORYCHECK_COMMAND_OPTIONS
        --trace-children=yes
        --leak-check=full
        --track-origins=yes
        --soname-synonyms=somalloc=*jemalloc*
        --error-exitcode=1)
set(MEMORYCHECK_SUPPRESSIONS_FILE ${PROJECT_SOURCE_DIR}/third_party/valgrind/valgrind.supp)


#### executables #############################################################################

foreach(test_src ${test_srcs} )
    # get test file name
    get_filename_component(test_bare_name ${test_src} NAME)
    string(REPLACE ".cpp" "" test_bare_name_without_extension ${test_bare_name})
    string(REPLACE "\"" "" test_name ${test_bare_name_without_extension})

    # create executable
    add_executable(${test_name}
            ${test_src}
            ${header_wrapped_files}
            ${cpp_moc_files}
            ${src_files}
            )
    add_dependencies(check
            ${test_name})

    target_include_directories(${test_name} PUBLIC
            ${MYSQLPP_INCLUDE_DIR}
            ${MYSQL_INCLUDE_DIR}
            ${header_dir_list}
            )

    # link libraries
    target_link_libraries(${test_name}
            Qt5::Core
            Qt5::Widgets
            Qt5::Gui
            ${mysql_files}
            ${mysqlpp_files}
#            ${src_files}
#            email
            gtest
            )



    # set target properties
    set_target_properties(${test_name} PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/test"
            COMMAND ${test_name}
            )

    # add test
    add_test(${test_name} ${MEMORYCHECK_COMMAND} ${MEMORYCHECK_COMMAND_OPTIONS}
            --suppressions=${MEMORYCHECK_SUPPRESSIONS_FILE} ${CMAKE_BINARY_DIR}/test/${test_name}
            --gtest_color=yes --gtest_output=xml:${CMAKE_BINARY_DIR}/test/unit_${test_name}.xml)
    add_test(${test_name} ${CMAKE_BINARY_DIR}/test/${test_name} --gtest_color=yes
            --gtest_output=xml:${CMAKE_BINARY_DIR}/test/${test_name}.xml)

endforeach(test_src ${test_srcs})

message(${CMAKE_SOURCE_DIR})
#foreach(test_src ${header_files})
#message(${test_src})
#endforeach(test_src ${header_files})