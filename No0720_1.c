#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define sqrto(n) n =((n)*(n))
#define sqr(n) ((n) * (n))

int CtoI(char *,int *);
int BattlePoint(int);
int make100(char *,int *,int);
int randmake100(char *,int *,int,int);
int make90110(char *,int *,int);

int main(void){
	char barcode[100];
	int barint[100];
	int ketasuu;
	int semiBP,sum = 0,tbp;
	int mainloop;
	for(mainloop = 0;mainloop < 3;mainloop++){
		printf("Please enter your Barcode !! \n >>>");
		scanf("%s",barcode);
		
		ketasuu = CtoI(barcode,barint);
		semiBP = make100(barcode,barint,ketasuu);
		printf("\n semiBP = %d\n",semiBP);

		tbp = BattlePoint(semiBP);

		sum += tbp;
		printf("BattlePoint[%d] = %d\n",mainloop + 1,tbp);
	}
	printf("BattlePoint[sum] = %d\n",sum);
	return 0;
}

int CtoI(char *barcode,int *barint){
	int loop = 0,loop2;
	int ketasuu = 0;
	while(loop < 100 && barcode[loop] != '\0'){
		if(barcode[loop] <= '9' && barcode[loop] >= '0'){
			barint[ketasuu] = barcode[loop] - '0';
			barcode[ketasuu] = barcode[loop];
			ketasuu++;
		}
		loop++;
	}
	barcode[ketasuu] = '\0';
	for(loop = 0;loop < 100 && barcode[loop] != '\0';){
		if(barint[loop] == 0 && barint[loop + 1] == 0){
			ketasuu--;
			for(loop2 = loop + 1;loop2 < 100 && barcode[loop] != '\0';loop2++){
				barint[loop2] = barint[loop2 + 1];
			}
		}else{
			loop++;
		}

	}
	printf("barint = ");
	for(loop = 0;loop < ketasuu;loop++){
		printf("%d",barint[loop]);
	}
	printf("\n");
	
	return ketasuu;
}

int make100(char *barcode,int *barint,int ketasuu){
	int loop = 0,hugou[100] = {0},kloop = ketasuu - 1,kloop2;
	int sum,near100;
	int looper;
	sqrto(kloop);
	sqrto(kloop);

	
	for(looper = 0,near100 = 0;near100 != 100;kloop--,looper++) {
		if(looper > 50000000){ near100 = randmake100(barcode,barint,ketasuu,near100); kloop = -1;}//near100 = make90110(barcode,barint,ketasuu);kloop = -1;}
		if(kloop < 0) break;
		loop = 0;
		sum = 0;
		while(barcode[loop] != '\0'){
			switch(hugou[loop]){
			case 0:sum += barint[loop];break;
			case 1:sum *= barint[loop];break;
			case 2:sum -= barint[loop];break;
			case 3:if(barint[loop] == 0){ goto skip;}   sum /= barint[loop];break;
			}
			loop++;
		}
		if(((100 - sum) * (100 - sum)) < sqr(100-near100)){
			if((100 - sum) * (100 - sum) >= 0){
				near100 = sum;
			}
		}
	skip:
		kloop2 = ketasuu - 1;
		hugou[kloop2]++;
		while(hugou[kloop2] >= 4 && kloop2 >= 0){
			hugou[kloop2] = 0;
			hugou[--kloop2]++;
		}

		printf("\rlooper = %8d sum = %08d near100 = %8d",looper,sum,near100);
	}
	return near100;
}

int BattlePoint(int semiBP){
	int bp;
	bp = 120000 - (12 * sqr(100 - semiBP));
	return bp;
}

int randmake100(char *barcode,int *barint,int ketasuu,int near100){
	
	int loop,hugou[100],kloop = ketasuu - 1;
	int sum;
	int looper;

	srand((unsigned) time (NULL));
 
	for(looper = 0,near100 = 0;near100 != 100;looper++) {

		if(looper > 5000) break;

		for(loop = 0;loop < kloop;loop++){
			hugou[loop] = rand() % 4;
		}

		loop = 1;
		sum = barint[0];
		while(barcode[loop] != '\0'){
			switch(hugou[loop]){
			case 0:sum += barint[loop];break;
			case 1:sum *= barint[loop];break;
			case 2:sum -= barint[loop];break;
			case 3:if(barint[loop] == 0){ goto retry;}   sum /= barint[loop];break;
			}
			loop++;
		}
		if(((100 - sum) * (100 - sum)) < sqr(100-near100)){
			near100 = sum;
		}
		
	retry:;
		printf("\rlooper = %8d sum = %08d near100 = %8d",looper,sum,near100);
	}

	if(near100 < 90 || near100 > 100) near100 = make90110(barcode,barint,ketasuu);
	return near100;
}

int make90110(char *barcode,int *barint,int ketasuu){
	printf("\n");
	int semiBP = barint[0],count = 1;
	while(ketasuu != 1){
		if(semiBP * barint[count] < 110 && barint[count] != 0) semiBP *= barint[count];
		else if(semiBP < 100) semiBP += barint[count];
		else semiBP -= barint[count];
		count++;
		printf("\r semiBP = %d",semiBP);
		scanf("%*c");
		ketasuu--;
	}
	return semiBP;
}
