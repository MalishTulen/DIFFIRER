#ifndef DIFFIRER_HEADER
#define DIFFIRER_HEADER


const int       POISON_VALUE        = -66;
const int       MAX_NAME_LEN        = 30;
const int       DATA_CAPACITY       = 300;
const int       MAX_INPUT_LENGTH    = 50;
const int       CMD_COMMAND_LEN     = 50;
const int       VARS_ARRAY_CAPACITY = 20;
const long long POISON_PTR          = 0xD01BAEB7;

enum errors_t
{
    DONE                        = 334,
    BAD_DATA_PTR                = 1,
    BAD_TREE_PTR                = 2,
    SIZE_UNDER_PLINTUS          = 3,
    CYCLIC_LEFT                 = 4,
    CYCLIC_RIGHT                = 5,
    BAD_INPUT_CODE              = 6,
    BAD_OPERATION_TYPE          = 7,
    BAD_PTR                     = 8,
    STOP                        = 69,
    SYNTAX_ERROR                = 1488,
    BAD_ENDING                  = -1488,
};

enum leaf_t
{
    LEAF     = 5,
    NOT_LEAF = 10,
};

enum operations_t
{
    POISON_OPERATION  = -1,
    ADD               = 1,
    SUB               = 2,
    MUL               = 3,
    DIV               = 4,
    EXP               = 5,
    SQR               = 6,
    SIN               = 7,
    COS               = 8,
    TG                = 9,
    CTG               = 10,

};

union object_t
{
    double          constant;
    operations_t    operation;
    char            var;
};

enum type_t
{
    NUM             = 1,
    OP              = 2,
    VAR             = 3,
    POISON_TYPE     = -6,
};

struct node_t
{
    type_t          type;
    object_t        object;
    node_t*         left;
    node_t*         right;
};

struct tree_t
{
    node_t*         array_data;
    char*           vars_array;
    const char*     operators_array;
    int             size;
    node_t*         g_root;
};

struct input_file_t
{
    char*           data;
    long            size;
};

const node_t ERROR_NODE = { .type = POISON_TYPE };
#define CHECK( ptr )                                                                                               \
        if ( tree_checker ( ptr ) == BAD_DATA_PTR )                                                                \
        {                                                                                                          \
            fprintf ( stderr, "BAD_DATA_PTR '%s' in file %s, line %d\n", #ptr, __FILE__, __LINE__ );               \
            return BAD_DATA_PTR;                                                                                   \
        }                                                                                                          \
        if ( tree_checker ( ptr ) == BAD_TREE_PTR )                                                                \
        {                                                                                                          \
            fprintf ( stderr, "BAD_TREE_PTR '%s' in file %s, line %d\n", #ptr, __FILE__, __LINE__ );               \
            return BAD_TREE_PTR;                                                                                   \
        }                                                                                                          \
        if ( tree_checker ( ptr ) == SIZE_UNDER_PLINTUS )                                                          \
        {                                                                                                          \
            fprintf ( stderr, "BAD_SIZE '%s' in file %s, line %d\n", #ptr, __FILE__, __LINE__ );                   \
            return SIZE_UNDER_PLINTUS;                                                                             \
        }                                                                                                          \
        if ( tree_checker ( ptr ) == CYCLIC_LEFT )                                                                 \
        {                                                                                                          \
            fprintf ( stderr, "CYCLIC_LEFT '%s' in file %s, line %d\n", #ptr, __FILE__, __LINE__ );                \
            return CYCLIC_LEFT;                                                                                    \
        }                                                                                                          \
        if ( tree_checker ( ptr ) == CYCLIC_RIGHT )                                                                \
        {                                                                                                          \
            fprintf ( stderr, "CYCLIC_RIGHT '%s' in file %s, line %d\n", #ptr, __FILE__, __LINE__ );               \
            return CYCLIC_RIGHT;                                                                                   \
        }                                                                                                          \
        if ( tree_checker ( ptr ) == BAD_OPERATION_TYPE )                                                          \
        {                                                                                                          \
            fprintf ( stderr, "BAD_OPERATION_TYPE '%s' in file %s, line %d\n", #ptr, __FILE__, __LINE__ );         \
            return BAD_OPERATION_TYPE;                                                                             \
        }

#define SYNTAX_ERROR( input_data, p )                                                                              \
        printf ( "SYNTAX ERROR in index %d ( '%c' )!\n", *p, input_data [ *p ] );                                  \
        abort ();

errors_t start_programm     ();
errors_t read_file          ( tree_t* tree, int* amount_of_pictures );
errors_t tree_ctor          ( tree_t* ptr_tree );
errors_t tree_checker       ( tree_t* ptr );
leaf_t   check_if_leaf      ( node_t* ptr_node );
errors_t reader             ( FILE* ptr_input_file, input_file_t* ptr_input_data );
node_t*  create_new_node    ( tree_t* ptr_tree, type_t type, object_t object, node_t* left, node_t* right );
errors_t array_dump         ( tree_t* tree, node_t* node );
errors_t improover_of_expression ( tree_t* ptr_tree );
node_t*  improver_of_node   ( node_t* node, int* amount_of_improves );
errors_t delete_node        ( node_t* node );
double   get_value          ( double left, double right, operations_t operation );
errors_t differer           ( tree_t* tree );
node_t*  differenciation    ( tree_t* tree, node_t* node );
errors_t get_string         ( tree_t* tree );
node_t*  get_symbol         ( node_t* node );
errors_t printf_operation   ( node_t* node );
node_t*  copy_sons_balls    ( tree_t* tree, node_t* node );

#endif
