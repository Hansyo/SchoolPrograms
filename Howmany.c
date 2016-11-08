#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <termio.h>


#define TATE 12
#define YOKO 12
#define TAKARA 11
#define clear printf("\033[2J")
#define move00 printf("\033[0;0H")

void makemap(char map[TATE][YOKO],int Player[2]);
int setupramda(int ramda[TAKARA]);
void PrintMap(char map[TATE][YOKO],int Player[2]);
int PlayerMove(int Player[2]);
int HitTAKARA(char map[TATE][YOKO],int Player[2],int ramda[TAKARA],int times);
void FinalPass(int finalramda);
void Info(void);
struct termio old_term,new_term;



int main(void){
  int finalramda;
  int ramda[TAKARA];
  int count,exit,count2;
  int player[2] = { 0 };
  int times = 0;
  char map[TATE][YOKO];

  ioctl(0, TCGETA, &old_term);
  new_term = old_term;
  new_term.c_lflag &= ~(ICANON);

  
  
  for(count = 0;count < TATE;count++){
    for(count2 = 0;count2 < YOKO;count2++){
      map[count][count2] = ' ';
    }
  }
  
  printf("\ntest\n");
	
	
  finalramda = setupramda(ramda);
  /*
    printf("finalramda = %2d\n",finalramda);

    for(count = 0;count < TAKARA;count++){
    printf("ramda[%2d] = %d\n",count + 1,ramda[count]);
    }
  */
  makemap(map,player);
  Info();
  
  ioctl(0, TCSETAW, &new_term);
  getchar();
  ioctl(0, TCSETAW, &old_term);
  while(times < 11){
    clear;
    move00;
    PrintMap(map,player);
    exit = PlayerMove(player);
    times = HitTAKARA(map,player,ramda,times);
    if(exit == 1) break;
  }
  clear;
  move00;
  FinalPass(finalramda);
  return 0;
}




void makemap(char map[TATE][YOKO],int player[2]){
  srand((unsigned)time(NULL));
  /*
    1:通路  = ' ';
    2:宝箱  = '&';
    3:敵   = '#';
    4:壁   = '$';
    5:自機 = '@';
  */
  int count,check1,check2,flag;
  for(count = 0;count < TAKARA + 1;count++){
    while(flag  - 1!= count){
      if(count != TAKARA){
	map[rand() % (TATE - 2) + 1][rand() % (YOKO - 2) + 1] = '&';
      }else{
	player[0] = rand() % (TATE - 2) + 1;
	player[1] = rand() % (YOKO - 2) + 1;
	map[player[0]][player[1]] = '@';
      }
      flag = 0;
      for(check1 = 0;check1 < TATE;check1++){
	for(check2 = 0;check2 < YOKO;check2++){
					
	  if(map[check1][check2] == '&' || map[check1][check2] == '@'){
	    flag++;
	  }
	}
      }
    }
  }
  for(check1 = 0;check1 < TATE;check1++){
    for(check2 = 0;check2 < YOKO;check2++){
      if(check1 == 0 || check1 == TATE - 1){
	map[check1][check2] = '$';
      }else if(check2 == 0 || check2 == YOKO - 1){
	map[check1][check2] = '$';
      }
    }
  }
}

int setupramda(int ramda[TAKARA]){
  srand((unsigned)time(NULL));

  int finalramda = 0;
  int count;
  for(count = 0;count < TAKARA;count ++){
    ramda[count] = rand() % 10;
  }
  for(count = 0;count < TAKARA;count ++){
    finalramda += ramda[count];
  }
  return  finalramda;
}
	

void PrintMap(char map[TATE][YOKO], int player[2]){
  int Tcount,Ycount;
  printf("\n");
  for(Tcount = 0;Tcount < TATE;Tcount++){
    for(Ycount = 0;Ycount < YOKO;Ycount++){
      if(Tcount == player[0] && Ycount == player[1]){
	map[Tcount][Ycount] = '@';
      }else if(map[Tcount][Ycount] == '@'){
	map[Tcount][Ycount] = ' ';
      }
      printf("%c",map[Tcount][Ycount]);
    }
    printf("\n");
  }
}


int PlayerMove(int player[2]){
  char wasd;
  printf("What advances to direction (wasd)? \n >>>");
  ioctl(0, TCSETAW, &new_term);
  wasd = getchar();
  ioctl(0, TCSETAW, &old_term);
  switch(wasd){
  case 'w': if(player[0] != 1 ){ player[0] --;} break;
  case 'a': if(player[1] != 1 ){ player[1] --;} break;
  case 's': if(player[0] != 10){ player[0] ++;} break;
  case 'd': if(player[1] != 10){ player[1] ++;} break;
  case 'n': return 1;
  }
  return 0;
}


int HitTAKARA(char map[TATE][YOKO] ,int player[2], int ramda[TAKARA],int times){
  char input;
  int check = 10;
  if (map[player[0]][player[1]] == '&'){
    printf("\nThis is a Treasure Box.\n");
    printf("You need to apply a password.\n");
    printf("Password's within the range from 0~9.\n");
    while(check != ramda[times]){
      printf("How many? >>");
      ioctl(0, TCSETAW, &new_term);
      input = getchar();
      ioctl(0, TCSETAW, &old_term);
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

void FinalPass(int finalramda){
  int input = 101;

  printf("This is a final Password !!\n");
  printf("Hint : What is the sum of the addition all of the password?\n");
  printf("If you give up ? Please enter the 100.\n");
  while (input != finalramda && input != 100){
    printf("Hoe many ? \n >>>");
    scanf("%d",&input);
  }

  if (input == 100){
    clear;
    move00;
    printf("\n@@@@@@@    @    @@   @@ @@@@@@@   @@@   @     @ @@@@@@@ @@@@@@\n");
    printf("@         @ @   @ @ @ @ @        @   @  @     @ @       @     @\n");
    printf("@         @ @   @  @  @ @       @     @  @   @  @       @     @\n");
    printf("@        @   @  @     @ @@@@@@  @     @  @   @  @@@@@@  @@@@@@ \n");
    printf("@    @@  @@@@@  @     @ @       @     @   @ @   @       @   @  \n");
    printf("@     @ @     @ @     @ @        @   @    @ @   @       @    @ \n");
    printf("@@@@@@@ @     @ @     @ @@@@@@@   @@@      @    @@@@@@@ @     @\n");
  }
  else if (input == finalramda){
    clear;
    move00;
    printf("\n@@@@@@@    @    @@   @@ @@@@@@@         @@@@@  @       @@@@@@@    @    @@@@@@ \n");
    printf("@         @ @   @ @ @ @ @              @     @ @       @         @ @   @     @\n");
    printf("@         @ @   @  @  @ @              @       @       @         @ @   @     @\n");
    printf("@        @   @  @     @ @@@@@@         @       @       @@@@@@   @   @  @@@@@@ \n");
    printf("@    @@  @@@@@  @     @ @              @       @       @        @@@@@  @   @  \n");
    printf("@     @ @     @ @     @ @              @     @ @       @       @     @ @    @ \n");
    printf("@@@@@@@ @     @ @     @ @@@@@@@         @@@@@  @@@@@@@ @@@@@@@ @     @ @     @\n");
  }
}

void Info(void){
  printf("You are coming to look fo treasure in a cave.\n");
  printf("\"@\" is you.\n");
  printf("Method of operation \n\n");
  printf("    ^   \n");
  printf("    w   \n");
  printf(" < asd > \n");
  printf("    V   \n");
  printf("Are you ready? Push to return key!!\n");
}
