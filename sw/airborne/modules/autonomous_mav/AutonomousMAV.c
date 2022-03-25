//
// Created by theo on 23-03-22.
//

#include "AutonomousMAV.h"


void heartbeat(struct MAV *self){

    // Variable to see whether the 10 minutes are over TODO: still needs work (use flight time?)
    bool timerIsDone = false;

    // Variable containing information whether system is in guided flight mode
//    bool inGuidedFlightMode = guidance_h.mode != GUIDANCE_H_MODE_GUIDED;
    bool inGuidedFlightMode = true;

    // Create local variable for the next state
    enum STATE nextState = self->currentState;

    // Print a few status messages
    fprintf(stderr, "\n--------------- Status -----------------\n");
    fprintf(stderr, "Autopilot mode: %d\n", autopilot.mode);
    fprintf(stderr, "Current guided mode: %d\n", guidance_h.mode);
    fprintf(stderr, "Current position (%d-%d) at %d meters\n\n",
            self->navigator->getPosition(self->navigator).x,
            self->navigator->getPosition(self->navigator).y,
            self->navigator->getPosition(self->navigator).alt);

    // Handle the state based actions and transitions
    switch (self->currentState) {

        case BOOT:

            // Make sure nothing is running
            self->navigator->stop(self->navigator);

            // Do not do anything in boot state, just transition to startup state
            nextState = STARTUP;
            break;

        case STARTUP:

            // Check for transitions
            if(self->aircraftHasError(self)){
                // If aircraft has a problem, go to fatal error directly
                nextState = FATAL_ERROR;
            } else {

                // Check if GPS signal is there and valid for 3D
                if(GpsFixValid()){

                    // After performing startup operations, directly transition to takeoff state
                    nextState = TAKEOFF;

                }

                // TODO: start the timer to measure when flight time is over

            }

            break;

        case FATAL_ERROR:
            // State based actions in fatal error
            break;

        case TAKEOFF:

            // State based action in takeoff
            if(self->aircraftHasError(self) || !inGuidedFlightMode)
                nextState = FATAL_ERROR;

            // If the autopilot is in flight mode, switch to normal maneuvers
            if(autopilot.in_flight && (self->navigator->getPosition(self->navigator).alt > TARGET_FLIGHT_ALTITUDE))
                nextState = NORMAL_MOVEMENT;

        case LANDING:
            // State based actions in state landing
            break;

        case NORMAL_MOVEMENT:
            // Compute route
            self->navigator->computePath(self->navigator, self->detector->getObstacleMap(self->detector));
            // State based action in normal movement
//            if(self->aircraftHasError(self) || timerIsDone)
//                nextState = LANDING;
//            if(self->detector->obstacleAhead(self->detector))
//                nextState = EVASIVE_MOVEMENT;
//            if(self->detector->outOfArena(self->detector))
//                nextState = REENTER_MOVEMENT;

            if(!inGuidedFlightMode)
                nextState = FATAL_ERROR;

            break;

        case REENTER_MOVEMENT:
            // Compute reenter route TODO: there should be another endpoint for computing reentry path
            self->navigator->computePath(self->navigator, self->detector->getObstacleMap(self->detector));
            if(!self->detector->outOfArena(self->detector))
                nextState = NORMAL_MOVEMENT;
            if(!inGuidedFlightMode)
                nextState = FATAL_ERROR;
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
            if(!inGuidedFlightMode)
                nextState = FATAL_ERROR;
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
        fprintf(stderr, "\n------------ Sate Transition --------------\n");
        fprintf(stderr, "Autonomous MAV changed state to: %d\n", nextState);

        // Handle all entry actions of the following state
        switch (nextState) {

            case BOOT:
                break;

            case STARTUP:

                // Initialize the navigation unit
                self->navigator->start(self->navigator);

                // Initialize the obstacle detector
                self->detector->init(self->detector);

                break;

            case FATAL_ERROR:

                // Land the aircraft immediately
                self->navigator->land(self->navigator);

                // Stop the navigator
                self->navigator->stop(self->navigator);

                break;

            case TAKEOFF:

                // Take off with the aircraft
                self->navigator->takeoff(self->navigator);

                break;

            case LANDING:

                // Initiate landing of the aircraft
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

    // Increment the number of system iterations by one
    self->iterationCounter = self->iterationCounter + 1;

}

int aircraftHasError(struct MAV *self){

    // TODO: check whether the aircraft is experiencing any errors

    return 0;
}

struct MAV createMAV(struct Navigator * nav, struct ObstacleDetector * detector){

    // Create an instance of type MAV
    struct MAV instance;

    // Initialize the attributes of the system
    instance.currentState = BOOT;
    instance.iterationCounter = 0;
    instance.navigator = nav;
    instance.detector = detector;

    // Append all methods to the instance
    instance.heartbeat = heartbeat;
    instance.aircraftHasError = aircraftHasError;

    // Return the instance to the calling block
    return instance;

}