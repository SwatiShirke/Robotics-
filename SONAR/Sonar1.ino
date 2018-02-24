int TrigPin=7;//output;
int EchoPin=8;//input;
int Window_size =5;                                               // array size and window size should be same here , then only code will work correctly
int value_array[]={10,10,10,10,10};

int Median_filter(int array1[],int n,int value);




void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); 
  pinMode(EchoPin, INPUT);
  pinMode(TrigPin, OUTPUT);
  

}

void loop() {
  // put your main code here, to run repeatedly:
  int distance,final_distance;
  digitalWrite(TrigPin,HIGH);
  delayMicroseconds(10);   
  digitalWrite(TrigPin,LOW);
  distance= (pulseIn(EchoPin, HIGH))*0.034/2;                                              // Prints the distance on the Serial Monitor
  if (distance<400)
  {
  final_distance = Median_filter(value_array,Window_size,distance);
   Serial.println(final_distance);    
  }//Serial.print("Distance: ");
  //delay(5);
  //while(1);
}





void shifting_array(int array1[],int n,int value)
{
  int i;
  
  for ( i=0; i<n-1; i++)
  { array1[i]= array1[i+1];    
  }
  array1[i]=value; 
    
  }
int Median_filter(int array1[],int n,int value)

{  
  int temp, i,j,array2[n];
  
  for ( i=0; i<n-1; i++)
  { array1[i]= array1[i+1];
    array2[i] = array1[i+1];    
  }
  array1[i]=value; 
  array2[i] =value;
    
  for (i=1;i<=n-1;i++)
  { //Serial.println("here");
    for(j=0;j<=n-1-i;j++)
    { 
      if(array2[j]>array2[j+1])
      { 
        temp = array2[j+1];
        array2[j+1]= array2[j];
        array2[j]=temp;
       
        }
      }
    }
  int ceil_val = ceil(n/2);
  int floor_val = floor(n/2);
  return ( (n%2==1)? array2[ceil_val] : ((array2[floor_val-1] + array2[floor_val] )/2) );
}

  
