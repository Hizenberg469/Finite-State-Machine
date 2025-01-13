#include <stdio.h>
#include <memory.h>
#include <fsm.h>

int main(){

    fsm_t* fsm = create_new_fsm("FSM_1");

    state_t* initial_state = create_new_state("q0", FSM_FALSE);
    set_fsm_initial_state(fsm, initial_state);

    

    return 0;
}