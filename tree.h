#ifndef TREE_HEADER
#define TREE_HEADER

errors_t tree_ctor          ( tree_t* ptr_tree );
errors_t tree_checker       ( tree_t* ptr );
leaf_t   check_if_leaf      ( node_t* ptr_node );
errors_t reader             ( FILE* ptr_input_file, input_file_t* ptr_input_data );
node_t*  create_new_node    ( tree_t* ptr_tree, type_t type, object_t object, node_t* left, node_t* right );
errors_t array_dump         ( tree_t* tree, node_t* node );
errors_t delete_sub_tree    ( node_t* node );
errors_t delete_node        ( node_t* node );
node_t*  copy_sons_balls    ( tree_t* tree, node_t* node );

#endif
