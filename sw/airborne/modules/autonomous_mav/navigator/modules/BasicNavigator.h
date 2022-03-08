//
// Created by Theo Rieken on 08.03.22.
//

#ifndef PAPARAZZI_BASICNAVIGATOR_H
#define PAPARAZZI_BASICNAVIGATOR_H

#include "../Navigator.h"

class BasicNavigator : public Navigator {

private:

    int a = 0;

public:

    BasicNavigator();
    void start() override;
    void stop() override;
    NAV_STATE getState() override;
    void computePath(bool** obstacleMap, double** distanceMap) override;
    bool hasError() override;
    void takeoff() override;
    void land() override;

};


#endif //PAPARAZZI_BASICNAVIGATOR_H
