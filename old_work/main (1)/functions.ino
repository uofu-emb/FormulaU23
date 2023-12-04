// Functions for Control

void steeringAngle() {
    oMax = 5.0;  // V
    oMin = 0.0;  // V
    nMax = pi / 6.0;  // rad
    nMin = -pi / 6.0;  // rad
    oRange = (oMax - oMin);
    nRange = (nMax - nMin);
    steeringAngle = (((steeringVolts - oMin) * nRange) / oRange) + nMin;  // rad
}

void totalTorque(){
    oMax = 5.0;  // V
    oMin = 0.0;  // V
    nMax = 2.2;  // Nm
    nMin = 0.0;  // Nm
    oRange = (oMax - oMin);
    nRange = (nMax - nMin);
    torqueOutput = (((throttleVolts - oMin) * nRange) / oRange) + nMin;  // rad
}

void FindRTurn(){
    Rturn = sqrt((pow((Tf / 2.0),2.0) + pow(L, 2.0) * pow(1.0 / (tan(steeringAngle)),2.0)) 
}

void Kinematics(){
    FindRTurn();
    N = (N1 + N2)/2;
    Vx = Rw*2.0*pi/60.0*N;
    
    if (steeringAngle < 0.0) {
        Ay = -1.0 * (pow(Vx, 2.0) / Rturn);
    }
    else{
        Ay = pow(Vx,2.0) / Rturn;
    }
    
    Ax = (Vx - Vx_old) / dt;
    Vy_old = Vy;
    Vy = Vy + Ay*dt;
}

void deltaTorque(){
    Ku = (Lr * M) / (Cyf * (Lf + Lr)) - (Lf * M) / (Cyr * (Lf + Lr));
    R = ((Lr + Lf) + Ku * pow(V,2.0)) / steeringAngle;
    yawRate = Vx / R;
    deltaTorque = Kr * yawRate + Kv * Vy  // Nm;
}

void WheelTorque(){
    Trr = (torqueOutput + deltaTorque) / 2.0;
    Trl = (torqueOutput - deltaTorque) / 2.0;
}

void MaxTorqueSlip(){
    if (steeringAngle < 0.0) {
        Fmax = M * ((Lf / L) * G + (Hcg / L) * Ax) * (0.5 + (Hcg / (Tr * G)) * Ay);
    }
    else {
        Fmax = M * ((Lf / L) * G + (Hcg / L) * Ax) * (0.5 - (Hcg / (Tr * G)) * Ay);
    }
    maxTorque = mu * Fmax * Rw;
}

void MaxTorquePower(){
    Tmax_power = Pmax*Kt/V;
}

void CheckSlip(){
    if (Trr > Tmax_slip) {
        deltaTorque = Trr - Trl;
        Trr = Tmax_slip;
        Trl = Trr - deltaTorque;
    }
    if else (Trl > Tmax_slip) {
        deltaTorque = Trl - Trr;
        Trl = Tmax_slip;
        Trr = Trl - deltaTorque;
    }
}

void CheckPower(){
    if (Trr > Tmax_power) {
        deltaTorque = Trr - Trl;
        Trr = Tmax_power;
        Trl = Trr - deltaTorque;
    }
    if else (Trl > Tmax_power) {
        deltaTorque = Trl - Trr;
        Trl = Tmax_power;
        Trr = Trl - deltaTorque;
    }
}

float ReadMotorController(float comport){
    // read the motor controller;
    values = 5.0;
    return values;
}

float TorqueRPMConversion(float Trr, float Trl){
    Vrr = 5000.0/1.1*Trr; // max RPM is 5000 and max torque is 1.1 Nm
    Vrl = 5000.0 / 1.1 * Trl;
    if (Vrr > 5000.0){
        Vrr = 5000.0;
    }
    if (Vrl > 5000.0){
        Vrl = 5000.0;
    }
    return Vrr, Vrl;
}


float RPMVoltageConversion(float Vrr, float Vrl){
    Voltrr = 3.3/5000.0*Vrr; // max RPM is 5000 and max torque is 1.1 Nm
    Voltrl = 3.3 / 5000.0 * Vrl;
    return Voltrr, Voltrl;
}

void PID(){
    errorTrr = Trr - TrrOutput;
    integralErrorTrr = integralErrorTrr + errorTrr*dt;
    derivativeErrorTrr = (errorTrr - errorLastTrr)/dt;
    error_lastTrr = errorTrr;
    Trr = Kp*errorTrr + Ki*integralErrorTrr + Kd*derivativeErrorTrr;

    errorTrl = Trl - TrlOutput;
    integralErrorTrl = integralErrorTrl + errorTrl*dt;
    derivativeErrorTrl = (errorTrl - errorLastTrl)/dt;
    error_lastTrl = errorTrl;
    Trl = Kp*errorTrl + Ki*integralErrorTrl + Kd*derivativeErrorTrl;
}
