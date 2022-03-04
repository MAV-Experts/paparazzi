/*
 * Copyright (C) Group 10
 *
 * TU Delft, 04. Mar 2022
 */
/**
 * @file "modules/autonomous_mav/AutonomousMAV.cpp"
 * @author Group 10
 *
 * This is the core of the greatest autonomous flight module every created in paparazzi. The architecture is built in
 * a finite state machine, that has two inflight states "Normal Movement" and "Evasive Maneuver" in which two main
 * modules are working: Navigator and ObstacleDetector.
 *
 * TODO:
 *  - Data exchange via ABI messages between the modules
 *  - Do we need a datalink or event function? Data exchange?
 *  - Consider using a set of detectors (maybe for different things?)
 */

#include "modules/autonomous_mav/AutonomousMAV.h"
#include "AutonomousMAV.h"

// TODO: why so ugly!? BAD STYLEE!!
#define NAV_C
#include "generated/flight_plan.h"

#define AUTONOMOUS_MAV_VERBOSE TRUE

#define PRINT(string,...) fprintf(stderr, "[autonomous_mav->%s()] " string,__FUNCTION__ , ##__VA_ARGS__)
#if AUTONOMOUS_MAV_VERBOSE
#define VERBOSE_PRINT PRINT
#else
#define VERBOSE_PRINT(...)
#endif

/*
 * Define ABI messaging event: TODO: define structure / type of events
 */
#ifndef AUTONOMOUS_MAV_VISUAL_DETECTION_ID
#define AUTONOMOUS_MAV_VISUAL_DETECTION_ID ABI_BROADCAST
#endif

/**
 * Constructor method of the Autonomous MAV
*/
void AutonomousMav::AutonomousFlyer(){

    // Set the initial state to startup
    this->currentState = STARTUP;

    // TODO: replace with implementation of navigator
    this->navigationUnit = new Navigator();

    // TODO: replace with implementation of obstacle detector
    this->detector = new ObstacleDetector();

}

/**
 * Heartbeat method that is periodically called to run state machine
*/
void AutonomousMav::heartbeat(){

    // Compute the time difference between start and now to get when 10 minutes are done
    std::chrono::steady_clock::time_point currentTime = std::chrono::steady_clock::now();
    int elapsedMinutes = std::chrono::duration_cast<std::chrono::minutes>(currentTime - this->operationStarted).count();
    bool timerIsDone = elapsedMinutes > 10;

    // Create local variable for the next state (to catch transitions)
    STATE nextState = this->currentState;

    // Handle state based actions and transitions
    switch (this->currentState){

        case STARTUP:
            // Initialize the navigation unit
            this->navigationUnit->start();
            // Initialize the obstacle detector
            this->detector->init();

            // Check for transitions
            if(this->aircraftHasError())
                nextState = FATAL_ERROR;

            // After performing startup operations, directly transition to takeoff state
            nextState = TAKEOFF;
            break;

        case FATAL_ERROR:
            // State based action in fatal error
            break;

        case TAKEOFF:
            // State based action in takeoff
            if(this->aircraftHasError())
                nextState = FATAL_ERROR;
            if(this->navigationUnit->getState() != NAV_NORMAL)
                nextState = NORMAL_MOVEMENT;
            break;

        case LANDING:
            // State based action in landing
            break;

        case NORMAL_MOVEMENT:
            // Compute route
            this->navigationUnit->computePath(this->detector->getObstacleMap(), this->detector->getDistanceMap());
            // State based action in normal movement
            if(this->aircraftHasError() || timerIsDone)
                nextState = LANDING;
            if(this->detector->obstacleAhead())
                nextState = EVASIVE_ACTION;
            break;

        case EVASIVE_ACTION:
            // Compute route
            this->navigationUnit->computePath(this->detector->getObstacleMap(), this->detector->getDistanceMap());
            // State based action in evasive action
            if(this->aircraftHasError() || timerIsDone)
                nextState = LANDING;
            if(!this->detector->obstacleAhead() && (!this->aircraftHasError()))
                nextState = NORMAL_MOVEMENT;
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
    if(this->currentState != nextState){
        switch (currentState){

            case STARTUP:
                // Check for transition based actions
                if(nextState == FATAL_ERROR){
                    // Transition based actions from startup to fatal error
                }
                if(nextState == TAKEOFF){
                    // Start the timer
                    this->operationStarted = std::chrono::steady_clock::now();
                }
                break;

            case FATAL_ERROR:
                // Entry actions for state FATAL_ERROR
                this->navigationUnit->land();
                this->navigationUnit->stop();
                break;

            case TAKEOFF:
                // Entry actions for state TAKEOFF
                this->navigationUnit->takeoff();
                // Check for transitions
                if(nextState == NORMAL_MOVEMENT){
                    // Transition based actions from takeoff to normal movement
                }
                break;

            case LANDING:
                // Entry actions for state LANDING
                this->navigationUnit->land();
                // Check for transitions
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

        // Update the state of the MAV
        this->currentState = nextState;
    }

}

/**
 * Method returns whether the aircraft is experiencing an error
 *
 * @return MAV experiencing an error
 */
bool AutonomousMav::aircraftHasError(){

    // Initiate the error variable with false
    bool hasError = false;

    // TODO: do some internal checking

    // Check whether navigator has outch
    if(this->navigationUnit.hasError())
        hasError = true;

    // Check whether obstacle detection module has outch
    if(this->detector.hasError())
        hasError = true;

    return hasError;

}