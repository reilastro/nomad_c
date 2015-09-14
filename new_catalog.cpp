#include "star.hpp"
#include "stdlib.h"
#include "stdio.h"
#include "math.h"
#include <iostream>
#include <fstream>
using namespace std;
#include <sys/stat.h>  //mkdir

#define R2D 180.0/PI
#define D2R PI/180.0

double radec_to_b(double ra, double dec) {
  double sinb=0.0;
  sinb+=sin(dec*D2R)*cos(62.6*D2R);
  sinb-=cos(dec*D2R)*sin((ra-282.25)*D2R)*sin(62.6*D2R);
  return asin(sinb)*R2D;
}


void radec_to_bl(double ra, double dec, double *b, double *l) {
  // http://scienceworld.wolfram.com/astronomy/GalacticCoordinates.html
  *b=radec_to_b(ra,dec);
  *l=ra;

  double sinb=0.0;
  sinb+=sin(dec*D2R)*cos(62.6*D2R);
  sinb-=cos(dec*D2R)*sin((ra-282.25)*D2R)*sin(62.6*D2R);
  *b=asin(sinb)*R2D;
}


int main(int argc, char *argv[]) {

  if (argc!=5) {
    printf("Usage: %s /media/NOMAD_SLAC/usno/nomad ",argv[0]);
    printf("/media/NOMAD_SLAC/usno/nomad16 16 0\n");
    return 1;
  }

  char *fromd=argv[1];
  char *tod=argv[2];
  float magcut=atof(argv[3]);
  float bcut=atof(argv[4]);


  printf("From: %s\n",fromd);
  printf("To  : %s\n",tod);
  printf("Rmag< %5.1f\n",magcut);

  char cfrom[1024];
  char cto[1024];
  char ctoa[1024];
  char ctod[1024];
  char oline[1024];

  for (int i=0; i<180; ++i) {
    sprintf(ctod  ,"%s/%03d",tod  ,i);
    int rval = mkdir(ctod, 0777);
    //printf("Create %s -> %d\n",ctod,rval);

    
    for (int j=0; j<10; ++j) {
      sprintf(cfrom,"%s/%03d/m%04d.cat",fromd,i,i*10+j);
      sprintf(cto  ,"%s/%03d/m%04d.cat",tod  ,i,i*10+j);
      sprintf(ctoa ,"%s/%03d/m%04d.acc",tod  ,i,i*10+j);

      ifstream *fin  = new ifstream(cfrom,ios::in|ios::binary);
      if (!fin->is_open()) {
	//printf("   skipped (fin)\n");
	continue;
      }


      ofstream *fout = new ofstream(cto,  ios::out|ios::binary);
      if (!fout->is_open()) {
	fin->close();
	//printf("   skipped (fout)\n");
	continue;
      }

      printf("%s -> %s\n",cfrom,cto);


      int values[22];
      float mult[22];
      float valuef[22];
      char buffer[88];

      for (int i= 0; i<10; ++i) mult[i]=0.001/60/60;
      for (int i=10; i<16; ++i) mult[i]=0.001;
      for (int i=16; i<22; ++i) mult[i]=1.0;

   
      int nread=0;
      int nkept=0;
      int cbin=0;

      int outarr[24*4];
      for (int i=0; i<24*4; ++i) outarr[i]=0;

      while (1==1) {
	fin->read(buffer,88);
	if (fin->eof()) break;
	nread+=1;

	// Look at magnitudes (10-16)
	for (int i=0; i<16; ++i) valuef[i]=mult[i]*bytes_to_int(&buffer[i*4]);

	bool skip=true;
	for (int i=10; i<16; ++i)
	  if (valuef[i]<magcut) skip=false;
	if (skip) continue;

	skip=false;
	double cra=valuef[0];
	double cdec=valuef[1];
	double b=radec_to_b(cra,cdec);
	if (fabs(b)<bcut) skip=true;
	if (skip) continue;

	//printf("RA=%5.2f DEC=%5.2f b=%5.2f\n",cra,cdec,b);
	while (cra>cbin*3.75) {
	  cbin+=1;
	}
	outarr[cbin-1]+=1;


	nkept+=1;
	fout->write(buffer,88);




      }
      printf("   %10d read %10d written\n",nread,nkept);

      ofstream *fouta = new ofstream(ctoa,  ios::out);
      if (!fouta->is_open()) {
	fin->close();
	printf("   skipped (fouta)\n");
	continue;
      }

      
      int laststart=1;
      for (int i=0; i<24*4; ++i) {
	int nbin=outarr[i];
	sprintf(oline,"%5.2f %11d %11d\n",0.25*i,laststart,nbin); 
	*fouta << oline;;
	laststart+=nbin;
      }
      fouta->close();

      fin->close();
      fout->close();
      //break;

    }
  }


  

  

}
