cmake_minimum_required(VERSION 3.1)
project(couchbase_python_client)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(THIRDPARTY_LCB_ROOT libcouchbase_src-prefix)
set(LCBCXX_ROOT libcouchbase-cxx-prefix/src/libcouchbase-cxx)
set(LCB_ROOT ${THIRDPARTY_LCB_ROOT}/src/libcouchbase_src)
set(PYCBC_CMAKE_CPYTHON_WRAPPER 1)
include(FindPythonInterp)
include(FindPythonLibs)
#set(Python_ADDITIONAL_VERSIONS 3.6)
#set(REQ_PYTHON 3.6)
if (PYTHON_LIBDIR)
    set(PYTHON_LIBDIR_HINT HINTS ${PYTHON_LIBDIR})
else(PYTHON_LIBDIR)
    set(PYTHON_LIBDIR_HINT "")
endif()

if (PYTHON_VERSION_EXACT)
    #find_package(PythonInterp ${PYTHON_VERSION_EXACT} EXACT REQUIRED)
    find_package(PythonLibs ${PYTHON_VERSION_EXACT} EXACT  REQUIRED)
else(PYTHON_VERSION_EXACT)
    find_package(PythonInterp REQUIRED)
    find_package(PythonLibs ${PYTHON_LIBDIR_HINT} REQUIRED)
endif()

#${REQ_PYTHON} REQUIRED)
cmake_policy(SET CMP0054 NEW)
if(WIN32)
    set(PYCBC_C_MOD_NAME "_libcouchbase.${PYTHONLIBS_VERSION_STRING}")
    set(PYCBC_C_MOD_SUFFIX ".pyd")
else()
    set(PYCBC_C_MOD_NAME "_libcouchbase")
    set(PYCBC_C_MOD_SUFFIX ".so")
endif()
if (CMAKE_BUILD_TYPE MATCHES DEBUG)
    set(RELEASE_TYPE Debug)
else()
    set(RELEASE_TYPE Release)
endif()
#set(HYBRID_BUILD TRUE)
if(HYBRID_BUILD)
set(PYCBC_C_MOD_NAME "_dummy")
endif()

macro(use_cxx11)
if (CMAKE_VERSION VERSION_LESS "3.1")
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")
endif ()
else ()
set (CMAKE_CXX_STANDARD 11)
endif ()
endmacro(use_cxx11)

set (LCB_CFLAGS ${CFLAGS} -fPIC)
set (LCB_CXXFLAGS ${CFLAGS} -fPIC)
if (WIN32)
else(WIN32)
#set (CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS}  -fPIC -fno-strict-aliasing -Wall -Wstrict-prototypes -pthread)
set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS}  -fPIC -fno-strict-aliasing -Wall -Wstrict-prototypes -pthread")
set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fno-common -dynamic -DNDEBUG -g -fwrapv")
endif()
#use_cxx11()

if ("$PYCBC_USE_CONAN")
    # Download automatically, you can also just copy the conan.cmake file
    include(${CMAKE_BINARY_DIR}/conan.cmake)
    if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
        message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
        file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/master/conan.cmake"
                "${CMAKE_BINARY_DIR}/conan.cmake")
    endif()
    conan_cmake_run(REQUIRES OpenSSL/1.0.2n@conan/stable
            BASIC_SETUP
            BUILD missing)
endif()

include(ExternalProject)

#set(PYCBC_CXX "true")
SET(CPP_DEPS "")
if ("${PYCBC_CXX}")
    set(CPP_DEPS libcouchbase-cxx)
    #binder)
    ExternalProject_Add(libcouchbase-cxx
            GIT_REPOSITORY https://github.com/griels/libcouchbase-cxx.git
            GIT_TAG master
            )
    #    ExternalProject_Add(binder
    #            GIT_REPOSITORY https://github.com/RosettaCommons/binder.git
    #    )
    #add_subdirectory(../cpycppyy/ ${CMAKE_CURRENT_BINARY_DIR}/cpycppyy)
    #add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/libcouchbase-cxx-prefix/src/libcouchbase-cxx)

endif()

set(LCB_CMAKE_ARGS -DLCB_NO_TESTS=1)
set(CMAKE_MACOSX_RPATH 1)
set(LCB_REPO_PROTOCOL http)
if (LCB_REPO_USERNAME)
    set(LCB_REPO_AT "@")
else(LCB_REPO_USERNAME)
    set(LCB_REPO_AT "")
endif()

include(cmake/json-cmake/JSONParser.cmake)
file(READ cbuild_cfg.json BUILDCFG)
sbeParseJson(build_cfg BUILDCFG)


if (PYCBC_LCB_API)
else()
    message("got build_cfg ${build_cfg}")
    set (PYCBC_LCB_API ${build_cfg.comp_options.PYCBC_LCB_API})
endif()
message("got PYCBC_LCB_API... ${PYCBC_LCB_API}")

if (NOT PYCBC_LCB_API)
    set(PYCBC_LCB_API 0x030000)
    message("empty PYCBC_LCB_API, reverting to ${PYCBC_LCB_API}")
endif()
if ((PYCBC_LCB_API GREATER 0x032000) OR(PYCBC_LCB_API EQUAL 0x032000))
    set(LCB_COLLECTIONS_REF refs/changes/22/104322/43)
else()
    if ((PYCBC_LCB_API GREATER 0x031000) OR(PYCBC_LCB_API EQUAL 0x031000))
        set(LCB_COLLECTIONS_REF refs/changes/22/104322/35)
    else()
        if ((PYCBC_LCB_API GREATER 0x030001) OR(PYCBC_LCB_API EQUAL 0x030001))
            set(LCB_COLLECTIONS_REF refs/changes/22/104322/24)
            #set(LCB_COLLECTIONS_REF refs/changes/22/104322/22)
        else()
            #    set(LCB_COLLECTIONS_REF refs/changes/05/104205/2)
            set(LCB_COLLECTIONS_REF refs/changes/84/102984/11)
            #set(LCB_COLLECTIONS_REF refs/changes/81/100581/15)
        endif()
    endif()
endif()
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DPYCBC_LCB_API=${PYCBC_LCB_API}")
#set(LCB_COLLECTIONS_TAG 48d1ba0010e5b408ddcf54994c41c766a607f4d7)
set(LCB_REPO ${LCB_REPO_PROTOCOL}://${LCB_REPO_AT}${LCB_REPO_USERNAME}review.couchbase.org/libcouchbase)
#set(LCB_TAG ${LCB_COLLECTIONS_TAG})
function(make_dir path)
    file(MAKE_DIRECTORY ${path})
endfunction(make_dir)

if (MAKE)
else()
set(MAKE make)
endif()

set(LCB_SRC TRUE)
if (LCB_SRC)
    SET(LCB_BUILD_DIR "${CMAKE_BINARY_DIR}/Release/lcb_build")
    SET(LCB_DBGBUILD_DIR "${CMAKE_BINARY_DIR}/Release/lcb_dbgbuild")
    SET(LIBCOUCHBASE_LIBRARY_NAME "libcouchbase.lib")
    SET(LIBCOUCHBASE_DBG_LIBRARY_NAME "libcouchbase_d.lib")
    SET(LIBCOUCHBASE_STATIC_LIBRARY_NAME "libcouchbase_d.lib")
    SET(LCB_CMAKE_ARGS -DLCB_NO_TOOLS=1 -DLCB_NO_TESTS=1)
#-DLCB_BUILD_EXAMPLES=1)
    if (WIN32)
    set(LCB_CMAKE_ARGS ${LCB_CMAKE_ARGS} -DLCB_NO_SSL=1 -DLCB_NO_MOCK=1)
    ExternalProject_Add(libcouchbase_src
            SET(LIBCOUCHBASE_LIBRARY_FILE "${LCB_BUILD_DIR}/lib/Release/${LIBCOUCHBASE_LIBRARY_NAME}")
            SET(LIBCOUCHBASE_DBG_LIBRARY_FILE "${LCB_DBGBUILD_DIR}/lib/Debug/${LIBCOUCHBASE_DBG_LIBRARY_NAME}")
            #PREFIX thirdparty/libcouchbase
            GIT_REPOSITORY ${LCB_REPO}
            GIT_TAG ${LCB_TAG}
            CONFIGURE_COMMAND ${CMAKE_COMMAND} -E make_directory ${LCB_BUILD_DIR}
            COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_CURRENT_BINARY_DIR}/install
            COMMAND git reset --hard
            COMMAND git fetch http://review.couchbase.org/libcouchbase ${LCB_COLLECTIONS_REF}

            COMMAND cd ${LCB_BUILD_DIR} && cmake -DLCB_NO_PLUGINS=1 <SOURCE_DIR> -G "Visual Studio 14 2015 Win64" ${LCB_CMAKE_ARGS}

            BUILD_COMMAND cd ${LCB_BUILD_DIR}/Release/Release && cmake --build <SOURCE_DIR> --config Release

            INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR}/install
            INSTALL_COMMAND ${CMAKE_COMMAND} -E copy_directory <SOURCE_DIR>/include <INSTALL_DIR>/include
            COMMAND ${CMAKE_COMMAND} -E copy_directory ${LCB_BUILD_DIR}/generated <INSTALL_DIR>/include
            COMMAND ${CMAKE_COMMAND} -E make_directory <INSTALL_DIR>/bin/
            COMMAND ${CMAKE_COMMAND} -E make_directory <INSTALL_DIR>/archive/
            COMMAND ${CMAKE_COMMAND} -E make_directory <INSTALL_DIR>/bin/Release
            COMMAND ${CMAKE_COMMAND} -E copy ${LCB_BUILD_DIR}/bin/Release/libcouchbase.dll <INSTALL_DIR>/bin/Release/
            COMMAND ${CMAKE_COMMAND} -E make_directory <INSTALL_DIR>/archive/Release
            COMMAND ${CMAKE_COMMAND} -E copy ${LCB_BUILD_DIR}/lib/Release/libcouchbase.lib <INSTALL_DIR>/archive/Release/
            COMMAND ${CMAKE_COMMAND} -E copy ${LCB_BUILD_DIR}/lib/Release/libcouchbase.exp <INSTALL_DIR>/archive/Release/

            COMMAND ${CMAKE_COMMAND} -E make_directory ${LCB_DBGBUILD_DIR}
            COMMAND cd ${LCB_DBGBUILD_DIR} && cmake <SOURCE_DIR> -DLCB_NO_PLUGINS=1 ${LCB_CMAKE_ARGS}

            COMMAND cd ${LCB_DBGBUILD_DIR} && cmake --build <SOURCE_DIR> --config Debug
            COMMAND ${CMAKE_COMMAND} -E copy_directory ${LCB_DBGBUILD_DIR}/generated <INSTALL_DIR>/include

            COMMAND ${CMAKE_COMMAND} -E make_directory <INSTALL_DIR>/bin/Debug
            COMMAND ${CMAKE_COMMAND} -E copy ${LCB_DBGBUILD_DIR}/bin/Debug/libcouchbase_d.dll <INSTALL_DIR>/bin/Debug/
            COMMAND ${CMAKE_COMMAND} -E make_directory <INSTALL_DIR>/archive/Debug
            COMMAND ${CMAKE_COMMAND} -E copy ${LCB_DBGBUILD_DIR}/lib/Debug/libcouchbase_d.lib <INSTALL_DIR>/archive/Debug/
            COMMAND ${CMAKE_COMMAND} -E copy ${LCB_DBGBUILD_DIR}/lib/Debug/libcouchbase_d.exp <INSTALL_DIR>/archive/Debug/


            #COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists_package.txt <INSTALL_DIR>/CMakeLists.txt
            )


else (WIN32)
    if (APPLE)
        SET(LIBCOUCHBASE_LIBRARY_NAME "libcouchbase.dylib")
        SET(LIBCOUCHBASE_DBG_LIBRARY_NAME "libcouchbase.dylib")
        SET(LIBCOUCHBASE_STATIC_LIBRARY_NAME "libcouchbaseS.a")

    else (APPLE)
        SET(LIBCOUCHBASE_LIBRARY_NAME "libcouchbase.so")
        SET(LIBCOUCHBASE_DBG_LIBRARY_NAME "libcouchbase.so")
    endif (APPLE)

    set(LCB_CMAKE_ARGS ${LCB_CMAKE_ARGS} -DLCB_NO_MOCK=1 -DLCB_NO_PLUGINS=1 -DLCB_BUILD_DTRACE=OFF)
    SET(_parallelism 4)
    if (LCB_COLLECTIONS_REF)
        set (REF_CMDS git fetch http://review.couchbase.org/libcouchbase ${LCB_COLLECTIONS_REF}
                && git reset --hard
                && git clean -f
                && git checkout FETCH_HEAD)
            endif()
    ExternalProject_Add(libcouchbase_src
            #PREFIX thirdparty/libcouchbase
            GIT_REPOSITORY ${LCB_REPO}
            GIT_TAG ${LCB_TAG}
            UPDATE_COMMAND ""
            CONFIGURE_COMMAND ${CMAKE_COMMAND} -E make_directory "${LCB_BUILD_DIR}"
            make_dir ${LCB_DBGBUILD_DIR}

            COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_CURRENT_BINARY_DIR}/install

            COMMAND ${REF_CMDS}
            #COMMAND git fetch http://review.couchbase.org/libcouchbase ${LCB_COLLECTIONS_REF}
            #COMMAND git reset --hard
            #COMMAND git clean -f
            #COMMAND git checkout FETCH_HEAD
            #COMMAND cd ${LCB_BUILD_DIR} && cmake <SOURCE_DIR> -DLCB_NO_MOCK=1 -DLCB_NO_PLUGINS=1 -DLCB_BUILD_DTRACE=OFF -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>/Release ${LCB_CMAKE_ARGS}
            COMMAND cd ${LCB_DBGBUILD_DIR} && cmake -E env CXXFLAGS=${LCB_CXXFLAGS}  env CFLAGS=${LCB_CFLAGS} cmake <SOURCE_DIR> ${LCB_CMAKE_ARGS}  -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>/Debug

            BUILD_IN_SOURCE 1
            BUILD_COMMAND ${MAKE} -j${_parallelism} all install -C ${LCB_DBGBUILD_DIR}
            #COMMAND ${MAKE} -j${_parallelism} all install -C ${LCB_BUILD_DIR}

            INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR}/install
            INSTALL_COMMAND ${CMAKE_COMMAND} -E rename <INSTALL_DIR>/Debug/include <INSTALL_DIR>/include
            COMMAND ${CMAKE_COMMAND} -E make_directory <INSTALL_DIR>/lib
            #COMMAND ${CMAKE_COMMAND} -E rename <INSTALL_DIR>/Release/lib <INSTALL_DIR>/lib/Release
            COMMAND ${CMAKE_COMMAND} -E rename <INSTALL_DIR>/Debug/lib <INSTALL_DIR>/lib/Debug
            COMMAND ${CMAKE_COMMAND} -E remove_directory <INSTALL_DIR>/Debug
            #COMMAND ${CMAKE_COMMAND} -E remove_directory <INSTALL_DIR>/Release
            #COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists_package.txt <INSTALL_DIR>/CMakeLists.txt
            )
    SET(LIBCOUCHBASE_LIBRARY_FILE "${LCB_BUILD_DIR}/lib/${LIBCOUCHBASE_LIBRARY_NAME}")
    SET(LIBCOUCHBASE_DBG_LIBRARY_FILE "${LCB_DBGBUILD_DIR}/lib/${LIBCOUCHBASE_DBG_LIBRARY_NAME}")

    # OS X-only: Custom post-build step to set the shared library install name.
    if (APPLE)
        ExternalProject_Add_Step(libcouchbase_src install_name
                #COMMAND install_name_tool -id @rpath/libcouchbase.dylib ${LIBCOUCHBASE_LIBRARY_FILE}
                COMMAND install_name_tool -id @rpath/libcouchbase.dylib ${LIBCOUCHBASE_DBG_LIBRARY_FILE}
                COMMAND install_name_tool -id @rpath/libcouchbase.2.dylib ${LCB_DBGBUILD_DIR}/lib/libcouchbase.2.dylib
                DEPENDEES build
                DEPENDERS install
                )
    endif (APPLE)
endif (WIN32)
endif (LCB_SRC)
set(LCBCXX_ROOT_PATH ${CMAKE_BINARY_DIR}/${LCBCXX_ROOT})



set(libcouchbase_src ${CMAKE_BINARY_DIR}/${LCB_ROOT})
set(install_dir ${CMAKE_BINARY_DIR}/install)
if (LCB_SRC)
    ExternalProject_Get_Property(libcouchbase_src install_dir)
    ExternalProject_Get_Property(libcouchbase_src source_dir)
    set(CPP_DEPS ${CPP_DEPS} libcouchbase_src)
else (LCB_SRC)
#    find_library(libcouchbase_src libcouchbase)
endif(LCB_SRC)

if("${PYCBC_ADD_INLINE}")
    add_subdirectory(${libcouchbase_src})
    if (PYCBC_CXX)
        add_subdirectory(${CMAKE_BINARY_DIR}/${LCBCXX_ROOT})
    endif()
endif()

cmake_policy(SET CMP0015 NEW)

if ("${PYCBC_CPPYY}")
    find_package(Cppyy REQUIRED)
    cppyy_add_bindings(
            "Stuff" "0" "Enrico" "enrico.guiraud@cern.ch"
            LANGUAGE_STANDARD "11"
            H_FILES "couchbase.h")
endif()
if(USE_BOOST)
    find_package(Boost COMPONENTS python27 REQUIRED)
    set(BOOST_SOURCES
            /usr/local/include/boost/python/module.hpp
            src/bindings.cpp)
else()
    set(Boost_LIBRARIES )
    set(Boost_INCLUDE_DIR )
    set(BOOST_SOURCES )
endif(USE_BOOST)
include_directories(
        ${PYTHON_INCLUDE_DIRS}
        ${PYTHON_INCLUDE_PATH}
        #${Boost_INCLUDE_DIR}
        /usr/local/include
        ${LCBCXX_ROOT_PATH}/include
        ${CMAKE_BINARY_DIR}/${install_dir}/include
        ${install_dir}/include
        ${source_dir} ${source_dir}/src/*
        ${source_dir}
        #cmake-build-release/install/include
)

#aux_source_directory(LIBCOUCHBASE ${CMAKE_BINARY_DIR}/${source_dir})
set(LIBCOUCHBASE_DBG_DIRPATH ${install_dir}/lib/Debug)
#set(LCB_STATIC TRUE)
if (LCB_STATIC)
    set(LIBCOUCHBASE_DBG_PATH ${LCB_DBGBUILD_DIR}/lib/${LIBCOUCHBASE_STATIC_LIBRARY_NAME})
else()
    set(LIBCOUCHBASE_DBG_PATH ${LIBCOUCHBASE_DBG_DIRPATH}/${LIBCOUCHBASE_DBG_LIBRARY_NAME})
endif()

set(PYCBC_SRC )
if(HYBRID_BUILD)
    set(ENABLE_HYBRID_BUILD)
endif()
if(ENABLE_HYBRID_BUILD)
    else()
    foreach(var ${build_cfg})
        message("${var} = ${${var}}")
        if ("${${var}}" MATCHES ".*\\.c(xx|pp|c)?")
            message("${var} = ${${var}}")
            set(PYCBC_SRC ${PYCBC_SRC} ${${var}})
        endif()
    endforeach()
endif()
add_library(couchbase_python_client SHARED
        acouchbase/tests/asyncio_tests.py
        acouchbase/tests/fixtures.py
        acouchbase/tests/py34only.py
        acouchbase/tests/py35only.py
        acouchbase/__init__.py
        acouchbase/asyncio_iops.py
        acouchbase/bucket.py
        acouchbase/py34only/iterator.py
        couchbase/asynchronous/__init__.py
        couchbase/asynchronous/bucket.py
        couchbase/asynchronous/events.py
        couchbase/asynchronous/n1ql.py
        couchbase/asynchronous/rowsbase.py
        couchbase/asynchronous/view.py
        couchbase/iops/__init__.py
        couchbase/iops/base.py
        couchbase/iops/select.py
        couchbase/tests/admin/__init__.py
        couchbase/tests/cases/__init__.py
        couchbase/tests/cases/admin_t.py
        couchbase/tests/cases/append_t.py
        couchbase/tests/cases/arithmetic_t.py
        couchbase/tests/cases/badargs_t.py
        couchbase/tests/cases/cbftstrings_t.py
        couchbase/tests/cases/cluster_t.py
        couchbase/tests/cases/connection_t.py
        couchbase/tests/cases/connstr_t.py
        couchbase/tests/cases/datastructures_t.py
        couchbase/tests/cases/delete_t.py
        couchbase/tests/cases/design_t.py
        couchbase/tests/cases/dupkeys_t.py
        couchbase/tests/cases/empty_key_t.py
        couchbase/tests/cases/encodings_t.py
        couchbase/tests/cases/endure_t.py
        couchbase/tests/cases/enh_err_t.py
        couchbase/tests/cases/excextra_t.py
        couchbase/tests/cases/flush_t.py
        couchbase/tests/cases/format_t.py
        couchbase/tests/cases/get_t.py
        couchbase/tests/cases/diag_t.py
        couchbase/tests/cases/iops_t.py
        couchbase/tests/cases/itertypes_t.py
        couchbase/tests/cases/itmops_t.py
        couchbase/tests/cases/ixmgmt_t.py
        couchbase/tests/cases/lock_t.py
        couchbase/tests/cases/lockmode_t.py
        couchbase/tests/cases/misc_t.py
        couchbase/tests/cases/mutationtokens_t.py
        couchbase/tests/cases/n1ql_t.py
        couchbase/tests/cases/n1qlstrings_t.py
        couchbase/tests/cases/observe_t.py
        couchbase/tests/cases/pipeline_t.py
        couchbase/tests/cases/results_t.py
        couchbase/tests/cases/rget_t.py
        couchbase/tests/cases/set_converters_t.py
        couchbase/tests/cases/set_t.py
        couchbase/tests/cases/spatial_t.py
        couchbase/tests/cases/stats_t.py
        couchbase/tests/cases/subdoc_t.py
        couchbase/tests/cases/touch_t.py
        couchbase/tests/cases/transcoder_t.py
        couchbase/tests/cases/verinfo_t.py
        couchbase/tests/cases/view_iterator_t.py
        couchbase/tests/cases/view_t.py
        couchbase/tests/cases/viewstrings_t.py
        couchbase/tests/cases/xattr_t.py
        couchbase/tests/__init__.py
        couchbase/tests/base.py
        couchbase/tests/importer.py
        couchbase/tests/test_sync.py
        couchbase/views/__init__.py
        couchbase/views/iterator.py
        couchbase/views/params.py
        couchbase/__init__.py
        couchbase/_bootstrap.py
        couchbase/_ixmgmt.py
        couchbase/_logutil.py
        couchbase/_pyport.py
        #couchbase/_version.py
        couchbase/admin.py
        couchbase/auth_domain.py
        couchbase/bucket.py
        couchbase/bucketmanager.py
        couchbase/analytics.py
        couchbase/cluster.py
        couchbase/connection.py
        couchbase/connstr.py
        couchbase/exceptions.py
        couchbase/experimental.py
        couchbase/fulltext.py
        couchbase/items.py
        couchbase/mockserver.py
        couchbase/mutation_state.py
        couchbase/n1ql.py
        couchbase/priv_constants.py
        couchbase/result.py
        couchbase/subdocument.py
        couchbase/transcoder.py
        couchbase/user_constants.py
        examples/abench.py
        examples/basic.py
        examples/bench.py
        examples/connection-pool.py
        examples/docloader.py
        examples/gbench.py
        examples/iops_demo.py
        examples/item.py
        examples/reversed_keys.py
        examples/search_keywords.py
        examples/twist-sample.py
        examples/txbasic.py
        examples/txbench.py
        examples/txview.py
        gcouchbase/tests/__init__.py
        gcouchbase/tests/test_api.py
        gcouchbase/tests/test_gevent.py
        gcouchbase/__init__.py
        gcouchbase/bucket.py
        gcouchbase/connection.py
        gcouchbase/iops_gevent0x.py
        gcouchbase/iops_gevent10.py
        txcouchbase/tests/__init__.py
        txcouchbase/tests/base.py
        txcouchbase/tests/test_n1ql.py
        txcouchbase/tests/test_ops.py
        txcouchbase/tests/test_txconn.py
        txcouchbase/tests/test_views.py
        txcouchbase/__init__.py
        txcouchbase/bucket.py
        txcouchbase/connection.py
        txcouchbase/iops.py
        couchbase_version.py
        setup.py

        ${PYCBC_SRC}
        ${BOOST_SOURCES}

        )
if(CMAKE_BUILD_TYPE MATCHES Debug)
#    link_libraries(${LIBCOUCHBASE_DBG_PATH} ${PYTHONLIB})
else()
#    link_libraries(${LIBCOUCHBASE_DBG_PATH} ${PYTHONLIB})
endif()
if (PYTHON_LIBDIR)
else()
    set(PYTHON_LIBDIR ${PYTHON_LIBRARIES})
endif()
link_directories(${CMAKE_BINARY_DIR}/${install}/lib/${RELEASE_TYPE} ${PYTHON_LIBDIR})
add_dependencies(couchbase_python_client ${CPP_DEPS})
target_include_directories(couchbase_python_client PUBLIC  ${install_dir}/include)

#if( ${_dynamic_lookup} )
#    message( STATUS "Not linking target ${_name} against libpython" )
#        set(PROPS LINK_FLAGS "-undefined dynamic_lookup") #-dynamic
#endif()

#target_link_directories(couchbase_python_client PUBLIC ${PYTHON_LIBDIR})
if (PYTHON_LIBFILE)
else()
    set(PYTHON_LIBFILE python)
endif()

# required rpaths
# OSX: loader_path
# Linux: $ORIGIN

set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
target_link_libraries(couchbase_python_client ${Boost_LIBRARIES}   ${LIBCOUCHBASE_DBG_PATH}  ${PYTHON_LIBRARIES} -Wl,-rpath,${LIBCOUCHBASE_DBG_DIRPATH})
#${PYTHON_LIBFILE})
set_target_properties(
        couchbase_python_client
        PROPERTIES COMPILE_FLAGS "${CMAKE_C_FLAGS}"
        INSTALL_RPATH "${LIBCOUCHBASE_DBG_DIRPATH};${INSTALL_RPATH}"
        PREFIX ""
        OUTPUT_NAME ${PYCBC_C_MOD_NAME}
        LINKER_LANGUAGE C
        SUFFIX ${PYCBC_C_MOD_SUFFIX}
)

