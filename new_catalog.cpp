#include "star.h"
#include "stdlib.h"
#include "stdio.h"
#include <iostream>
#include <fstream>
using namespace std;
#include <sys/stat.h>  //mkdir
#include "math.h"

int main(int argc, char *argv[]) {

  if (argc!=5) {
    printf("Usage: %s /media/NOMAD_SLAC/usno/nomad ",argv[0]);
    printf("/media/NOMAD_SLAC/usno/nomad16 16 0\n");
    printf("Usage: %s /orig/nomad/loc ",argv[0]);
    printf("/new/nomad/loc MAXR MIN_GAL_LAT\n");
    return 1;
  }

  char *fromd=argv[1];
  char *tod=argv[2];
  float magcut=atof(argv[3]);
  float bcut=atof(argv[4]);

  printf("From: %s\n",fromd);
  printf("To  : %s\n",tod);
  printf("Rmag< %5.1f\n",magcut);
  printf("b>= %5.1f\n",bcut);

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

      // Input to output statement
      printf("\n%s -> %s\n",cfrom,cto);
 
      // Open output file
      ofstream *fout = new ofstream(cto,  ios::out|ios::binary);
      if (!fout->is_open()) {
	printf("Could not create %s\n",cto);
	continue;
      }

      // Load all stars from file
      star *stars;
      int nstars=readfile(cfrom,stars);

      // Write stars that make the cut
      int nkept=0;
      int cbin=0;
      int outarr[24*4];
      for (int i=0; i<24*4; ++i) outarr[i]=0;

      printf("      %d stars read in\n",nstars);

      for (int i=0; i<nstars; ++i) {

	//printf("%5.1f %5.1f\n",stars[i].RA,stars[i].DEC);
	//printf("%5.1f %5.1f %5.1f %5.1f %5.1f %5.1f\n",
	//stars[i].B,stars[i].V,stars[i].R,
	//stars[i].J,stars[i].H,stars[i].K);
	if ((stars[i].B>magcut) && 
	    (stars[i].V>magcut) &&  
	    (stars[i].R>magcut) && 
	    (stars[i].J>magcut) && 
	    (stars[i].H>magcut) && 
	    (stars[i].K>magcut)) continue;
	double cra=(double)stars[i].RA;;
	if (bcut>0) {
	  double cdec=(double)stars[i].DEC;
	  double b=radec_to_b(cra,cdec);
	  if (fabs(b)<bcut) continue;
	}
	//printf("RA=%5.2f DEC=%5.2f b=%5.2f\n",cra,cdec,b);
	while (cra>cbin*3.75) {
	  cbin+=1;
	}
	outarr[cbin-1]+=1;


	nkept+=1;
	fout->write(stars[i].raw,88);




      }
      fout->close();
      printf("   %10d read %10d written\n",nstars,nkept);

      // Write accel file
      ofstream *fouta = new ofstream(ctoa,  ios::out);      
      int laststart=1;
      for (int i=0; i<24*4; ++i) {
	int nbin=outarr[i];
	sprintf(oline,"%5.2f %11d %11d\n",0.25*i,laststart,nbin); 
	*fouta << oline;;
	laststart+=nbin;
      }
      fouta->close();
      //break;

    }
  }


  

  

}
