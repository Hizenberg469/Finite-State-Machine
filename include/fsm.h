#ifndef __FSM__
#define __FSM__


/***Constants***/


#define space "    " // 4 ws

#define MAX_INP_BUFFER_LEN          128
#define MAX_TRANSITION_TABLE_SIZE   128
#define MAX_STATE_NAME_SIZE         32
#define MAX_FSM_NAME_SIZE           32
#define MAX_TRANSITION_KEY_SIZE     64
#define MAX_ALP_BUFFER_SIZE         30
#define MAX_FINAL_STATE             128
#define MAX_NUM_OF_STATES           128
#define MAX_FSM_OUTPUT_BUFFER       1024


/***Data structures and custom datatypes***/

typedef struct fsm_ fsm_t;
typedef struct state_ state_t;
typedef struct fsm_output_buff_ fsm_output_buff_t;

/**Function Pointer**/

typedef void (*output_fn)(state_t *, state_t *,
                            char *, unsigned int,
                            fsm_output_buff_t *);

/*Custom-defined datatype
* to define boolean value
* for FSM*/
typedef enum{

    FSM_FALSE,
    FSM_TRUE

}fsm_bool_t;

typedef enum{

    FSM_NO_TRANSITION,
    FSM_NO_ERROR

}fsm_error_t;


/*This data structure act similar 
* to the behaviour of transition 
* function.*/
typedef struct tt_entry_{

    /* The input symbol */
    char transition_key[MAX_TRANSITION_KEY_SIZE];

    /* Size of the input symbol */
    unsigned int transition_key_size;

    /*Callback to execute logic to output value from fsm*/
    output_fn outp_fn;

    /* Next state: The state which is the output
    *  of transition function, i.e. 
    *  f(state_i, input_symbol) = (state_i+1)*/
    state_t *next_state;

}tt_entry_t;

typedef struct tt_{
    tt_entry_t  tt_entry[MAX_TRANSITION_TABLE_SIZE];
}tt_t;

struct state_{

    /*Name/identifier of the state.
    * This should be unique*/
    char state_name[MAX_STATE_NAME_SIZE];

    /*Transition table of the state*/
    tt_t state_trans_table;

    /*Boolean value to distinguish b/w 
    * accept/final state or vice-versa*/
    fsm_bool_t is_final;

    /*FSM which the current state belongs to*/
    char fsm[MAX_FSM_NAME_SIZE];
};

struct fsm_output_buff_{
    char output_buffer[MAX_FSM_OUTPUT_BUFFER];
    unsigned int curr_pos;
};

struct fsm_{

    /*Initial state of FSM to start with*/
    state_t *initial_state;

    /*Number of States*/
    state_t *states[MAX_NUM_OF_STATES];

    /*Number of states in FSM*/
    unsigned int state_count;

    /*Set of alphabet*/
    char alphabet[MAX_ALP_BUFFER_SIZE];

    /*Count of No. of alphabets*/
    unsigned int alphabet_count;

    /*Set of final/accept states*/
    char final_states[MAX_FINAL_STATE];

    /*Name of FSM*/
    char fsm_name[MAX_FSM_NAME_SIZE];

    /*Input data fed by application to parse
    * by FSM library*/
    char input_buffer[MAX_INP_BUFFER_LEN];

    /*Size of input buffer*/
    unsigned int input_buffer_size;

    /*Pointer which point to the current input
    * from input buffer*/
    unsigned int input_buffer_cursor;

    /* If FSM need to produce some output, the output
    * data shall be stored in this buffer*/
    fsm_output_buff_t fsm_output_buff;
};


/***Function Prototypes***/

fsm_t*
create_new_fsm(const char *fsm_name, const char *inp_alpha, unsigned int alpha_count);

state_t*
create_new_state(const char *state_name,
                    fsm_bool_t is_final,
                    const char *fsm_name,
                    fsm_t *fsm);

void
set_fsm_initial_state(fsm_t *fsm, state_t *state);

tt_entry_t*
create_and_insert_new_tt_entry(tt_t *trans_table,
                                const char *transition_key,
                                unsigned int sizeof_key,
                                output_fn outp_fn,
                                state_t* next_state);


tt_entry_t*
get_next_empty_tt_entry(tt_t *trans_table);

fsm_error_t
execute_fsm(fsm_t *fsm,
            char *input_buffer,
            unsigned int size,
            fsm_output_buff_t *output_buffer,
            fsm_bool_t *fsm_result);


//Generic printing function for printing fsm.
void
print_fsm(fsm_t *fsm);

void
init_fsm_output_buffer(fsm_output_buff_t *fsm_output_buff);


/**static(hidden) functions***/
static inline fsm_bool_t
is_tt_entry_empty(tt_entry_t *tt_entry){

    if( tt_entry != NULL && !tt_entry->next_state)
        return FSM_TRUE;

    return FSM_FALSE;
}



/***Iterators' Macro***/

#define FSM_ITERATE_TRANS_TABLE_BEGIN(tr_table_ptr, tt_entry_ptr)   \
    do{                                                             \
        unsigned int index = 0;                                     \
        for( ; index < MAX_TRANSITION_TABLE_SIZE ; index++ ){       \
            tt_entry_ptr = &(tr_table_ptr->tt_entry[index]);

#define FSM_ITERATE_TRANS_TABLE_END(tr_table_ptr, tt_entry_ptr)     \
        }                                                           \
    }while(0);

#endif