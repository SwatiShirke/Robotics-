#include <MedianFilter.h>
#include <Arduino.h>


void MedianFilter::Median_init(int window_size)
{
  windowSize = window_size;
  inputArray = new int [window_size];
  //for (int i=0; i<window_size-1; i++) inputArray[i] = init_array[i];

}


int MedianFilter::applyFilter(int value)
{ int *sortedArray = new int[windowSize];
  //shift both the arrays lrfy by one postion and add newer value to both
  int i;
  for(i=0;i< windowSize-1;i++)
  { inputArray[i] = inputArray[i+1];
    sortedArray[i] = inputArray[i+1];    
   }
   
   inputArray[i] = value;
   sortedArray[i]= value;
  

  //sorting of array
  for (int i=1;i<=windowSize-1;i++)
  { //Serial.println("here");
    for(int j=0;j<=windowSize-1-i;j++)
    { 
      if(sortedArray[j]>sortedArray[j+1])
      { 
        int temp = sortedArray[j+1];
        sortedArray[j+1]= sortedArray[j];
        sortedArray[j]=temp;
       
        }
      }
    }
   int ceil_val = ceil(windowSize/2);
   int floor_val = floor(windowSize/2);
   result =(windowSize %2==1)? sortedArray[ceil_val] : ((sortedArray   
    [floor_val-1] + sortedArray[floor_val] )/2);
   return result;
}



