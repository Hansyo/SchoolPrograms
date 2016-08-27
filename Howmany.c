#include <stdio.h>
#include <stdlib.h>

#define tate 12
#define yoko 12
#define takara 11
#define clear printf("\033[2J")
#define move00 printf("\033[0;0H")

void makemap(char map[tate][yoko],int Player[2]);
int setupramda(int ramda[takara]);
void PrintMap(char map[tate][yoko],int Player[2]);
int PlayerMove(int Player[2]);
int HitTakara(char map[tate][yoko],int Player[2],int ramda[takara],int times);
void FinalPass(int Finalramda);
void setumei(void);




int main(void){
	int Finalramda;
	int ramda[takara];
	int count,exit;
	int Player[2] = { 0 };
	int times = 0;
	char map[tate][yoko] = {' '};

	
	
	Finalramda = setupramda(ramda);
	/*
	printf("Finalramda = %2d\n",Finalramda);

	for(count = 0;count < takara;count++){
		printf("ramda[%2d] = %d\n",count + 1,ramda[count]);
	}
	*/
	makemap(map,Player);
	setumei();
	while(times < 11){
		//		clear;
		//		move00;
		PrintMap(map,Player);
		exit = PlayerMove(Player);
		times = HitTakara(map,Player,ramda,times);
		if(exit == 1) break;
	}
	//clear;
	//move00;
	FinalPass(Finalramda);
	return 0;
}




void makemap(char map[tate][yoko],int Player[2]){
	srand((unsigned)time(NULL));
	/*
		1:’Ê˜H  = ' ';
		2:•ó”   = '&';
		3:“G   = '#';
		4:•Ç   = '$';
		5:Ž©‹@ = '@';
	*/
	int count,check1,check2,flag;
	for(count = 0;count < takara + 1;count++){
		while(flag  - 1!= count){
			if(count != takara){
				map[rand() % (tate - 2) + 1][rand() % (yoko - 2) + 1] = '&';
			}else{
				Player[0] = rand() % (tate - 2) + 1;
				Player[1] = rand() % (yoko - 2) + 1;
				map[Player[0]][Player[1]] = '@';
			}
			flag = 0;
			for(check1 = 0;check1 < tate;check1++){
				for(check2 = 0;check2 < yoko;check2++){
					
					if(map[check1][check2] == '&' || map[check1][check2] == '@'){
						flag++;
					}
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
			}
		}
	}
}

int setupramda(int ramda[takara]){
	srand((unsigned)time(NULL));

	int Finalramda = 0;
	int count;
	for(count = 0;count < takara;count ++){
		ramda[count] = rand() % 10;
	}
	for(count = 0;count < takara;count ++){
		Finalramda += ramda[count];
	}
	return  Finalramda;
}
	

void PrintMap(char map[tate][yoko], int Player[2]){
	int Tcount,Ycount;
	printf("\n");
	for(Tcount = 0;Tcount < tate;Tcount++){
		for(Ycount = 0;Ycount < yoko;Ycount++){
			if(Tcount == Player[0] && Ycount == Player[1]){
				map[Tcount][Ycount] = '@';
			}else if(map[Tcount][Ycount] == '@'){
				map[Tcount][Ycount] = ' ';
			}
			printf("%c",map[Tcount][Ycount]);
		}
		printf("\n");
	}
}


int PlayerMove(int Player[2]){
	char wasd;
	printf("What advances to direction ? \n >>>");
	wasd = getche();
	switch(wasd){
	case 'w': if(Player[0] != 1 ){ Player[0] --;} break;
	case 'a': if(Player[1] != 1 ){ Player[1] --;} break;
	case 's': if(Player[0] != 10){ Player[0] ++;} break;
	case 'd': if(Player[1] != 10){ Player[1] ++;} break;
	case 'n': return 1;
	}
	return 0;
}


int HitTakara(char map[tate][yoko] ,int Player[2], int ramda[takara],int times){
	char input;
	int check = 10;
	if (map[Player[0]][Player[1]] == '&'){
		printf("This is a Treasure Box.\n");
		printf("You need to apply a password.\n");
		printf("Password's within the range from 0~9.\n");
		while(check != ramda[times]){
			printf("How many? >>");
			input = getche();
			check = input - '0';
			if(check == ramda[times]){
				printf("\nThats right!\n");
				printf("Good to remenber that numbers...\n");
			}else if(check > ramda[times]){
				printf("\nToo big...\n");
			}else{
				printf("\nToo small...\n");
			}
		}
		sleep(2);
		return times + 1;
	}
	return times;
}

void FinalPass(int Finalramda){
	int input = 101;

	printf("This is a Final Password !!\n");
	printf("Hint : What is the sum of the addition all of the password?\n");
	printf("If you give up ? Please enter the 100.\n");
	while (input != Finalramda && input != 100){
	printf("Hoe many ? \n >>>");
	scanf("%d",&input);
	}

	if (input == 100){
		//		clear;
		//		move00;
		printf("\n@@@@@@@    @    @@   @@ @@@@@@@   @@@   @     @ @@@@@@@ @@@@@@\n");
		printf("@         @ @   @ @ @ @ @        @   @  @     @ @       @     @\n");
		printf("@         @ @   @  @  @ @       @     @  @   @  @       @     @\n");
		printf("@        @   @  @     @ @@@@@@  @     @  @   @  @@@@@@  @@@@@@ \n");
		printf("@    @@  @@@@@  @     @ @       @     @   @ @   @       @   @  \n");
		printf("@     @ @     @ @     @ @        @   @    @ @   @       @    @ \n");
		printf("@@@@@@@ @     @ @     @ @@@@@@@   @@@      @    @@@@@@@ @     @\n");
	}
	else if (input == Finalramda){
		//		clear;
		//		move00;
		printf("\n@@@@@@@    @    @@   @@ @@@@@@@         @@@@@  @       @@@@@@@    @    @@@@@@ \n");
		printf("@         @ @   @ @ @ @ @              @     @ @       @         @ @   @     @\n");
		printf("@         @ @   @  @  @ @              @       @       @         @ @   @     @\n");
		printf("@        @   @  @     @ @@@@@@         @       @       @@@@@@   @   @  @@@@@@ \n");
		printf("@    @@  @@@@@  @     @ @              @       @       @        @@@@@  @   @  \n");
		printf("@     @ @     @ @     @ @              @     @ @       @       @     @ @    @ \n");
		printf("@@@@@@@ @     @ @     @ @@@@@@@         @@@@@  @@@@@@@ @@@@@@@ @     @ @     @\n");
	}
}

void setumei(void){
	printf("You are coming to look fo treasure in a cave.\n");
	printf("\"@\" is you.\n");
	printf("Method of operation \n\n");
	printf("   ^   \n");
	printf("   w   \n");
	printf(" <asd> \n");
	printf("   v   \n");
}
