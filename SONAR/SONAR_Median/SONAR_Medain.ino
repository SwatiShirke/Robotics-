#include <MedianFilter.h>
#include <Arduino.h>
void setup() {
  // put your setup code here, to run once:
  
  Serial.begin(9600); 
  pinMode(8, INPUT);
  pinMode(7, OUTPUT);
  
}
class Sonar
{
  int trigPin, echoPin, windowSize, distance;                                               // array size and window size should be same here , then only code will work correctly
  MedianFilter M1;
  public:
  
  Sonar(int trig_pin, int echo_pin, int window_size);
  int Sonar_continue(void);
  
};


  Sonar:: Sonar(int trig_pin, int echo_pin, int window_size)
  {
    trigPin=trig_pin;
    echoPin=echo_pin;
    windowSize=window_size;
    M1.Median_init(window_size);
     
    }

int Sonar::Sonar_continue(void)
{ int Distance;
      
       digitalWrite(trigPin,HIGH);
        delayMicroseconds(10);   
        digitalWrite(trigPin,LOW);
        Distance= (pulseIn(echoPin, HIGH))*0.034/2; 
        if (Distance < 400 )
        {return ( M1.applyFilter(Distance)) ;
        }  
  }  

  
  

Sonar S1(7,8,10);
void loop() {
 // Serial.println("  swati ");
  // put your main code here, to run repeatedly:
  int val = S1.Sonar_continue();
  Serial.println(val);
  delay(100);
}
