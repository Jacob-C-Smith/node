# Need CMake 3.28
cmake_minimum_required (VERSION 3.28)

# This is the name of the repository
project ("node")

# Set the correct paths
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)
set(CMAKE_PREFIX_PATH ${CMAKE_SOURCE_DIR}/extern/)

# Comment out for Debug mode
set(IS_DEBUG_BUILD CMAKE_BUILD_TYPE STREQUAL "Debug")

# Set for debug mode
if (${IS_DEBUG_BUILD})
    add_compile_definitions(NDEBUG)
endif ()

add_compile_options(-gdwarf-4 -Wall -Wextra -Wpointer-arith -Wstrict-prototypes -Wformat-security -Wfloat-equal -Wshadow -Wconversion -Wlogical-not-parentheses -Wnull-dereference)

# Find the hash cache module
if ( NOT "${HAS_HASH_CACHE}")

    # Add the hash cache directory
    add_subdirectory(${CMAKE_SOURCE_DIR}/extern/hash-cache ${CMAKE_SOURCE_DIR}/build/hash-cache)

    # Let any parent project know that this project has hash cache
    if(NOT PROJECT_IS_TOP_LEVEL)
        set(HAS_HASH_CACHE true PARENT_SCOPE)
        set(HASH_CACHE_INCLUDE_DIR ${HASH_CACHE_INCLUDE_DIR} PARENT_SCOPE)
    endif()
endif()

# Find the array module
if ( NOT "${HAS_ARRAY}")
    
    # Add the array directory
    add_subdirectory(${CMAKE_SOURCE_DIR}/extern/array ${CMAKE_SOURCE_DIR}/build/array)

    # Let any parent project know that this project has array
    if(NOT PROJECT_IS_TOP_LEVEL)
        set(HAS_ARRAY true PARENT_SCOPE)
        set(ARRAY_INCLUDE_DIR ${ARRAY_INCLUDE_DIR} PARENT_SCOPE)
    endif()
endif()

# Find the dict module
if ( NOT "${HAS_DICT}")
    
    # Add the dict directory
    add_subdirectory(${CMAKE_SOURCE_DIR}/extern/dict ${CMAKE_SOURCE_DIR}/build/dict)

    # Let any parent project know that this project has dict
    if(NOT PROJECT_IS_TOP_LEVEL)
        set(HAS_DICT true PARENT_SCOPE)
        set(DICT_INCLUDE_DIR ${DICT_INCLUDE_DIR} PARENT_SCOPE)
    endif()
endif()

# Find the sync module
if ( NOT "${HAS_SYNC}")

    # Add the sync directory
    add_subdirectory(${CMAKE_SOURCE_DIR}/extern/sync ${CMAKE_SOURCE_DIR}/build/sync)

    # Let any parent project know that this project has sync
    if(NOT PROJECT_IS_TOP_LEVEL)
        set(HAS_SYNC true PARENT_SCOPE)
        set(SYNC_INCLUDE_DIR ${SYNC_INCLUDE_DIR} PARENT_SCOPE)
    endif()
endif()

# Find the log module
if ( NOT "${HAS_LOG}")

    # Add the log directory
    add_subdirectory(${CMAKE_SOURCE_DIR}/extern/log ${CMAKE_SOURCE_DIR}/build/log)

    # Let any parent project know that this project has log
    if(NOT PROJECT_IS_TOP_LEVEL)
        set(HAS_LOG true PARENT_SCOPE)
        set(LOG_INCLUDE_DIR ${LOG_INCLUDE_DIR} PARENT_SCOPE)
    endif()
endif()

# Find the json module
if ( NOT "${HAS_JSON}")

    # Add the json directory
    add_subdirectory(${CMAKE_SOURCE_DIR}/extern/json ${CMAKE_SOURCE_DIR}/build/json)

    # Let any parent project know that this project has json
    if(NOT PROJECT_IS_TOP_LEVEL)
        set(HAS_JSON true PARENT_SCOPE)
        set(JSON_INCLUDE_DIR ${JSON_INCLUDE_DIR} PARENT_SCOPE)
    endif()
endif()

# Set the node module
if ( NOT "${HAS_NODE}")

    # Log
    message("[node] Adding node project")

    # Set the node include
    set(NODE_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/include)

    # Let any parent project know that it has the node project
    if(NOT PROJECT_IS_TOP_LEVEL)
        set(HAS_NODE true PARENT_SCOPE)
        set(NODE_INCLUDE_DIR ${NODE_INCLUDE_DIR} PARENT_SCOPE)
    endif()
endif()


# Add source to this project's executable.
add_executable (node_example "main.c")
add_dependencies(node_example node)
target_include_directories(node_example PUBLIC ${NODE_INCLUDE_DIR} ${JSON_INCLUDE_DIR} ${ARRAY_INCLUDE_DIR} ${DICT_INCLUDE_DIR})
target_link_libraries(node_example node json array dict sync)

## Add source to the tester
# add_executable (node_test "node_test.c")
# add_dependencies(node_test node json array dict sync log)
# target_include_directories(node_test PUBLIC ${NODE_INCLUDE_DIR} ${JSON_INCLUDE_DIR} ${LOG_INCLUDE_DIR} ${ARRAY_INCLUDE_DIR} ${HASH_CACHE_INCLUDE_DIR} ${DICT_INCLUDE_DIR} ${SYNC_INCLUDE_DIR})
# target_link_libraries(node_test node json array dict sync log)

# Add source to this project's library
add_library (node SHARED "node.c")
add_dependencies(node json array dict sync log)
target_include_directories(node PUBLIC  ${NODE_INCLUDE_DIR} ${JSON_INCLUDE_DIR} ${ARRAY_INCLUDE_DIR} ${HASH_CACHE_INCLUDE_DIR} ${DICT_INCLUDE_DIR} ${SYNC_INCLUDE_DIR})
target_link_libraries(node PUBLIC json array dict sync log)