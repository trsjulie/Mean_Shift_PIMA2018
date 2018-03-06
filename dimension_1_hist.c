#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "/home/julie/Documents/projet/imageprojects/imlib/imlib.h"

/* Compute the 1 dimension distance between "x" and "y" */
unsigned char distance(unsigned char x, unsigned char y){
	return abs(x - y);
}

/* Compute the weigh of a neighbour following the gaussian kernel thanks to the distance between the current dot and one of its neighbours. */
double gaussian_kernel(unsigned char x){
	return pow(2*M_PI,-0.5)*exp(-0.5 * x * x);
}



double derivee_gaussian_kernel(double x){
   return pow(2*M_PI, -0.5)*exp(-0.5*x*x)*x;
  //  return 1.0 - x;
}

void build_histogram(unsigned char *original_dot_set, unsigned int cardinal, unsigned char *hist){
	unsigned int i;

	for(i = 0; i < 256; i++){
		hist[i] = 0;
	}

	for(i = 0; i < cardinal; i++){
		hist[original_dot_set[i]] ++;
	}
}
/* val c'est la valeur du pixel qu'on veut faire converger, h c'est la bande passante*/
void mean_shift(unsigned char *original_dot_set, unsigned char *shifted_hist,  unsigned int cardinal, unsigned char h, unsigned char val, unsigned char *hist){
	printf("NEW PIXEL\n");
	double scale_factor;/*den*/
	double numerator;
	double weight;
	int i;
	unsigned char inf, sup;
	double error = 1;
	/*inf plus petit pixel du voisinage*/
	inf = (val - h / 2.0 >= 0) ? val - h / 2.0 : 0;
	sup = (val + h / 2.0 <= 255) ? val + h / 2.0 : 255;
	/*on initialise la valeur du nouvel histogramme avec la valeur val*/
	shifted_hist[val] = val;

	
	while(error > 0){
	  /*la borne inf et la borne sup sont calculées pour chaque erreur*/
	  inf = (shifted_hist[val] - h / 2.0 >= 0) ? shifted_hist[val] - h / 2.0 : 0;
	  sup = (shifted_hist[val] + h / 2.0 <= 255) ? shifted_hist[val] + h / 2.0 : 255; 
	  
	  printf("compute until convergence\n");
	  
	  for(i = inf; i <= sup; i++){
	    error = shifted_hist[val];
	    numerator = 0.0;
	    //weight = gaussian_kernel(pow((shifted_hist[val] - i) / h ,2));
	    weight = derivee_gaussian_kernel((double)pow((double)(i /(double) h) ,2.0));
	    scale_factor += (weight *(double)hist[i]);
	    numerator += ((double)i *(double) hist[i] *(double) weight);

	    sup = (shifted_hist[val] + h / 2.0 <= 255) ? shifted_hist[val] + h / 2.0 : 255;
	  }
	  shifted_hist[val] =	(unsigned char)(numerator / scale_factor);
	  
	  error -= shifted_hist[val];
	  if(error < 0) error = -error;	
	}

	
}

/*Appel de mean shift pour chaque valeur de l'histogramme*/
void compute_mean_shift(unsigned char *original_dot_set, unsigned char *shifted_dot_set, unsigned int cardinal, unsigned char h){
	printf("Debut compute mean shift\n");
	unsigned char *hist = (unsigned char *) malloc(sizeof(unsigned char) * 256);
	unsigned char *shifted_hist = (unsigned char *)malloc(sizeof(unsigned char) * 256);
	unsigned int i;

	printf("Avant build hist\n");
	build_histogram(original_dot_set, cardinal, hist);
	printf("Apres build hist\n");
	for(i = 0; i < 256;i++){
	  printf("VAL DERIV GAUS KER\n");
	  printf("%lf\n", derivee_gaussian_kernel((double)i));
	}
	    
	for(i = 0; i < 256; i++)
		printf("%u ", hist[i]);
	putchar('\n');
	/*mean shift pour toutes les valeurs de l'histogramme*/
	for(i = 0; i <= 255; i++){
		printf("Debut for\n");
		if(hist[i] != 0){
			printf("Dans condition\n");
			mean_shift(original_dot_set, shifted_hist, cardinal, h, i, hist);
		}
	}
	
	for(i = 0; i < cardinal; i++){
		shifted_dot_set[i] = shifted_hist[original_dot_set[i]];
		if(shifted_dot_set[i] > 255) shifted_dot_set[i] = 255;
	       	if(shifted_dot_set[i] < 0) shifted_dot_set[i] = 0;
		
	}

	for(i = 0; i < 256; i++)
		printf("%u ", shifted_hist[i]);
	putchar('\n');
}
