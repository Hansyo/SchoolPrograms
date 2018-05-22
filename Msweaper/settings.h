#ifndef settings_h
#define settings_h

#define BIAS 1
#define NUM_SCANF_MAP 3
#define NUM_SCANF_STATE 2
#define CONTINUE 1
#define EXIT 0
#define OUTOFRANGE -1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum BOOLEAN{
  false,
  true
}BOOLEAN;

typedef enum STATE{
  s_false,
  s_true,
  s_flag
}STATE;

typedef struct SIZE{
  int x;
  int y;
  int bombs;
}SIZE;
/*
  # FIELDの仕様
  ## state
  - s_true マスクがかかっている状態
  - s_false かかっていなくて見える状態
  - s_flag フラグを立てた状態

  ## bomb
  - true 存在する
  - false 存在しない

  ## qty
  - 数字の個数だけ、回りにMが存在する

  **すべてのフィールドは上下左右に一行大きめにとる事に注意**

  ## size
  ###
  - x 横の長さ
  - y 縦の長さ
  - bombs 爆弾の総数

*/
typedef struct FIELD{
  STATE **state;
  BOOLEAN **bomb;
  int **qty;
  SIZE size;
}FIELD;


// プレイヤーにフィールドを表示する為の関数
void print_field              (FIELD map);
void print_all_field          (FIELD map);
void make_field               (FIELD *map);
void set_bomb                 (BOOLEAN **bomb,SIZE size);
int  clear_check              (FIELD map);
int  game_continue            (void);
void free_map_data            (FIELD *map);
void calc_bomb                (int **qty,BOOLEAN **bomb,int x,int y);
void loop_calc_bomb           (FIELD *map);
int  data_input               (FIELD *map,int *x,int *y);
void print_precedents         (void);
void determinate_field        (FIELD *map,int x,int y);
void print_scale_tens_place   (int map_x);
void print_scale_digit_of_one (int map_x);

void print_field(FIELD map){
  int x,y;
  printf("\n\n");
  print_scale_tens_place(map.size.x);
  print_scale_digit_of_one(map.size.x);
  for(y = 0;y < map.size.y;y++){
    if(y % 5 == 0 && y != 0){
      if(y % 2 == 1) printf(" %d ",y/10%10);
      else printf("   ");
      for(x = 0;x < map.size.x;x++){
        if(x % 5 == 0 ) printf("+");
        printf("-");
      }
      if(y % 2 == 1) printf("  %d",y/10%10);
      printf("\n");
    }
    for(x = 0;x < map.size.x;x++){
      if(x == 0){
        switch(y%10){
        case 0:case 4:case 5:case 9: printf(" |%d",y%10);break;
        default: printf(" ||");
        }
        printf(" ");
      }else if(x % 5 == 0) printf("|");
      // bombは、上下左右に一列ずつ
      // 大きくしているため、
      // この様な表示の仕方をする。
      if(map.state[y + BIAS][x + BIAS] == s_false){
        if(map.bomb[y + BIAS][x + BIAS] == true) printf("M");
        else{
          if(map.qty[y + BIAS][x + BIAS] == 0) printf(".");
          else printf("%d",map.qty[y + BIAS][x + BIAS]);
        }
      }else if(map.state[y + BIAS][x + BIAS] == s_flag) printf("!");
      else printf("#");
      if(x == map.size.x - 1){
        switch(y%10){
        case 0:case 4:case 5:case 9: printf(" %d|",y%10);break;
        default: printf(" ||");
        }
      }
    }
    printf("\n");
  }
  print_scale_digit_of_one(map.size.x);
  print_scale_tens_place(map.size.x);
  printf("\n");
}

// ゲーム終了時の表示関数
void print_all_field(FIELD map){
  int x,y;
  printf("\n\n");
  print_scale_tens_place(map.size.x);
  print_scale_digit_of_one(map.size.x);
  for(y = 0;y < map.size.y;y++){
    if(y % 5 == 0 && y != 0){
      if(y % 2 == 1) printf(" %d  ",y/10%10);
      else printf("    ");
      for(x = 0;x < map.size.x;x++){
        if(x % 5 == 0 && x != 0) printf("+");
        printf("-");
      }
      if(y % 2 == 1) printf("  %d",y/10%10);
      printf("\n");
    }
    for(x = 0;x < map.size.x;x++){
      if(x == 0){
        switch(y%10){
        case 0:case 4:case 5:case 9: printf(" |%d",y%10);break;
        default: printf(" ||");
        }
        printf(" ");
      }else if(x % 5 == 0) printf("|");
      if(map.state[y + BIAS][x + BIAS] == s_flag){
        if(map.bomb[y + BIAS][x + BIAS] == true) printf("!");
        else printf("&");
      }else if(map.bomb[y + BIAS][x + BIAS] == true) printf("M");
      else{
        if(map.qty[y + BIAS][x + BIAS] == 0) printf(".");
        else printf("%d",map.qty[y + BIAS][x + BIAS]);
      }
      if(x == map.size.x - 1){
        switch(y%10){
        case 0:case 4:case 5:case 9: printf(" %d|",y%10);break;
        default: printf(" ||");
        }
      }
    }
    printf("\n");
  }
  print_scale_digit_of_one(map.size.x);
  print_scale_tens_place(map.size.x);
  printf("\n");
}

// mallocを使い、フィールドを生成
// 動的に取得するため、メモリの負担を抑えられる。
void make_field(FIELD *map){
  int i,j;
  BOOLEAN flag;
  do{
    do{
      flag = false;
      printf("x,y,m > ");
      if(scanf("%d,%d,%d",&(map -> size.x),&(map -> size.y),&(map -> size.bombs)) != NUM_SCANF_MAP){
        flag = true;
        puts("Enter the correct value.");
      }
      while(getchar() != '\n');
    }while(flag != 0);
  }while(map -> size.x * map -> size.y  <=  map -> size.bombs || \
         map -> size.bombs <= 0 || \
         map -> size.x <= 0 || \
         map -> size.y <= 0 );
  map -> qty   = (int **)     malloc(sizeof(int *)     * (map -> size.y + BIAS*2));
  map -> bomb  = (BOOLEAN **) malloc(sizeof(BOOLEAN *) * (map -> size.y + BIAS*2));
  map -> state = (STATE **)   malloc(sizeof(STATE *)   * (map -> size.y + BIAS*2));
  for(i = 0;i < map -> size.y + BIAS*2;i++){
    map -> qty[i]   = (int *)      malloc(sizeof(int)     * (map -> size.x + BIAS*2));
    map -> bomb[i]  = (BOOLEAN *)  malloc(sizeof(BOOLEAN) * (map -> size.x + BIAS*2));
    map -> state[i] = (STATE *)    malloc(sizeof(STATE)   * (map -> size.x + BIAS*2));
  }
  for(  i = 0;i < (map -> size.y) + (BIAS*2);i++){
    for(j = 0;j < (map -> size.x) + (BIAS*2);j++){
      map -> state[i][j] = s_true;
      map -> qty[i][j]   = OUTOFRANGE;
    }
  }
}

// 他の言語のいわゆるシャッフルってやつ。
void set_bomb (BOOLEAN **bomb,SIZE size){
  int i,j,k,l,calc;
  STATE state;
  for(i = 0;i < size.y + BIAS*2;i++){
    for(j = 0;j < size.x + BIAS*2;j++){
      bomb[i][j] = false;
    }
  }
  for(i = BIAS,calc = 0;i < size.y + BIAS && calc < size.bombs;i++){
    for(j = BIAS;j < size.x + BIAS && calc < size.bombs;j++){
      bomb[i][j] = true;
      calc++;
    }
  }

  for(calc = 0;calc < size.x * size.y;calc++){
    i = (rand() % size.x) + BIAS;
    j = (rand() % size.y) + BIAS;

    k = (rand() % size.x) + BIAS;
    l = (rand() % size.y) + BIAS;

    state = bomb[j][i];
    bomb[j][i] = bomb[l][k];
    bomb[l][k] = state;
  }

}

// ゲームクリアできているかの確認用
// 後々は、旗の数とボムの位置関係でわかるようにするつもり
int clear_check(FIELD map){
  int x,y;
  for(y = 0;y < map.size.y;y++){
    for(x = 0;x < map.size.x;x++){
      if(map.state[y + BIAS][x + BIAS] != s_false){
        if(map.bomb[y + BIAS][x + BIAS] == false) return CONTINUE;
      }
    }
  }
  printf("\n\n\n GAME CLEAR!! \n");
  return EXIT;
}

// ゲームを続けるか聞くだけ。
// main関数内で呼び出され、
// do文の条件となっている。
int game_continue(void){
  char c;
  do{
    printf("Continue?(y/n) > ");
    scanf("%c",&c);
    while(getchar() != '\n');
  }while(c != 'n' && c != 'y');
  if(c == 'n') return EXIT;
  else return CONTINUE;
}

// もう一度mallocできるように、
// 開放するための関数
void free_map_data(FIELD *map){
  int i;
  for(i = 0;i < map -> size.y + BIAS*2;i++){
    free(map -> qty[i]);
    free(map -> bomb[i]);
    free(map -> state[i]);
  }

  free(map -> qty);
  // enum型はfreeの仕様が
  // 若干違うらしい...ややこしい...
}

// 周囲にあるボムの数をカウントし、格納する。
void calc_bomb(int **qty,BOOLEAN **bomb,int x,int y){
  int i,j;
  qty[y + BIAS][x + BIAS] = 0;
  for(i = 0;i < 3;i++){
    for(j = 0;j < 3;j++){
      if(bomb[i + y][j + x]) qty[y + BIAS][x + BIAS]++;
    }
  }
}

// calc_bombをフィールドサイズ分
// カウントするだけ。
void loop_calc_bomb(FIELD *map){
  int x,y;
  for(x = 0;x < map -> size.x;x++){
    for(y = 0;y < map -> size.y;y++){
      calc_bomb(map -> qty,map -> bomb,x,y);
    }
  }
}

int data_input(FIELD *map,int *x,int *y){
  static BOOLEAN mode = true;
  BOOLEAN flag;
  do{
    flag = true;
    if(mode == true) printf ("openmode ");
    else printf("flagmode ");
    printf("x,y > ");
    if(scanf("%d,%d",x,y) != NUM_SCANF_STATE) flag = false;
    while(getchar() != '\n') flag = false;
  }while(flag != true);

  if((0 > *x || *x >= map -> size.x ) || (0 > *y || *y >= map -> size.y)){
    if(mode == true) mode = false;
    else mode = true;
  }else{
    if(mode == true){
      // 爆弾の判定をして、無かったら一気に開ける関数に飛ぶ
      if(map -> state[(*y) + BIAS][(*x) + BIAS] != s_flag){
        if(map -> bomb[(*y) + BIAS][(*x) + BIAS] == true) return EXIT;
        else determinate_field(map,(*x)+1,(*y)+1);
      };
    }else{
      if(map -> state[(*y) + BIAS][(*x) + BIAS] == s_flag)
        map -> state[(*y) + BIAS][(*x) + BIAS] = s_true;
      else
        map -> state[(*y) + BIAS][(*x) + BIAS] = s_flag;
    }
  }
  return CONTINUE;
}

void determinate_field(FIELD *map,int x,int y){
  int i,j;
  if(map -> state[y][x] != s_true) return;
  map -> state[y][x] = s_false;
  if((0 <= x && x <= map -> size.x + BIAS ) && (0 <= y && y <= map -> size.y + BIAS)){
    if(map -> qty[y][x] == 0){
      for(j = -1;j < 3 - 1;j++){
        for(i = -1;i < 3 - 1;i++){
          determinate_field(map,x+i,y+j);
        }
      }
    }
  }
}

void print_precedents(void){
  printf("\
quantity : Quantity of surrounding M. \n\
    #    : Squares that are not open. \n\
    .    : Squares that are open. \n\
    !    : Flag to stand in a position considered to be a bomb. \n\
If you want to chamge mode , you shoud type \"-1,-1\". \n");
}

void print_scale_tens_place(int map_x){
  int x;
  printf("    ");
  for(x = 0;x < map_x;x++){
    if(x % 10 == 4) printf("-");
    switch(x%10){
    case 4: printf("%d",x/10%10); break;
    default: printf("-");
    }
    if(x % 10 == 9) printf(" ");
  }
  printf("\n");
}

void print_scale_digit_of_one(int map_x){
  int x;
  printf("    ");
  for(x = 0;x < map_x;x++){
    switch(x%10){
    case 0:case 4:case 5:case 9: printf("%d",x%10);break;
    default: printf("-");
    }
    if(x % 5 == 4) printf(" ");
  }
  printf("\n");
}
#endif
