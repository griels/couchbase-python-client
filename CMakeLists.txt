cmake_minimum_required(VERSION 3.8)
project(couchbase_python_client)
set(THIRDPARTY_LCB_ROOT libcouchbase-prefix)
set(LCBCXX_ROOT libcouchbase-cxx-prefix/src/libcouchbase-cxx)
set(LCB_ROOT ${THIRDPARTY_LCB_ROOT}/src/libcouchbase)
set(PYCBC_CMAKE_CPYTHON_WRAPPER 1)
include(FindPythonLibs)
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

macro(use_cxx11)
if (CMAKE_VERSION VERSION_LESS "3.1")
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")
endif ()
else ()
set (CMAKE_CXX_STANDARD 11)
endif ()
endmacro(use_cxx11)

use_cxx11()

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

#set(LCB_COLLECTIONS_REF refs/changes/81/100581/15)
set(LCB_COLLECTIONS_REF refs/changes/84/102984/11)
set(LCB_COLLECTIONS_TAG 48d1ba0010e5b408ddcf54994c41c766a607f4d7)
set(LCB_REPO ${LCB_REPO_PROTOCOL}://${LCB_REPO_AT}${LCB_REPO_USERNAME}review.couchbase.org/libcouchbase)
#set(LCB_TAG ${LCB_COLLECTIONS_TAG})
function(make_dir path)
    file(MAKE_DIRECTORY ${path})
endfunction(make_dir)

set(LCB_SRC TRUE)
if (LCB_SRC)
if (WIN32)
    set(LCB_CMAKE_ARGS -DLCB_NO_SSL=1 -DLCB_NO_MOCK=1 )
    ExternalProject_Add(libcouchbase_src
            SET(LIBCOUCHBASE_LIBRARY_NAME "libcouchbase.lib")
            SET(LIBCOUCHBASE_DBG_LIBRARY_NAME "libcouchbase_d.lib")
            SET(LIBCOUCHBASE_LIBRARY_FILE "<SOURCE_DIR>/build/lib/Release/${LIBCOUCHBASE_LIBRARY_NAME}")
            SET(LIBCOUCHBASE_DBG_LIBRARY_FILE "<SOURCE_DIR>/dbgbuild/lib/Debug/${LIBCOUCHBASE_DBG_LIBRARY_NAME}")

            #PREFIX thirdparty/libcouchbase
            GIT_REPOSITORY ${LCB_REPO}
            GIT_TAG ${LCB_TAG}
            CONFIGURE_COMMAND ${CMAKE_COMMAND} -E make_directory <SOURCE_DIR>/build
            COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_CURRENT_BINARY_DIR}/install
            COMMAND git reset --hard
            COMMAND git fetch http://review.couchbase.org/libcouchbase ${LCB_COLLECTIONS_REF}

            COMMAND cd <SOURCE_DIR>/build && cmake -DLCB_NO_PLUGINS=1 .. -G "Visual Studio 14 2015 Win64" ${LCB_CMAKE_ARGS}

            BUILD_COMMAND cd <SOURCE_DIR>/build && cmake --build . --config Release ${LCB_CMAKE_ARGS}

            INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR}/install
            INSTALL_COMMAND ${CMAKE_COMMAND} -E copy_directory <SOURCE_DIR>/include <INSTALL_DIR>/include
            COMMAND ${CMAKE_COMMAND} -E copy_directory <SOURCE_DIR>/build/generated <INSTALL_DIR>/include
            COMMAND ${CMAKE_COMMAND} -E make_directory <INSTALL_DIR>/bin/
            COMMAND ${CMAKE_COMMAND} -E make_directory <INSTALL_DIR>/archive/
            COMMAND ${CMAKE_COMMAND} -E make_directory <INSTALL_DIR>/bin/Release
            COMMAND ${CMAKE_COMMAND} -E copy <SOURCE_DIR>/build/bin/Release/libcouchbase.dll <INSTALL_DIR>/bin/Release/
            COMMAND ${CMAKE_COMMAND} -E make_directory <INSTALL_DIR>/archive/Release
            COMMAND ${CMAKE_COMMAND} -E copy <SOURCE_DIR>/build/lib/Release/libcouchbase.lib <INSTALL_DIR>/archive/Release/
            COMMAND ${CMAKE_COMMAND} -E copy <SOURCE_DIR>/build/lib/Release/libcouchbase.exp <INSTALL_DIR>/archive/Release/

            COMMAND ${CMAKE_COMMAND} -E make_directory <SOURCE_DIR>/dbgbuild
            COMMAND cd <SOURCE_DIR>/dbgbuild && cmake .. -DLCB_NO_PLUGINS=1 ${LCB_CMAKE_ARGS}

            COMMAND cd <SOURCE_DIR>/dbgbuild && cmake --build . --config Debug ${LCB_CMAKE_ARGS}

            COMMAND ${CMAKE_COMMAND} -E make_directory <INSTALL_DIR>/bin/Debug
            COMMAND ${CMAKE_COMMAND} -E copy <SOURCE_DIR>/dbgbuild/bin/Debug/libcouchbase_d.dll <INSTALL_DIR>/bin/Debug/
            COMMAND ${CMAKE_COMMAND} -E make_directory <INSTALL_DIR>/archive/Debug
            COMMAND ${CMAKE_COMMAND} -E copy <SOURCE_DIR>/dbgbuild/lib/Debug/libcouchbase_d.lib <INSTALL_DIR>/archive/Debug/
            COMMAND ${CMAKE_COMMAND} -E copy <SOURCE_DIR>/dbgbuild/lib/Debug/libcouchbase_d.exp <INSTALL_DIR>/archive/Debug/


            #COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists_package.txt <INSTALL_DIR>/CMakeLists.txt
            )


else (WIN32)
    if (APPLE)
        SET(LIBCOUCHBASE_LIBRARY_NAME "libcouchbase.dylib")
        SET(LIBCOUCHBASE_DBG_LIBRARY_NAME "libcouchbase.dylib")

    else (APPLE)
        SET(LIBCOUCHBASE_LIBRARY_NAME "libcouchbase.so")
        SET(LIBCOUCHBASE_DBG_LIBRARY_NAME "libcouchbase.so")
    endif (APPLE)
    SET(LIBCOUCHBASE_LIBRARY_FILE "<SOURCE_DIR>/build/lib/${LIBCOUCHBASE_LIBRARY_NAME}")
    SET(LIBCOUCHBASE_DBG_LIBRARY_FILE "<SOURCE_DIR>/dbgbuild/lib/${LIBCOUCHBASE_DBG_LIBRARY_NAME}")


    SET(_parallelism 4)
    ExternalProject_Add(libcouchbase_src
            #PREFIX thirdparty/libcouchbase
            GIT_REPOSITORY ${LCB_REPO}
            GIT_TAG ${LCB_TAG}
            UPDATE_COMMAND ""
            CONFIGURE_COMMAND ${CMAKE_COMMAND} -E make_directory <SOURCE_DIR>/build
            make_dir <SOURCE_DIR>/dbgbuild
            COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_CURRENT_BINARY_DIR}/install
            COMMAND git fetch http://review.couchbase.org/libcouchbase ${LCB_COLLECTIONS_REF}
            COMMAND git reset --hard
            COMMAND git clean -f
            COMMAND git checkout FETCH_HEAD
            COMMAND cd <SOURCE_DIR>/build && cmake .. -DLCB_NO_MOCK=1 -DLCB_NO_PLUGINS=1 -DLCB_BUILD_DTRACE=OFF -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>/Release ${LCB_CMAKE_ARGS}
            COMMAND cd <SOURCE_DIR>/dbgbuild && cmake .. -DLCB_NO_MOCK=1 -DLCB_NO_PLUGINS=1 -DLCB_BUILD_DTRACE=OFF -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>/Debug ${LCB_CMAKE_ARGS}

            BUILD_IN_SOURCE 1
            BUILD_COMMAND $(MAKE) -j${_parallelism} all install -C build
            COMMAND $(MAKE) -j${_parallelism} all install -C dbgbuild

            INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR}/install
            INSTALL_COMMAND ${CMAKE_COMMAND} -E rename <INSTALL_DIR>/Release/include <INSTALL_DIR>/include
            COMMAND ${CMAKE_COMMAND} -E make_directory <INSTALL_DIR>/lib
            COMMAND ${CMAKE_COMMAND} -E rename <INSTALL_DIR>/Release/lib <INSTALL_DIR>/lib/Release
            COMMAND ${CMAKE_COMMAND} -E rename <INSTALL_DIR>/Debug/lib <INSTALL_DIR>/lib/Debug
            COMMAND ${CMAKE_COMMAND} -E remove_directory <INSTALL_DIR>/Debug
            COMMAND ${CMAKE_COMMAND} -E remove_directory <INSTALL_DIR>/Release
            #COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists_package.txt <INSTALL_DIR>/CMakeLists.txt
            )

    # OS X-only: Custom post-build step to set the shared library install name.
    if (APPLE)
        ExternalProject_Add_Step(libcouchbase_src install_name
                COMMAND install_name_tool -id @rpath/libcouchbase.dylib ${LIBCOUCHBASE_LIBRARY_FILE}
                COMMAND install_name_tool -id @rpath/libcouchbase.dylib ${LIBCOUCHBASE_DBG_LIBRARY_FILE}
                DEPENDEES build
                DEPENDERS install
                )
    endif (APPLE)
endif (WIN32)
endif (LCB_SRC)
#set(LCBCXX_ROOT_PATH ${CMAKE_BINARY_DIR}/${LCBCXX_ROOT})
set(LCBCXX_ROOT_PATH /Users/ellis_breen/root/workspaces/couchbase/couchbase-python-client-mainline/cmake-build-release/libcouchbase-cxx-prefix/src/libcouchbase-cxx)
if("${PYCBC_ADD_INLINE}")
    #add_subdirectory(${CMAKE_BINARY_DIR}/${LCB_ROOT})
    #add_subdirectory(${CMAKE_BINARY_DIR}/${LCBCXX_ROOT})
endif()



set(LCB_INSTALL /Users/ellis_breen/root/workspaces/couchbase/couchbase-python-client-mainline/cmake-build-release/install/lib/Debug/libcouchbase.dylib)
if (LCB_SRC)
    ExternalProject_Get_Property(libcouchbase_src install_dir)
    ExternalProject_Get_Property(libcouchbase_src source_dir)
    set(CPP_DEPS ${CPP_DEPS} libcouchbase_src)
else (LCB_SRC)
    find_library(LCB_ROOT_FND libcouchbase)
endif(LCB_SRC)


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
        #${Boost_INCLUDE_DIR}
        /usr/local/include
        ${LCBCXX_ROOT_PATH}/include
        ${CMAKE_BINARY_DIR}/${install_dir}/include
        ${install_dir}/include
        ${source_dir} ${source_dir}/src/*
        ${source_dir}
        ${PYTHON_INCLUDE_DIRS}
        ${PYTHON_INCLUDE_PATH}
        cmake-build-release/install/include
)

aux_source_directory(LIBCOUCHBASE ${CMAKE_BINARY_DIR}/${source_dir})
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

        src/bucket.c
        src/callbacks.c
        src/cntl.c
        src/connevents.c
        src/constants.c
        src/convert.c
        src/crypto.c
        src/counter.c
        src/ctranscoder.c
        src/exceptions.c
        src/ext.c
        src/fts.c
        src/get.c
        src/htresult.c
        src/http.c
        src/iops.c
        src/iops.h
        src/ixmgmt.c
        src/miscops.c
        src/mresdict.h
        src/multiresult.c
        src/n1ql.c
        src/observe.c
        src/opresult.c
        src/oputil.c
        src/oputil.h
        src/pipeline.c
        src/pycbc.h
        src/result.c
        src/store.c
        src/typeutil.c
        src/views.c
        ${BOOST_SOURCES}

        )
add_dependencies(couchbase_python_client ${CPP_DEPS})

link_directories(${CMAKE_BINARY_DIR}/${install}/lib/${RELEASE_TYPE} /usr/local/lib)
#link_libraries(/Users/ellis_breen/root/workspaces/couchbase/couchbase-python-client-mainline/build/cmake/install/lib/Debug/libcouchbase.2.dylib)
if(CMAKE_BUILD_TYPE MATCHES Debug)
    link_libraries(${CMAKE_BINARY_DIR}/${install}/lib/${RELEASE_TYPE}/${LIBCOUCHBASE_DBG_LIBRARY_NAME})
else()
    link_libraries(${CMAKE_BINARY_DIR}/${install}/lib/${RELEASE_TYPE}/${LIBCOUCHBASE_LIBRARY_NAME})
endif()
target_link_libraries(couchbase_python_client ${Boost_LIBRARIES}  ${PYTHON_LIBRARIES} couchbase)

set_target_properties(
        couchbase_python_client
        PROPERTIES
        PREFIX ""
        OUTPUT_NAME ${PYCBC_C_MOD_NAME}
        LINKER_LANGUAGE C
        SUFFIX ${PYCBC_C_MOD_SUFFIX}
)

