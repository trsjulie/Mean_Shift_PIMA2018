#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <Imlib2.h>
#include <string.h>
#include "/home/julie/Documents/projet/imageprojects/imlib/imlib.h"

void compute_mean_shift(unsigned char *original_dot_set, unsigned char *shifted_dot_set, unsigned int cardinal, unsigned char h);

unsigned char * read_grayscale(char *fname, int *dimx, int *dimy)
{
    Imlib_Image *image;

    image = (Imlib_Image *)imlib_load_image(fname);
    if(image)
    {
        unsigned char *buf, *data;
        int l,c;

        imlib_context_set_image( image);
        *dimx = imlib_image_get_width();
        *dimy = imlib_image_get_height();
        data  = (unsigned char *)imlib_image_get_data();
        buf = (unsigned char*) malloc(sizeof(char)**dimx**dimy);
        for(l=0;l<*dimy;l++)
	  for(c=0;c<*dimx;c++)
            {
                *buf++ = (*data + *(data+1) + *(data+2))/3;
                data += 4;
            }
        imlib_free_image();
        return buf-*dimx**dimy;
    }
    else
    {
        fprintf( stderr, "Erreur: image %s n'a pu etre ouverte.\n", fname);
        exit(33);
    }
}

/*
 * Ecrit une image sur le disque. L'extension du nom de fichier
 * donne le format: Par exemple: toto.jpg -> l'image sera écrite
 * au format Jpeg.
 */

void write_grayscale(char *fname, int dimx, int dimy, unsigned char *buf)
{
    Imlib_Image *image;
    unsigned char *data;
    int l, c;
    char *ext;

    if( !(ext = strchr( fname, '.')))
    {
        fprintf( stderr, "Erreur: format image non reconnu\n");
        exit(34);
    }

    data = (unsigned char *)malloc(sizeof(char)*dimx*dimy*4);
    for( l=0; l<dimy; l++)
      for( c=0; c<dimx; c++)
        {
            *data = *buf;
            *(data+1)= *buf;
            *(data+2) = *buf;
            *(data+3) = 0;
            buf ++; data += 4;
        }
    data -= dimx*dimy*4;
    image = (Imlib_Image *)imlib_create_image_using_data( dimx, dimy, (DATA32*)data);
    imlib_context_set_image( image);
    imlib_image_set_format(ext+1);
    imlib_save_image( fname);
    imlib_free_image();
    free( data);
}

/*
 * Un exemple d'utilisation des deux fonctions précédentes.
 */

void process_image();



int main(int argc, char **argv){
	char *inName, *outName;
    double seuil;
    if(argc > 3)
		{
			unsigned char *buf_in, *buf_out;
			int dimx, dimy; /* nombre de colonnes, de lignes de l'image */
			int x, y;
			
			/* parametres de la commande */
			inName=argv[1];
			outName=argv[2];
			seuil = atof(argv[3]);
			
			/* lecture image */
			buf_in = read_grayscale(inName, &dimx, &dimy);
			write_grayscale("original_dot_set.jpg", dimx, dimy, buf_in);
			/* traitement sur l'image */
			buf_out = (unsigned char *)malloc(dimx*dimy);

			printf("Avant compute mean shift\n");
			compute_mean_shift(buf_in, buf_out, dimx* dimy, 20);
			printf("Après compute mean shift\n");
			/* ecriture image */
			write_grayscale(outName, dimx, dimy, buf_out);
			
			free(buf_in);
			free(buf_out);
		}
    else
        printf("Usage: %s image-in image-out seuil\n", *argv);
	
	return 0;
}
