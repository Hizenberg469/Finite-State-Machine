#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <assert.h>
#include "fsm.h"

fsm_t *
create_new_fsm(const char* fsm_name, const char *inp_alpha)
{

    fsm_t *fsm = calloc(1, sizeof(fsm_t));
    strncpy(fsm->fsm_name, fsm_name, strlen(fsm_name) );
    fsm->fsm_name[strlen(fsm_name)] = '\0';
    fsm->state_count = 0;
    
    strncpy(fsm->alphabet, inp_alpha, strlen(inp_alpha));
    fsm->alphabet[strlen(inp_alpha)] = '\0';

    memset(fsm->final_states, 0, 1);
    memset(fsm->input_buffer, 0, 1);
    fsm->input_buffer_cursor = 0;
    fsm->input_buffer_size = 0;
    return fsm;

}

state_t *
create_new_state(const char *state_name,
                    fsm_bool_t is_final,
                    const char *fsm_name,
                    fsm_t *fsm){

    assert(state_name);

    state_t *state = calloc(1, sizeof(state_t));

    strncpy(state->state_name, state_name, strlen(state_name));
    state->state_name[strlen(state_name)] = '\0';

    tt_t *trans_table = &(state->state_trans_table);
    tt_entry_t *tt_entry_ptr = NULL;
    
    FSM_ITERATE_TRANS_TABLE_BEGIN(trans_table, tt_entry_ptr){
        tt_entry_ptr->next_state = NULL;
    }FSM_ITERATE_TRANS_TABLE_END(trans_table, tt_entry_ptr)

    state->is_final = is_final;

    char temp_state_name[strlen(state_name)+2];
    strncpy(temp_state_name, state_name, strlen(state_name));
    temp_state_name[strlen(state_name)] = '\0';

    if(state->is_final == FSM_TRUE){
        strcat(temp_state_name,",");
        strcat(fsm->final_states,temp_state_name);
    }
    // fsm->final_states[strlen(fsm->final_states)+2] = '\0';

    strncpy(state->fsm, fsm_name, strlen(fsm_name));
    state->fsm[strlen(fsm_name)] = '\0';

    fsm->state_count++;

    return state;
}

void
set_fsm_initial_state(fsm_t *fsm, state_t *state){

    assert(!fsm->initial_state);
    fsm->initial_state = state;
}


tt_entry_t *
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

tt_entry_t *
create_and_insert_new_tt_entry(tt_t *trans_table,
                                const char *transition_key,
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