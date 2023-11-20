/****************************
 ** #defines and #includes **
 ****************************/

/***********************
 ** Global Variables ***
 ***********************/

// Vehicle variables
float Lf = 0.3;      // [m] Length to front of wheelbase from CG
float Lr = 0.3;      // [m] Length to back of wheelbase from CG
float L = 0.6;       // [m] Length of car (wheelbase)
float M = 20.0;      // [kg] Mass of car
float Rw = 0.05;     // [m] Wheel radius
float Cyf = 500.0;   // [N/deg] Front cornering stiffness
float Cyr = 600.0;   // [N/deg] Rear cornering stiffness
float Kr = 0.1;      //
float Kv = 0.01;     // Motor Voltage Constant
float Kt = 8.3/Kv;   // [Nm/A] Torque Constant (Measured by steady state rpm @constant current)
float G = 9.81;      // [m/s^2] Gravity
float Hcg = 0.05;    // [m] Height of CG
float dt = 0.005;    // [ms] Time step
float Pmax = 1000.0; // [W] Max power
float Tf = 0.15;     // [m]
float Tr = 0.15;     // [m]
float mu = 1.0;      //
float V = 40.0;      // Voltage of battery

// Input  Variables
float steeringVolts;
float steeringAngle = 0.0;  // rad
float throttleVolts;
float torqueOutput = 0.0;   // Nm
float dt; // time (s)
float TrrOutput;
float TrlOutput;

// Torque variables
float deltaTorque; // Nm
float Trr; // Nm
float Trl; // Nm
float Tmax_slip;
float Tmax_power;


// Kinematics Variables
float Rturn;
float N1;
float N2;
float Vcg = 0.0;            // m/s
float Vx = 0.0;             // m/s
float Vx_old = 0.0;         // m/s
float Vy = 0.0;             // m/s
float Vy_old = 0.0;         // m/s
float Ax = 0.0;             // m/s^2
float Ay = 0.0;             // m/s^2

// PID variables
float Kp;
float Ki;
float Kd;
float errorTrr;
float integralErrorTrr;
float derivativeErrorTrr;
float errorLastTrr;
float errorTrl;
float integralErrorTrl;
float derivativeErrorTrl;
float errorLastTrl;
