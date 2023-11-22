
// Computes the steering angle of the wheels based on the steering sensor input and range of wheel turn
void fsteeringAngle() {
    float oMax = 5.0;  // V
    float oMin = 0.0;  // V
    float nMax = pi / 4.0;  // rad
    float nMin = -pi / 4.0;  // rad
    float oRange = (oMax - oMin);
    float nRange = (nMax - nMin);
    steeringAngle = (((steeringVolts - oMin) * nRange) / oRange) + nMin + 7*pi/180;  // rad
    if (steeringAngle < 10*(pi/180) & steeringAngle > -10*(pi/180)){
      steeringAngle = 0.0;
    }
}