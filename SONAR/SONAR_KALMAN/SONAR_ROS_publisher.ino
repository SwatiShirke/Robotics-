
/*Autor: Swati V. Shirke
Date: 23/12/2018 
last modified - 30/12/2018*/


#include <SONAR.h>
#include <ros.h>
#include <my_pack/NumArray.h>


//global variable 
my_pack::NumArray val_list;
ros::NodeHandle  nh;
ros::Publisher chatter("chatter", &val_list);

int TrigPin= 7; // trigger pin of SONAR 
int EchoPin = 8; // Echo pin 



void setup()
{
  //Serial.begin(9600);
  pinMode(EchoPin, INPUT);
  pinMode(TrigPin, OUTPUT);
  nh.initNode();
  nh.advertise(chatter);
}

SONAR S1(TrigPin,EchoPin);

void loop()
{ int i=0;
 do 
  {
    float distance = S1.SONAR_continue();
    if(distance <400)
      {
        val_list.numArray[i] = distance;
        i=i+1;
        //Serial.println(distance);
        
      }
  }
  while(i!= 10);
  
  //publish now 
  chatter.publish( &val_list );
  nh.spinOnce();
  delay(10);
}
