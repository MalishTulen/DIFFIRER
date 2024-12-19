#ifndef REK_SPUSK_HEADER
#define REK_SPUSK_HEADER

errors_t create_tree_from_input_data ( input_file_t* ptr_input_data, tree_t* ptr_tree );
node_t*  get_dollar           ( input_file_t* ptr_input_data, tree_t* ptr_tree, int* p );
node_t*  get_sum              ( input_file_t* ptr_input_data, tree_t* ptr_tree, int* p );
node_t*  get_mul              ( input_file_t* ptr_input_data, tree_t* ptr_tree, int* p );
node_t*  get_brackets         ( input_file_t* ptr_input_data, tree_t* ptr_tree, int* p );
node_t*  get_number           ( input_file_t* ptr_input_data, tree_t* ptr_tree, int* p );
node_t*  get_exp              ( input_file_t* input_data, tree_t* tree, int* p );
node_t*  get_log_or_sin_or_cos              ( input_file_t* input_data, tree_t* tree, int* p );
#endif
