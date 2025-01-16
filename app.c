#include <stdio.h>
#include <memory.h>
#include <string.h>
#include "fsm.h"

#define MAX_FSM_STATE 24

#define modify_string(state,num) #state ## #num
#define modify_var(state,num) state ## num

state_t *fsm_states[MAX_FSM_STATE];


void print_fsm(fsm_t *fsm){

    printf("FSM name : %s\n", fsm->fsm_name);
    printf("States : ");
    char state[6];
    for(unsigned int i = 0 ; i < fsm->state_count ; i++ ){
        sprintf(state,"q%1d",i);
        printf("%s", state);

        if( i < fsm->state_count - 1 )
            printf(", ");
    }
    putchar('\n');

    printf("Alphabets : %s\n", fsm->alphabet);
    printf("Initial State : %s\n", fsm->initial_state->state_name);
    printf("Final States : %s\n", fsm->final_states);

    printf("Transition Table :\n");

    printf("        |   1       0   \n");
    printf("------------------------\n");
    
    
    char state1[6] = {0};
    char state2[6] = {0};
    char state3[6] = {0};

    tt_t *table_ptr = NULL;
    tt_entry_t *entry_ptr = NULL;
    for( unsigned int i = 0 ; i < fsm->state_count ; i++ ){
    
        strncpy(state1, fsm_states[i]->state_name, 
                strlen(fsm_states[i]->state_name));

        table_ptr = &(fsm_states[i]->state_trans_table);
        FSM_ITERATE_TRANS_TABLE_BEGIN(table_ptr, entry_ptr){
            
            if( is_tt_entry_empty(entry_ptr) == FSM_FALSE ){
                if( strcmp(entry_ptr->transition_key, "1") == 0 ){
                    strncpy(state2, entry_ptr->next_state->state_name,
                        strlen(entry_ptr->next_state->state_name));
                }
                else{
                    strncpy(state3, entry_ptr->next_state->state_name,
                        strlen(entry_ptr->next_state->state_name));
                }
            }

        }FSM_ITERATE_TRANS_TABLE_END(table_ptr, entry_ptr)

        printf("  %s    |   %s      %s  \n", state1, state2, state3);
   
    }

}

int main(){

    fsm_t *fsm = create_new_fsm("FSM_1", "0,1");

    //States
    state_t *q0 = create_new_state("q0", FSM_FALSE, "FSM_1", fsm);
    state_t *q1 = create_new_state("q1", FSM_TRUE, "FSM_1", fsm);
    state_t *q2 = create_new_state("q2", FSM_TRUE, "FSM_1", fsm);
    state_t *q3 = create_new_state("q3", FSM_FALSE, "FSM_1", fsm);

    fsm_states[0] = q0;
    fsm_states[1] = q1;
    fsm_states[2] = q2;
    fsm_states[3] = q3;

    set_fsm_initial_state(fsm, q0);

    //State q0
    tt_t *trans_table = &(q0->state_trans_table);
    create_and_insert_new_tt_entry(trans_table,
                                    "0",
                                    1,
                                    q2);
    
    create_and_insert_new_tt_entry(trans_table,
                                    "1",
                                    1,
                                    q1);

    //State q1
    trans_table = &(q1->state_trans_table);
    create_and_insert_new_tt_entry(trans_table,
                                    "0",
                                    1,
                                    q2);
    
    create_and_insert_new_tt_entry(trans_table,
                                    "1",
                                    1,
                                    q3);

    //State q2
    trans_table = &(q2->state_trans_table);
    create_and_insert_new_tt_entry(trans_table,
                                    "0",
                                    1,
                                    q3);

    create_and_insert_new_tt_entry(trans_table,
                                    "1",
                                    1,
                                    q1);
    
    //State q3
    trans_table = &(q3->state_trans_table);
    create_and_insert_new_tt_entry(trans_table,
                                    "0",
                                    1,
                                    q3);
    
    create_and_insert_new_tt_entry(trans_table,
                                    "1",
                                    1,
                                    q3);


    print_fsm(fsm);

    return 0;
}