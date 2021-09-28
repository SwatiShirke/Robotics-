#ifndef _MedianFilter_h
#define _MedianFilter_h

class MedianFilter
{ 
  int windowSize;
  int *inputArray;
  int result;

  public :
  MedianFilter(void){}
  void Median_init(int window_size);
  int applyFilter(int value);
  
  
  
  };


#endif

