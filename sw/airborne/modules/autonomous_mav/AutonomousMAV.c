#include "AutonomousMAV.h"


void heartbeat(struct MAV *self){

    // Compute the time difference between start and now to get when 10 minutes are done
//    std::chrono::steady_clock::time_point currentTime = std::chrono::steady_clock::now();
//    int elapsedMinutes = std::chrono::duration_cast<std::chrono::minutes>(currentTime - this->operationStarted).count();
//    bool timerIsDone = elapsedMinutes > 10;
    int timerIsDone = 1;

    // Create local variable for the next state (to catch transitions)
    enum STATE nextState = self->currentState;

    // Handle state based actions and transitions
    switch (self->currentState){

        case STARTUP:
            // Initialize the navigation unit
            self->navigator->start(self->navigator);
            // Initialize the obstacle detector
            self->detector->init(self->detector);

            // Check for transitions
            if(self->aircraftHasError(self))
                nextState = FATAL_ERROR;

            // After performing startup operations, directly transition to takeoff state
            nextState = TAKEOFF;
            break;

        case FATAL_ERROR:
            // State based action in fatal error
            break;

        case TAKEOFF:
            // State based action in takeoff
            if(self->aircraftHasError(self))
                nextState = FATAL_ERROR;
            if(self->navigator->getState(self->navigator) != NAV_NORMAL)
                nextState = NORMAL_MOVEMENT;
            break;

        case LANDING:
            // State based action in landing
            break;

        case NORMAL_MOVEMENT:
            // Compute route
            self->navigator->computePath(self->navigator, self->detector->getObstacleMap(self->detector), self->detector->getDistanceMap(self->detector));
            // State based action in normal movement
            if(self->aircraftHasError(self) || timerIsDone)
                nextState = LANDING;
            if(self->detector->obstacleAhead(self->detector))
                nextState = EVASIVE_ACTION;
            break;

        case EVASIVE_ACTION:
            // Compute route
            self->navigator->computePath(self->navigator, self->detector->getObstacleMap(self->detector), self->detector->getDistanceMap(self->detector));
            // State based action in evasive action
            if(self->aircraftHasError(self) || timerIsDone)
                nextState = LANDING;
            if(!self->detector->obstacleAhead(self->detector) && (!self->aircraftHasError(self)))
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
    if(self->currentState != nextState){
        switch (self->currentState){

            case STARTUP:
                // Check for transition based actions
                if(nextState == FATAL_ERROR){
                    // Transition based actions from startup to fatal error
                }
                if(nextState == TAKEOFF){
                    // Start the timer
                    // TODO: solve issues with this
//                    self->timeWhenStarted = std::chrono::steady_clock::now();
                }
                break;

            case FATAL_ERROR:
                // Entry actions for state FATAL_ERROR
                self->navigator->land(self->navigator);
                self->navigator->stop(self->navigator);
                break;

            case TAKEOFF:
                // Entry actions for state TAKEOFF
                self->navigator->takeoff(self->navigator);
                // Check for transitions
                if(nextState == NORMAL_MOVEMENT){
                    // Transition based actions from takeoff to normal movement
                }
                break;

            case LANDING:
                // Entry actions for state LANDING
                self->navigator->land(self->navigator);
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
        self->currentState = nextState;
    }

}

int aircraftHasError(struct MAV *self){

    // Initiate the error variable with false
    int hasError = 0;

    // TODO: do some internal checking

    // Check whether navigator has outch
    if(self->navigator->hasError(self->navigator))
        hasError = 1;

    // Check whether obstacle detection module has outch
    if(self->detector->hasDetectionError(self->detector))
        hasError = 1;

    return hasError;

}

struct MAV createMAV(struct Navigator * nav, struct ObstacleDetector * detector){
    // Create an object of it
    struct MAV instance;

    // Do initialization of object
    instance.currentState = STARTUP;
    instance.navigator = nav;
    instance.detector = detector;

    // Append all methods to the struct
    instance.heartbeat = heartbeat;
    instance.aircraftHasError = aircraftHasError;

    // Return the instance
    return instance;
}