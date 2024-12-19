#include <stdio.h>
#include <stdlib.h>

#include "diffirer.h"
#include "tree.h"
#include "improver.h"

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

    if ( node->left != NULL )
    {
//fprintf ( stderr, "IMPR1\n");
        improver_of_node ( node->left, amount_of_improves );
    }

    if ( node->right != NULL )
    {
//fprintf ( stderr, "IMPR2\n");
        improver_of_node ( node->right, amount_of_improves );
    }

    if ( node->type == OP)
    {
        if ( node->object.operation == MUL ) // TODO: move to function
        {
            if ( node->left->type == NUM )
            {
                if ( abs ( node->left->object.constant ) < EPCILON )
                {
        //fprintf ( stderr, "MUL NA ZERO\n" );/*
                    node_t* left_son = node->left;
                    node_t* right_son = node->right;

                    (*amount_of_improves)++;
                    node->type = NUM;
                    node->object.constant = 0;
                    node->left = NULL;
                    node->right = NULL;
                    delete_sub_tree ( left_son );
                    delete_sub_tree ( right_son );
                }

                else if ( abs ( node->left->object.constant - 1 ) < EPCILON )
                {
        //fprintf ( stderr, "MUL NA ODIN (L)\n" );
                    node_t* left_son = node->left;
                    node_t* right_son = node->right;

                    (*amount_of_improves)++;
                    node->type = node->right->type;
                    node->object = node->right->object;
                    node->left = node->right->left;
                    node->right = node->right->right;
        //fprintf ( stderr, "BALLS1\n" );
                    delete_node ( left_son );
                    delete_node ( right_son );
        //fprintf ( stderr, "BALLS2\n" );
                }
            }

            else if ( node->right->type == NUM )
            {
                if ( abs ( node->right->object.constant ) < EPCILON )
                {
                    node_t* left_son = node->left;
                    node_t* right_son = node->right;

                    (*amount_of_improves)++;
                    node->type = NUM;
                    node->object.constant = 0;
                    node->left = NULL;
                    node->right = NULL;
                    delete_sub_tree ( left_son );
                    delete_sub_tree ( right_son );
                }

                else if ( abs ( node->right->object.constant - 1 ) < EPCILON )
                {
                    node_t* left_son = node->left;
                    node_t* right_son = node->right;

                    (*amount_of_improves)++;
                    node->type = node->left->type;
                    node->object = node->left->object;
                    node->right = node->left->right;
                    node->left = node->left->left;

                    delete_node ( left_son );
                    delete_node ( right_son );
                }
            }
        }
        else if ( node->object.operation == SUM || node->object.operation == SUB )
        {
            if ( node->left->type == NUM )
            {
                if ( abs ( node->left->object.constant ) < EPCILON )
                {
                    node_t* left_son = node->left;
                    node_t* right_son = node->right;

                    (*amount_of_improves)++;
                    node->type = node->right->type;
                    node->object = node->right->object;
                    node->left = node->right->left;
                    node->right = node->right->right;

                    delete_node ( left_son );
                    delete_node ( right_son );
                }
            }
            else if ( node->right->type == NUM )
            {
                if ( abs ( node->right->object.constant ) < EPCILON )
                {
                    node_t* left_son = node->left;
                    node_t* right_son = node->right;

                    (*amount_of_improves)++;
                    node->type = node->left->type;
                    node->object = node->left->object;
                    node->right = node->left->right;
                    node->left = node->left->left;

                    delete_node ( left_son );
                    delete_node ( right_son );
                }
            }
        }

        else if ( node->object.operation == EXP )
        {
            if ( node->left->type == NUM )
            {
                if ( abs ( node->left->object.constant - 1 ) < EPCILON )
                {
                    node_t* left_son = node->left;
                    node_t* right_son = node->right;

                    (*amount_of_improves)++;
                    node->type = NUM;
                    node->object.constant = 1;
                    node->left = NULL;
                    node->right = NULL;

                    delete_sub_tree ( left_son );
                    delete_node ( right_son );
                }

                else if ( abs ( node->left->object.constant ) < EPCILON )
                {
                    node_t* left_son = node->left;
                    node_t* right_son = node->right;

                    (*amount_of_improves)++;
                    node->type = NUM;
                    node->object.constant = 0;
                    node->left = NULL;
                    node->right = NULL;
                    delete_sub_tree ( left_son );
                    delete_sub_tree ( right_son );
                }
            }
            else if ( node->right->type == NUM)
            {
                if ( abs ( node->right->object.constant - 1 ) < EPCILON )
                {
                    node_t* left_son = node->left;
                    node_t* right_son = node->right;

                    (*amount_of_improves)++;
                    node->type = node->left->type;
                    node->object = node->left->object;
                    node->right = node->left->right;
                    node->left = node->left->left;

                    delete_sub_tree ( right_son );
                    delete_node ( left_son );
                }

                else if ( abs ( node->right->object.constant ) < EPCILON )
                {
                    node_t* left_son = node->left;
                    node_t* right_son = node->right;

                    (*amount_of_improves)++;
                    node->type = NUM;
                    node->object.constant = 1;
                    node->left = NULL;
                    node->right = NULL;
                    delete_sub_tree ( left_son );
                    delete_sub_tree ( right_son );
                }
            }

        }

        if ( node->object.operation == SUM || node->object.operation == SUB || node->object.operation == MUL )
        {
            if ( node->left != NULL && node->left->type == NUM && node->right != NULL && node->right->type == NUM )
            {
                (*amount_of_improves)++;
                double left_value = node->left->object.constant;
                double right_value = node->right->object.constant;

                delete_sub_tree ( node->left );
                delete_sub_tree ( node->right );

                node->type = NUM;
                node->object.constant = get_value ( left_value, right_value, node->object.operation );
                node->left = NULL;
                node->right = NULL;
            }
        }
    }

    return node;
}

double get_value ( double left, double right, operations_t operation )
{
    if ( operation == SUM)
        return left+right;
    else if ( operation == SUB)
        return left-right;
    else if ( operation == MUL)
        return left*right;

    return POISON_VALUE;
}
