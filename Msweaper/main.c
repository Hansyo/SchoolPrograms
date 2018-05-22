#include <time.h>
#include "settings.h"
// settings.hにおいて呼び出し済み
// #include <stdio.h>
// #include <stdlib.h>

void game(void);

int main(void){
  srand((unsigned)time(NULL));
  do game();
  while(game_continue() != EXIT);
  return 0;
}

void game(void){
  FIELD *map = (FIELD *)malloc(sizeof(FIELD));
  // x:横 y:縦
  int x,y;
  make_field(map);
  set_bomb(map -> bomb,map -> size);
  loop_calc_bomb(map);
  puts("if you want change(toggle) openmode or flagmode, you should type \"-1,-1\"");
  do{
    if(clear_check(*map) == EXIT) break;
    print_field(*map);
    print_precedents();
  }while(data_input(map,&x,&y) == CONTINUE);
  print_all_field(*map);
  if(map -> bomb[y + BIAS][x + BIAS] == true){
    printf("\n\n\n GAME OVER. \n\n\n");
  }
  free_map_data(map);
  free(map);
}
