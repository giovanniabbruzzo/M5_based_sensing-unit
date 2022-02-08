#include "common_math.h"

double variance(float* array, uint32_t len){
  double mean = average(array,len);

  int i;
  double var = 0, sq = 0;

  for(i = 0; i < len; i++){
    sq = array[i] - mean;
    sq = sq*sq;
    var += sq;
  }

  return (double)(var/(len-1));
}

double average(float *array, int len){
  double sum = 0 ; 
  for (int i = 0 ; i < len ; i++){
    sum += array [i] ;
  }
  return (double)(sum/len);  
}

