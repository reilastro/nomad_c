#include <stddef.h>

#ifndef MAXSTARSKEPT
#define MAXSTARSKEPT 6000000
#endif

struct star{
  float RA;
  float DEC;
  float sRA;
  float sDEC;
  float vRA;
  float vDEC;
  float svRA;
  float svDEC;
  float ceRA;
  float ceDEC;

  float B;
  float V;
  float R;
  float J;
  float H;
  float K;

  float cRA;
  float cDEC;

  int USNO_B1;
  int MASS2;
  int YB6;
  int UCAC_2;
  int Tycho2;

  int flags;
  int bad;

  char raw[88];

};

int readfile(char *name, star* &stars, int nin=0, float *cut=NULL);
float angdist(float ra1, float dec1,float ra2, float dec2, int verbose=0);
int bytes_to_int(char *in);
int recenter(float ra, float dec, int nstars, star *stars);
int count_stars(float ra1, float dec1, float rad, int nstars, star *stars);

double radec_to_b(double ra, double dec);
double radecb_to_l(double ra, double dec, double b);
void radec_to_bl(double ra, double dec, double *b, double *l);
