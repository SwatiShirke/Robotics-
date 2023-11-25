#ifndef motor_h
#define motor_h
#include "Arduino.h"

/*
  Class Motor : This class holds variables and functions related to motor
*/
class Motor;
class Motor
{

    //data member
  private:
    //motor and encoder pins
    int pin_pwm;
    int pin_IN1;
    int pin_IN2;
    int pin_ENCA;
    int pin_ENCB;

    //motor info
    int desired_pwm = 0;                 //desired pwm to be given as per required speed and pid result
    int last_pwm = 0;                 //holds last value of pwm given to motor
    int max_pwm = 200;                 //max value of PWM that can be given to motor and is decided by designer
    int max_step = 60;                 // max acceptable change in PWM value

    bool desired_direction;              //direction given by user
    float motor_speed = 0;                             //speed of motor in rpm sensed by encoder
    float desired_speed = 0;                             //speed given by user in rpm
   
    long int desired_count = 0;

    //encoder info
    float time_interval =0.1;                             // time interval after which speed is calculated here 100 msseconds
    int cpr = 1633;                                //as per datasheet of polulu motor                                           //////////copy to new code
    float count_per_cm  = 74.18;                     //as per wheels specification
 volatile long int previous_count = 0;              //get updated after time_interval
   

//PID

    int control_method = 0;
         float  cumm_error = 0;
    float last_error = 0;

float kp,ki,kd;
/*****************************************
   // float kp1 =0.11 , ki1 = 0.0015, kd1 = 0.02367;               //////////for Motor1
  // float kp2 =0.09 , ki2 = 0.0015, kd2 = 0.02043;              //////////for Motor 2


//for M1 Ku = 0.5 tu= 0.71 sec     kp = 0.1, ti=0.355 td= 0.2367, ki= 0.281, kd= 0.02367
//for M2 i. e. p = 0.45 is Ku that is gain at which it gives sustained oscillations Tu =0.68 sec kp = 0.09, Ti = 0.34 Td= 0.227 ki= 0.26 kd= 0.02043 calculated as per ziglar nicholas
//final well tested 
//  float kp1 =0.11 , ki1 = 0.0015, kd1 = 0.02367;               //////////for Motor1
//   float kp2 =0.09 , ki2 = 0.0015, kd2 = 0.02043;               //////////for Motor 2

  
   // int I_count = 0;
    //float ki_count = 0.0001;
*************************/
    public: 
   volatile long int current_count = 0;               // gets updated after every interrupt
    int8_t motor_direction = 0;           // direction of motor sensed by Encoder (0,-1,1)
  private:

  
   void run1 ();

  public:
    void init( int pwm_pin, int IN1_pin, int IN2_pin, int ENCA_pin, int ENCB_pin, float Kp, float Ki, float Kd); // constructor to initialize motor class  
int get_pwm(){return last_pwm;}
void set_max_step(int val){  max_step = min(60, abs(val));}
//*************************************************************************//
//encoder related functions
 
 float get_speed(void){ return motor_speed;} 
 void find_speed(void);  
long int get_count (void){  return current_count;}
int8_t get_direction (void){  return motor_direction;}
//*****************************************************************************// 
//void close_loop(float my_speed);
void run_motor(float my_speed, int ctrl_method); 
void stop_motor();   
  void apply_control();
  void refresh_PID();    
 void run_motor_pwm(int my_pwm,int ctrl_method);
void clear_motor_count();
};


void Motor::init( int pwm_pin, int IN1_pin, int IN2_pin, int ENCA_pin, int ENCB_pin, float Kp, float Ki, float Kd)
{ //Serial.begin(9600);
  //set pin mode as output
  pinMode(pwm_pin, OUTPUT);
  pinMode(IN1_pin, OUTPUT);
  pinMode(IN2_pin, OUTPUT);
  pinMode(ENCA_pin, INPUT);
  pinMode(ENCB_pin, INPUT);


  pin_pwm = pwm_pin;
  pin_IN1 = IN1_pin;
  pin_IN2 = IN2_pin;
  pin_ENCA = ENCA_pin;
  pin_ENCB = ENCB_pin;
  
  kp =Kp;
  ki = Ki;
  kd = Kd;
   
  //Serial.println("done pin assignment!");


}



void Motor::run_motor(float my_speed, int ctrl_method)
{
    desired_speed = my_speed;
    control_method = ctrl_method;
    run1();

}

void Motor::stop_motor(){ 
control_method = 2;
desired_pwm =0;
last_pwm = 0;
analogWrite(pin_pwm, 0);
cumm_error = 0;
last_error = 0;
} 

void Motor::run_motor_pwm(int my_pwm,int ctrl_method)
{
control_method = ctrl_method;
desired_pwm = my_pwm;
run1();
} 

void Motor::run1 ()
{



  //it checks if given pwm is out of range (0 to 200)
  // max_pwm is settable
 
  int temp_pwm = desired_pwm;
  temp_pwm = min(max_pwm, temp_pwm);                      //limit pwm to max allowed pwm on positive side
  temp_pwm = max(-(max_pwm), temp_pwm);                   //limit pwm to max allowed pwm on negative side
  // soft start is implemented here
  
  int  lower_limit,  upper_limit;
  lower_limit =  last_pwm - max_step;
  upper_limit =  last_pwm + max_step;

  temp_pwm = min(upper_limit, temp_pwm);
  temp_pwm = max(lower_limit, temp_pwm);
   
  if ( temp_pwm >= 0)
  {
    digitalWrite(pin_IN1, 1);
    digitalWrite(pin_IN2, 0);


  }
  else
  {
    digitalWrite(pin_IN1, 0);
    digitalWrite(pin_IN2, 1);

  }
  //Serial.print(temp_pwm);
   //Serial.print("  ");
 //Serial.print(temp_pwm);


  analogWrite(pin_pwm,  abs(temp_pwm));
  last_pwm = temp_pwm;
//Serial.println( temp_pwm  );
}



/************************************/

void Motor::find_speed()                                                                                                                    //////copy to new code
{ static float k;
   k =  60/(time_interval*cpr) ;  
  long int temp_count = current_count - previous_count;
  motor_speed = float (temp_count)*k;
  //Serial.println( k );
  previous_count = current_count;
}

void Motor::apply_control()
{
   if ( control_method == 0)
{//open loop with given speed
// control method  = 0

      desired_pwm = 0.29 * desired_speed;   //(70/240 = pwm / speed )from known values
}
   if ( control_method == 1 )

{ //close loop with given speed
   //control method = 1
  float error = desired_speed - motor_speed; 
  cumm_error += error;
  //I_count += my_count;
  float pid_term = ( error * kp )+ ( cumm_error * ki ) +( (last_error - error) * kd );

  last_error = error;
   desired_pwm = constrain((last_pwm + int( pid_term)), -255, 255) ;
  

  }
  
//control method  = 2 reserved for open loop control by passing pwm, dont use it for other controller  
}

void Motor::refresh_PID()
{
cumm_error = 0;
last_error = 0;
}

void Motor::clear_motor_count()
{
current_count = 0; 
}







#endif





