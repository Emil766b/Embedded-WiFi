/* 
 * File:   main.h
 * Author: alpin
 *
 * Created on 6. januar 2022, 08:43
 */

#include "mcc_generated_files/mcc.h"
#include "string.h"
#include "stdio.h"
#include "ctype.h"
#include "I2C_header.h"
#include "function.h"
#include "main.h"
#include "StateMachine.h"


typedef struct {
    state_t currState;
    event_t event;
    state_t nextState;
} stateTransMatrixRow_t;

// State Matrix
static stateTransMatrixRow_t stateTransMatrix[] = {
    { Idle_State,  Ready_Event,   CWMODE_State},
    { CWMODE_State,  ok_Event,   CWJAP_State},
    { CWMODE_State,  error_Event,   CWMODE_State},
    { CWJAP_State,   ok_Event,      IP_Read_State},
    { CWJAP_State,  Fail_Event,   CWMODE_State},
    { IP_Read_State,    ok_Event,    TCP_State},
    { TCP_State,    ok_Event,    Maxcon_State},
    { Maxcon_State,    ok_Event,    Server_State},
    { Server_State,    ok_Event,     Second_Idle_State},
    { Second_Idle_State,    Connect_Event,    ATSEND1_State},
    { ATSEND1_State,   ok_Event,    Header_State},
    { Header_State,    Send_Event,    ATSEND2_State},
    { ATSEND2_State,    ok_Event,    HTML_State},
    { HTML_State,    ok_Event,    Close_State},
    { Close_State,    ok_Event,    Second_Idle_State},
    
};

typedef struct {
    const char * name;
    void (*func)(void);
} stateFunctionRow_t;

// State funktioner
static stateFunctionRow_t stateFunctionA[] = {
    { "Idle_State",     &Idle },
    { "Second_Idle_State",     &Second_Idle },
    { "CWMODE_State",     &CWMODE },
    { "CWJAP_State",     &CWJAP },
    { "IP_Read_State",     &IP_Read },
    { "TCP_State",  &TCP },
    { "Maxcon_State",      &Maxcon},
    { "Server_State",      &Server },
    { "HTML_State",      &HTML },
    { "ATSEND1_State",      &ATSEND1},
    { "Header_State",      &Header },
    { "ATSEND2_State",      &ATSEND2},
    { "Web_State",      &Web},
    { "Close_State",      &Close},
};

void StateMachine_Init(stateMachine_t * stateMachine) {
    stateMachine->currState = Idle_State;
}


void StateMachine_RunIteration(stateMachine_t *stateMachine, event_t event) {

    // Kør igennem state matrix og events
    for(int i = 0; i < sizeof(stateTransMatrix)/sizeof(stateTransMatrix[0]); i++) {
        if(stateTransMatrix[i].currState == stateMachine->currState) {
            if(stateTransMatrix[i].event == event) 
            {

                // Vælg næste state
                stateMachine->currState =  stateTransMatrix[i].nextState;

                // Kald funktionen ved den state
                (stateFunctionA[stateMachine->currState].func)();
                break;
            }
        }
    }
}

const char * StateMachine_GetStateName(state_t state) {
    return stateFunctionA[state].name;
}

