#include <stdio.h>
#include <stdlib.h>
#define WIDTH 256
#define HEIGHT 256


int main() {
	FILE *fptr, *optr;
	fptr = fopen("data.txt", "rb");
	optr = fopen("img.raw", "wb");

	unsigned char *img;
	img = (unsigned char*)malloc(WIDTH * HEIGHT * sizeof(unsigned char) * 3);

	for(int y = 0 ; y < HEIGHT ; y++){
		for(int x = 0 ; x < WIDTH ; x++){
			img[(y * WIDTH + x) * 3 + 0] = 0;
			img[(y * WIDTH + x) * 3 + 1] = 0;
			img[(y * WIDTH + x) * 3 + 2] = 0;
		}
	}


	int Counter = 0;
	do {
		int x = 0;
		int y = 0;
		int v = 0;
		char c;
		int R = 0;
		int G = 0;
		int B = 0;
		
		fscanf(fptr, "%d", &x);
		fscanf(fptr, "%c", &c);
		fscanf(fptr, "%d", &y);
		fscanf(fptr, "%c", &c);
		fscanf(fptr, "%d", &v);
		fscanf(fptr, "%c", &c);
		fscanf(fptr, "%d", &R);
		fscanf(fptr, "%c", &c);
		fscanf(fptr, "%d", &G);
		fscanf(fptr, "%c", &c);
		fscanf(fptr, "%d", &B);

		printf("%d %d %d\n", x, y, v);

		if(v == 0){
			img[(y * WIDTH + x) * 3 + 0] = 0;
			img[(y * WIDTH + x) * 3 + 1] = 0;
			img[(y * WIDTH + x) * 3 + 2] = 0;
		}
		else{
			img[(y * WIDTH + x) * 3 + 0] = R;
			img[(y * WIDTH + x) * 3 + 1] = G;
			img[(y * WIDTH + x) * 3 + 2] = B;
		}

		Counter++;
	}while (fgetc(fptr) != EOF);
	
	printf("%d\n", Counter);
	
	for(int idx = 0 ; idx < WIDTH * HEIGHT ; idx++){
		fputc(img[idx * 3 + 0], optr);
		fputc(img[idx * 3 + 1], optr);
		fputc(img[idx * 3 + 2], optr);
	}



	fclose(fptr);
	fclose(optr);

	return 0;
}