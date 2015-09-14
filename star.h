#ifndef PI
#define PI 3.1415926535 //8979323846264338327950288419
#endif
#ifndef DEG2RAD
#define DEG2RAD (PI/180.0)
#endif
#ifndef RAD2DEG
#define RAD2DEG (180.0/PI)
#endif

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

};

int readfile(char *name, float *cut, star* &stars,int nin=0);
float angdist(float ra1, float dec1,float ra2, float dec2, int verbose=0);
int bytes_to_int(char *in);
int recenter(float ra, float dec, int nstars, star *stars);
int count_stars(float ra1, float dec1, float rad, int nstars, star *stars);
