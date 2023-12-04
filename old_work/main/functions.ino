// Functions for Control


int steeringAngle(float steeringVolts) {
    oMax = 5.0;  // V
    oMin = 0.0;  // V
    nMax = pi / 6.0;  // rad
    nMin = -pi / 6.0;  // rad
    oRange = (oMax - oMin);
    nRange = (nMax - nMin);
    steeringAngle = (((steeringVolts - oMin) * nRange) / oRange) + nMin;  // rad
    return steeringAngle;
}

float totalTorque(float throttleVolts){
    oMax = 5.0;  // V
    oMin = 0.0;  // V
    nMax = 2.2;  // Nm
    nMin = 0.0;  // Nm
    oRange = (oMax - oMin);
    nRange = (nMax - nMin);
    torqueOutput = (((throttleVolts - oMin) * nRange) / oRange) + nMin;  // rad
    return torqueOutput;
}

float deltaTorque(float Lr, float M, float Cyf, float Lf, float Cyr, float Vx, float steeringAngle, float Vy, float Kr, float Kv){
    Ku = (Lr * M) / (Cyf * (Lf + Lr)) - (Lf * M) / (Cyr * (Lf + Lr));
    R = ((Lr + Lf) + Ku * V^2.0) / steeringAngle;
    yawRate = Vx / R;
    deltaTorque = Kr * yawRate + Kv * Vy  // Nm;
    return deltaTorque; 
}

float FindRTurn(float Tf, float L, float steeringAngle){
    Rturn = sqrt(((Tf / 2.0)^2.0) + (L^2.0) * (1.0 / tan(steeringAngle))^2.0) //(arctan(steeringAngle)^2.0)) //(1.0 / tan(steeringAngle))^2.0);
    //Rturn = ((Lr + Lf) + Ku * Vx^2.0) / steeringAngle;
    return Rturn;
}

float FindVelocity(float N1, float N2, float Rw){
    N = (N1 + N2)/2;
    Vx = Rw*2.0*pi/60.0*N;
    return Vx;
}


float Kinematics(float Vx, float Vx_old, float Vy, float dt, float Rturn, float steeringAngle){
    if steeringAngle < 0.0:
        Ay = -1.0 * (Vx^2.0 / Rturn);
    else:
        Ay = Vx^2.0 / Rturn;
    Ax = (Vx - Vx_old) / dt;
    
    Vy_old = Vy;
    Vy = Vy + Ay*dt;
    return Vy, Vy_old, Ax, Ay;
}
float WheelTorque(float torqueOutput, float deltaTorque, float steeringAngle){
    Trr = (torqueOutput + deltaTorque) / 2.0;
    Trl = (torqueOutput - deltaTorque) / 2.0;
    return Trr, Trl;
}

float MaxTorqueSlip(float M, float L, float Lf, float G, float Hcg, float Ax, float Tr, float Ay, float mu, float Rw, float steeringAngle){
    if steeringAngle < 0.0:
        Fmax = M * ((Lf / L) * G + (Hcg / L) * Ax) * (0.5 + (Hcg / (Tr * G)) * Ay);
    else:
        Fmax = M * ((Lf / L) * G + (Hcg / L) * Ax) * (0.5 - (Hcg / (Tr * G)) * Ay);
    maxTorque = mu * Fmax * Rw;
    return maxTorque;
}

float MaxTorquePower(float Pmax, float V, float Kt){
    Tmax_power = Pmax*Kt/V;
    return Tmax_power;
}

float ReadMotorController(float comport){
    // read the motor controller;
    values = 5.0;
    return values;
}

float CheckSlip(float Trl, float Trr, float Tmax){
    if (Trr > Tmax) {
        deltaTorque = Trr - Trl;
        Trr = Tmax;
        Trl = Trr - deltaTorque;
    }
    if else (Trl > Tmax) {
        deltaTorque = Trl - Trr;
        Trl = Tmax;
        Trr = Trl - deltaTorque;
    }
    return Trr, Trl;
}

float CheckPower(float Trl, float Trr, float Tmax){
    if (Trr > Tmax) {
        deltaTorque = Trr - Trl;
        Trr = Tmax;
        Trl = Trr - deltaTorque;
    }
    if else (Trl > Tmax) {
        deltaTorque = Trl - Trr;
        Trl = Tmax;
        Trr = Trl - deltaTorque;
    }
    return Trr, Trl;
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

float PID(float Kp, float Ki, float Kd, float Tdes, float Toutput, float dt, float errorLast, float integralError){
    error = Tdes - Toutput;
    integralError = integralError + error*dt;
    derivativeError = (error - errorLast)/dt;
    error_last = error;
    T = Kp*error + Ki*integralError + Kd*derivativeError;
    return T, errorLast, integralError;
}
