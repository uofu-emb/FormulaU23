// System Variables
// Check values below before continuing
// CG = Center of Gravity


void setup() {
  // put your setup code here, to run once:
}

void loop() {
  // put your main code here, to run repeatedly:

  // --- CONVERT INPUTS ---
  // find steering angle of wheels
  steeringAngle = SteeringAngle();

  // find total torque output from input throttle
  torqueOutput = TotalTorque();

  // --- UPDATE KINEMATICS ---
  // Inputs from motor controllers
  N1 = motor_data.motor_rpm  // RPM of motor 1
  N2 = motor_data.motor_rpm  // RPM of motor 2 

  // Update kinematics from the new motor controller output data
  Kinematics();
  
  // --- FIND DESIRED DIFFERENCE IN TORQUE ---
  DeltaTorque();
  
  // --- FIND TORQUE TO EACH WHEEL ---
  WheelTorque();

  // --- PID FOR EACH TORQUE ---
  
  
  // --- CHECK FOR LIMITATIONS
  // Find max torque for slipping
  MaxTorqueSlip();

  // Find max torque for max power
  MaxTorquePower();
  
  // Limit torques due do slip and max power
  CheckSlip();
  CheckPower();

  // Convert from torque to RPM
  //Vrr, Vrl = TorqueRPMConversion(Trr, Trl,);
  
  //Vor, Vol = RPMVoltageConversion(Vrr, Vrl);
  //[Trr, Trl] = CheckSlip(Trl, Trr, maxTorque)
  //N1 = Vrr;
  //N2 = Vrl;

}
