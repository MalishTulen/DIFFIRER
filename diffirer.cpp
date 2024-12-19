#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <direct.h>
#include <assert.h>
#include <ctype.h>

#include "diffirer.h"
#include "tree.h"
#include "grafic_print.h"
#include "schitivalka.h"
#include "improver.h"

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
    input_file_t* input_data = &data; // TODO: remove nah

//fprintf ( stderr, "READFILE1\n" );
    /*if (system ("rm -rf IMAGES_FOLDER && mkdir IMAGES_FOLDER") == 0)
        printf ( "Folder with images created!\n" );*/

    struct stat file_info = {};                     // TODO: func get_file_size
    fstat ( fileno ( input_file ), &file_info );
//fprintf ( stderr, "size = %ld\n", file_info.st_size );
    input_data->size = ( file_info.st_size );
//fprintf ( stderr, "READFILE2\n" );

    input_data->data = ( char* ) calloc ( input_data->size, sizeof ( char ) );

    reader ( input_file, input_data );

    printf ( "DATA: %s\n", input_data->data );

//fprintf ( stderr, "READFILE3\n" );
    create_tree_from_input_data ( input_data, tree ); // TODO: zaebal

    make_grafic_dump ( tree, amount_of_pictures );

    improover_of_expression ( tree );

fprintf ( stderr, "IMPROOVED!\n" );
    make_grafic_dump ( tree, amount_of_pictures );

    get_string ( tree ); // TODO: print

    differer ( tree );

    make_grafic_dump ( tree, amount_of_pictures );

    improover_of_expression ( tree );

fprintf ( stderr, "DIFFERING!\n" );

    //array_dump ( tree, tree->g_root );

    make_grafic_dump ( tree, amount_of_pictures );

    get_string ( tree );

    return DONE;
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
//fprintf ( stderr, "operation: %d\n", node->object.operation );
        switch ( node->object.operation )
        {
            case SUM:
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

                node->object.operation = SUM;
                node->left = create_new_node ( tree, OP, {.operation = MUL}, copy_left, right_no_diff );
                node->right = create_new_node ( tree, OP, {.operation = MUL}, left_no_diff, copy_right );

                break;
            }

            case DIV:
            {
                node_t* copy_left = copy_sons_balls ( tree, node->left );
                node_t* copy_right = copy_sons_balls ( tree, node->right );
                node_t* copy_for_denominator = copy_sons_balls ( tree, node->right );
                differenciation ( tree, copy_left );
                differenciation ( tree, copy_right );

                node->left = create_new_node ( tree, OP, {.operation = SUB}, create_new_node( tree, OP, {.operation = MUL}, copy_left, node->right ), create_new_node ( tree, OP, {.operation = MUL}, copy_right, node->left ) );
                node->right = create_new_node ( tree, OP, {.operation = EXP}, copy_for_denominator, create_new_node ( tree, NUM, {.constant = 2}, NULL, NULL ) );
                break;
            }

            case EXP:
            {
                node_t* copy_left = copy_sons_balls ( tree, node->left );
                node_t* copy_right = copy_sons_balls ( tree, node->right );
                node_t* copy_node  = copy_sons_balls ( tree, node );
                differenciation ( tree, copy_left );
                differenciation ( tree, copy_right );

                if ( node->right->type == NUM )
                {
fprintf ( stderr, "1 VARIANT\n" );
                    node->object.operation = MUL;
                    delete_sub_tree ( node->left );
                    node->left = create_new_node ( tree, OP, {.operation = MUL }, copy_left, copy_node );
                    node->left->right->right->object.constant -= 1;
                    delete_node ( copy_right );
                }

                else if ( node->left->type == NUM && node->right->type != NUM )
                {
fprintf ( stderr, "2 VARIANT\n" );
                    node->object.operation = MUL;
                    delete_sub_tree ( node->right );
                    delete_node ( copy_left );
array_dump ( tree, copy_left );
                    node->right = create_new_node ( tree, OP, {.operation = LN }, NULL, node->left );
                    node->left = create_new_node ( tree, OP, {.operation = MUL }, copy_node, copy_right );
                }

                else if ( node->left->type != NUM && node->right->type != NUM )
                {
fprintf ( stderr, "3 VARIANT\n" );
//array_dump ( tree, copy_node );
//array_dump ( tree, copy_left );
                    node->object.operation = MUL;
                    //delete_sub_tree ( node->left );
                    node_t* ln_node = create_new_node ( tree, OP, {.operation = LN }, NULL, node->left );
                    node_t* copy_ln_node = copy_sons_balls ( tree, ln_node );
                    node->left = copy_node;
fprintf ( stderr, "DEBUG1\n" );

fprintf ( stderr, "DEBUG1.2\nCOPYLN_NODE:\n" );
array_dump ( tree, copy_ln_node );
                    differenciation ( tree, copy_ln_node );


fprintf ( stderr, "DEBUG2\nLNNODE:\n" );
                    node_t* nodeRL = create_new_node ( tree, OP, {.operation = MUL }, copy_right, ln_node );
                    node_t* nodeRR = create_new_node ( tree, OP, {.operation = MUL }, node->right, copy_ln_node );
fprintf ( stderr, "DEBUG3\nNODE88:\n" );
                    node->right = create_new_node ( tree, OP, {.operation = SUM }, nodeRL, nodeRR );
fprintf ( stderr, "DEBUG4\n" );
                }

                break;
            }

            case LN:
            {
                node_t* copy_right = copy_sons_balls ( tree, node->right );
                differenciation ( tree, copy_right );
                node->object.operation = DIV;
                node->left = copy_right;

                break;
            }

            case SIN:
            {
                node_t* copy_node = copy_sons_balls ( tree, node );
                node_t* copy_right = copy_sons_balls ( tree, node->right );
                differenciation ( tree, copy_right );

                node->object.operation = MUL;
                node->left = copy_node;
                node->left->object.operation = COS;
                node->right = copy_right;

                break;
            }

            case COS:
            {
                node_t* copy_node = copy_sons_balls ( tree, node );
                node_t* copy_right = copy_sons_balls ( tree, node->right );
                node_t* minus_1 = create_new_node ( tree, NUM, {.constant = 1}, NULL, NULL );
                differenciation ( tree, copy_right );

                node_t* nodex = create_new_node ( tree, OP, {.operation = MUL}, minus_1, copy_right );
                node->object.operation = MUL;
                node->left = copy_node;
                node->left->object.operation = SIN;
                node->right = nodex;

                break;
            }

            default:
                printf ( "ERROR IN DIFF!\nBAD_VALUE(%d)", node->object.operation );
        }
    }

    return ( node_t* ) POISON_PTR;
}

// TODO: add DSL cL_ cR_ dL_ dR_

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
        printf ( "(%.3lf)", node->object.constant );
    else if ( node->type == VAR )
        printf ( "(%c)", node->object.var );
    else if ( node->type == OP )
    {
        printf ( "( " );

        if ( node->left != NULL )
            get_symbol ( node->left );

        printf_operation ( node );

        get_symbol ( node->right );

        printf ( " )" );
    }

    return node;
}

errors_t printf_operation ( node_t* node )
{
    switch ( node->object.operation )
    {
        case SUM:
            printf ( "+" );
            break;
        case SUB:
            printf ( "-" );
            break;
        case MUL:
            printf ( "*" );
            break;
        case DIV:
            printf ( "/" );
            break;
        case EXP:
            printf ( "^" );
            break;
        case SQR:
            printf ( "sqrt" );
            break;
        case LN:
            printf ( "ln" );
            break;
        case SIN:
            printf ( "sin" );
            break;
        case COS:
            printf ( "cos" );
            break;

        default:
            printf ( "ERROR!\n");
            break;
    }

    return DONE;
}
// TODO: PUT ASSERTS!!!
