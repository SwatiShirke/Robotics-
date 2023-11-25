//date - 24 march 2017
#include <motor.h>
#include <TimerOne.h>
/**********************************************golbal variables and functions********************************/
int8_t lookup_table[16] = {0, -1, 1, 0, 1, 0, 0, -1, -1, 0, 0, 1, 0, 1, -1, 0};
Motor M1;
void update_count_18_19();
Motor M2;
void update_count_2_3();
float kp1 = 0.11 , ki1 = 0.0015, kd1 = 0.02367;              //////////for Motor1
float kp2 = 0.09 , ki2 = 0.0015, kd2 = 0.02043;             //////////for Motor 2
int cpr = 1633;
float distance_per_rotation = 0.22;
void find_speed();
int g_flag = 0;
float bot_width = 0.18;

int my_ctrl = 0;
float my_speed = 0;
float distance = 0;//it is in meter/seconds
int no_of_sides = 0;
int my_pwm = 0;
int angle = 0;
bool to_do_flag = 0; // this flag = 0 means listen to command and 1 means act as per command
bool SONAR_action_flag = 0;
int SONAR_data [5];
void get_command(void);
void get_SONAR_data(void);
/***********************************************************************************************/
void setup()
{
  Serial.begin(9600);
  M1.init(10, 23, 21, 18, 19, kp1, ki1, kd1);
  //M1.give_parameter(200,0.1,1);
  attachInterrupt(digitalPinToInterrupt(18), update_count_18_19, CHANGE);
  attachInterrupt(digitalPinToInterrupt(19), update_count_18_19, CHANGE);
  M2.init(9, 8, 7, 2, 3, kp2, ki2, kd2);
  //M2.give_parameter(200 ,0.1,1);
  attachInterrupt(digitalPinToInterrupt(2), update_count_2_3, CHANGE);
  attachInterrupt(digitalPinToInterrupt(3), update_count_2_3, CHANGE);
  Timer1.initialize(100000);
  Timer1.attachInterrupt(find_speed);

}


/*****************************************************************************************************/
void update_count_18_19() {
  //this function is for pin no-18,19 of arduino mega ADK: pin 18 & 19 are pin 3 and 2 of portD respectivly
  //use this function with Motor M1 object only

  static uint8_t enc_status1 = 0;
  enc_status1 = enc_status1 << 2;
  enc_status1 = enc_status1 | ((PIND & 0b00001100) >> 2);
  M1.motor_direction = lookup_table[enc_status1 & 0b1111];
  M1.current_count = M1.current_count + M1. motor_direction;
}

void update_count_2_3() {
  //this function is for pin no-2,3 of arduino mega ADK: pin 2,3 are pin 4 and 5 of portE respectivly

  static uint8_t enc_status = 0;
  enc_status = enc_status << 2;
  enc_status = enc_status | ((PINE & 0b110000) >> 4);
  M2.motor_direction = lookup_table[enc_status & 0b1111];
  M2.current_count = M2.current_count + M2.motor_direction;
}

void find_speed()
{
  M1.find_speed();
  M2.find_speed();
  M1.apply_control();
  M2.apply_control();
  g_flag = 1;
}
/********************************************************************************************/
void run_robot(float my_speed1, float my_speed2, int ctrl_method )
{
  M1.run_motor(my_speed1, ctrl_method);
  M2.run_motor(my_speed2, ctrl_method);

}

void run_robot_pwm(int my_pwm1, int my_pwm2)
{
  M1.run_motor_pwm(my_pwm1, 2);
  M2.run_motor_pwm(my_pwm2, 2);

}
void stop_robot ()
{

  M1.stop_motor();
  M2.stop_motor();

}

void refresh_PID()
{
  M1.refresh_PID();
  M2.refresh_PID();
}


/******************************************************************************************/
void loop()
{
  while (to_do_flag == 0)
  {
    get_command();
  }
 // Serial.println(my_ctrl);
 // Serial.println(my_speed);
//  Serial.println(distance);
 // Serial.println(no_of_sides);
  //Serial.println(my_pwm);
  //Serial.println(angle );
  if (to_do_flag == 1)
  { //do actions
    to_do_flag = 0;
    switch (my_ctrl)
    {
      case 0:
        //call controller stop()
       // Serial.println("0");
        stop_robot ();
        break;
      case 1:
        //call open loop go_straight
      //  Serial.println("1");
        open_loop_go_straight( my_pwm);
        break;
      case 2:
        //call open loop rotate clkwise
       // Serial.println("2");
        open_loop_clk( my_pwm);
        break;
      case 3 :
        //call open loop rotate anti clkwise
        //Serial.println("3");
        open_loop_aclk( my_pwm);
        break;
      case 4:
        // call close loop go straight
       // Serial.println("4");
        go_straight_new(my_speed, distance);
        break;
      case 5:
        //call close loop turn right
        //Serial.println("5");
        clk_rotate(my_speed, angle);
        break;
      case 6:
        ////call close loop turn left
       // Serial.println("6");
        aclk_rotate(my_speed, angle);
        break;
      case 7:
        //call follow circle
       // Serial.println("7");
        circle( my_speed , distance , no_of_sides);
        break;
      case 8:
        //call polygon
      //  Serial.println("8");
        polygon(my_speed, distance, no_of_sides);
        break;
        case 9:
        //call polygon
      //  Serial.println("9");
        obstacle_avoidance();
        break; 
      default:
        stop_robot ();
        break;

    }
   // Serial.println("sucess");

  }



}
/******************************************************************************/
void get_command(void)
{ if (Serial.available() > 0 && Serial.read() == 'S')
  { to_do_flag = 1;
    int my_data [7];
    char data_string[30];
    //Serial.println("swati");
    int data_new;
    int k = 0;
    do  {
      //Serial.println("here");
      if (Serial.available() > 0)
      {
        data_string[k] = Serial.read();

        //Serial.print(data_string[k]);
        k++;
      }
    }
    while (data_string[k - 1] != 'E');
    int i = 0;
    int j = 0;
    while (data_string[i] != 'E')
    {
      data_new = 0;
      while (data_string[i] != '\n')
      {
        //Serial.print(data_string[i]);
        data_new = ( data_new  * 10) + (int (data_string[i] ) - int ('0'));
        //Serial.println(data_new);
        i++;

      }
      my_data[j] = data_new;
      j++;
      i++;

    }

    my_ctrl = my_data[0];
    if (my_data[2] == 1)  {
      my_speed = -(abs(my_data[1]));
      my_pwm = -( abs(my_data[5]));
    }
    else
    { my_speed = abs(my_data[1]);
      my_pwm = abs (my_data[5]);
    }
    distance = my_data[3] / 100.00;
    no_of_sides =  my_data[4];

    angle = my_data[6];
  }
}
/****************************************************/
void get_SONAR_data()
{ if (Serial.available() > 0 && Serial.read() == 'S')
  { SONAR_action_flag = 1;

    char data_string[30];
    int data_new;
    int k = 0;
    do  {
      //Serial.println("here");
      if (Serial.available() > 0)
      {
        data_string[k] = Serial.read();

        //Serial.print(data_string[k]);
        k++;
      }
    }
    while (data_string[k - 1] != 'E');
    int i = 0;
    int j = 0;
    while (data_string[i] != 'E')
    {
      data_new = 0;
      while (data_string[i] != '\n')
      {
        //Serial.print(data_string[i]);
        data_new = ( data_new  * 10) + (int (data_string[i] ) - int ('0'));
        //Serial.println(data_new);
        i++;

      }
      SONAR_data[j] = data_new;
      j++;
      i++;

    }
  }
}
/**********************************************************/
void go_straight_new(float my_speed, float distance)
{ Serial.println("inside go straight");
  refresh_PID();
  long int cummulative_error = 0;
  float ki = 0.01;
  float kp = 0.00;
  float val_left = 0;
  float val_right = 0;
  long int target_count1 = 0;
  long int target_count2 = 0;
  long int count = (distance / distance_per_rotation) * cpr ;
  long int  current_count1 = M1.get_count();
  long int  current_count2 = M2.get_count();
  long int prior_count1 = current_count1;
  long int prior_count2 = current_count2;
  if (my_speed > 0)
  {

    target_count1 = current_count1 + count;
    target_count2 = current_count2 + count;
    while (M1.get_count() < target_count1 || M2.get_count() < target_count2)
    {
      if (g_flag == 1)
      {
        g_flag == 0;
        current_count1 = M1.get_count();
        current_count2 = M2.get_count();

        long int count_diff =  abs(current_count1 - prior_count1) - abs(current_count2 - prior_count2);
        cummulative_error += count_diff;
        prior_count1 = current_count1;
        prior_count2 = current_count2;
        //Serial.print( "cumm_error" );
        //Serial.println( cummulative_error);
        if ( cummulative_error > 200)
        {

          // Serial.println("I am inside case1");
          val_left = -( abs(cummulative_error * ki + count_diff * kp));
          val_right =  abs(cummulative_error * ki + count_diff * kp);

        }
        else if ( cummulative_error < -200)
        {
          val_left = abs(cummulative_error * ki + count_diff * kp);
          val_right =  -(abs(cummulative_error * ki + count_diff * kp));
          //Serial.println("I am inside case2");
        }
        else
        {

          val_left = -(abs(cummulative_error * ki + count_diff * kp));
          val_right = -(abs(cummulative_error * ki + count_diff * kp));
          // Serial.println("I am inside case3");

        }

      }

      run_robot(my_speed + val_left, my_speed + val_right , 1);
      /*Serial.print(my_speed - val_left);
        Serial.print("  ");
        Serial.println(my_speed -  val_right);
      */


    }
    stop_robot();
    //run_robot(0, 0 , 1);

  }

  else if (my_speed < 0)
  {
    target_count1 = current_count1 - count;
    target_count2 = current_count2 - count;
    while (M1.get_count() > target_count1 || M2.get_count() > target_count2)
    {
      if (g_flag == 1)
      {
        g_flag == 0;
        current_count1 = M1.get_count();
        current_count2 = M2.get_count();

        long int count_diff =  abs(current_count1 - prior_count1) - abs(current_count2 - prior_count2);
        cummulative_error += count_diff;
        prior_count1 = current_count1;
        prior_count2 = current_count2;
        //Serial.println( count_diff );
        //Serial.println( cummulative_error);
        if ( cummulative_error > 200)
        {

          //Serial.println("I am inside case1");
          val_left = -( abs(cummulative_error * ki + count_diff * kp));
          val_right =  abs(cummulative_error * ki + count_diff * kp);

        }
        else if ( cummulative_error < -200)
        {
          val_left = abs(cummulative_error * ki + count_diff * kp);
          val_right =  -(abs(cummulative_error * ki + count_diff * kp));
          //Serial.println("I am inside case2");
        }
        else
        {

          val_left = -(abs(cummulative_error * ki + count_diff * kp));
          val_right = -(abs(cummulative_error * ki + count_diff * kp));
          // Serial.println("I am inside case3");

        }

      }

      run_robot(my_speed - val_left, my_speed - val_right , 1);
      // Serial.println( my_speed - val_left);
      //Serial.println( my_speed - val_right);


    }
    stop_robot();
    //run_robot(0, 0 , 1);


  }
  else stop_robot();
  // run_robot(0, 0 , 1);


}


void circle(float my_speed, float radius, int count)
{
  refresh_PID();
  float  d1 = 2 * 3.14 * (radius + (bot_width / 2));
  float d2 =  2 * 3.14 * (radius - (bot_width / 2));
  long int my_count1;
  long int my_count2;
  float M2_speed;
  float M1_speed;

  if (my_speed > 0)
  { //Serial.println("here...");
    my_count1 = M1.get_count() + ( (d1 / distance_per_rotation) * cpr * count) ;
    my_count2 = M2.get_count() + ( (d2 / distance_per_rotation) * cpr * count) ;

    while ( M1.get_count() < my_count1 && M2.get_count () < my_count2)
    {

      M1_speed = abs (my_speed);
      M2_speed = abs((d2 / d1) * M1_speed);

      run_robot(M1_speed, M2_speed , 1);

    }
  }
  else if (my_speed < 0)
  {

    my_count1 = M1.get_count() + ( (d2 / distance_per_rotation) * cpr * count) ;
    my_count2 = M2.get_count() + ((d1 / distance_per_rotation) * cpr * count );
    Serial.println(my_count1);
    Serial.println(my_count2);


    while ( M1.get_count() < my_count1 && M2.get_count() < my_count2 )
    {
      M2_speed = abs(my_speed);
      M1_speed = abs((d2 / d1) * M2_speed);
      run_robot(M1_speed, M2_speed , 1);
    }

  }
  //Serial.println("here..");
  stop_robot();
}



void clk_rotate(float my_speed, int angle)
{
  refresh_PID();
  float distance = 2 * 3.1414 * bot_width;
  long int rcount = (distance / distance_per_rotation) * cpr ;
  long int  target_count = M1.get_count() + ((abs(angle ) * (rcount / 360.00) ) * 0.8);
  // target_count += target_count * 0.12;
  //Serial.println(  M1.get_count() );
  // Serial.println( "    " );
  //Serial.println(  target_count );
  do {
    run_robot(my_speed, -(my_speed) , 1);
    // Serial.println(  M1.get_count() );
    //open_loop_clk(80);
  }
  while (M1.get_count () < target_count);
  stop_robot ();
  // run_robot(0,0 , 1);

}

void aclk_rotate(float my_speed, int angle)
{ refresh_PID();
  float distance = 2 * 3.14 * bot_width;
  long int rcount = (distance / distance_per_rotation) * cpr ;
  long int  target_count = M1.get_count() + (abs(angle * 1) * (rcount / 360));
  // target_count += target_count * 0.24;
  do {
    run_robot(-(my_speed), my_speed, 1);
  }
  while (M2.get_count () < target_count);
  stop_robot ();
  // Serial.println(target_count);
}


void polygon(float my_speed, float side_distance, int no_side)
{ refresh_PID();
  int side_count = 0;
  my_speed = abs(my_speed);
  float angle = 180 - ((180 * (no_side - 2)) / no_side);
  //Serial.println(angle);
  while ( side_count < no_side)
  { //if(no_side < 3 || no_side > 8) break;
    go_straight_new(my_speed, side_distance);
    delay(500);
    if (side_count < (no_side - 1))
    {

      clk_rotate(my_speed, angle);
      delay(500);
    }
    side_count++;
  }


}
/****************************open loop user function**************************************************/
void open_loop_go_straight(int my_pwm)
{
  run_robot_pwm(my_pwm, my_pwm);

}

void open_loop_clk(int my_pwm)
{
  run_robot_pwm(my_pwm, -(my_pwm));

}

void open_loop_aclk(int my_pwm)
{
  run_robot_pwm(-(my_pwm), my_pwm);

}
/**************************************************************************************************/
 void obstacle_avoidance(void)
 {
  while(SONAR_action_flag ==0 )
  {
    get_SONAR_data();
    }
  if(SONAR_action_flag ==1)
  {
    SONAR_action_flag =0;
    //write your algorithm here.... 
    
    
    
    
    }
    
  
  
  
  }
