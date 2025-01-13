#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "fsm.h"

fsm_t*
create_new_fsm(const char* fsm_name)
{

    fsm_t *fsm = calloc(1, sizeof(fsm_t));
    strncpy(fsm->fsm_name, fsm_name, MAX_FSM_NAME_SIZE - 1 );
    fsm->fsm_name[MAX_FSM_NAME_SIZE] = '\0';
    return fsm;

}

state_t*
create_new_state(char *state_name,
                    fsm_bool_t is_final){

    assert(state_name);

    state_t *state = calloc(1, sizeof(state_t));

    strncpy(state->state_name, state_name, MAX_STATE_NAME_SIZE - 1);
    state->state_name[MAX_STATE_NAME_SIZE] = '\0';

    tt_t *trans_table = &(state->state_trans_table);
    tt_entry_t *tt_entry_ptr = NULL;
    
    FSM_ITERATE_TRANS_TABLE_BEGIN(trans_table, tt_entry_ptr){
        tt_entry_ptr->next_state = NULL;
    }FSM_ITERATE_TRANS_TABLE_END(trans_table, tt_entry_ptr)

    state->is_final = is_final;

    return state;
}

void
set_fsm_initial_state(fsm_t *fsm, state_t *state){

    assert(!fsm->initial_state);
    fsm->initial_state = state;
}


tt_entry_t*
get_next_empty_tt_entry(tt_t *trans_table){

    tt_entry_t* tt_entry_ptr = NULL;

    assert(trans_table);

    FSM_ITERATE_TRANS_TABLE_BEGIN(trans_table, tt_entry_ptr){
        if( is_tt_entry_empty(tt_entry_ptr) == FSM_TRUE )
                break;
    }FSM_ITERATE_TRANS_TABLE_END(trans_table, tt_entry_ptr)


    if(is_tt_entry_empty(tt_entry_ptr) == FSM_TRUE)
        return tt_entry_ptr;

    return NULL;
}

tt_entry_t*
create_and_insert_new_tt_entry(tt_t *trans_table,
                                char *transition_key,
                                unsigned int sizeof_key,
                                state_t* next_state){

    
    assert( sizeof transition_key < MAX_TRANSITION_KEY_SIZE);

    tt_entry_t *tt_entry_ptr = get_next_empty_tt_entry(trans_table);

    if(!tt_entry_ptr){
        printf("FATAL : Transition Table is Full\n");
        return NULL;
    }

    memcpy(tt_entry_ptr->transition_key , transition_key, sizeof_key);
    tt_entry_ptr->transition_key[sizeof_key] = '\0';
    tt_entry_ptr->transition_key_size = sizeof_key;
    tt_entry_ptr->next_state = next_state;

    return tt_entry_ptr;
}