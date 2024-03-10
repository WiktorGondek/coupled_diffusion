/*********************************************************
* This program solves coupled equations which describes
* the diffusion-damped system with constant coefficients.
* A time splitting technique by Marchuk–Strang is used. 
* For two subproblems, A1 and A2, we solve A1 with time length
* tau=(1/2)*dt then solve the other one, A2, for time length
* dt and solve with A1 again for time length tau=(1/2)*dt.
********************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define PI 3.141592

void read_input(double *D, double *L, int *nx, double *t_F);

int main(void) {
  /* Declarations of parameters */
  
  /* Number of grid points */                  
  int nx;
  /* Length of domain */
  double L;
  /* Equation coefficients */
  double D;
  /* Length of time to run simulation. */
  double t_F;

  /* Read in from file; */
  read_input(&D, &L, &nx, &t_F);

  /* Grid spacing */
  double dx = L/(nx-1);
  /* Second order spatial derivative */
  double invdx2 = 1.0/(dx*dx);      
  /* Time step */
  double dt = (dx*dx)/(2*D);
 
  /* Solution Storage at Current / Next time step */
  double *uc, *un, *vc, *vn;
  /* Time splitting solutions */
  double *uts1, *uts2, *vts1, *vts2;
  /* Derivative used in finite difference */
  double deriv;


  /* Allocate memory according to size of nx */
  uc = malloc(sizeof(double)*nx);
  un = malloc(sizeof(double)*nx);
  vc = malloc(sizeof(double)*nx);
  vn = malloc(sizeof(double)*nx);
  uts1 = malloc(sizeof(double)*nx);
  uts2 = malloc(sizeof(double)*nx);
  vts1 = malloc(sizeof(double)*nx);
  vts2 = malloc(sizeof(double)*nx);

  /* Check the allocation pointers */
  if (uc==NULL||un==NULL||vc==NULL||vn==NULL||uts1==NULL||
  uts2==NULL||vts1==NULL||vts2==NULL) {
    printf("Memory allocation failed\n");
    return 1;
  }

  /* Spatial index */
  int k;
  /* x position */
  double x;
  /* Current time */
  double ctime;

  /* Initialise arrays */
  for(k = 0; k <= nx; k++) {
    x = k*dx;
    uc[k]  = 1.0 + sin(2.0*PI*x/L);
    vc[k]  = 0.0;

    /* Set other arrays to 0 */
    uts1[k] = 0; uts2[k] = 0;
    vts1[k] = 0; vts2[k] = 0;
  }

  /* Open output file*/
  FILE *outfile=fopen("diffusion_data","w");
  if (outfile == NULL) {
      printf("Error opening file\n");
      exit(-1);
  }

  /* Loop over timesteps */ 
  while (ctime < t_F){
  
    /* Rotation factors for time-splitting scheme. */
    double cfac = cos(dt);
    double sfac = sin(dt);

    /* First substep for diffusion equation, A_1 */ 
    for (k = 0; k <= nx-1; k++) {
      x = k*dx;
      deriv = (uc[(k-1+nx)%(nx-1)] + uc[(k+1)%(nx-1)] - 2*uc[k])*invdx2;
      uts1[k] = uc[k] + D * deriv * 0.5 * dt;
      deriv = (vc[(k-1+nx)%(nx-1)] + vc[(k+1)%(nx-1)] - 2*vc[k])*invdx2;
      vts1[k] = vc[k] + D * deriv * 0.5 * dt;
    }

    /* Second substep for decay/growth terms, A_2 */
    for (k = 0; k < nx; k++) {
      x = k*dx;
      /* Apply rotation matrix to u and v, */
      uts2[k] = cfac*uts1[k] + sfac*vts1[k];
      vts2[k] = -sfac*uts1[k] + cfac*vts1[k];
    }

    /* Third substep for diffusion terms, A_1 */
    for (k = 0; k <= nx-1; k++) {
      x = k*dx;
      deriv = (uts2[(k-1+nx)%(nx-1)] + uts2[(k+1)%(nx-1)] - 2*uts2[k])*invdx2;
      un[k] = uts2[k] + D * deriv * 0.5 * dt;
      deriv = (vts2[(k-1+nx)%(nx-1)] + vts2[(k+1)%(nx-1)] - 2*vts2[k])*invdx2;
      vn[k] = vts2[k] + D * deriv * 0.5 * dt;
    }
	
    /*Apply periodic boundary conditions*/
    un[nx-1] = un[0];
	vn[nx-1] = vn[0];

    /* Copy next values at timestep to u, v arrays. */
    double *tmp;
    tmp = un;
    un = uc;
    uc = tmp;
    tmp = vn;
    vn = vc;
    vc = tmp;


    /* Increment time. */   
    ctime += dt;

    /*Print results to terminal and output file*/
    for (k = 0; k < nx; k++ ) {
      x = k*dx;
      printf("%g %g %g %g\n",ctime,x,un[k],vn[k]);
      fprintf(outfile, "%g,%g,%g,%g\n", ctime,x,un[k],vn[k]);
    }
  

  }

  fclose(outfile);
  
  /* Free allocated memory */
  free(uc); free(un);
  free(vc); free(vn);
  free(uts1); free(uts2);
  free(vts1); free(vts2);

  return 0;
}

/* Reading from input file */
void read_input(double *D, double *L, int *nx, double *t_F) {
   FILE *infile;
   if(!(infile=fopen("input.txt","r"))) {
       printf("Error opening file\n");
       exit(1);
   }
   if(4!=fscanf(infile,"%lf %lf %d %lf",D,L,nx,t_F)) {
       printf("Error reading parameters from file\n");
       exit(1);
   }
   fclose(infile);
}

