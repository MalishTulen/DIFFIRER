#ifndef GRAFIC_PRINT_HEADER
#define GRAFIC_PRINT_HEADER

errors_t    make_grafic_dump    ( tree_t* ptr_data, int* amount_of_pictures );
errors_t    create_dot_code     ( tree_t* ptr_data, FILE* ptr_dot_file );
errors_t    make_picture        ( const char* dot_file, const char* output_file, int* amount_of_pictures );
errors_t    branch_connector    ( node_t* ptr_index, tree_t* ptr_data, FILE* ptr_dot_file );

#endif
