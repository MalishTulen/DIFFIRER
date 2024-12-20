#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "diffirer.h"
#include "schitivalka.h"
#include "tree.h"

errors_t create_tree_from_input_data ( input_file_t* input_data, tree_t* tree )
{
    int p = 0;
    tree->g_root = get_dollar ( input_data, tree, &p );
//fprintf ( stderr, "Groot = %p\n", tree->g_root );
    array_dump ( tree, tree->g_root );

    return DONE;
}

node_t* get_dollar ( input_file_t* input_data, tree_t* tree, int* p )
{
    char* daata = input_data->data;
//fprintf ( stderr, "DOLLAR SCANNER = %c\n", daata [ *p ] );
    node_t* value = get_sum ( input_data, tree, p );
//fprintf ( stderr, "DOLLAR SCANNER = %c\n", daata [ *p ] );

    if ( daata [ *p ] != '$' )
    {
        SYNTAX_ERROR ( input_data->data, p );
    }
    (*p)++;

    return value;
}

node_t* get_sum ( input_file_t* input_data, tree_t* tree, int* p )
{
    char* daata = input_data->data;
//fprintf ( stderr, "SUM SCANNER = %c\n", daata [ *p ] );
    node_t* value = get_mul ( input_data, tree, p );
//fprintf ( stderr, "SUM SCANNER = %c\n", daata [ *p ] );
    node_t* value3 = NULL;
    int checker = 0;
    while ( ( daata [ *p ] == '+' ) || ( daata [ *p ] == '-' ) )
    {
        char op = daata [ *p ];
        (*p)++;

        node_t* value2 = get_mul ( input_data, tree, p );

        if ( checker == 1)
        {
            if ( op == '+' )
                value3->right = create_new_node ( tree, OP, {.operation = SUM}, value3->right, value2 );
            else
                value3->right = create_new_node ( tree, OP, {.operation = SUB}, value3->right, value2 );

            value3 = value3->right;
        }

        else
        {
            if ( op == '+' )
                value = create_new_node ( tree, OP, {.operation = SUM}, value, value2 );
            else
                value = create_new_node ( tree, OP, {.operation = SUB}, value, value2 );

            checker = 1;
            value3 = value;
        }
    }

    return value;
}

node_t* get_mul ( input_file_t* input_data, tree_t* tree, int* p )
{
    char* daata = input_data->data;
//fprintf ( stderr, "MUL SCANNER = %c\n", daata [ *p ] );
    node_t* value = get_exp ( input_data, tree, p );
//fprintf ( stderr, "MUL SCANNER = %c\n", daata [ *p ] );
    node_t* value3 = NULL;
    int checker = 0;
    while ( ( daata [ *p ] == '*' ) || ( daata [ *p ] == '/' ) )
    {
        char op = daata [ *p ];
        (*p)++;

        node_t* value2 = get_exp ( input_data, tree, p );

        if ( checker == 1)
        {
            if ( op == '*' )
                value3->right = create_new_node ( tree, OP, {.operation = MUL}, value3->right, value2 );
            else
                value3->right = create_new_node ( tree, OP, {.operation = DIV}, value3->right, value2 );

            value3 = value3->right;
        }

        else
        {
            if ( op == '*' )
                value = create_new_node ( tree, OP, {.operation = MUL}, value, value2 );
            else
                value = create_new_node ( tree, OP, {.operation = DIV}, value, value2 );

            checker = 1;
            value3 = value;
        }
    }

    return value;
}
node_t* get_exp ( input_file_t* input_data, tree_t* tree, int* p )
{
    char* daata = input_data->data;
//fprintf ( stderr, "EXP SCANNER = %c\n", daata [ *p ] );
    node_t* value = get_log_or_sin_or_cos ( input_data, tree, p );
//fprintf ( stderr, "EXP SCANNER = %c\n", daata [ *p ] );

node_t* value3 = NULL;
    int checker = 0;
    while ( ( daata [ *p ] == '^' ) || ( daata [ *p ] == 's' ) )
    {
        char op = daata [ *p ];
        (*p)++;

        node_t* value2 = get_log_or_sin_or_cos ( input_data, tree, p );

        if ( checker == 1)
        {
            if ( op == '^' )
                value3->right = create_new_node ( tree, OP, {.operation = EXP}, value3->right, value2 );
            else
                value3->right = create_new_node ( tree, OP, {.operation = SQR}, value3->right, value2 );

            value3 = value3->right;
        }

        else
        {
            if ( op == '^' )
                value = create_new_node ( tree, OP, {.operation = EXP}, value, value2 );
            else
                value = create_new_node ( tree, OP, {.operation = SQR}, value, value2 );

            checker = 1;
            value3 = value;
        }
    }

    return value;
}

node_t* get_log_or_sin_or_cos ( input_file_t* input_data, tree_t* tree, int* p )
{
    char* daata = input_data->data;

//fprintf ( stderr, "LOG SCANNER = %c\n", daata [ *p ] );

    if ( daata  [ *p ] == 'L' )
    {
//fprintf ( stderr, "FOUND LOG!\n" );
        (*p)++;
        node_t* value = get_brackets ( input_data, tree, p );

        return create_new_node ( tree, OP, {.operation = LN }, NULL, value );
    }
    else if ( daata  [ *p ] == 'S' )
    {
//fprintf ( stderr, "FOUND LOG!\n" );
        (*p)++;
        node_t* value = get_brackets ( input_data, tree, p );

        return create_new_node ( tree, OP, {.operation = SIN }, NULL, value );
    }
    else if ( daata  [ *p ] == 'C' )
    {
//fprintf ( stderr, "FOUND LOG!\n" );
        (*p)++;
        node_t* value = get_brackets ( input_data, tree, p );

        return create_new_node ( tree, OP, {.operation = COS }, NULL, value );
    }
    else
        return get_brackets ( input_data, tree, p );
}

node_t* get_brackets ( input_file_t* input_data, tree_t* tree, int* p )
{
    char* daata = input_data->data;
//fprintf ( stderr, "BRACKETS SCANNER = %c\n", daata [ *p ] );

    if ( daata [ *p ] == '(' )
    {
        (*p)++;
        node_t* value = get_sum ( input_data, tree, p );

//fprintf ( stderr, "simbol = %c\n", daata [ *p ] );

        if ( daata [ *p ] != ')' )
        {
            SYNTAX_ERROR ( input_data->data, p );
        }
        (*p)++;
        return value;
    }
    else
        return get_number ( input_data, tree, p );
}

node_t* get_number ( input_file_t* input_data, tree_t* tree, int* p )
{
    int old_p = *p;
    int num_checker = 0;
    object_t value = {};
    char* daata = input_data->data;
//fprintf ( stderr, "CONST SCANNER = %c\n", daata [ *p ] );


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
