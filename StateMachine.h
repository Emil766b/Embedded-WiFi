/* 
 * File:   StateMachine.h
 * Author: alpin
 *
 * Created on 6. januar 2022, 08:47
 */

#ifndef STATEMACINE_H
#define	STATEMACINE_H

// Forskellige states
typedef enum
{
    Idle_State,
    Second_Idle_State,
    CWMODE_State,
    CWJAP_State,
    IP_Read_State,
    TCP_State,
    Maxcon_State,
    Server_State,
    HTML_State,
    ATSEND1_State,
    Header_State,
    ATSEND2_State,
    Web_State,
    Close_State,
                 
} state_t;

typedef struct {
    state_t currState;
} stateMachine_t;

// Event typer
typedef enum
{
    Any_Event,
    Ready_Event,
    ok_Event,
    error_Event,
    Fail_Event,
    Connect_Event,
    Send_Event
               
} event_t;

void StateMachine_Init(stateMachine_t * stateMachine);
void StateMachine_RunIteration(stateMachine_t *stateMachine, event_t event);
const char * StateMachine_GetStateName(state_t state);

#endif	/* STATEMACINE_H */

