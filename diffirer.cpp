#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <direct.h>
#include <assert.h>
#include <ctype.h>

#include "diffirer.h"
#include "grafic_print.h"

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

    return DONE;
}

errors_t tree_ctor ( tree_t* tree )
{
    tree->array_data = ( node_t* ) calloc ( DATA_CAPACITY, sizeof ( node_t ) );
    tree->vars_array = ( char* ) calloc ( VARS_ARRAY_CAPACITY, sizeof ( char ) );
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

errors_t create_tree_from_input_data_mega_gay_porno_huli_takoe_dlinnoye_imya_adolf_hitler ( input_file_t* input_data, tree_t* tree )
{
    int p = 0;
    tree->g_root = get_g ( input_data, tree, &p );
    array_dump ( tree );

    return DONE;
}

node_t* get_g ( input_file_t* input_data, tree_t* tree, int* p )
{
    node_t* value = get_e ( input_data, tree, p );
    char* daata = input_data->data;
fprintf ( stderr, "Gsimbol = %c\n", daata [ *p ] );


    if ( daata [ *p ] != '$' )
    {
        SYNTAX_ERROR ( input_data->data, p );
    }
    (*p)++;

    return value;
}

node_t* get_e ( input_file_t* input_data, tree_t* tree, int* p )
{
    char* daata = input_data->data;
    node_t* value = get_t ( input_data, tree, p );
fprintf ( stderr, "Esimbol = %c\n", daata [ *p ] );


    if ( ( daata [ *p ] == '+' ) || ( daata [ *p ] == '-' ) )
    {
        (*p)++;
        node_t* value2 = get_t ( input_data, tree, p );

        if ( daata [ *p ] == '+' )
        {
            return create_new_node ( tree, OP, {.operation = ADD}, value, value2 );
        }
        else
            return create_new_node ( tree, OP, {.operation = SUB}, value, value2 );

    }

    return value;
}

node_t* get_t ( input_file_t* input_data, tree_t* tree, int* p )
{
    char* daata = input_data->data;
    node_t* value = get_p ( input_data, tree, p );

fprintf ( stderr, "Tsimbol = %c\n", daata [ *p ] );

    if ( daata [ *p ] == '*' || daata [ *p ] == '/' )
    {
        (*p)++;
        node_t* value2 = get_p ( input_data, tree, p );

        if ( daata [ *p ] == '*' )
            return create_new_node ( tree, OP, {.operation = MUL}, value, value2 );
        else
            return create_new_node ( tree, OP, {.operation = DIV}, value, value2 );
    }

    return value;
}

node_t* get_p ( input_file_t* input_data, tree_t* tree, int* p )
{
    char* daata = input_data->data;
fprintf ( stderr, "Psimbol = %c\n", daata [ *p ] );

    if ( daata [ *p ] == '(' )
    {
        (*p)++;
        node_t* value = get_e ( input_data, tree, p );

//fprintf ( stderr, "simbol = %c\n", daata [ *p ] );

        if ( daata [ *p ] != ')' )
        {
            SYNTAX_ERROR ( input_data->data, p );
        }
        (*p)++;
        return value;
    }
    else
        return get_n ( input_data, tree, p );
}

node_t* get_n ( input_file_t* input_data, tree_t* tree, int* p )
{
    int old_p = *p;
    int num_checker = 0;
    object_t value = {};
    char* daata = input_data->data;
fprintf ( stderr, "Nsimbol = %c\n", daata [ *p ] );


    if ( '0' <= daata [ *p ] && daata [ *p ] <= '9' )
    {
        num_checker = 1;
        while ( '0' <= daata [ *p ] && daata [ *p ] <= '9' )
        {
    fprintf ( stderr, "Nsimbol = %c\n", daata [ *p ] );
            value.constant = value.constant * 10 + ( daata [ *p ] - '0' );
            (*p)++;
        }
    }
    else if ( ( isalpha ( daata [ *p ])) != 0 )
    {
        value.var = daata [ *p ];
        (*p)++;
    }


    if ( old_p != *p )
    {
        if ( num_checker == 1)
            return create_new_node ( tree, NUM, value, NULL, NULL );
        else
            return create_new_node ( tree, VAR, value, NULL, NULL );
    }
    else
    {
        SYNTAX_ERROR ( input_data->data, p );
    }
}

errors_t improover_of_expression ( tree_t* tree )
{
    int improving = 1;
    int amount_of_improves = 0;

    while ( improving )
    {
        amount_of_improves = 0;

        improver_of_node ( tree->g_root, &amount_of_improves );

        printf ( "amount of improves: %d\n", amount_of_improves );

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
fprintf ( stderr, "IMPR1\n");
        left_operand = improver_of_node ( node->left, amount_of_improves );
    }

    if ( node->right != NULL )
    {
fprintf ( stderr, "IMPR2\n");
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
        case DIV:
            return left/right;
        case POISON_OPERATION:
            printf ( "POISON OPERATION!\n");
            break;

        default:
            printf ( "UNKNOWN OPERATION! ( %d )\n", operation );
            break;
    }

    return POISON_VALUE;
}
errors_t array_dump ( tree_t* tree )
{
    printf ( "************\nHUI TIGRA\n" );

    for ( int i = 0; i < DATA_CAPACITY; i++ )
    {
        printf ( "node%d\n type = %d\n value = ", i+1, tree->array_data [ i ].type );

        if ( tree->array_data [ i ].type == NUM )
            printf ( "%lf\n", tree->array_data [ i ].object.constant );
        else if ( tree->array_data [ i ].type == OP )
            printf ( "%d\n", tree->array_data [ i ].object.operation );
        else if ( tree->array_data [ i ].type == VAR )
            printf ( "%c\n", tree->array_data [ i ].object.var );
        else if ( tree->array_data [ i ].type == POISON_TYPE )
            printf ( "NONE:)\n" );

    }
    printf ( "#####################\n" );

    return DONE;
}
