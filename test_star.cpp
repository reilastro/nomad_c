#include <iostream>
#include <fstream>
#include "star.h"

using namespace std;

int main(int argc, char *argv[]) {
  //char *fname="/data/nomadBOSS/s2/r06m18dec048ra072.cat";
  //char *fname="/data/nomadBOSS/s2/r06m18dec084ra000.cat";
  char *fname="/data/nomadBOSS/s2/r06m18dec090ra000.cat";

  star *stars;

  float cut[5]={-99,-99,-99,12,10};

  int nstar=readfile(fname,stars,0,cut);
  printf("%10d stars read in.\n",nstar);
  int ngstar=0;
  for (int i=0; i<nstar; ++i) {
    if (stars[i].bad) continue;
    ngstar+=1;
  }
  printf("%10d good stars read in.\n",ngstar);

  recenter(70, 55, nstar, stars);

  for (int i=0; i<nstar; ++i) {
    printf("%f %f  ",stars[i].cRA,stars[i].cDEC);
    printf("%f %f\n",stars[i].RA,stars[i].DEC);
    if (i>10) break;
  }

}
