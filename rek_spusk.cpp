#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "diffirer.h"
#include "rek_spusk.h"

errors_t create_tree_from_input_data_mega_gay_porno_huli_takoe_dlinnoye_imya_adolf_hitler ( input_file_t* input_data, tree_t* tree )
{
    int p = 0;
    tree->g_root = get_g ( input_data, tree, &p );

    return DONE;
}

node_t* get_g ( input_file_t* input_data, tree_t* tree, int* p )
{
    node_t* value = get_e ( input_data, tree, p );
    char* daata = input_data->data;
//fprintf ( stderr, "Gsimbol = %c\n", daata [ *p ] );


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
//fprintf ( stderr, "Esimbol = %c\n", daata [ *p ] );
    int index = *p;


    if ( ( daata [ index ] == tree->operators_array [ 0 ] ) || ( daata [ index ] == tree->operators_array [ 1 ] ) )
    {
        (*p)++;
        node_t* value2 = get_t ( input_data, tree, p );

        if ( daata [ index ] == tree->operators_array [ 0 ] )
        {
            return create_new_node ( tree, OP, {.operation = ADD}, value, value2 );
        }
        else
        {
fprintf ( stderr, "simbol = %c and should be equal to %c\n", daata [ index ], tree->operators_array [ 0 ] );
fprintf ( stderr, "MINUS\n" );
            return create_new_node ( tree, OP, {.operation = SUB}, value, value2 );
        }
    }

    return value;
}

node_t* get_t ( input_file_t* input_data, tree_t* tree, int* p )
{
    char* daata = input_data->data;
    node_t* value = get_h ( input_data, tree, p );
    int index = *p;

//fprintf ( stderr, "Tsimbol = %c\n", daata [ *p ] );

    if ( ( daata [ index ] == tree->operators_array [ 2 ] ) || ( daata [ index ] == tree->operators_array [ 3 ] ) )
    {
        (*p)++;
        node_t* value2 = get_h ( input_data, tree, p );

        if ( daata [ index ] == tree->operators_array [ 3 ] )
            return create_new_node ( tree, OP, {.operation = DIV}, value, value2 );
        else
            return create_new_node ( tree, OP, {.operation = MUL}, value, value2 );
    }

    return value;
}

node_t* get_h ( input_file_t* input_data, tree_t* tree, int* p )
{
    char* daata = input_data->data;
    node_t* value = get_p ( input_data, tree, p );
fprintf ( stderr, "Psimbol = %c\n", daata [ *p ] );

    if ( daata [ *p ] == tree->operators_array[ 4 ] || daata [ *p ] == tree->operators_array [ 5 ] )
    {
        (*p)++;
        node_t* value2 = get_p ( input_data, tree, p );

        if ( daata [ *p ] == tree->operators_array [ 4 ] )
            return create_new_node ( tree, OP, {.operation = EXP }, value, value2 );
        else
            return create_new_node ( tree, OP, {.operation = SQR }, value, value2 );

//fprintf ( stderr, "simbol = %c\n", daata [ *p ] );

    }

    return value;
}

node_t* get_p ( input_file_t* input_data, tree_t* tree, int* p )
{
    char* daata = input_data->data;
//fprintf ( stderr, "Psimbol = %c\n", daata [ *p ] );

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
//fprintf ( stderr, "Nsimbol = %c\n", daata [ *p ] );


    if ( '0' <= daata [ *p ] && daata [ *p ] <= '9' )
    {
        num_checker = 1;
        while ( '0' <= daata [ *p ] && daata [ *p ] <= '9' )
        {
//fprintf ( stderr, "Nsimbol = %c\n", daata [ *p ] );
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
