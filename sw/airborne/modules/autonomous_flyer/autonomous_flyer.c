/*
 * Copyright (C) Group 10
 *
 * TU Delft, 04. Mar 2022
 */
/**
 * @file "modules/autonomous_flyer/autonomous_flyer.c"
 * @author Group 10
 *
 * This is the core of the greatest autonomous flight module every created in paparazzi. The architecture is built in
 * a finite state machine, that has two inflight states "Normal Movement" and "Evasive Maneuver" in which two main
 * modules are working: Navigator and ObstacleDetector.
 *
 * TODO:
 *  - How do we proceed with C++/C only modules? Is paparazzi able to compile C++?
 *  - Need to organize the software differently in paparazzi modules
 *  - Data exchange via ABI messages
 *  - Do we need a datalink or event function? Data exchange? Need to rethink architecture
 *
 */

#include "modules/autonomous_flyer/autonomous_flyer.h"
#include "firmwares/rotorcraft/navigation.h"
#include "generated/airframe.h"
#include "state.h"
#include "subsystems/abi.h"
#include <time.h>
#include <stdio.h>

// needed to get the nav functions like Inside...
#define NAV_C
#include "generated/flight_plan.h"

#define AUTONOMOUS_FLYER_VERBOSE TRUE

#define PRINT(string,...) fprintf(stderr, "[autonomous_flyer->%s()] " string,__FUNCTION__ , ##__VA_ARGS__)
#if AUTONOMOUS_FLYER_VERBOSE
#define VERBOSE_PRINT PRINT
#else
#define VERBOSE_PRINT(...)
#endif

enum mav_state_t {
    STARTUP,
    FATAL_ERROR,
    TAKEOFF,
    LANDING,
    NORMAL_MOVEMENT,
    EVASIVE_ACTION,
    DONE
};

// define and initialise global variables
enum navigation_state_t currentState = STARTUP;

/*
 * Define ABI messaging event: TODO: define structure / type of events
 */
#ifndef AUTONOMOUS_FLYER_VISUAL_DETECTION_ID
#define AUTONOMOUS_FLYER_VISUAL_DETECTION_ID ABI_BROADCAST
#endif


/**
 * Function that is called once for initialization
 */
void autonomous_flyer_init(void){

    // TODO: Perform initialization actions (before state machine starts)

}

/**
 * Function that is called periodically (for FSM logic)
 */
void autonomous_flyer_periodic(void){

    // Temporarily define action variables that are later replaced by module activity
    bool startupOkay = false;
    bool hasError = false;
    bool takeoffSuccessful = false;
    bool timerIsDone = false;
    bool obstacleAhead = false;
    bool freeOfObstacles = false;

    // Create local variable for the next state (to catch transitions)
    navigation_state_t nextState = currentState;

    // Handle state based actions and transitions
    switch (currentState){
        case STARTUP:
            // State based action in startup
            if(hasError){
                nextState = FATAL_ERROR;
            }
            if(startupOkay){
                nextState = TAKEOFF;
            }
            break;
        case FATAL_ERROR:
            // State based action in fatal error
            break;
        case TAKEOFF:
            // State based action in takeoff
            if(hasError){
                nextState = FATAL_ERROR;
            }
            if(takeoffSuccessful){
                nextState = NORMAL_MOVEMENT;
            }
            break;
        case LANDING:
            // State based action in landing
            break;
        case NORMAL_MOVEMENT:
            // State based action in normal movement
            if(hasError || timerIsDone){
                nextState = LANDING;
            }
            if(obstacleAhead){
                nextState = EVASIVE_ACTION;
            }
            break;
        case EVASIVE_ACTION:
            // State based action in evasive action
            if(hasError || timerIsDone){
                nextState = LANDING;
            }
            if(freeOfObstacles){
                nextState = NORMAL_MOVEMENT;
            }
            break;
        case DONE:
            // State based action in done
            break;
        default:
            // If for any reason one ends up here, go to fatal error
            nextState = FATAL_ERROR;
            break;
    }

    // Handle transition based actions and entry actions
    if(currentState != nextState){
        switch (currentState){
            case STARTUP:
                // Entry actions for state STARTUP
                if(nextState == FATAL_ERROR){
                    // Transition based actions from startup to fatal error
                }
                if(nextState == TAKEOFF){
                    // TODO: start the timer
                }
                break;
            case FATAL_ERROR:
                // Entry actions for state FATAL_ERROR
                break;
            case TAKEOFF:
                // Entry actions for state TAKEOFF
                if(nextState == NORMAL_MOVEMENT){
                    // Transition based actions from takeoff to normal movement
                }
                break;
            case LANDING:
                // Entry actions for state LANDING
                if(nextState == FATAL_ERROR){
                    // Transition based actions from landing to fatal error
                }
                if(nextState == DONE){
                    // Transition based actions from landing to done
                }
                break;
            case NORMAL_MOVEMENT:
                // Entry actions for state NORMAL_MOVEMENT
                if(nextState == EVASIVE_ACTION){
                    // Transition based actions from normal movement to evasive action
                }
                if(nextState == LANDING){
                    // Transition based actions from normal movement to landing
                }
                break;
            case EVASIVE_ACTION:
                // Entry actions for state EVASIVE_ACTION
                if(nextState == NORMAL_MOVEMENT){
                    // Transition based actions from evasive action to normal movement
                }

                break;
            default:
                // If for any reason one ends up here, go to fatal error
                nextState = FATAL_ERROR;
                break;
        }
    }

    return;
}
