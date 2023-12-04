
// Computes the steering angle of the wheels based on the steering sensor input and range of wheel turn
void fsteeringAngle(float steering_sensor_voltage) {
    float oMax = 5.0;  // V
    float oMin = 0.0;  // V
    float nMax = pi / 4.0;  // rad
    float nMin = -pi / 4.0;  // rad
    float oRange = (oMax - oMin);
    float nRange = (nMax - nMin);
    float steering_angle;
    steering_angle = (((steering_sensor_voltage - oMin) * nRange) / oRange) + nMin + 7*pi/180;  // rad
    if (steering_angle < 10*(pi/180) & steering_angle > -10*(pi/180)){
      steering_angle = 0.0;
    }
    return steering_angle;
}