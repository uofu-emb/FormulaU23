/***********************
 ** Global Variables ***
 ***********************/
// Vehicle variables: Change these variables based on the vehicle it is being implemented on
float Lf = 0.88265;      // [m] Length to front of wheelbase from CG 34.75"
float Lr = 0.264;      // [m] Length to back of wheelbase from CG 10.375"
float L = 1.146;       // [m] Length of car (wheelbase) 45.125"
float M = 60.0;      // [kg] Mass of car 
float Rw = 0.073025;     // [m] Wheel radius
float Cyf = 500.0;   // [N/deg] Front cornering stiffness
float Cyr = 600.0;   // [N/deg] Rear cornering stiffness
float Kr = 1.3;      //
float Kv = 0.7;     // Motor Voltage Constant
float Kt = 8.3/Kv;   // [Nm/A] Torque Constant (Measured by steady state rpm @constant current)
float G = 9.81;      // [m/s^2] Gravity
float Hcg = 0.229;    // [m] Height of CG 9"
float dt = 0.005;    // [ms] Time step
float Pmax = 2000.0; // [W] Max power
float Tf = 0.433;     // [m] 34.0625" /2
float Tr = 0.433;     // [m]
float mu = 1.0;      // 
float V = 34.0;      // Voltage of battery



/** Initiate vesc1Uart class */
// TODO: Refactor
// VescUart vesc1;
// VescUart vesc2;

float pi = 3.14;
float Vrr = 0.0;
float Vrl = 0.0;
float Voltrr = 0.0;
float Voltrl = 0.0;

double deltaT = 10.0;
double TimeStamp = 0.0;
double prevtime = 0.0;
int r = 0;

// Input  Variables
float steeringVolts;
float steeringAngle = 0.0;  // rad
float throttleVolts;
float torqueOutput = 0.0;   // Nm
float TrrOutput;
float TrlOutput;
double TrrActual;
double TrlActual;

// Torque variables
float deltaTorque; // Nm
double Trr; // Nm
double Trl; // Nm
float Tmax_slip;
float Tmax_power;
float currentRR;
float currentRL;
double TrrOut = 0.0;
double TrlOut = 0.0;

float yawRate = 0.0;


// Kinematics Variables
float Rturn;
float Nrr;
float Nrl;
float N;
float Vcg = 0.0;            // m/s
float Vx = 0.0;             // m/s
float Vx_old = 0.0;         // m/s
float Vy = 0.0;             // m/s
float Vy_old = 0.0;         // m/s
float Ax = 0.0;             // m/s^2
float Ay = 0.0;             // m/s^2

// PID variables
double Kp = 0.2;
double Ki = 9.6;
double Kd = 0.001;
float errorTrr;
float integralErrorTrr;
float derivativeErrorTrr;
float errorLastTrr;
float errorTrl;
float integralErrorTrl;
float derivativeErrorTrl;
float errorLastTrl;

// Additionial variables
float current = 0.0; /** The current in amps */
int throt = 0;
int stear = 0;
int counter = 0;
int thPin = A0; //TODO: modify to be consistent w/new board
int th2Pin = A1; //TODO: modify to be consistent w/new board
int stePin = A2; //TODO: modify to be consistent w/new board
float v = 0.0;
float p1 =  -0.001201;
float p2 =  2.835;
float p3 =  -620.6;

float brakeCurrentRL = 0.0;
float brakeCurrentRR = 0.0;
