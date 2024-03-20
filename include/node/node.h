/** !
 * Header for node module
 * 
 * @file node/node.h
 * 
 * @author Jacob Smith
 */

// Include guard
#pragma once

// Standard library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// sync submodule
#include <sync/sync.h>

// log submodule
#include <log/log.h>

// array submodule
#include <array/array.h>

// dict submodule
#include <dict/dict.h>

// json submodule
#include <json/json.h>

// Platform dependent macros
#ifdef _WIN64
    #define DLLEXPORT extern __declspec(dllexport)
#else
    #define DLLEXPORT
#endif

// Set the reallocator for the dict module
#ifdef DICT_REALLOC
    #undef DICT_REALLOC
    #define DICT_REALLOC(p, sz) realloc(p, sz)
#endif

// Set the reallocator for the array module
#ifdef ARRAY_REALLOC
    #undef ARRAY_REALLOC
    #define ARRAY_REALLOC(p, sz) realloc(p, sz)
#endif

// Set the reallocator for the json module
#ifdef JSON_REALLOC
    #undef JSON_REALLOC
    #define JSON_REALLOC(p, sz) realloc(p, sz)
#endif

// Memory management macro
#ifndef NODE_REALLOC
    #define NODE_REALLOC(p, sz) realloc(p,sz)
#endif

// Structure declarations
struct node_s;
struct node_graph_s;

// Type definitions
typedef struct node_s node;
typedef struct node_graph_s node_graph;

// Function declarations
// Initializer
/** !
 * This gets called at runtime before main. 
 * 
 * @param void
 * 
 * @return void
 */
DLLEXPORT void node_init ( void ) __attribute__((constructor));

// Constructor
/** !
 * Construct a node graph from a json object
 * 
 * @param pp_node result
 * @param p_value the json object
 * 
 * @return 1 on success, 0 on error
 */
DLLEXPORT int node_graph_construct (
    node_graph **pp_node_graph,
    const json_value *const p_value
);

DLLEXPORT int node_graph_print ( node_graph *p_node_graph );
