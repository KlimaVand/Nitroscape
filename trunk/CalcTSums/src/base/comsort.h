#ifndef COMSORT_H_
#define COMSORT_H_

typedef float temp_double_array[2000];
typedef int temp_int_array[2000];
typedef void * temp_ptr_array[2000];

void Comsort(temp_double_array double_array, temp_ptr_array ptr_array, int Hi);
void quicksort2(temp_double_array a, temp_ptr_array b, int left, int right);

#endif /*COMSORT_H_*/
