/** !
 * Node implementation
 * 
 * @file node.c
 * 
 * @author Jacob Smith
 */

// Header
#include <node/node.h>

// Structure definitions
struct node_s
{
    char _name[255 + 1];

    struct
    {
        char _name[31+1];
        void *value;
        size_t in_index;
        node *p_in;
    } in [64];

    struct
    {
        char _name[31+1];
        void *value;
        size_t out_index;
        node *p_out;
    } out [64];

    size_t in_quantity;
    size_t out_quantity;

    void *value;
};

struct node_graph_s
{
    dict *p_nodes;

    struct
    {
        int i;
    } functions;
    
    size_t node_quantity;
    node *_p_nodes[];
};

// Data
static bool initialized = false;

int node_construct ( node **pp_node, const char *const p_name, const json_value *const p_value );

// Function definitions
void node_init ( void ) 
{

    // State check
    if ( initialized == true ) return;

    // Initialize the sync library
    sync_init();
    
    // Initialize the log library
    log_init();

    // Initialize the array library
    array_init();

    // Initialize the dict library
    dict_init();

    // Initialize the json library
    json_init();

    // Set the initialized flag
    initialized = true;

    // Done
    return; 
}

int node_create ( node **pp_node )
{

    // Argument check
    if ( pp_node == (void *) 0 ) goto no_node;

    // Initialized data
    node *p_node = NODE_REALLOC(0, sizeof(node));

    // Error check
    if ( p_node == (void *) 0 ) goto no_mem;

    // Initialize memory
    memset(p_node, 0, sizeof(node));

    // Return a pointer to the caller
    *pp_node = p_node;

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_node:
                #ifndef NDEBUG
                    log_error("[node] Null pointer provided for parameter \"pp_node\" in call to function \"%s\"\n", __FUNCTION__);
                #endif
                
                // Error
                return 0;
        }

        // Standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    log_error("[Standard Library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif
                
                // Error
                return 0;
        }
    }
}

int node_graph_create ( node_graph **pp_node_graph )
{

    // Argument check
    if ( pp_node_graph == (void *) 0 ) goto no_node_graph;

    // Initialized data
    node_graph *p_node_graph = NODE_REALLOC(0, sizeof(node_graph));

    // Error check
    if ( p_node_graph == (void *) 0 ) goto no_mem;

    // Initialize memory
    memset(p_node_graph, 0, sizeof(node_graph));

    // Return a pointer to the caller
    *pp_node_graph = p_node_graph;

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_node_graph:
                #ifndef NDEBUG
                    log_error("[node] Null pointer provided for parameter \"pp_node_graph\" in call to function \"%s\"\n", __FUNCTION__);
                #endif
                
                // Error
                return 0;
        }

        // Standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    log_error("[Standard Library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif
                
                // Error
                return 0;
        }
    }
}

int node_graph_construct ( node_graph **pp_node_graph, const json_value *const p_value )
{

    // Argument check
    if ( pp_node_graph ==        (void *) 0 ) goto no_node_graph;
    if ( p_value       ==        (void *) 0 ) goto no_value;
    if ( p_value->type != JSON_VALUE_OBJECT ) goto wrong_type;

    // Initialized data
    node_graph *p_node_graph = (void *) 0;

    // Allocate a node graph
    if ( node_graph_create(&p_node_graph) == 0 ) goto failed_to_allocate_node_graph;

    // Parse the json object into a node graph
    {

        // Initialized data
        dict *p_dict = p_value->object;
        json_value *p_nodes       = dict_get(p_dict, "nodes"),
                   *p_connections = dict_get(p_dict, "connections");

        // Missing properties
        if ( p_nodes == (void *) 0 ) goto missing_nodes_value;

        // Type check
        if ( p_nodes->type != JSON_VALUE_OBJECT ) goto wrong_nodes_type;

        // Parse the nodes
        {

            // Initialized data
            dict *p_dict = p_nodes->object;
            size_t node_quantity = dict_keys(p_dict, 0);
            const char **pp_keys = (void *) 0; 

            // Error check
            if ( node_quantity == (void *) 0 ) goto no_nodes;

            // Grow the allocation
            p_node_graph = NODE_REALLOC(p_node_graph, sizeof(node_graph) + (node_quantity * sizeof(node *)));

            // Store the node quantity
            p_node_graph->node_quantity = node_quantity;

            // Allocate memory for keys
            pp_keys = NODE_REALLOC(0, node_quantity * sizeof(node *));

            // Construct a lookup for nodes
            if ( dict_construct(&p_node_graph->p_nodes, node_quantity, 0) == 0 ) goto failed_to_construct_dict;

            // Get the keys
            if ( dict_keys(p_dict, pp_keys) == 0 ) goto failed_to_get_keys;

            // Construct each node
            for (size_t i = 0; i < node_quantity; i++)
            {
                
                // Initialized data
                node *p_node = (void *) 0;
                const char *const p_key = pp_keys[i];

                // Construct the node
                if ( node_construct(&p_node, p_key, dict_get(p_dict, p_key)) == 0 ) goto failed_to_construct_node;

                // Store the node in the node graph
                p_node_graph->_p_nodes[i] = p_node;

                // Add the node to the dictionary
                dict_add(p_node_graph->p_nodes, p_node->_name, p_node);
            }
            
            // Release memory
            pp_keys = NODE_REALLOC(pp_keys, 0);
        }
        
        // Parse the connections
        {

        }
    }

    // Return a pointer to the caller
    *pp_node_graph = p_node_graph;

    // Success
    return 1;

    no_nodes:
    failed_to_construct_dict:
    failed_to_get_keys:
    missing_nodes_value:
    failed_to_construct_node:
    wrong_nodes_type:

        // Error
        return 0;

    // Error handling
    {
        
        // Argument errors
        {
            no_node_graph:
                #ifndef NDEBUG
                    log_error("[node] Null pointer provided for parameter \"pp_node_graph\" in call to function \"%s\"\n", __FUNCTION__);
                #endif
                
                // Error
                return 0;

            no_value:
                #ifndef NDEBUG
                    log_error("[node] Null pointer provided for parameter \"p_value\" in call to function \"%s\"\n", __FUNCTION__);
                #endif
                
                // Error
                return 0;

            wrong_type:
                #ifndef NDEBUG
                    log_error("[node] Parameter \"p_value\" must be of type [ object ] in call to function \"%s\"\n", __FUNCTION__);
                #endif
                
                // Error
                return 0;
        }
 
        // Node errors
        {
            failed_to_allocate_node_graph:
                #ifndef NDEBUG
                    log_error("[node] Failed to allocate node graph in call to function \"%s\"\n", __FUNCTION__);
                #endif
                
                // Error
                return 0;
        }
    }
}

int node_construct ( node **pp_node, const char *const p_name, const json_value *const p_value )
{

    // Argument check
    if ( pp_node       ==        (void *) 0 ) goto no_node;
    if ( p_name        ==        (void *) 0 ) goto no_name;
    if ( p_value       ==        (void *) 0 ) goto no_value;
    if ( p_value->type != JSON_VALUE_OBJECT ) goto wrong_type;

    // Initialized data
    node *p_node = (void *) 0;

    // Allocate a node
    if ( node_create(&p_node) == 0 ) goto failed_to_create_node;

    // Construct a node from a json value
    {

        // Initialized data
        dict *p_dict = p_value->object;
        json_value *p_out = dict_get(p_dict, "out"),
                   *p_in  = dict_get(p_dict, "in");
        
        // Set the name
        {
            size_t len = strlen(p_name);

            // Copy the string
            strncpy(p_node->_name, p_name, len);
        }

        // Set the out
        if ( p_out ) 
        {

            // Initialized data
            array *p_array = p_out->list;
            size_t len = array_size(p_array);

            p_node->out_quantity = len;

            for (size_t i = 0; i < len; i++)
            {
                
                // Initialized data
                json_value *i_value = (void *) 0;

                array_index(p_array, i, &i_value);

                strncpy(p_node->out[i]._name, i_value->string, 63);
            }
        }

        // Set the in
        if ( p_in ) 
        {

            // Initialized data
            array *p_array = p_in->list;
            size_t len = array_size(p_array);

            p_node->in_quantity = len;

            for (size_t i = 0; i < len; i++)
            {
                
                // Initialized data
                json_value *i_value = (void *) 0;

                array_index(p_array, i, &i_value);

                strncpy(p_node->in[i]._name, i_value->string, 63);
            }
        }
    }

    // Return a pointer to the caller
    *pp_node = p_node;

    // Success
    return 1;

    // Error handling
    {

        // Argument check
        {
            no_node:
                #ifndef NDEBUG
                    log_error("[node] Null pointer provided for parameter \"pp_node\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_name:
                #ifndef NDEBUG
                    log_error("[node] Null pointer provided for parameter \"p_name\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;

            no_value:
                #ifndef NDEBUG
                    log_error("[node] Null pointer provided for parameter \"p_value\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
            
            wrong_type:
                #ifndef NDEBUG
                    log_error("[node] Parameter \"p_value\" must be of type [ object ] in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // Node errors
        {
            failed_to_create_node:
                #ifndef NDEBUG
                    log_error("[node] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int node_graph_print ( node_graph *p_node_graph )
{

    printf("Node Graph:\n");
    printf(" - nodes: \n");
    for (size_t i = 0; i < p_node_graph->node_quantity; i++)
    {
        printf("    - \"%s\":\n", p_node_graph->_p_nodes[i]->_name);
        
        if ( p_node_graph->_p_nodes[i]->out_quantity )
        {
            printf("      - out:\n");
            for (size_t j = 0; j < p_node_graph->_p_nodes[i]->out_quantity; j++)
            {
                printf(
                    "         [%d] - %s --> %s\n",
                    j,
                    p_node_graph->_p_nodes[i]->out[j]._name,
                    p_node_graph->_p_nodes[i]->out[j].p_out->_name,
                    p_node_graph->_p_nodes[i]->out[j].p_out->in[p_node_graph->_p_nodes[i]->out[j].out_index]._name);
                
            }
        }
        if ( p_node_graph->_p_nodes[i]->in_quantity )
        {
            printf("      - in:\n");
            for (size_t j = 0; j < p_node_graph->_p_nodes[i]->in_quantity; j++)
            {
                printf(
                    "         [%d] - %s <-- %s\n",
                    j,
                    p_node_graph->_p_nodes[i]->in[j]._name,
                    p_node_graph->_p_nodes[i]->in[j].p_in->_name,
                    p_node_graph->_p_nodes[i]->in[j].p_in->in[p_node_graph->_p_nodes[i]->in[j].in_index]._name);
            }
        }
    }

    // Success
    return 1;
}
