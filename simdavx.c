#include <stdio.h>
#include <stdlib.h>
#include <math.h>


int main()
{
	/* screen ( integer) coordinate */
	int iX, iY;
	const int iXmax = 16384;
	const int iYmax = 16384;
	/* world ( double) coordinate = parameter plane*/
	double Cxn[4], Cy, Cyn[4];
	const double CxnMin = -2.5;
	const double CxnMax = 1.5;
	const double CyMin = -2.0;
	const double CyMax = 2.0;
	double soma[4];
	int ite[4], pos[4];
	/* */
	double PixelWidth = (CxnMax - CxnMin) / iXmax;
	double PixelHeight = (CyMax - CyMin) / iYmax;
	/* color component ( R or G or B) is coded from 0 to 255 */
	/* it is 24 bit color RGB file */
	const int MaxColorComponentValue = 255;
	FILE * fp;
	char *filename = "_simd_avx.ppm";
	static unsigned char color[3];
	/* Z=Zx+Zy*i  ;   Z0 = 0 */
	double Zx[4], Zy[4];
	double Zx2[4], Zy2[4]; /* Zx2=Zx*Zx;  Zy2=Zy*Zy  */
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
	for (iY = 0; iY<iYmax; iY++)
	{
		Cy = CyMin + iY*PixelHeight;
		if (fabs(Cy)< PixelHeight / 2) Cy = 0.0; /* Main antenna */
		for (iX = 0; iX<iXmax; iX=iX+4)
		{
			Cxn[0] = CxnMin + iX*PixelWidth;
			Cxn[1] = CxnMin + (iX + 1)*PixelWidth;
			Cxn[2] = CxnMin + (iX + 2)*PixelWidth;
			Cxn[3] = CxnMin + (iX + 3)*PixelWidth;
			Cyn[3] = Cy; Cyn[2] = Cy; Cyn[1] = Cy; Cyn[0] = Cy;

			/* initial value of orbit = critical point Z= 0 */
			Zx[3] = 0.0; Zx[2] = 0.0; Zx[1] = 0.0; Zx[0] = 0.0;
			Zy[3] = 0.0; Zy[2] = 0.0; Zy[1] = 0.0; Zy[0] = 0.0;
			Zx2[3] = Zx[3] * Zx[3]; Zx2[2] = Zx[2] * Zx[2]; Zx2[1] = Zx[1] * Zx[1]; Zx2[0] = Zx[0] * Zx[0];
			Zy2[3] = Zy[3] * Zy[3]; Zy2[2] = Zy[2] * Zy[2]; Zy2[1] = Zy[1] * Zy[1]; Zy2[0] = Zy[0] * Zy[0];
			pos[3] = 0; pos[2] = 0; pos[1] = 0; pos[0] = 0;

			__asm{
				vmovupd ymm0, Zx[0]
					vmovupd ymm1, Zy[0]
					vmovupd ymm2, Zx2[0]
					vmovupd ymm3, Zy2[0]
					vmovupd ymm4, Cxn[0]
					vmovupd ymm5, Cyn[0]
			}

			for (Iteration = 0; Iteration<IterationMax; Iteration++)
			{
				int i;
				__asm{
				
						vmulpd ymm1, ymm0, ymm1
						vaddpd ymm1, ymm1, ymm1
						vaddpd ymm1, ymm1, ymm5      /*zy*/
						vsubpd ymm0, ymm2, ymm3
						vaddpd ymm0, ymm0, ymm4     /*zx*/
						vmulpd ymm2, ymm0, ymm0     /*zx2*/
						vmulpd ymm3, ymm1, ymm1     /*zy2*/
						vaddpd ymm6, ymm2, ymm3     /*zx2+zy2*/
						vmovupd soma[0], ymm6          /*movo a soma de zx2 e zy2 para um vetor para poder fazer as comparações*/
				}
				for (i = 0; i < 4; i++){
					if (soma[i] >= ER2 && pos[i] == 0){ //se o erro foi maior ou igual a ER2, salva a iteração e atribui 1 no vetor posição 
						ite[i] = Iteration;            //para nao salvar a iteração novamente. Serve para setar em preto ou colorido o pixel
						pos[i] = 1;
					}
				}


			};
			/* compute  pixel color (24 bit = 3 bytes) */
			int j;
			for (j = 0; j < 4; j++){
				if (pos[j] == 0)
				{ /*  interior of Mandelbrot set = black */
					color[0] = 0;
					color[1] = 0;
					color[2] = 0;
				}
				else
				{ /* exterior of Mandelbrot set = white */
					color[0] = ((IterationMax - ite[j]) % 8) * 63;  /* Red */
					color[1] = ((IterationMax - ite[j]) % 4) * 127;  /* Green */
					color[2] = ((IterationMax - ite[j]) % 2) * 255;  /* Blue */
				};
				/*write color to the file*/
				fwrite(color, 1, 3, fp);
			}
		}
		__asm{
			emms
		}
	}
	fclose(fp);
	return 0;
}
