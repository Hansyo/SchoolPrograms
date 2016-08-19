#include <stdio.h>
#include <stdlib.h>

#define tate 12
#define yoko 12
#define takara 11

void makemap(char map[tate][yoko]);

int main(void){
	int Finalramda;
	int ramda[takara];
	int count,cons = 100,check1,check2;
	char map[tate][yoko];

	
	srand((unsigned)time(NULL));
	
	Finalramda = rand() % 100;

	printf("Finalramda = %2d\n",Finalramda);

	while(cons != Finalramda){
		for(count = 0;count < takara;count++){
			ramda[count] = rand() % 10;
		}
		cons = ramda[0];
		for(count = 1;count < takara;count++){
			cons += ramda[count];
		}
		/*for(count = 0;count < takara;count++){
			printf("ramda[%2d] = %2d\n",count + 1,ramda[count]);
			}
			printf("Please push anywhere and return");
			scanf("%*c%*c");
		*/
	}
	for(count = 0;count < takara;count++){
		printf("ramda[%2d] = %2d\n",count + 1,ramda[count]);
	}
	makemap(map);
	for(check1 = 0;check1 < tate;check1++){
		for(check2 = 0;check2 < yoko;check2++){
			printf("%c",map[check1][check2]);
		}
		printf("\n");
	}
	return 0;
}

void makemap(char map[tate][yoko]){
	srand((unsigned)time(NULL));
	/*
		1:’Ê˜H = ' ';
		2:•ó”  = '&';
		3:“G  = '#';
		4:•Ç  = '$';
	*/
	int count,check1,check2,flag;
	for(count = 0;count < takara;count++){
		while(flag  - 1!= count){
			map[rand() % (tate - 2) + 1][rand() % (yoko - 2) + 1] = '&';
			flag = 0;
			for(check1 = 0;check1 < tate;check1++){
				for(check2 = 0;check2 < yoko;check2++){
					if(map[check1][check2] == '&') flag++;
				}
			}
		}
	}
	for(check1 = 0;check1 < tate;check1++){
		for(check2 = 0;check2 < yoko;check2++){
			if(check1 == 0 || check1 == tate - 1){
				map[check1][check2] = '$';
			}else if(check2 == 0 || check2 == yoko - 1){
				map[check1][check2] = '$';
			}else if(map[check1][check2] != '&'){
				map[check1][check2] = ' ';
			}
		}
	}
}
