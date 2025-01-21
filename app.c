#include <stdio.h>
#include <memory.h>
#include <string.h>
#include "fsm.h"

#define modify_string(state,num) #state ## #num
#define modify_var(state,num) state ## num



fsm_t *
fsm_end_with_strict_pattern_01_or_10(){

    fsm_t *fsm = create_new_fsm("FSM_1", "0,1",2);

    //States
    state_t *q0 = create_new_state("q0", FSM_FALSE, "FSM_1", fsm);
    state_t *q1 = create_new_state("q1", FSM_TRUE, "FSM_1", fsm);
    state_t *q2 = create_new_state("q2", FSM_TRUE, "FSM_1", fsm);
    state_t *q3 = create_new_state("q3", FSM_FALSE, "FSM_1", fsm);

    fsm->states[0] = q0;
    fsm->states[1] = q1;
    fsm->states[2] = q2;
    fsm->states[3] = q3;

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

    return fsm;
}

fsm_t *
fsm_end_with_00_or_11(){

    fsm_t *fsm = create_new_fsm("FSM_2", "0,1",2);

    state_t *q0 = create_new_state("q0", FSM_FALSE, "FSM_2", fsm);
    state_t *q1 = create_new_state("q1", FSM_FALSE, "FSM_2", fsm);
    state_t *q2 = create_new_state("q2", FSM_FALSE, "FSM_2", fsm);
    state_t *q3 = create_new_state("q3", FSM_TRUE, "FSM_2", fsm);
    state_t *q4 = create_new_state("q4", FSM_TRUE, "FSM_2", fsm);

    fsm->states[0] = q0;
    fsm->states[1] = q1;
    fsm->states[2] = q2;
    fsm->states[3] = q3;
    fsm->states[4] = q4;

    set_fsm_initial_state(fsm, q0);

    tt_t *trans_table = NULL;
    //State q0
    trans_table = &(q0->state_trans_table);
    create_and_insert_new_tt_entry(trans_table, "0", 1, q1);
    create_and_insert_new_tt_entry(trans_table, "1", 1, q2);

    //State q1
    trans_table = &(q1->state_trans_table);
    create_and_insert_new_tt_entry(trans_table, "0", 1, q3);
    create_and_insert_new_tt_entry(trans_table, "1", 1, q2);

    //State q2
    trans_table = &(q2->state_trans_table);
    create_and_insert_new_tt_entry(trans_table, "0", 1, q1);
    create_and_insert_new_tt_entry(trans_table, "1", 1, q4);

    //State q3
    trans_table = &(q3->state_trans_table);
    create_and_insert_new_tt_entry(trans_table, "0", 1, q3);
    create_and_insert_new_tt_entry(trans_table, "1", 1, q2);

    //State q4
    trans_table = &(q4->state_trans_table);
    create_and_insert_new_tt_entry(trans_table, "0", 1, q1);
    create_and_insert_new_tt_entry(trans_table, "1", 1, q4);
    
    return fsm;
}

fsm_t *
fsm_odd_1(){

    fsm_t *fsm = create_new_fsm("FSM_3", "0,1",2);

    state_t *q0 = create_new_state("q0", FSM_FALSE, "FSM_3", fsm);
    state_t *q1 = create_new_state("q1", FSM_TRUE, "FSM_3", fsm);

    fsm->states[0] = q0;
    fsm->states[1] = q1;

    set_fsm_initial_state(fsm, q0);

    tt_t *trans_table = NULL;
    //State q0
    trans_table = &(q0->state_trans_table);
    create_and_insert_new_tt_entry(trans_table, "0", 1, q0);  
    create_and_insert_new_tt_entry(trans_table, "1", 1, q1);

    //State q1
    trans_table = &(q1->state_trans_table);
    create_and_insert_new_tt_entry(trans_table, "0", 1, q1);  
    create_and_insert_new_tt_entry(trans_table, "1", 1, q0);

    return fsm;  
}

fsm_t * 
fsm_balanced_paranthesis_3_levels(){
    fsm_t *fsm = create_new_fsm("FSM_4", "(,)",2);


    state_t *q0 = create_new_state("q0", FSM_FALSE, "FSM_4", fsm);
    state_t *q1 = create_new_state("q1", FSM_FALSE, "FSM_4", fsm);
    state_t *q2 = create_new_state("q2", FSM_FALSE, "FSM_4", fsm);
    state_t *q3 = create_new_state("q3", FSM_FALSE, "FSM_4", fsm);
    state_t *q4 = create_new_state("q4", FSM_FALSE, "FSM_4", fsm);
    state_t *q5 = create_new_state("q5", FSM_FALSE, "FSM_4", fsm);
    state_t *q6 = create_new_state("q6", FSM_TRUE, "FSM_4", fsm);
    state_t *q7 = create_new_state("q7", FSM_FALSE, "FSM_4", fsm);
    state_t *q8 = create_new_state("q8", FSM_FALSE, "FSM_4", fsm);
    state_t *q9 = create_new_state("q9", FSM_FALSE, "FSM_4", fsm);
    state_t *q10 = create_new_state("q10", FSM_FALSE, "FSM_4", fsm);
    state_t *q11 = create_new_state("q11", FSM_TRUE, "FSM_4", fsm);
    state_t *q12 = create_new_state("q12", FSM_FALSE, "FSM_4", fsm);
    state_t *q13 = create_new_state("q13", FSM_FALSE, "FSM_4", fsm);
    state_t *q14 = create_new_state("q14", FSM_TRUE, "FSM_4", fsm);
    state_t *q15 = create_new_state("q15", FSM_FALSE, "FSM_4", fsm);
    state_t *q16 = create_new_state("q16", FSM_FALSE, "FSM_4", fsm);
    state_t *q17 = create_new_state("q17", FSM_FALSE, "FSM_4", fsm);

    set_fsm_initial_state(fsm, q0);

    fsm->states[0] = q0;
    fsm->states[1] = q1;
    fsm->states[2] = q2;
    fsm->states[3] = q3;
    fsm->states[4] = q4;
    fsm->states[5] = q5;
    fsm->states[6] = q6;
    fsm->states[7] = q7;
    fsm->states[8] = q8;
    fsm->states[9] = q9;
    fsm->states[10] = q10;
    fsm->states[11] = q11;
    fsm->states[12] = q12;
    fsm->states[13] = q13;
    fsm->states[14] = q14;
    fsm->states[15] = q15;
    fsm->states[16] = q16;
    fsm->states[17] = q17;

    tt_t *trans_table = NULL;

    //State q0 (initial state)
    trans_table = &(q0->state_trans_table);
    create_and_insert_new_tt_entry(trans_table, "(", 1, q1);
    create_and_insert_new_tt_entry(trans_table, ")", 1, q17);

    //State q1
    trans_table = &(q1->state_trans_table);
    create_and_insert_new_tt_entry(trans_table, "(", 1, q2);
    create_and_insert_new_tt_entry(trans_table, ")", 1, q11);

    //State q2
    trans_table = &(q2->state_trans_table);
    create_and_insert_new_tt_entry(trans_table, "(", 1, q3);
    create_and_insert_new_tt_entry(trans_table, ")", 1, q13);

    //State q3
    trans_table = &(q3->state_trans_table);
    create_and_insert_new_tt_entry(trans_table, "(", 1, q17);
    create_and_insert_new_tt_entry(trans_table, ")", 1, q4);

    //State q4
    trans_table = &(q4->state_trans_table);
    create_and_insert_new_tt_entry(trans_table, "(", 1, q17);
    create_and_insert_new_tt_entry(trans_table, ")", 1, q5);

    //State q5
    trans_table = &(q5->state_trans_table);
    create_and_insert_new_tt_entry(trans_table, "(", 1, q17);
    create_and_insert_new_tt_entry(trans_table, ")", 1, q6);

    //State q6
    trans_table = &(q6->state_trans_table);
    create_and_insert_new_tt_entry(trans_table, "(", 1, q7);
    create_and_insert_new_tt_entry(trans_table, ")", 1, q17);

    //State q7
    trans_table = &(q7->state_trans_table);
    create_and_insert_new_tt_entry(trans_table, "(", 1, q8);
    create_and_insert_new_tt_entry(trans_table, ")", 1, q17);

    //State q8
    trans_table = &(q8->state_trans_table);
    create_and_insert_new_tt_entry(trans_table, "(", 1, q9);
    create_and_insert_new_tt_entry(trans_table, ")", 1, q17);

    //State q9
    trans_table = &(q9->state_trans_table);
    create_and_insert_new_tt_entry(trans_table, "(", 1, q17);
    create_and_insert_new_tt_entry(trans_table, ")", 1, q10);

    //State q10
    trans_table = &(q10->state_trans_table);
    create_and_insert_new_tt_entry(trans_table, "(", 1, q17);
    create_and_insert_new_tt_entry(trans_table, ")", 1, q5);

    //State q11
    trans_table = &(q11->state_trans_table);
    create_and_insert_new_tt_entry(trans_table, "(", 1, q12);
    create_and_insert_new_tt_entry(trans_table, ")", 1, q17);

    //State q12
    trans_table = &(q12->state_trans_table);
    create_and_insert_new_tt_entry(trans_table, "(", 1, q17);
    create_and_insert_new_tt_entry(trans_table, ")", 1, q11);

    //State q13
    trans_table = &(q13->state_trans_table);
    create_and_insert_new_tt_entry(trans_table, "(", 1, q17);
    create_and_insert_new_tt_entry(trans_table, ")", 1, q14);

    //State q14
    trans_table = &(q14->state_trans_table);
    create_and_insert_new_tt_entry(trans_table, "(", 1, q15);
    create_and_insert_new_tt_entry(trans_table, ")", 1, q17);

    //State q15
    trans_table = &(q15->state_trans_table);
    create_and_insert_new_tt_entry(trans_table, "(", 1, q16);
    create_and_insert_new_tt_entry(trans_table, ")", 1, q17);

    //State q16
    trans_table = &(q16->state_trans_table);
    create_and_insert_new_tt_entry(trans_table, "(", 1, q17);
    create_and_insert_new_tt_entry(trans_table, ")", 1, q13);

    //State q17
    trans_table = &(q17->state_trans_table);
    create_and_insert_new_tt_entry(trans_table, "(", 1, q17);
    create_and_insert_new_tt_entry(trans_table, ")", 1, q17);

    return fsm;
}


fsm_t *
fsm_phone_validation_6_digits(){

    fsm_t *fsm = create_new_fsm("FSM_5", "0,1,2,3,4,5,6,7,8,9",10);

    state_t *q0 = create_new_state("q0", FSM_FALSE, "FSM_5", fsm);
    state_t *q1 = create_new_state("q1", FSM_FALSE, "FSM_5", fsm);
    state_t *q2 = create_new_state("q2", FSM_FALSE, "FSM_5", fsm);
    state_t *q3 = create_new_state("q3", FSM_FALSE, "FSM_5", fsm);
    state_t *q4 = create_new_state("q4", FSM_FALSE, "FSM_5", fsm);
    state_t *q5 = create_new_state("q5", FSM_FALSE, "FSM_5", fsm);
    state_t *q6 = create_new_state("q6", FSM_TRUE, "FSM_5", fsm);
    state_t *q7 = create_new_state("q7", FSM_FALSE, "FSM_5", fsm); // Dead state

    set_fsm_initial_state(fsm, q0);

    fsm->states[0] = q0;
    fsm->states[1] = q1;
    fsm->states[2] = q2;
    fsm->states[3] = q3;
    fsm->states[4] = q4;
    fsm->states[5] = q5;
    fsm->states[6] = q6;
    fsm->states[7] = q7;

    tt_t *trans_table = NULL;
    //State q0
    trans_table = &(q0->state_trans_table);
    create_and_insert_new_tt_entry(trans_table, "0", 1, q7);
    create_and_insert_new_tt_entry(trans_table, "1", 1, q1);
    create_and_insert_new_tt_entry(trans_table, "2", 1, q1);
    create_and_insert_new_tt_entry(trans_table, "3", 1, q1);
    create_and_insert_new_tt_entry(trans_table, "4", 1, q1);
    create_and_insert_new_tt_entry(trans_table, "5", 1, q1);
    create_and_insert_new_tt_entry(trans_table, "6", 1, q1);
    create_and_insert_new_tt_entry(trans_table, "7", 1, q1);
    create_and_insert_new_tt_entry(trans_table, "8", 1, q1);
    create_and_insert_new_tt_entry(trans_table, "9", 1, q1);

    for(int i = 1 ; i <= 6 ; i++){

        trans_table = &(fsm->states[i]->state_trans_table);
        create_and_insert_new_tt_entry(trans_table, "0", 1, fsm->states[i+1]);
        create_and_insert_new_tt_entry(trans_table, "1", 1, fsm->states[i+1]);
        create_and_insert_new_tt_entry(trans_table, "2", 1, fsm->states[i+1]);
        create_and_insert_new_tt_entry(trans_table, "3", 1, fsm->states[i+1]);
        create_and_insert_new_tt_entry(trans_table, "4", 1, fsm->states[i+1]);
        create_and_insert_new_tt_entry(trans_table, "5", 1, fsm->states[i+1]);
        create_and_insert_new_tt_entry(trans_table, "6", 1, fsm->states[i+1]);
        create_and_insert_new_tt_entry(trans_table, "7", 1, fsm->states[i+1]);
        create_and_insert_new_tt_entry(trans_table, "8", 1, fsm->states[i+1]);
        create_and_insert_new_tt_entry(trans_table, "9", 1, fsm->states[i+1]);

    }


    trans_table = &(fsm->states[7]->state_trans_table);
    create_and_insert_new_tt_entry(trans_table, "0", 1, fsm->states[7]);
    create_and_insert_new_tt_entry(trans_table, "1", 1, fsm->states[7]);
    create_and_insert_new_tt_entry(trans_table, "2", 1, fsm->states[7]);
    create_and_insert_new_tt_entry(trans_table, "3", 1, fsm->states[7]);
    create_and_insert_new_tt_entry(trans_table, "4", 1, fsm->states[7]);
    create_and_insert_new_tt_entry(trans_table, "5", 1, fsm->states[7]);
    create_and_insert_new_tt_entry(trans_table, "6", 1, fsm->states[7]);
    create_and_insert_new_tt_entry(trans_table, "7", 1, fsm->states[7]);
    create_and_insert_new_tt_entry(trans_table, "8", 1, fsm->states[7]);
    create_and_insert_new_tt_entry(trans_table, "9", 1, fsm->states[7]);
    return fsm;
}

int main(){

    
    fsm_t *fsm1 = fsm_end_with_strict_pattern_01_or_10();
    print_fsm(fsm1);

    fsm_t *fsm2 = fsm_end_with_00_or_11();
    print_fsm(fsm2);

    fsm_t *fsm3 = fsm_odd_1();
    print_fsm(fsm3);

    fsm_t *fsm4 = fsm_balanced_paranthesis_3_levels();
    print_fsm(fsm4);

    fsm_t *fsm5 = fsm_phone_validation_6_digits();
    print_fsm(fsm5);

    /*FSM Algorithm execution*/

    // fsm_bool_t fsm_result;
    // fsm_error_t fsm_error;

    // fsm_error = execute_fsm(fsm1,
    //                 "00",
    //                 strlen("00"),
    //                 &fsm_result);
    
    // if(fsm_error == FSM_NO_ERROR ){
    //     if( fsm_result == FSM_TRUE ){
    //         printf("Input String is Validated\n");
    //     }
    //     else{
    //         printf("Input String is not Valid\n");
    //     }
    // }

    return 0;
}