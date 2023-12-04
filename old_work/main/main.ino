void setup() {
  // put your setup code here, to run once:

  // System Variables
  // Check values below before continuing
  // CG = Center of Gravity
  Lf = 0.3  // [m] Length to front of wheelbase from CG
  Lr = 0.3  // [m] Length to back of wheelbase from CG
  L = 0.6  // [m] Length of car (wheelbase)
  M = 20.0  // [kg] Mass of car
  Rw = 0.05  // [m] Wheel radius
  Cyf = 500.0  // [N/deg] Front cornering stiffness
  Cyr = 600.0  // [N/deg] Rear cornering stiffness
  Kr = 0.1  //
  Kv = 0.01  // Motor Voltage Constant
  Kt = 8.3/Kv  // [Nm/A] Torque Constant (Measured by steady state rpm @constant current)
  G = 9.81  // [m/s^2] Gravity
  Hcg = 0.05  // [m] Height of CG
  dt = 0.005  // [ms] Time step
  Pmax = 1000.0  // [W] Max power
  Tf = 0.15  // [m]
  Tr = 0.15  // [m]
  mu = 1.0  //
  V = 40.0  // Voltage of battery
  
  // Updating Variables
  steeringAngle = 0.0  // rad
  torqueOutput = 0.0  // Nm
  Vcg = 0.0  // m/s
  Vx = 0.0  // m/s
  Vx_old = 0.0  // m/s
  Vy = 0.0  // m/s
  Vy_old = 0.0  // m/s
  Ax = 0.0  // m/s^2
  Ay = 0.0  // m/s^2

}

void loop() {
  // put your main code here, to run repeatedly:
  steeringAngle = SteeringAngle(ADC_Value[0]*5.0/0x7fffff)
  torqueOutput = TotalTorque(ADC_Value[1]*5.0/0x7fffff)
  // Finding new velocity from motor controller data
  //N1 = motor_data.motor_rpm  // RPM of motor 1
  //N2 = motor_data.motor_rpm  // RPM of motor 2
  
  Vx = FindVelocity(N1, N2, Rw)

  // Update kinematics from the new motor controller output data
  Rturn = FindRTurn(Tf, L, steeringAngle)
  Vy = 0.0
  Vy, Vy_old, Ax, Ay = Kinematics(Vx, Vx_old, Vy, dt, Rturn, steeringAngle)
  Vx_old = Vx
  // Find desired torque vectoring (difference in torque)
  deltaTorque = DeltaTorque(Lr, M, Cyf, Lf, Cyr, Vx, steeringAngle, Vy, Kr, Kv)
  
  // Find torque to each wheel
  Trr, Trl = WheelTorque(torqueOutput, deltaTorque, steeringAngle)
  
  // Find max torque due to slip and power limitations
  Tmax_slip = MaxTorqueSlip(M, L, Lf, G, Hcg, Ax, Tr, Ay, mu, Rw, steeringAngle)
  Tmax_power = MaxTorquePower(Pmax, V, Kt)
  
  
  // Limit torques due do slip
  //Trr, Trl = CheckSlip(Trl, Trr, Tmax_slip)

  // Limit torque due to max power
  //Trr, Trl = CheckPower(Trl, Trr, Tmax_power)

  // Convert from torque to RPM
  Vrr, Vrl = TorqueRPMConversion(Trr, Trl,)
  
  Vor, Vol = RPMVoltageConversion(Vrr, Vrl)
  
  print(time.time(), Vor, Vol, Vrr, Vrl, Trr, Trl, steeringAngle, torqueOutput)

  //print('Vrr =', Vrr)

  //print('Vrl = ', Vrl)
  //[Trr, Trl] = CheckSlip(Trl, Trr, maxTorque)
  N1 = Vrr
  N2 = Vrl

}
