#ifndef IMPROVER_HEADER
#define IMPROVER_HEADER

errors_t improover_of_expression ( tree_t* ptr_tree );
node_t*  improver_of_node   ( node_t* node, int* amount_of_improves );
double   get_value          ( double left, double right, operations_t operation );

#endif
