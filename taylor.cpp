#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <direct.h>
#include <assert.h>
#include <ctype.h>

#include "diffirer.h"
#include "tree.h"
#include "taylor.h"
#include "improver.h"
#include "grafic_print.h"

errors_t taylor ( tree_t* tree, int* amount_of_pictures, int decomposition_number )
{
    node_t* tree_copy = tree->g_root;
    node_t* mul_node = create_new_node ( tree, NUM, {.constant = 1}, NULL, NULL );
    for ( int i = 0; i < decomposition_number + 1; i++ )
    {
        tree_copy = copy_sons_balls ( tree, tree_copy );

        copy_sons_balls ( tree, mul_node );
        mul_node = create_new_node ( tree, OP, {.operation = MUL}, mul_node,  )

        int improving = 1;
        int amount_of_improves = 0;

        while ( improving )
        {
            amount_of_improves = 0;

            improver_of_node ( tree_copy, &amount_of_improves );

            //printf ( "amount of improves: %d\n", amount_of_improves );

            if ( amount_of_improves == 0 )
                improving = 0;
        }
        make_grafic_dump ( tree, amount_of_pictures );

        differenciation ( tree, tree_copy );
    }

    return DONE;
}

int factorial ( int number )
{
    if ( number == 1 )
        return 1;
    else
        return number*( factorial ( number - 1 ) );
}
