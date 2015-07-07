#include <stdio.h>
#include <math.h>

int main()
{
	/* screen ( integer) coordinate */
	int iX, iY, i;

	const int iXmax = 16384;
	const int iYmax = 16384;
	/* world ( double) coordinate = parameter plane*/
	double Cx, Cy[4], Res[4], Res1[4];
	const double CxMin = -2.5;
	const double CxMax = 1.5;
	const double CyMin = -2.0;
	const double CyMax = 2.0;
	/* */
	double PixelWidth = (CxMax - CxMin) / iXmax;
	double PixelHeight = (CyMax - CyMin) / iYmax;
	/* color component ( R or G or B) is coded from 0 to 255 */
	/* it is 24 bit color RGB file */
	const int MaxColorComponentValue = 255;
	FILE * fp;
	char *filename = "_simd_avx.ppm";
	static unsigned char color[3];
	/* Z=Zx+Zy*i  ;   Z0 = 0 */
	double Zx, Zy;
	double Zx2, Zy2; /* Zx2=Zx*Zx;  Zy2=Zy*Zy  */
	/*  */
	int Iteration;
	const int IterationMax = 256;
	/* bail-out value , radius of circle ;  */
	const double EscapeRadius = 2;
	double ER2 = EscapeRadius*EscapeRadius;
	/*create new file,give it a name and open it in binary mode  */
	fp = fopen(filename, "wb"); /* b -  binary mode */
	/*write ASCII header to the file*/
	fprintf(fp, "P6\n %d\n %d\n %d\n", iXmax, iYmax, MaxColorComponentValue);
	/* compute and write image data bytes to the file*/

	Res[0] = PixelHeight; Res[1] = PixelHeight; Res[2] = PixelHeight; Res[3] = PixelHeight;
	Res1[0] = CyMin; Res1[1] = CyMin; Res1[2] = CyMin; Res1[3] = CyMin;

	for (iY = 0; iY<iYmax; iY+=4)
	{
		Cy[0] = iY; Cy[1] = iY+1; Cy[2] = iY+2; Cy[3] = iY+3;

		__asm{
			vmovupd ymm0, Cy          //aqui colocamos o conteúdo dos vetores em registradores de 256 bits
			vmovupd ymm1, Res
			vmovupd ymm2, Res1
			vmulpd ymm0, ymm0, ymm1  //fazemos as operações de acordo com o algoritmo de Mandelbrot
			vaddpd ymm0, ymm0, ymm2
			vmovupd Cy, ymm0
		}
		// agora Cy já foi calculado para 4 iterações
		for (i = 0; i < 4; i++){
			if (fabs(Cy[i]) < PixelHeight / 2) Cy[i] = 0.0; /* Main antenna */
			for (iX = 0; iX < iXmax; iX++)
			{
				Cx = CxMin + iX*PixelWidth;
				/* initial value of orbit = critical point Z= 0 */
				Zx = 0.0;
				Zy = 0.0;
				Zx2 = Zx*Zx;
				Zy2 = Zy*Zy;
				/* */
				for (Iteration = 0; Iteration < IterationMax && ((Zx2 + Zy2) < ER2); Iteration++)
				{
					Zy = 2 * Zx*Zy + Cy[i];
					Zx = Zx2 - Zy2 + Cx;
					Zx2 = Zx*Zx;
					Zy2 = Zy*Zy;
				};
				/* compute  pixel color (24 bit = 3 bytes) */
				if (Iteration == IterationMax)
				{ /*  interior of Mandelbrot set = black */
					color[0] = 0;
					color[1] = 0;
					color[2] = 0;
				}
				else
				{ /* exterior of Mandelbrot set = white */
					color[0] = ((IterationMax - Iteration) % 8) * 63;  /* Red */
					color[1] = ((IterationMax - Iteration) % 4) * 127;  /* Green */
					color[2] = ((IterationMax - Iteration) % 2) * 255;  /* Blue */
				};
				/*write color to the file*/
				fwrite(color, 1, 3, fp);
			}
		}
	}
	fclose(fp);
	return 0;
}
