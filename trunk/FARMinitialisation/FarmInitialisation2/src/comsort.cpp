#include "comsort.h"

void swapint(int& a , int& b)
{
   int t;
   t = a;
   a = b;
   b = t;
}

void swapdoub(float& a , float& b)
{
   double t;
   t = a;
   a = b;
   b = t;
}

void swappoint(void * &a, void * &b)
{
 void *t;
   t = a;
   a = b;
   b = t;
}

int newGap(int gap)
{
  gap = (gap * 10) / 13;
  if (gap == 9 || gap == 10)
    gap = 11;
  if (gap < 1)
    gap = 1;
  return gap;
}

void Comsort(temp_double_array double_array, temp_ptr_array ptr_array, int Hi)
{
   int gap = Hi;
   while (true)
   {
      gap = newGap(gap);
      bool swapped=false;
      for (int k = 0; k < Hi-gap; k++)
      {
         int j = k + gap;
         if (double_array[k] > double_array[j])
         {
            float a=double_array[k];
            double_array[k]=double_array[j];
            double_array[j]=a;
            void * p=ptr_array[k];
            ptr_array[k]=ptr_array[j];
            ptr_array[j]=p;
            swapped = true;
         }
      }
      if (gap == 1 && !swapped)
         break;
   }
}

// sort with pointer as passenger
int partition(temp_double_array a, temp_ptr_array b, int left, int right)
//    { nested function inherits definitions of }
//    { "left" and "right" from enclosing procedure }
{
   double v ;            //{ pivot }
   int l, r ;
   v = a[right];  //{ select pivot }
   l = left;
   r = right-1;
   do
   {
    	if ((a[l] < v)){do {l=l+1;}  while (a[l] < v);}
    	if ((a[r] >= v)&& (l < r)) {do {r = r-1;}  while ((a[r] >= v)&& (l < r));}
    	swapdoub(a[l],a[r]);
    	swappoint(b[l],b[r]);
//    swap(a[l], a[r]);
   } while (l<r);
   swapdoub(a[l], a[r]);      // { undo that final swap }
   swapdoub(a[l], a[right]);
   swappoint(b[l], b[r]);       //{ undo that final swap }
   swappoint(b[l], b[right]);
   return  l;
}

void quicksort2(temp_double_array a, temp_ptr_array b, int left, int right)
{
   int p;
   if (left < right)
   {
    	p = partition(a,b, left, right);
    	quicksort2(a,b,left,p-1);
    	quicksort2(a,b,p+1,right);
   }
}

