#include<stdlib.h>
#include<stdio.h>

float Red = 255.0;
float Green = 255.0;
float Blue = 255.0;

int numPts = 50000;

int main(int argc, char* argv)
{
  float colors[50000][3];

  for (int i = 0; i < numPts; i++)
  {
     colors[i][0] = Red;
     colors[i][1] = Green;
     colors[i][2] = Blue;

     printf("%5d %8.3f %8.3f %8.3f\n", i, Red, Green, Blue);

     if(Green == 0 && Blue == 0)
     {
       Green = 255.0;
       Blue = 255.0;
       Red -= 3;
     }
     else if(Blue == 0)
     {
       Blue = 255.0;
       Green -= 3;
     }
     else
     {
       Blue -= 3;
     }
  }

  return 0;
}
