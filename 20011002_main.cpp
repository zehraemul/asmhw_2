#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <iostream>
#include "image_processing.cpp"

using namespace std;

void Dilation(int n, int filter_size, short* resimadres_org);
void Erosion(int n, int filter_size, short* resimadres_org);

int main(void) {
	int M, N, Q, i, j, filter_size;
	bool type;
	int efile;
	char org_resim[100], dil_resim[] = "dilated.pgm", ero_resim[] = "eroded.pgm";
	do {
		printf("Orijinal resmin yolunu (path) giriniz:\n-> ");
		scanf("%s", &org_resim);
		system("CLS");
		efile = readImageHeader(org_resim, N, M, Q, type);
	} while (efile > 1);
	int** resim_org = resimOku(org_resim);

	printf("Orjinal Resim Yolu: \t\t\t%s\n", org_resim);

	short* resimdizi_org = (short*)malloc(N * M * sizeof(short));

	for (i = 0; i < N; i++)
		for (j = 0; j < M; j++)
			resimdizi_org[i * N + j] = (short)resim_org[i][j];

	int menu;
	printf("Yapmak istediginiz islemi giriniz...\n");
	printf("1-) Dilation\n");
	printf("2-) Erosion\n");
	printf("3-) Cikis\n> ");
	scanf("%d", &menu);
	printf("Filtre boyutunu giriniz: ");
	scanf("%d", &filter_size);

	switch (menu) {
	case 1:
		Dilation(N * M, filter_size, resimdizi_org);
		resimYaz(dil_resim, resimdizi_org, N, M, Q);
		printf("\nDilation islemi sonucunda resim \"dilated.pgm\" ismiyle olusturuldu...\n");
		break;
	case 2:
		Erosion(N * M, filter_size, resimdizi_org);
		resimYaz(ero_resim, resimdizi_org, N, M, Q);
		printf("\nErosion islemi sonucunda resim \"eroded.pgm\" ismiyle olusturuldu...\n");
		break;
	case 3:
		system("EXIT");
		break;
	default:
		system("EXIT");
		break;
	}

	system("PAUSE");
	return 0;
}


void Dilation(int n, int filter_size, short* resim_org) {
	short* img2 = (short*)malloc((n + 1) * sizeof(short));
	for (int i = 0; i < n; i++)
		img2[i] = resim_org[i];

	__asm {
		MOV EAX, filter_size
		CMP EAX, 3
		JE uc

		uc : MOV ESI, img2
		ADD ESI, 1026
		MOV EBX, resim_org
		ADD EBX, 1026
		MOV ECX, 510
		dl1 : PUSH ECX
		MOV ECX, 510
		dl2 : MOV EDX, [ESI]
		MOV EAX, [ESI - 1026]
		CMP EAX, EDX
		JGE d1
		MOV EDX, EAX
		d1 : MOV EAX, [ESI - 1024]
		CMP EAX, EDX
		JGE d2
		MOV EDX, EAX
		d2 : MOV EAX, [ESI - 1022]
		CMP EAX, EDX
		JGE d3
		MOV EDX, EAX
		d3 : MOV EAX, [ESI - 2]
		CMP EAX, EDX
		JGE d4
		MOV EDX, EAX
		d4 : MOV EAX, [ESI + 2]
		CMP EAX, EDX
		JGE d5
		MOV EDX, EAX
		d5 : MOV EAX, [ESI + 1022]
		CMP EAX, EDX
		JGE d6
		MOV EDX, EAX
		d6 : MOV EAX, [ESI + 1024]
		CMP EAX, EDX
		JGE d7
		MOV EDX, EAX
		d7 : MOV EAX, [ESI + 1026]
		CMP EAX, EDX
		JGE d8
		MOV EDX, EAX
		d8 : MOV[EBX], EDX
		ADD ESI, 2
		ADD EBX, 2
		LOOP dl2
		POP ECX
		ADD ESI, 4
		ADD EBX, 4
		LOOP dl1
	}

	free(img2);
}
	void Erosion(int n, int filter_size, short* resim_org){
		short* img3 = (short*)malloc((n + 1) * sizeof(short));
		for (int i = 0; i < n; i++)
			img3[i] = resim_org[i];

		__asm {
			MOV EAX, filter_size
			CMP EAX, 3
			JE uc1

			uc1 : MOV ESI, img3
			ADD ESI, 1026
			MOV EBX, resim_org
			ADD EBX, 1026
			MOV ECX, 510
			el1 : PUSH ECX
			MOV ECX, 510
			el2 : MOV EDX, [ESI]
			MOV EAX, [ESI - 1026]
			CMP EAX, EDX
			JLE e1
			MOV EDX, EAX
			e1 : MOV EAX, [ESI - 1024]
			CMP EAX, EDX
			JLE e2
			MOV EDX, EAX
			e2 : MOV EAX, [ESI - 1022]
			CMP EAX, EDX
			JLE e3
			MOV EDX, EAX
			e3 : MOV EAX, [ESI - 2]
			CMP EAX, EDX
			JLE e4
			MOV EDX, EAX
			e4 : MOV EAX, [ESI + 2]
			CMP EAX, EDX
			JLE e5
			MOV EDX, EAX
			e5 : MOV EAX, [ESI + 1022]
			CMP EAX, EDX
			JLE e6
			MOV EDX, EAX
			e6 : MOV EAX, [ESI + 1024]
			CMP EAX, EDX
			JLE e7
			MOV EDX, EAX
			e7 : MOV EAX, [ESI + 1026]
			CMP EAX, EDX
			JLE e8
			MOV EDX, EAX
			e8 : MOV[EBX], EDX
			ADD ESI, 2
			ADD EBX, 2
			LOOP el2
			POP ECX
			ADD ESI, 4
			ADD EBX, 4
			LOOP el1

		}
		free(img3);
	}