
PID myPID1(&TrlActual, &TrlOut, &Trl, Kp, Ki, Kd, DIRECT);
PID myPID2(&TrrActual, &TrrOut, &Trr, Kp, Ki, Kd, DIRECT);
    
void setup() 
{
  myPID1.SetMode(AUTOMATIC);
  myPID2.SetMode(AUTOMATIC);
  
  /** Setup Serial port to display data */
  //Serial.begin(115200);

  /** Setup SoftwareSerial port */
  Serial1.begin(115200);
  Serial2.begin(115200);
  Serial.begin(115200);

  /** Define which ports to use as UART */
  vesc1.setSerialPort(&Serial1);
  vesc2.setSerialPort(&Serial2);

  //Serial.println("t,StVin,ThVin,TrlOut,TrrOut,Trl,Trr,deltaTorque,Nrr,Nrl,YawRate,Vy,");
}

void loop() {
  if (TimeStamp + deltaT <= millis()){
    TimeStamp = millis();
    dt = double(TimeStamp - prevtime) / 1000.0;
    // put your main code here, to run repeatedly:
    vesc1.getVescValues();
    vesc2.getVescValues();

    //Update the PID Variables through serial
    //getSerial();

    
    throt = analogRead(thPin);
    stear = analogRead(stePin);
    throttleVolts = map(throt, 75, 385, 0, 5000)*0.001;
    if (throttleVolts < 0.5){
      throttleVolts = 0.0;
    }
    
    stear = p1*pow(stear,2)+p2*stear + p1;
    steeringVolts = map((stear), 1130, 1640, 0, 5000)*0.001 ;
    //Serial.println(stear);
    
    // --- CONVERT INPUTS ---
    // find steering angle of wheels
    fsteeringAngle(); //Overide steering angle
    //Serial.println(steeringAngle);
    //steeringAngle = 0.0;
    // find total torque output from input throttle
    TotalTorque();
  
    // --- UPDATE KINEMATICS ---
    // Inputs from motor controllers
    Nrr = vesc2.data.rpm /14; // RPM of motor 1
    Nrl = vesc1.data.rpm /14; // RPM of motor 2
    //TorqueRPMConversion(Trr,Trl);
    //Nrr = Nrr *(16.0/66.0);
    //Nrl = Nrl *(16.0/66.0);
    Vrr = vesc2.data.inpVoltage;
    Vrl = vesc1.data.inpVoltage;
    //Vrr = 36.0;
    //Vrl = 36.0;
  
    // Update kinematics from the new motor controller output data
    Kinematics();
    
    // --- FIND DESIRED DIFFERENCE IN TORQUE ---
    DeltaTorque();
    //deltaTorque = 0.0;
    
    // --- FIND TORQUE TO EACH WHEEL ---
    WheelTorque();
    
    // --- PID FOR EACH TORQUE ---
    TrlActual = CurrentToTorque(vesc1.data.avgMotorCurrent,Nrl,Vrl);
    //TrlActual = CurrentToTorque(Trl,Nrl,Vrl);
    TrrActual = CurrentToTorque(vesc2.data.avgMotorCurrent,Nrr,Vrr);
    //TrrActual = CurrentToTorque(Trr,Nrr,Vrr);
    //myPID1.Compute();
    //myPID2.Compute();
    TrlOut = Trl;
    TrrOut = Trr;
    
    // --- CHECK FOR LIMITATIONS
    // Find max torque for slipping
    MaxTorqueSlip();
  
    // Find max torque for max power
    MaxTorquePower();
    
    // Limit torques due do slip and max power
    CheckSlip();
    CheckPower();
    
    // Convert output torque to current
    currentRR =  TorqueToCurrent(TrrOut, Nrr, Vrr);
    currentRL =  TorqueToCurrent(TrlOut, Nrl, Vrl);
    
    // limit current 
    if (currentRR > 45.0) {
      currentRR = 45.0;
    }
    if (currentRR < -45.0) {
      currentRR = -45.0;
    }
    if (currentRL > 45.0) {
      currentRL = 45.0;
    }
    if (currentRL < -45.0) {
      currentRL = -45.0;
    }

    // Max RPM is ~2000 brake current is max 5amp so 0.0025*2000 = 5amp
    brakeCurrentRL = (0.25 * Nrl);
    if(throttleVolts == 0.0 & Nrl > 50){
      vesc1.setBrakeCurrent(brakeCurrentRL);
    }
    else{
      //vesc1.setBrakeCurrent(0.0);
    }
    // Max RPM is ~2000 brake current is max 5amp so 0.0025*2000 = 5amp
    brakeCurrentRR = (0.25 * Nrr);
    if(throttleVolts == 0.0 & Nrr > 50){
      vesc2.setBrakeCurrent(brakeCurrentRR);
    }
    else{
      //vesc2.setBrakeCurrent(0.0);
    }
    
    vesc1.setCurrent(currentRL);
    vesc2.setCurrent(currentRR);
    
    Serial.print(TimeStamp);
    Serial.print("\t");
    Serial.print(Vx);
    Serial.print("\t");
    Serial.print(Vy);
    Serial.print("\t");
    Serial.print(steeringAngle*180.0/pi);
    Serial.print("\t");
    Serial.print(deltaTorque*1000.0);
    Serial.print("\t");
    Serial.print(torqueOutput*1000.0);
    Serial.print("\t");
    Serial.print(Trl*1000.0);
    Serial.print("\t");
    Serial.print(Trr*1000.0);
    Serial.print("\t");
    Serial.print(TrlActual*1000.0);
    Serial.print("\t");
    Serial.print(TrrActual*1000.0);
    Serial.print("\t");
    Serial.print(Nrl);
    Serial.print("\t");
    Serial.print(Nrr);
    Serial.print("\t");
    Serial.print(currentRL);
    Serial.print("\t");
    Serial.print(currentRR);
    Serial.print("\t");
    Serial.println(Rturn);
    
    prevtime = TimeStamp;
    
  }
}
