//
// Created by theo on 23-03-22.
//

#include "AutonomousMAV.h"


void heartbeat(struct MAV *self){

    // Variable to see whether the 10 minutes are over TODO: still needs work
    int timerIsDone = 0;

    // Create local variable for the next state
    enum STATE nextState = self->currentState;

    // Handle the state based actions and transitions
    switch (self->currentState) {

        case STARTUP:
            // Initialize the navigation unit
            self->navigator->start(self->navigator);
            // Initialize the obstacle detector
            self->detector->init(self->detector);

            // Check for transitions
            if(self->aircraftHasError(self)){
                // If aircraft has a problem, go to fatal error directly
                nextState = FATAL_ERROR;
            } else {
                // After performing startup operations, directly transition to takeoff state
                nextState = TAKEOFF;

                // TODO: start the timer to measure when flight time is over

            }

            break;

        case FATAL_ERROR:
            // State based actions in fatal error
            break;

        case TAKEOFF:

            // TODO: remove this later!! just for demo purposes
            self->detector->obstacleAhead(self->detector);

            // State based action in takeoff
            if(self->aircraftHasError(self))
                nextState = FATAL_ERROR;
            if(self->navigator->getState(self->navigator) != NAV_NORMAL)
                nextState = NORMAL_MOVEMENT;

        case LANDING:
            // State based actions in state landing
            break;

        case NORMAL_MOVEMENT:
            // Compute route
            self->navigator->computePath(self->navigator, self->detector->getObstacleMap(self->detector));
            // State based action in normal movement
            if(self->aircraftHasError(self) || timerIsDone)
                nextState = LANDING;
            if(self->detector->obstacleAhead(self->detector))
                nextState = EVASIVE_MOVEMENT;
            if(self->detector->outOfArena(self->detector))
                nextState = REENTER_MOVEMENT;
            break;

        case REENTER_MOVEMENT:
            // Compute reenter route TODO: there should be another endpoint for computing reentry path
            self->navigator->computePath(self->navigator, self->detector->getObstacleMap(self->detector));
            if(!self->detector->outOfArena(self->detector))
                nextState = NORMAL_MOVEMENT;
            break;

        case EVASIVE_MOVEMENT:
            // Compute path
            self->navigator->computePath(self->navigator, self->detector->getObstacleMap(self->detector));
            // State transitions
            if(self->aircraftHasError(self) || timerIsDone)
                nextState = LANDING;
            if(!self->detector->obstacleAhead(self->detector) && !(self->aircraftHasError(self)))
                nextState = NORMAL_MOVEMENT;
            if(!self->detector->obstacleAhead(self->detector) && self->detector->outOfArena(self->detector) && !(self->aircraftHasError(self)))
                nextState = REENTER_MOVEMENT;
            break;

        case DONE:
            // Nothing to do in state done
            break;

        default:
            // If the state is unknown go to the fatal error state
            nextState = FATAL_ERROR;
            break;

    }

    if(self->currentState != nextState){

        // Log the transition of MAV states
        fprintf(stderr, "Autonomous MAV changed state: %d\n", nextState);

        // Handle all entry actions of the following state
        switch (nextState) {

            case STARTUP:
                break;

            case FATAL_ERROR:
                self->navigator->land(self->navigator);
                self->navigator->stop(self->navigator);
                break;

            case TAKEOFF:
                self->navigator->takeoff(self->navigator);
                break;

            case LANDING:
                self->navigator->land(self->navigator);
                break;

            case NORMAL_MOVEMENT:
                break;

            case EVASIVE_MOVEMENT:
                break;

            case REENTER_MOVEMENT:
                break;

            case DONE:
                break;

        }

        // Perform the state transition in the new state
        self->currentState = nextState;
    }

}

int aircraftHasError(struct MAV *self){

    // TODO: check whether the aircraft is experiencing any errors

    return 0;
}

struct MAV createMAV(struct Navigator * nav, struct ObstacleDetector * detector){

    // Create an instance of type MAV
    struct MAV instance;

    // Initialize the attributes of the system
    instance.currentState = STARTUP;
    instance.navigator = nav;
    instance.detector = detector;

    // Append all methods to the instance
    instance.heartbeat = heartbeat;
    instance.aircraftHasError = aircraftHasError;

    // Return the instance to the calling block
    return instance;

}