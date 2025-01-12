#ifndef __FSM__
#define __FSM__

typedef struct fsm_ fsm_t;
typedef struct state_ state_t;

#define MAX_INP_BUFFER_SIZE         128
#define MAX_TRANSITION_TABLE_SIZE   128
#define MAX_STATE_NAME_SIZE         32
#define MAX_FSM_NAME_SIZE           32
#define MAX_TRANSITION_KEY_SIZE     64

/*Custom-defined datatype
* to define boolean value
* for FSM*/
typedef enum{

    FSM_FALSE,
    FSM_TRUE

}fsm_bool_t;


/*This data structure act similar 
* to the behaviour of transition 
* function.*/
typedef struct tt_entry_{

    /* The input symbol */
    char transition_key[MAX_TRANSITION_KEY_SIZE];

    /* Size of the input symbol */
    unsigned int transition_key_size;

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
};

struct fsm_{

    /*Initial state of FSM to start with*/
    state_t *initial_state;

    /*Name of FSM*/
    char fsm_name[MAX_FSM_NAME_SIZE];

    /*Input data fed by application to parse
    * by FSM library*/
    unsigned int input_buffer_size;

    /*Pointer which point to the current input
    * from input buffer*/
    unsigned int input_buffer_cursor;
};

#endif