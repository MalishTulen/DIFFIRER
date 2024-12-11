#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <direct.h>
#include <assert.h>
#include <ctype.h>

#include "diffirer.h"
#include "grafic_print.h"
#include "rek_spusk.h"

errors_t start_programm ()
{
    tree_t example_tree = {};
    tree_t* tree = &example_tree;
    tree_ctor ( tree );

    static int amount_of_pictures = 1;

    //make_grafic_dump ( tree, &amount_of_pictures );

    read_file ( tree, &amount_of_pictures );

    printf ( "SHIT DONE!\n" );
    return DONE;
}

errors_t read_file ( tree_t* tree, int* amount_of_pictures )
{
    FILE *input_file = fopen ( "input_file.txt", "r" );

    input_file_t data = {};
    input_file_t* input_data = &data;

fprintf ( stderr, "READFILE1\n" );
    /*if (system ("rm -rf IMAGES_FOLDER && mkdir IMAGES_FOLDER") == 0)
        printf ( "Folder with images created!\n" );*/

    struct stat file_info;
    fstat ( fileno ( input_file ), &file_info );
fprintf ( stderr, "size = %ld\n", file_info.st_size );
    input_data->size = ( file_info.st_size );
fprintf ( stderr, "READFILE2\n" );

    input_data->data = ( char* ) calloc ( input_data->size, sizeof ( char ) );

    reader ( input_file, input_data );

    printf ( "DATA: %s", input_data->data );

fprintf ( stderr, "READFILE3\n" );
    create_tree_from_input_data_mega_gay_porno_huli_takoe_dlinnoye_imya_adolf_hitler ( input_data, tree );

    make_grafic_dump ( tree, amount_of_pictures );

    improover_of_expression ( tree );

fprintf ( stderr, "IMPROOVED!\n" );
    make_grafic_dump ( tree, amount_of_pictures );

    get_string ( tree );


    differer ( tree );
    improover_of_expression ( tree );

fprintf ( stderr, "DIFFERING!\n" );

    make_grafic_dump ( tree, amount_of_pictures );

    get_string ( tree );

    return DONE;
}

errors_t tree_ctor ( tree_t* tree )
{
    tree->array_data = ( node_t* ) calloc ( DATA_CAPACITY, sizeof ( node_t ) );
    tree->vars_array = ( char* ) calloc ( VARS_ARRAY_CAPACITY, sizeof ( char ) );
    tree->operators_array = "+-*/";

    tree->size = 0;

    for ( int i = 0; i < DATA_CAPACITY; i++ )
    {
        tree->array_data [ i ].right            = NULL;
        tree->array_data [ i ].left             = NULL;
        tree->array_data [ i ].type             = POISON_TYPE;
        tree->array_data [ i ].object.constant  = POISON_VALUE;
        tree->array_data [ i ].object.var       = 0;
        tree->array_data [ i ].object.operation = POISON_OPERATION;
    }


    CHECK ( tree );

    return DONE;
}

errors_t tree_checker ( tree_t* ptr )
{
    if ( !ptr )
        return BAD_TREE_PTR;
    if ( !ptr->array_data )
        return BAD_DATA_PTR;
    if ( ptr->size < 0 )
        return SIZE_UNDER_PLINTUS;

    for ( int i = 0; i < DATA_CAPACITY; i++ )
    {
        if ( ptr->array_data[ i ].left == &ptr->array_data[ i ] )
            return CYCLIC_LEFT;

        if ( ptr->array_data[ i ].right == &ptr->array_data[ i ] )
            return CYCLIC_RIGHT;
    }

    return DONE;
}

leaf_t check_if_leaf ( node_t* ptr_node )
{
    if ( ptr_node->right == NULL && ptr_node->left == NULL )
        return LEAF;

    return NOT_LEAF;
}

errors_t reader ( FILE* input_file, input_file_t* input_data )
{
    if ( !input_file || !input_data )
        assert ( 0 && "bad_ptr" );

    for ( int i = 0; i < input_data->size; i++ )
    {
        char symbol = 0;

        fscanf ( input_file, " %c", &symbol );

        input_data->data[ i ] = symbol;
    }

    return DONE;
}

node_t* create_new_node ( tree_t* tree, type_t type, object_t value, node_t* left, node_t* right )
{
    tree->array_data [ tree->size ].type = type;
    if ( tree->array_data [ tree->size ].type == NUM )
        tree->array_data [ tree->size ].object.constant = value.constant;
    else if ( tree->array_data [ tree->size ].type == OP )
        tree->array_data [ tree->size ].object.operation = value.operation;
    else if ( tree->array_data [ tree->size ].type == VAR )
        tree->array_data [ tree->size ].object.var = value.var;

    tree->array_data [ tree->size ].left = left;
    tree->array_data [ tree->size ].right = right;

    return &tree->array_data [ tree->size++ ];
}

errors_t delete_node ( node_t* node )
{
    if ( !node )
    {
        printf ( "BAD_PTR\n" );
        return BAD_PTR;
    }

    node->left = ( node_t* ) POISON_PTR;
    node->right = ( node_t* ) POISON_PTR;
    node->type = POISON_TYPE;
    node->object.constant = POISON_VALUE;

    return DONE;
}

errors_t improover_of_expression ( tree_t* tree )
{
    int improving = 1;
    int amount_of_improves = 0;

    while ( improving )
    {
        amount_of_improves = 0;

        improver_of_node ( tree->g_root, &amount_of_improves );

        //printf ( "amount of improves: %d\n", amount_of_improves );

        if ( amount_of_improves == 0 )
            improving = 0;
    }

    return DONE;
}

node_t* improver_of_node ( node_t* node, int* amount_of_improves )
{
    node_t* left_operand = NULL;
    node_t* right_operand = NULL;

    if ( node->left != NULL )
    {
//fprintf ( stderr, "IMPR1\n");
        left_operand = improver_of_node ( node->left, amount_of_improves );
    }

    if ( node->right != NULL )
    {
//fprintf ( stderr, "IMPR2\n");
        right_operand = improver_of_node ( node->right, amount_of_improves );
    }

    if ( left_operand != NULL && left_operand->type == NUM && right_operand != NULL && right_operand->type == NUM )
    {
        (*amount_of_improves)++;
        double left_value = left_operand->object.constant;
        double right_value = right_operand->object.constant;

        delete_node ( left_operand );
        delete_node ( right_operand );

        node->type = NUM;
        node->object.constant = get_value ( left_value, right_value, node->object.operation );
        node->left = NULL;
        node->right = NULL;
    }

    return node;
}

double get_value ( double left, double right, operations_t operation )
{
    switch ( operation )
    {
        case ADD:
            return left+right;
        case SUB:
            return left-right;
        case MUL:
            return left*right;
        case POISON_OPERATION:
            printf ( "POISON OPERATION!\n");
            break;

        default:
            printf ( "UNKNOWN OPERATION! ( %d )\n", operation );
            break;
    }

    return POISON_VALUE;
}

errors_t differer ( tree_t* tree )
{
    differenciation ( tree, tree->g_root );

    return DONE;
}

node_t* differenciation ( tree_t* tree, node_t* node )
{
    int type = node->type;
    if ( type == NUM )
    {
        node->object.constant = 0;
        return node;
    }
    else if ( type == VAR )
    {
        node->type = NUM;
        node->object.constant = 1;
        return node;
    }
    else if ( type == OP )
    {
fprintf ( stderr, "operation: %d\n", node->object.operation );
        switch ( node->object.operation )
        {
            case ADD:
                differenciation ( tree, node->left );
                differenciation ( tree, node->right );
                break;
            case SUB:
                differenciation ( tree, node->left );
                differenciation ( tree, node->right );
                break;
            case MUL:
            {
                node_t* left_no_diff = node->left;
                node_t* right_no_diff = node->right;
                node_t* copy_left = copy_sons_balls ( tree, node->left );
                node_t* copy_right = copy_sons_balls ( tree, node->right );
                differenciation ( tree, copy_left );
                differenciation ( tree, copy_right );

                node->object.operation = ADD;
                node->left = create_new_node ( tree, OP, {.operation = MUL}, copy_left, right_no_diff );
                node->right = create_new_node ( tree, OP, {.operation = MUL}, left_no_diff, copy_right );

//array_dump ( tree, copy_left );
//array_dump ( tree, copy_right );
/*array_dump ( tree, left_no_diff );
array_dump ( tree, right_no_diff );
fprintf ( stderr, "node = %p\n", node );
fprintf ( stderr, "node->left = %p\n", node->left );
fprintf ( stderr, "node->right = %p\n", node->right );

fprintf ( stderr, "node->left->left = %p\n", node->right->right );

fprintf ( stderr, "node->right->left = %p\n", node->right->left );
fprintf ( stderr, "left no diff = %p\n", left_no_diff );
fprintf ( stderr, "node->right->right = %p\n", node->left->right );
fprintf ( stderr, "copyright = %p\n", copy_right );
fprintf ( stderr, "right no diff = %p\n", right_no_diff );



array_dump ( tree, node->left );
array_dump ( tree, node->right );
*/
//array_dump ( tree, node );

                break;
            }

            case DIV:
            {
                node_t* left_no_diff = node->left;
                node_t* right_no_diff = node->right;
                node_t* copy_left = copy_sons_balls ( tree, node->left );
                node_t* copy_right = copy_sons_balls ( tree, node->right );
                differenciation ( tree, copy_left );
                differenciation ( tree, copy_right );

                node->object.operation = DIV;
                node->left = create_new_node ( tree, OP, {.operation = SUB}, create_new_node( tree, OP, {.operation = MUL}, copy_left, node->right ), create_new_node ( tree, OP, {.operation = MUL}, copy_right, node->left ) );
                node->right = create_new_node ( tree, OP, {.operation = MUL}, node->right, node->right );
                break;
            }

            default:
                printf ( "ERROR IN DIFF!\nBAD_VALUE(%d)", node->object.operation );
        }
    }

    return ( node_t* ) POISON_PTR;
}

node_t* copy_sons_balls ( tree_t* tree, node_t* node )
{
    node_t* new_node = create_new_node ( tree, node->type, node->object, NULL, NULL );
    if ( node->left != NULL )
        new_node->left = copy_sons_balls ( tree, node->left );
    if ( node->right != NULL )
        new_node->right = copy_sons_balls ( tree, node->right );

    return new_node;
}
errors_t get_string ( tree_t* tree )
{
    printf ( "Current expression: " );
    get_symbol ( tree->g_root );
    printf ( "\n" );

    return DONE;
}

node_t* get_symbol ( node_t* node )
{
    if ( node->type == NUM )
        printf ( "%.3lf", node->object.constant );
    else if ( node->type == VAR )
        printf ( "%c", node->object.var );
    else if ( node->type == OP )
    {
        printf ( "( " );

        if ( node->left != NULL )
            get_symbol ( node->left );

        printf_operation ( node );

        printf ( " )" );
    }

    return node;
}

errors_t printf_operation ( node_t* node )
{
    switch ( node->object.operation )
    {
        case ADD:
            printf ( "+" );
            get_symbol ( node->right );
            break;
        case SUB:
            printf ( "-" );
            get_symbol ( node->right );
            break;
        case MUL:
            printf ( "*" );
            get_symbol ( node->right );
            break;
        case DIV:
            printf ( "/" );
            get_symbol ( node->right );
            break;

        default:
            printf ( "ERROR!\n");
            break;
    }

    return DONE;
}

errors_t array_dump ( tree_t* tree, node_t* node )
{
    if ( node->type == NUM )
        printf ( "node: type = NUM, value = %.3lf\n", node->object.constant );
    else if ( node->type == VAR )
        printf ( "node: type = VAR, value = %c\n", node->object.var );
    else if ( node->type == OP )
    {
        printf ( "( \n" );

        if ( node->left != NULL )
            array_dump ( tree, node->left );

        printf ( "node: type = %d, value = %c\n", node->type, tree->operators_array [ node->object.operation - 1 ] );

        if ( node->right != NULL )
            array_dump ( tree, node->right );
        printf ( " )\n" );
    }

    return DONE;
}
