#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <assert.h>
#include "fsm.h"

fsm_t *
create_new_fsm(const char* fsm_name, const char *inp_alpha, unsigned int alpha_count)
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
    fsm->alphabet_count = alpha_count;

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
                                output_fn outpfn,
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
    tt_entry_ptr->outp_fn = outpfn;

    return tt_entry_ptr;
}


static state_t *
transition(state_t *curr_state, char *input_buffer, unsigned int *inp_ptr,
            fsm_output_buff_t *output_buffer){

    assert(curr_state); // To check if curr_state is not NULL.
    assert(input_buffer); // To check input_buffer is not NULL.

    unsigned int rem_len_inp_buf = strlen(input_buffer);

    tt_t *trans_table = &(curr_state->state_trans_table);

    tt_entry_t *entry_ptr = NULL;

    state_t *next_state = NULL;
    FSM_ITERATE_TRANS_TABLE_BEGIN( trans_table, entry_ptr ){
        if( entry_ptr->transition_key_size <= rem_len_inp_buf &&
            strncmp(entry_ptr->transition_key,
            input_buffer + *inp_ptr,
            entry_ptr->transition_key_size) == 0 &&
            is_tt_entry_empty(entry_ptr) == FSM_FALSE ){

            next_state = entry_ptr->next_state;

            if( entry_ptr->outp_fn ){
                entry_ptr->outp_fn(curr_state,
                                    next_state,
                                    input_buffer + *inp_ptr,
                                    entry_ptr->transition_key_size,
                                    output_buffer);
            }
            *inp_ptr += entry_ptr->transition_key_size;
            break;
        }
    }FSM_ITERATE_TRANS_TABLE_END( trans_table, entry_ptr)

    return next_state;
}


fsm_error_t
execute_fsm(fsm_t *fsm,
            char *input_buffer,
            unsigned int size,
            fsm_output_buff_t *output_buffer, /*output buffer to write data into*/
            fsm_bool_t *fsm_result){

    fsm_error_t algo_output = FSM_NO_ERROR;

    char *buffer_to_parse;
    unsigned int input_buffer_len = 0; // length of input buffer being used
    fsm->input_buffer_cursor = 0;
    
    state_t *curr_state = NULL; /*Pointer to the current state
                                    of given FSM*/
    curr_state = fsm->initial_state;
    
    state_t *next_state = NULL;

    /*FSM has a problem.
      It don't has any initial
      state.*/

    if( curr_state == NULL ){
        printf("FSM is not programmed correctly.\n \
FSM don't have an initial state.\n");
        algo_output = FSM_NO_TRANSITION;
        
        return algo_output;
    }

    /*Use buffer sent by applicaton*/
    if( input_buffer && size > 0 ){
        buffer_to_parse = input_buffer;
        input_buffer_len = size;
    }
    else{
    /*Use buffer set to fsm by application*/
        buffer_to_parse = fsm->input_buffer;
        input_buffer_len = fsm->input_buffer_size;
    }


    if( input_buffer_len != strlen(buffer_to_parse) ){
        
        printf("Size of input buffer given and actual size of input buffer is different\n");

        algo_output = FSM_NO_TRANSITION;
        return algo_output;
    }

    /*If application has not supplied output buffer,
    * Use FSM's internal output buffer*/
    if(!output_buffer){
        output_buffer = &fsm->fsm_output_buff;
    }

    init_fsm_output_buffer(output_buffer);

    while( fsm->input_buffer_cursor < input_buffer_len ){

        next_state = transition(curr_state, buffer_to_parse, &fsm->input_buffer_cursor,
                                output_buffer);  // Transition function
                                                            // to get to next state
                                                            // for current input 
                                                            // symbol if it exist.

        if( next_state == NULL ){
            printf("Somthing went wrong!!\n \
The next state for the current state (%s) and remaining input \
string (%s) with current input pointer at (%u) is NULL\n \
This means either there is no transition for the current input \
symbol or the next state for the current input symbol is NULL\n \
For both case, the FSM is programmed not correctly.\n", curr_state->state_name,
                                                        buffer_to_parse + fsm->input_buffer_cursor,
                                                        fsm->input_buffer_cursor);
            
            algo_output = FSM_NO_TRANSITION;
            return algo_output;
        }

        //update...
        curr_state = next_state;
        next_state = NULL;

    }

    //Now, the input buffer is processed without any issue.
    //Therefore, there is no transition error.

    algo_output = FSM_NO_ERROR;
    //Checking if the curr_state is accept state or not.

    if( curr_state->is_final == FSM_TRUE){
        if( fsm_result )
            *fsm_result = FSM_TRUE;
    }
    else{
        if( fsm_result )
            *fsm_result = FSM_FALSE;
    }

    return algo_output;
}

void
print_fsm(fsm_t *fsm){

    char state[6];
    char input[fsm->alphabet_count][6];
    char next_state[fsm->alphabet_count + 1][6];

    memset(state, '\0', sizeof state);
    for(unsigned int i = 0 ; i < fsm->alphabet_count ; i++ ){
        memset(input[i], '\0', sizeof input[i]);
        memset(next_state[i], '\0', sizeof next_state[i]);
    }

    char alpha[strlen(fsm->alphabet)+1];
    memset(alpha , 0, strlen(fsm->alphabet)+1);
    strncpy(alpha, fsm->alphabet, strlen(fsm->alphabet)+1);
    int i = 0;
    char *ch = NULL;
    ch = strtok(alpha, ",");
    do{
        strncpy(input[i++],ch,strlen(ch)); 
    }while( (ch = strtok(NULL, ",")) );
    
    printf("FSM name : %s\n", fsm->fsm_name);
    printf("Alphabets : %s\n", fsm->alphabet);
    printf("States : ");
    for(unsigned int i = 0 ; i < fsm->state_count ; i++ ){
        sprintf(state,"q%d",i);
        printf("%s", state);

        if( i < fsm->state_count - 1 )
            printf(", ");
    }
    putchar('\n');

    printf("Initial State : %s\n", fsm->initial_state->state_name);
    printf("Final States : %s\n", fsm->final_states);

    printf("Transition Table :\n");

    //Header line
    printf("         "space"|");
    for(unsigned int i = 0 ; i < fsm->alphabet_count ; i++ ){
        printf(space"%5s",input[i]);
    }
    putchar('\n');
    printf("--------------");
    for(unsigned int i = 0 ; i < fsm->alphabet_count ; i++ )
        printf("---------");
    printf("---------\n");
    
    

    tt_t *table_ptr = NULL;
    tt_entry_t *entry_ptr = NULL;
    for( unsigned int i = 0 ; i < fsm->state_count ; i++ ){
    
        strncpy(next_state[0], fsm->states[i]->state_name, 
                strlen(fsm->states[i]->state_name));

        table_ptr = &(fsm->states[i]->state_trans_table);
        FSM_ITERATE_TRANS_TABLE_BEGIN(table_ptr, entry_ptr){
            
            if( is_tt_entry_empty(entry_ptr) == FSM_FALSE ){
                for(unsigned int j = 0 ; j < fsm->alphabet_count ; j++ ){
                    if( strncmp(entry_ptr->transition_key, input[j],
                                entry_ptr->transition_key_size) == 0){
                        
                        strncpy(next_state[j+1], entry_ptr->next_state->state_name,
                                strlen(entry_ptr->next_state->state_name));
                    }
                }
            }

        }FSM_ITERATE_TRANS_TABLE_END(table_ptr, entry_ptr)

        printf(space"%5s"space"|", next_state[0]);
        memset(next_state[0], '\0', sizeof state[0]);
        for(unsigned int k = 0 ; k < fsm->alphabet_count ; k++ ){
            printf(space"%5s", next_state[k+1]);
            memset(next_state[k+1], '\0', sizeof next_state[k+1]);
        }
        putchar('\n');
   
    }

}


void
init_fsm_output_buffer(fsm_output_buff_t *fsm_output_buff){
    memset(fsm_output_buff->output_buffer, 0, MAX_FSM_OUTPUT_BUFFER);
    fsm_output_buff->curr_pos = 0;
}