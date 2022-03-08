//
// Created by Theo Rieken on 08.03.22.
//

#ifndef PAPARAZZI_BASICDETECTOR_H
#define PAPARAZZI_BASICDETECTOR_H


class BasicDetector : public ObstacleDetector {

public:

    void init() override;
    bool obstacleAhead() override;
    bool** getObstacleMap() override;
    double** getDistanceMap() override;
    bool hasError() override;

};


#endif //PAPARAZZI_BASICDETECTOR_H
