#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

//#include "stack.h"
#include "diffirer.h"
#include "tree.h"
#include "grafic_print.h"

errors_t make_grafic_dump ( tree_t* ptr_data, int* amount_of_pictures )
{
    printf ( "GRAFIC_DUMP!\n" );

    CHECK ( ptr_data );

    const char  dot_file[] = "dot_file.dot";
    const char  output_file[] = "a";

    FILE* ptr_dot_file = fopen ( "dot_file.dot", "w" );

    fprintf ( ptr_dot_file, "digraph data{\n" );
    fprintf ( ptr_dot_file, "rankdir = TB;\n" );

    create_dot_code ( ptr_data->g_root, ptr_data, ptr_dot_file ); // TODO: create_node_data
    branch_connector ( ptr_data->g_root, ptr_data, ptr_dot_file );
    fprintf ( ptr_dot_file, "}\n" );

    fclose ( ptr_dot_file );

    make_picture ( dot_file, output_file, amount_of_pictures );

    return DONE;
}

// TODO: create_dot_file -> create_node_data, branch_connector

// TODO: func create_node_data

errors_t create_dot_code ( node_t* node, tree_t* ptr_data, FILE* ptr_dot_file )
{
    node_t* cur_symbol = node;
    int checker_if_leaf = check_if_leaf ( cur_symbol );
        if ( checker_if_leaf == LEAF )
        {
            if ( cur_symbol->type != POISON_TYPE )
            {
                if ( cur_symbol->type == NUM ) // TODO: switch-case
                    fprintf ( ptr_dot_file, "node%p[ shape = record, style = bold, color = \"#580612\", label = \" { <f2> %lf | { <f3> LEFT | <f4> RIGHT } } \"];\n", cur_symbol, cur_symbol->object.constant );
                else if ( cur_symbol->type == OP )
                    printf ( "Error in node %p : the tree ends with OP\n", node );
                else if ( cur_symbol->type == VAR )
                    fprintf ( ptr_dot_file, "node%p[ shape = record,style = bold, color = \"#5b63e0\", penwidth = 2.0, label = \" { <f2> %c | { <f3> LEFT | <f4> RIGHT } } \"];\n", cur_symbol, cur_symbol->object.var );
            }
        }
        else if ( checker_if_leaf == NOT_LEAF )
        {
            if ( cur_symbol->type != POISON_TYPE )
            {
                if ( cur_symbol->type == NUM )
                    printf ( "Error in node %p: the tree ends with NUM\n", node );
                else if ( cur_symbol->type == OP )
                    fprintf ( ptr_dot_file, "node%p[ shape = record,style = bold, color = \"#09752e\", label = \" { <f2> %c | { <f3> LEFT | <f4> RIGHT } } \"];\n", cur_symbol, ptr_data->operators_array[ cur_symbol->object.operation - 1 ] );
                else if ( cur_symbol->type == VAR )
                    printf ( "Error in node %p: the tree ends with VAR\n", node );
            }
        }
    if ( node->left != NULL )
        create_dot_code ( node->left, ptr_data, ptr_dot_file );

    if ( node->right != NULL )
        create_dot_code ( node->right, ptr_data, ptr_dot_file );
//fprintf ( stderr, "HUI1\n");
//fprintf ( stderr, "HUI2\n");

    return DONE;
}

errors_t make_picture ( const char* dot_file, const char* output_file, int* amount_of_pictures )
{
    char cmd_command [ CMD_COMMAND_LEN ] = {};
    sprintf ( cmd_command, "dot %s -T png -o %s%d.png", dot_file, output_file, *amount_of_pictures );

    ( *amount_of_pictures )++;
    //fprintf ( stderr, "command = '%s'\n", cmd_command );
    system ( cmd_command );

    return DONE;
}

errors_t branch_connector ( node_t* ptr_index, tree_t* ptr_data, FILE* ptr_dot_file )
{
    //int checker_if_leaf = 0;
//fprintf ( stderr, "POSIT = '%p'\n", ptr_index->left_branch );
    if ( ptr_index->left != NULL)
    {
        fprintf ( ptr_dot_file, "node%p:<f3>->node%p\n", ptr_index, ptr_index->left );
        branch_connector ( ptr_index->left, ptr_data, ptr_dot_file );
        //checker_if_leaf += 2;
    }
    if ( ptr_index->right != NULL)
    {
        fprintf ( ptr_dot_file, "node%p:<f4>->node%p\n", ptr_index, ptr_index->right );
        branch_connector ( ptr_index->right, ptr_data, ptr_dot_file );
        //checker_if_leaf++;
    }

    /*if ( checker_if_leaf == 0 )
        return ALL_GOOD;
    else
        if ( checker_if_leaf == 1 )
        else if ( checker_if_leaf == 2)
            branch_connector ( ptr_index->left_branch, ptr_data, ptr_dot_file );
        else if ( checker_if_leaf == 3)
            branch_connector ( ptr_index->left_branch, ptr_data, ptr_dot_file );*/

    return DONE;
}

