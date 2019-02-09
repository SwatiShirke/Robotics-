/*Author : Swati V. Shirke
 Date: 23/12/2018
 This is header file of SONAR CLASS*/
#ifndef SONAR_h
#define SONAR_h

class SONAR
{
  int TrigPin;
  int EchoPin;
  float Distance;

  //Functions
public:
  SONAR( int trig_pin, int echo_pin);
  float SONAR_continue();
};



//define functions here

SONAR::SONAR( int trig_pin, int echo_pin)
{
  TrigPin = trig_pin;
  EchoPin = echo_pin;
  Distance= 0;

}


float SONAR::SONAR_continue()
{
  digitalWrite(TrigPin,HIGH);
  delayMicroseconds(10);   
  digitalWrite(TrigPin,LOW);
  Distance = (pulseIn(EchoPin, HIGH))*0.034/2;  
return (Distance);
}
#endif
