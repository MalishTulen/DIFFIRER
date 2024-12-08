#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

//#include "stack.h"
#include "diffirer.h"
#include "grafic_print.h"

errors_t make_grafic_dump ( tree_t* ptr_data, int* amount_of_pictures )
{
    CHECK ( ptr_data );

    const char  dot_file[] = "dot_file.dot";
    const char  output_file[] = "a";

    FILE* ptr_dot_file = fopen ( "dot_file.dot", "w" );

    create_dot_code ( ptr_data, ptr_dot_file );

    fclose ( ptr_dot_file );


    make_picture ( dot_file, output_file, amount_of_pictures );

    return DONE;
}

errors_t create_dot_code ( tree_t* ptr_data, FILE* ptr_dot_file )
{
    fprintf ( ptr_dot_file, "digraph data{\n" );
    fprintf ( ptr_dot_file, "rankdir = TB;\n" );

    for ( int i = 0; i < DATA_CAPACITY; i++ )
    {
        int checker_if_leaf = check_if_leaf ( &ptr_data->array_data[ i ] );
            if ( checker_if_leaf == LEAF )
            {
                if ( ptr_data->array_data [ i ].type == NUM )
                    fprintf ( ptr_dot_file, "node%p[ shape = record, style = bold, color = \"#580612\", label = \" {<f0> NODE%d(%p) | <f1> TYPE %d | <f2> value =%lf | { <f3> LEFT ( %p ) | <f4> RIGHT ( %p ) } } \"];\n", &ptr_data->array_data[ i ], i, &ptr_data->array_data[ i ], ptr_data->array_data[ i ].type, ptr_data->array_data[ i ].object.constant, ptr_data->array_data[ i ].left, ptr_data->array_data[ i ].right );
                if ( ptr_data->array_data [ i ].type == OP )
                    printf ( "Error in index %d: the tree ends with OP\n", i);
                if ( ptr_data->array_data [ i ].type == VAR )
                    fprintf ( ptr_dot_file, "node%p[ shape = record,style = bold, color = \"#5b63e0\", penwidth = 2.0, label = \" {<f0> NODE%d(%p) | <f1> TYPE %d | <f2> value =%c | { <f3> LEFT ( %p ) | <f4> RIGHT ( %p ) } } \"];\n", &ptr_data->array_data[ i ], i, &ptr_data->array_data[ i ], ptr_data->array_data[ i ].type, ptr_data->array_data[ i ].object.var, ptr_data->array_data[ i ].left, ptr_data->array_data[ i ].right );
            }
            if ( checker_if_leaf == NOT_LEAF )
            {
                if ( ptr_data->array_data [ i ].type == NUM )
                    printf ( "Error in index %d: the tree ends with NUM\n", i);
                if ( ptr_data->array_data [ i ].type == OP )
                    fprintf ( ptr_dot_file, "node%p[ shape = record,style = bold, color = \"#09752e\", label = \" {<f0> NODE%d(%p) | <f1> TYPE %d | <f2> value = %d | { <f3> LEFT ( %p ) | <f4> RIGHT ( %p ) } } \"];\n", &ptr_data->array_data[ i ], i, &ptr_data->array_data[ i ], ptr_data->array_data[ i ].type, ptr_data->array_data[ i ].object.operation, ptr_data->array_data[ i ].left, ptr_data->array_data[ i ].right  );
                if ( ptr_data->array_data [ i ].type == VAR )
                    printf ( "Error in index %d: the tree ends with VAR\n", i);
            }
    }

//fprintf ( stderr, "HUI1\n");
    branch_connector ( ptr_data->g_root, ptr_data, ptr_dot_file );
//fprintf ( stderr, "HUI2\n");
    fprintf ( ptr_dot_file, "}\n" );

    return DONE;
}

errors_t make_picture ( const char* dot_file, const char* output_file, int* amount_of_pictures )
{
    char cmd_command [ CMD_COMMAND_LEN ] = {};
    sprintf ( cmd_command, "dot %s -T png -o %s%d.png", dot_file, output_file, *amount_of_pictures );

    ( *amount_of_pictures )++;
    fprintf ( stderr, "command = '%s'\n", cmd_command );
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

