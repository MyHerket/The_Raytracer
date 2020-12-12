#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "lodepng.h"
//#include <iostream>


// Escritura de un arreglo 1D en una imagen PNG
int savePNG(const char *filename, unsigned char *img, int width, int height)
{
    unsigned char *png;
    size_t pngsize;
    int error = lodepng_encode24(&png, &pngsize, img, width, height);
    if(!error)
    {
        lodepng_save_file(png, pngsize, filename);
    }

    if(error)
        printf("\tError %u al grabar la imagen %s: %s\n", error, filename,
                lodepng_error_text(error));

    printf("Termina %s\n", filename);
    free(png);
    return(error);
}


unsigned char *readPNG(const char* filename, unsigned int &width, unsigned int &height,
             unsigned int &bitdepth, unsigned int &bitsXpixel, unsigned int &channels,
             unsigned int &isGrey, unsigned int &haveAlpha) {
  unsigned error;
  unsigned char* image;
  unsigned char* png = 0;
  size_t pngsize;
  LodePNGState state;

  lodepng_state_init(&state);

  error = lodepng_load_file(&png, &pngsize, filename);
  if(!error) error = lodepng_decode(&image, &width, &height, &state, png, pngsize);
  if(error) printf("error %u: %s\n", error, lodepng_error_text(error));
  free(png);

  LodePNGColorMode& color = state.info_png.color;

  bitdepth   = color.bitdepth;
  bitsXpixel = lodepng_get_bpp(&color);
  channels   = lodepng_get_channels(&color);
  isGrey     = lodepng_is_greyscale_type(&color);
  haveAlpha  = lodepng_can_have_alpha(&color);

  lodepng_state_cleanup(&state);
  return(image);
}

// A partir de una imagen img1D codificada como mediante un arreglo 1D, esta funcion
// devuelve un arreglo 2D que tiene la componente de color indicada por index:
// index = 0  para recuperar el canal rojo
// index = 1  para recuperar el canal verde
// index = 2  para recuperar el canal azul
unsigned char **getChannel2D(unsigned char *img1D, unsigned int h, unsigned int w,
                             unsigned int index)
{
    unsigned int    i, j, k, l;
    unsigned int    channels = 3;
    unsigned char **mat;

    // Reservamos memoria
    mat    = (unsigned char **) malloc( h*sizeof(unsigned char *));
    if(mat==NULL) return(NULL);
    mat[0] = (unsigned char *) malloc( h*w*sizeof(unsigned char));
    if(mat[0]==NULL) return(NULL);
    for(i=1; i<h; ++i)
        mat[i] = mat[i-1] + w;

    // Lectura de los datos
    l = (channels + 1);
    for(i=0; i<h; i++) {
        k = i*w*(channels+1);
        for(j=0; j<w; j++) {
            mat[i][j] = img1D[j*l + index + k];
        }
    }

    return(mat);
}

// Convierte un arraglo 2D que tiene la informacion de una imagen a un arreglo 1D
// como se requiere para poderlo grabarlo como imagen PNG en escala de grises
unsigned char *convert2Dto1D(unsigned char **img2D, unsigned int h, unsigned int w)
{
    unsigned char *img1D = (unsigned char *) malloc(sizeof(unsigned char)*h*w*3);
    unsigned int   i, j, k, l;
    unsigned char  val;

    for(i=0; i<h; i++) {
        k = 3*w*i;
        for(j=0; j<w; j++) {
            val = (unsigned char) img2D[i][j];
            l   = 3*j + k;
            img1D[l]   = val;
            img1D[l+1] = val;
            img1D[l+2] = val;
        }
    }
    return(img1D);
}

// Combina la informacion de tres arreglos 2D para formar un arreglo 1D para una imagen a color
unsigned char *convert2Dto1D(unsigned char **imgR, unsigned char **imgG,
                             unsigned char **imgB, unsigned int h, unsigned int w)
{
    unsigned char *img1D = (unsigned char *) malloc(sizeof(unsigned char)*h*w*3);
    unsigned int   i, j, k, l;

    for(i=0; i<h; i++) {
        k = 3*w*i;
        for(j=0; j<w; j++) {
            l   = 3*j + k;
            img1D[l]   = imgR[i][j];
            img1D[l+1] = imgG[i][j];
            img1D[l+2] = imgB[i][j];
        }
    }
    return(img1D);
}


// Libera la memoria del arreglo bidimensional
void freeImage2D(unsigned char **mat) {
    free(mat[0]);
    free(mat);
}

/*
int main(int argc, char *argv[]) /*list the chunks*/
/*{
    if(argc < 2)
    {
        printf("De el nombre de la imagen.\n");
        return 0;
    }
    const char* filename = argv[1];

    unsigned char *image1D;
    unsigned int    w, h, bitdepth, bitsXpixel, channels, isGrey, haveAlpha;

    // Lectura de la imagen
    image1D = readPNG(filename, w, h, bitdepth, bitsXpixel, channels, isGrey, haveAlpha);

    printf(" Dimensiones de la imagen: %d x %d\n", h, w);
    printf("      Bits de profundidad: %d\n", bitdepth);
    printf("           Bits por pixel: %d\n", bitsXpixel);
    printf("        Numero de canales: %d\n", channels);
    printf("Esta en escala de grises?: %d\n", isGrey);
    printf("     Tiene un canal alfa?: %d\n", haveAlpha);

    // Arreglos 2D de cada componente de la imagen. Si la imagen esta
    // en escala de grises, los tres arreglos son iguales.
    // Este formato es mas util para trabajar con la imagen como si
    // fuera una matriz pues los valores de los pixeles se pueden obtener
    // como img[i][j] para i=0,...,h-1 y j=0,...,w-1
    unsigned char **imgR = getChannel2D(image1D, h, w, 0);
    unsigned char **imgG = getChannel2D(image1D, h, w, 1);
    unsigned char **imgB = getChannel2D(image1D, h, w, 2);

    // Por ejemplo, el siguiente ciclo cambia los tonos de gris en un rectangulo.
    // Note que la imagen al menos tiene que tener 180 filas y 80 columnas para que
    // haya un error
    for(int i=0; i<40; i++)
        for(int j=20; j<80; j++) {
            imgR[i+20][j]  = (unsigned char) 255;
            imgG[i+80][j]  = (unsigned char) 255;
            imgB[i+140][j] = (unsigned char) 255;
        }

    // Se convierte cada canal a un arreglo 1D por separado para poder grabarlos como PNG
    unsigned char *imgR1 = convert2Dto1D(imgR, h, w);
    unsigned char *imgG1 = convert2Dto1D(imgG, h, w);
    unsigned char *imgB1 = convert2Dto1D(imgB, h, w);

    // Se combinan los tres canales en un solo arreglo 1D para formar una imagen a color
    unsigned char *imgRGB1 = convert2Dto1D(imgR, imgG, imgB, h, w);

    // Se graban todos los arreglos 1D como imagen PNG.
    savePNG("canal_R.png", imgR1, w, h);
    savePNG("canal_G.png", imgG1, w, h);
    savePNG("canal_B.png", imgB1, w, h);
    savePNG("canales_RGB.png", imgRGB1, w, h);

    free(image1D);
    freeImage2D(imgR);
    freeImage2D(imgG);
    freeImage2D(imgB);
    free(imgR1);
    free(imgG1);
    free(imgB1);
    free(imgRGB1);

    return(0);
}
*/

