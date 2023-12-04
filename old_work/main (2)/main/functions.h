// Functions for Control

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

// Computes the total torque from the throttle input
// the total torque is the torque being sent to both motors combined
void TotalTorque(){
    float oMax = 5.0;  // V
    float oMin = 0.0;  // V
    float nMax = 16.0;  // Nm
    float nMin = 0.0;  // Nm
    float oRange = (oMax - oMin);
    float nRange = (nMax - nMin);
    torqueOutput = (((throttleVolts - oMin) * nRange) / oRange) + nMin;  // rad
}

// DONE

// Computes the radius of the turn based on the steering angle and vehicle parameters
void FindRTurn(){
    Rturn = sqrt((pow((Tf / 2.0),2.0) + pow(L, 2.0) * pow(1.0 / (tan(steeringAngle)),2.0)));
    if(Rturn > 100){
      Rturn = 100;
    }
}

// Updated the kinematic parameters based on the RPM and radius of the turn
void Kinematics(){
    FindRTurn();
    N = (Nrr + Nrl)/2;
    Vx = Rw*2.0*pi/60*N;
    
    if (steeringAngle < 0.0) {
        Ay = -1.0 * (pow(Vx, 2.0) / Rturn);
    }
    else{
        Ay = pow(Vx,2.0) / Rturn;
    }
    
    Ax = (Vx - Vx_old) / dt;
    Vy_old = Vy;
    Vy = Ay*dt; //Vy + Ay*dt;
}

// Computes the desired difference in torque between the rear right and the rear left wheel
// from steering angle, kinematics parameters and vehicle parameters
void DeltaTorque(){
    float Ku = (Lr * M) / (Cyf * (Lf + Lr)) - (Lf * M) / (Cyr * (Lf + Lr));
    float R = ((Lr + Lf) + Ku * pow(Vrr,2.0)) / steeringAngle;
    yawRate = Vx / R;
    deltaTorque = Kr * yawRate + Kv * -Vy;  // Nm;
}

// Computes the desired toque on each wheel from the total toque and the desired difference in torque
void WheelTorque(){
    Trr = (torqueOutput + deltaTorque) / 2.0;
    Trl = (torqueOutput - deltaTorque) / 2.0;
}

// Computes slip conditions
void MaxTorqueSlip(){
    float Fmax;
    if (steeringAngle < 0.0) {
        Fmax = M * ((Lf / L) * G + (Hcg / L) * Ax) * (0.5 + (Hcg / (Tr * G)) * Ay);
    }
    else {
        Fmax = M * ((Lf / L) * G + (Hcg / L) * Ax) * (0.5 - (Hcg / (Tr * G)) * Ay);
    }
    Tmax_slip = mu * Fmax * Rw;
    if(Tmax_slip < 0.0){
      Tmax_slip = 0.0;
    }
}

// Computes power torque limitation
void MaxTorquePower(){
    Tmax_power = Pmax*Kt/V;
}

// Check that the desired torque to each wheel is within the slip condition 
// if not within slip conditions it updates the torque to each wheel
void CheckSlip(){
    if (Trr > Tmax_slip) {
        //deltaTorque = Trr - Trl;
        Trr = Tmax_slip;
        Trl = Trr - deltaTorque;
    }
    else if (Trl > Tmax_slip) {
        //deltaTorque = Trl - Trr;
        Trl = Tmax_slip;
        Trr = Trl - deltaTorque;
    }
}

// Check that the desired torque to each wheel is within the power condition 
// if not within power conditions it updates the torque to each wheel
void CheckPower(){
    if (Trr > Tmax_power) {
        //deltaTorque = Trr - Trl;
        Trr = Tmax_power;
        Trl = Trr - deltaTorque;
    }
    else if (Trl > Tmax_power) {
        //deltaTorque = Trl - Trr;
        Trl = Tmax_power;
        Trr = Trl - deltaTorque;
    }
}

float ReadMotorController(float comport){
    // read the motor controller;
    float values = 5.0;
    return values;
}

//
float TorqueRPMConversion(float Trr, float Trl){
    Nrr = 5000.0/1.1*Trr; // max RPM is 5000 and max torque is 1.1 Nm
    Nrl = 5000.0 / 1.1 * Trl;
    if (Nrr > 5000.0){
        Nrr = 5000.0;
    }
    if (Nrl > 5000.0){
        Nrl = 5000.0;
    }
    return Nrr, Nrl;
}


float RPMVoltageConversion(float Vrr, float Vrl){
    Voltrr = 3.3/5000.0*Vrr; // max RPM is 5000 and max torque is 1.1 Nm
    Voltrl = 3.3 / 5000.0 * Vrl;
    return Voltrr, Voltrl;
}

float TorqueToCurrent(float torque, float N, float V) {
  float current = torque * 48.0 / 8.0;
  return current;
}

float CurrentToTorque(float current, float N, float V) {
  float torque = current* (8.0 / 48.0);
  return torque;
}

// PID controller for each both rear right torque and rear left torque
void currentPID(){
    errorTrr = Trr - TrrOutput;
    integralErrorTrr = integralErrorTrr + errorTrr*dt;
    derivativeErrorTrr = (errorTrr - errorLastTrr)/dt;
    errorLastTrr = errorTrr;
    Trr = Kp*errorTrr + Ki*integralErrorTrr + Kd*derivativeErrorTrr;

    //Limit the error total
    if (integralErrorTrr > 0.5){
      integralErrorTrr = 0.5;
    }
    else if (integralErrorTrr < -0.5){
      integralErrorTrr = -0.5;
    }

    errorTrl = Trl - TrlOutput;
    integralErrorTrl = integralErrorTrl + errorTrl*dt;
    derivativeErrorTrl = (errorTrl - errorLastTrl)/dt;
    errorLastTrl = errorTrl;
    Trl = Kp*errorTrl + Ki*integralErrorTrl + Kd*derivativeErrorTrl;

    //Limit the error total
    if (integralErrorTrl > 0.5){
      integralErrorTrl = 0.5;
    }
    else if (integralErrorTrl < -0.5){
      integralErrorTrl = -0.5;
    }
}
