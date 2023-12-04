/**
 * Function to calculate the total torque based on the throttle sensor voltage.
 * 
 * @param throttle_sensor_voltage The voltage reading from the throttle sensor.
 * @param oMax The maximum voltage output from the throttle sensor.
 * @param oMin The minimum voltage output from the throttle sensor.
 * @param nMax The maximum torque output from the motor.
 * @param nMin The minimum torque output from the motor.
 * @param oRange The range of the throttle sensor voltage.
 * @param nRange The range of the torque output.
 * @return torque_outputThe calculated torque output in newton meters
 */
float TotalTorque(float throttle_sensor_voltage){
    
    float oMax = 5.0;  // V
    float oMin = 0.0;  // V
    float nMax = 16.0;  // Nm
    float nMin = 0.0;  // Nm
    float oRange = (oMax - oMin);
    float nRange = (nMax - nMin);
    return (((throttle_sensor_voltage - oMin) * nRange) / oRange) + nMin;
}