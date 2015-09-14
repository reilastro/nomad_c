#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <math.h>
#include <string.h>
#include <sys/stat.h>
#include "star.hpp"

using namespace std;

int bytes_to_int(char *in) {
  int int32=0;
  int32=(int32 << 8 ) + (unsigned char)in[3];
  int32=(int32 << 8 ) + (unsigned char)in[2];
  int32=(int32 << 8 ) + (unsigned char)in[1];
  int32=(int32 << 8 ) + (unsigned char)in[0];
  return int32;
}


float angdist(float ra1, float dec1,float ra2, float dec2, int verbose) {
  //degrees in degeres out
  double deg2rad=PI/180.0;

  if (verbose>0) {
    printf("%f %f\n",cos(0.01*deg2rad),acos(cos(0.01*deg2rad)));
    printf("angdist ra %6.3f...%6.2f dec %6.2f...%6.3f\n",ra1,ra2,dec1,dec2);
  }


  double s1=sin(dec1*deg2rad);
  double s2=sin(dec2*deg2rad);
  double c1=cos(dec1*deg2rad);
  double c2=cos(dec2*deg2rad);
  double c12=cos((ra1-ra2)*deg2rad);

  double rval=s1*s2+c1*c2*c12;

  return (float)acos(rval)/deg2rad;
}

int count_stars(float ra1, float dec1, float rad, int nstars, star *stars){
  int num=0;
  for (int i=0; i<nstars; ++i) {
    if (stars[i].bad>0) continue; 
    float d=angdist(ra1,dec1,stars[i].cRA,stars[i].cDEC);
    if (d<rad) num+=1;
  }
  return num;
}


int recenter(float ra, float dec, int nstars, star *stars) {
  //degrees is the unit used


  for (int i=0; i<nstars; ++i) {
    double sra=stars[i].cRA;
    double sdec=stars[i].cDEC;

    if (sra> 180.0) sra-=360.0;
    if (sra<-180.0) sra+=360.0;

    double newdec=sdec-dec;

    double newra=(sra-ra);// *cos((dec+sdec)/2.0*DEG2RAD);
    if (newra<-180.0) newra+=360.0;
    if (newra> 180.0) newra-=360.0;

    if (newra<-180.0 || newra>180.0) {
      newdec=(180-(dec+sdec));
    } else {
      newdec=(sdec-dec);
    }

    double s1i=sin((sdec)*DEG2RAD);
    double s2i=sin((dec)*DEG2RAD);

    double c1i=cos((sdec)*DEG2RAD);
    double c2i=cos((dec)*DEG2RAD);
    double c12i=cos((sra-ra)*DEG2RAD);

    double c1f=cos((newdec)*DEG2RAD);
    //double c2f=cos((0)*DEG2RAD);         one
    double s1f=sin((newdec)*DEG2RAD);
    //double s2f=sin((0)*DEG2RAD);         zero
    //double c12f=cos(newra);

    // s1i*s2i + c1i*c2i*c12i == dist before == s1f*s2f + c1f*c2f*c12f
    //c12f = (s1i*s2i + c1i*c2i*c12i - s1f*s2f)/c1f*c2f;
    //c12f = (s1i*s2i + c1i*c2i*c12i)/c1f;

    double rv=(s1i*s2i+c1i*c2i*c12i)/(c1f);
    newra=acos(rv)*RAD2DEG;
    if (sra<ra) newra*=-1;

    //if (fabs(sra-ra)<0.1)




    if (1==2) {
      printf("%.1f %.1f %.1f %.1f c12=%.6f c1f=%.1f rv=%.6f %.1f \n",
	     c1i,c2i,s1i,s2i,c12i,c1f,rv,acos(rv)*RAD2DEG);
    }
    //newra*=(c2*c1+s2*s1)/c2;

    stars[i].cRA=(float)newra;
    stars[i].cDEC=(float)newdec;

    
    double d1=angdist(ra,dec,sra,sdec);
    double d2=angdist(0.0,0.0,newra,newdec);


    if (1==2) {
      printf("%f %f %0.1f%% ",d1,d2,100*(d1-d2)/d1);
      printf("RA %4.1f DEC %4.1f SRA %.1f SDEC %.1f NSRA %.1f NSDEC %.1f\n",
	     ra,dec,sra,sdec,newra,newdec);
    }
  }
}

int radec2gal(float ra, float dec, float *b, float *l) {
  

}

int gal2radec(float ra, float dec, float *b, float *l) {

}

int recenter_v1(float ra, float dec, int nstars, star *stars) {
  //degrees and degrees in

  // Does not work above 88.5 degrees declination for a 1.5 degree radius 
  // field.


  for (int i=0; i<nstars; ++i) {
    

    float sra=stars[i].cRA;
    float sdec=stars[i].cDEC;

    float newdec=sdec-dec;

    if (1==1) {
      while (sra-ra > 180) sra-=360;
      while (sra-ra < -180) sra+=360;
    }
    float newra=(sra-ra)*cos((dec+sdec)/2.0*DEG2RAD);

    stars[i].cRA=newra;
    stars[i].cDEC=newdec;

    if (fabs(newra)>1.5)
    printf("%f %f %f %f %f %f\n",ra,dec,sra,sdec,newra,newdec);

  }
}




int readfile(char *name, float *cut, star* &stars,int nin) {

  /*
  char *names[25]={"RA","DEC","sRA","sDEC",                   //4
		   "vRA","vDEC","svRA","svDEC",               //4
		   "ceRA","ceDEC",                            //2
		   "B","V","R","J","H","K",                   //6
		   "USNO-B1","2MASS","YB6","UCAC-2","Tycho2", //5
		   "flags",                                  //1
		   "cRA","cDEC","good"};
  */


  ifstream fin(name,ios::in|ios::binary);

  int values[22];
  float mult[22];
  float valuef[22];
  char buffer[88];

  for (int i= 0; i<10; ++i) mult[i]=0.001/60/60;
  for (int i=10; i<16; ++i) mult[i]=0.001;
  for (int i=16; i<22; ++i) mult[i]=1.0;

  struct stat results;
  if (stat(name, &results)!=0) {
    return nin;
  }
  int nstars=results.st_size/88;
  if (nstars==0) return nin;

  if (1==2) {
    printf("%d stars in file\n",nstars);
    printf("cuts are RA=%f DEC=%f ang=%f bri=%f fai=%f\n",cut[0],cut[1],cut[2],cut[4],cut[3]);
  }

  if (stars==NULL || nin==0) {
    stars=(struct star*)calloc(nstars,sizeof(star));
  } else {
    stars=(struct star*)realloc(stars,(nstars+nin)*sizeof(star));
  }

  int nread=0;
  int nkept=nin;
  if (fin.is_open()) {
    while (1==1) {
      fin.read(buffer,88);
      if (fin.eof()) break;
      nread+=1;

      // Look at first two values (RA and DEC)
      for (int i=0; i<2; ++i) valuef[i]=mult[i]*bytes_to_int(&buffer[i*4]);
      valuef[1]=90.0-valuef[1];

      // Cut on RA and DEC
      if (cut[2]<360 and cut[2]>0) {
	float ad=angdist(cut[0],cut[1],valuef[0],valuef[1]);

	
	//if (ad<10.0)
	//  printf("ad=%5.1f vs %5.1f     %f %f %f %f\n",ad,cut[2],
	//	 cut[0],valuef[0],cut[1],valuef[1]);
	if (ad>cut[2]) continue;
      }

      // Look at magnitudes (10-16)
      for (int i=10; i<16; ++i) valuef[i]=mult[i]*bytes_to_int(&buffer[i*4]);

      // Cut on magnitudes
      int ncut=6;
      for (int i=10; i<16; ++i) {
	if (cut[3]>=30 and cut[4]>=30) break;
	if (valuef[i]>cut[4] && valuef[i]<cut[3]) ncut-=1;
      }
      stars[nkept].bad=0;
      if (ncut>=6) stars[nkept].bad=1;
      if (stars[nkept].bad==1) continue;

      // Load values 2-10 and 16-22
      for (int i=2; i<10; ++i) valuef[i]=mult[i]*bytes_to_int(&buffer[i*4]);
      for (int i=16; i<22; ++i) values[i]=bytes_to_int(&buffer[i*4]);

      stars[nkept].RA=valuef[0];
      stars[nkept].DEC=valuef[1];
      stars[nkept].sRA=valuef[2];
      stars[nkept].sDEC=valuef[3];
      stars[nkept].vRA=valuef[4];
      stars[nkept].vDEC=valuef[5];
      stars[nkept].svRA=valuef[6];
      stars[nkept].svDEC=valuef[7];
      stars[nkept].ceRA=valuef[8];
      stars[nkept].ceDEC=valuef[9];
      
      stars[nkept].B=valuef[10];
      stars[nkept].V=valuef[11];
      stars[nkept].R=valuef[12];
      stars[nkept].J=valuef[13];
      stars[nkept].H=valuef[14];
      stars[nkept].K=valuef[15];
      
      stars[nkept].USNO_B1=values[16];
      stars[nkept].MASS2=values[17];
      stars[nkept].YB6=values[18];
      stars[nkept].UCAC_2=values[19];
      stars[nkept].Tycho2=values[20];
      
      stars[nkept].flags=values[21];
      
      stars[nkept].cRA=valuef[0];
      stars[nkept].cDEC=valuef[1];


      //for (int i= 0; i<16; ++i) outf[nkept*16+i]=valuef[i];
      //for (int i=16; i<22; ++i) outi[nkept* 6+i]=values[i];

      nkept+=1;
      if (nkept>=MAXSTARSKEPT) break;


    }                                       // while 1==1 (loop over input file
  } else {
    printf("Could not open file name %s.\n",name);
  }

  //printf("Star list now size=%d\n",PyList_Size(starlist));

  
  star *tstars=(struct star*)calloc(nkept,sizeof(star));
  for (int i=0; i<nkept; ++i) {
    tstars[i]=stars[i];
  }
  free(stars);
  stars=tstars;

  return nkept;
  //return nread;
}
