#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

struct dataset {
  double x;
  double y;
};

void inputData(struct dataset data[], int dataNumber, char filename[256])
{
  FILE *fp;
  int i;

  fp = fopen(filename, "r");
  if(fp==NULL) {
    printf("Error.\n");
    printf("Can't open file.\n");
    exit(1);
  }
  
  for(i=0; i<dataNumber; i++) {
    fscanf(fp, "%lf", &data[i].x);
    fscanf(fp, "%lf", &data[i].y);
  }
  
  fclose(fp);
}

void dispData(struct dataset data[], int dataNumber)
{
  int i;
  
  for(i=0; i<dataNumber; i++) {
    printf("%lf %lf\n", data[i].x, data[i].y);
  }

}

double sum_pow(struct dataset data[],double data_y[],int dataNumber,int pow_x)
{
  double sum;
  int i;
  for(i = 0,sum = 0;i < dataNumber;i++){
    sum += powl(data[i].x,pow_x) * data_y[i];
  }
  return sum;
}

// $B$*$=$i$/$3$l$,(Binverse_matrix$B$K$J$k(B
// void inverse_matrix(const int N,double **matrix,double **matrix_inverse)
// $B2?8N$+>e$N$d$D$@$H$&$^$/0J2<$J$$(B...
// $B1J1s$N(B0$B$J$i$L!"1J1s$NFf(B...
void inverse_matrix(const int N,double matrix[N+1][N+1],double matrix_inverse[N+1][N+1])
{
  int i,j,k;
  double cache;
  // $BC10L9TNs$r@8@.(B
  for(i = 0;i < N+1;i++){
    for(j = 0;j < N+1;j++){
      if(i == j) matrix_inverse[i][i] = 1;
      else matrix_inverse[i][j] = 0;
    }
  }
  // $B9T4pK\JQ7A$rMQ$$$?5U9TNs$N7W;;(B
  for(i = 0;i < N+1;i++){
    // $BC10L9TNs$KEv$?$kItJ,$r(B1$B$K$9$k=hM}(B
    cache = matrix[i][i];
    for(j = 0;j < N+1;j++){
      if(matrix[i][j] != 0) matrix[i][j] /= cache;
      if(matrix_inverse[i][j] != 0) matrix_inverse[i][j] /= cache;
    }
    // $B$"$k9T$KBP$9$k9T4pK\JQ7A(B
    for(j = 0;j < N+1;j++){
      if(j != i){ // $B:#$$$k9T$,!"JQ7A$r9T$&4pAC$H$J$k9T$GL5$$$3$H$N3NG'(B
        cache = matrix[j][i];
        for(k = 0;k < N+1;k++){
          matrix[j][k] -= matrix[i][k] * cache;
          matrix_inverse[j][k] -= matrix_inverse[i][k] * cache;
        }
      }
    }
  }
  for(i = 0;i < N+1;i++){
    for(j = 0;j < N+1;j++){
      matrix[i][j] = matrix_inverse[i][j];
    }
  }

}

void lsm(struct dataset data[], int dataNumber)
{
  const int N = 1;
  dispData(data, dataNumber);
  /*
  printf("\n");
  printf("$B$o$+$j$d$9$/=PNO(B\n");
  printf("(%lf, %lf)\n", data[0].x, data[0].y);
  printf("(%lf, %lf)\n", data[1].x, data[1].y);
  printf("(%lf, %lf)\n", data[2].x, data[2].y);
  */
  
  /* $B$3$3$+$i:G>.Fs>hK!$N%W%m%0%i%`$r=q$/(B */
  double matrix_X[N+1],matrix_B[N+1];
  double matrix_A[N+1][N+1],matrix_invers_A[N+1][N+1];
  double data_y[dataNumber];
  int i,j;

  // sum_pow$BMQ$K!"(Bdata_y$B$r(B1$B$G=i4|2=(B
  for(i = 0;i < dataNumber;i++) data_y[i] = 1;
  // matrix_A$B$K:G>.Fs>hK!$NNc$N9TNs$rBeF~(B
  // $BO"N)J}Dx<0$N78?t$KEv$?$kItJ,$r:n@.$7$F$$$k(B
  for(i = 0;i < N+1;i++) {
    for(j = 0;j < N+1;j++) {
      matrix_A[i][j] = sum_pow(data,data_y,dataNumber,2 * N - (i+j));
      // printf("A[%d][%d] = %f\n",i,j,matrix_A[i][j]);
    }
  }
  // sum_pow$BMQ$K!"(Bdata_y$B$r(Bdata.y$B$G=i4|2=(B($B=i4|2=$H$$$&0UL#$,$"$C$F$$$k$+$I$&$+$o$+$i$J$$(B)
  // $B$"$/$^$G$b%K%e%"%s%9(B($B%$%a!<%8(B)
  for(i = 0;i < dataNumber;i++) data_y[i] = data[i].y;
  // $BO"N)J}Dx<0$NEz$($KEv$?$kItJ,$N:n@.(B
  for(i = 0;i < N+1;i++){
    matrix_B[i] = sum_pow(data,data_y,dataNumber,N - i);
    // printf("B[%d] = %f\n",i,matrix_B[i]);
  }
  // $B5U9TNs$N7W;;(B
  // $B%G!<%?$N7?$,0c$&$H8@$o$l$?$,!"2?$,0c$&$N$+$h$/$o$+$C$F$J$$(B
  inverse_matrix(N,matrix_A , matrix_invers_A);
  for(i = 0;i < N+1;i++) {
    for(j = 0;j < N+1;j++) {
      // printf("in_A[%d][%d] = %f\n",i,j,matrix_invers_A[i][j]);
    }
  }

  // $BI=<(MQ$NItJ,(B
  // $B8e!94X?t2=$9$kM=Dj(B
  for(i = 0;i < N+1;i++){
    printf("(a%d * x^%d)",i,N - i);
    if(i != N) printf(" + ");
    else printf("\n");
  }
  // $BO"N)J}Dx<0$r7W;;$7!"7k2L$rBeF~(B
  for(i = 0;i < N+1;i++){
    matrix_X[i] = 0;
    for(j = 0;j < N+1;j++){
      matrix_X[i] += matrix_B[j] * matrix_invers_A[i][j];
    }
    // $B<B:]$N78?t$rI=<((B
    printf("a%d = %f\n",i,matrix_X[i]);
  }

}


int main(int argc, char *argv[])
{
  struct dataset *data;
  int dataNumber;
  char filename[256];

  if(argc-1 != 2) {
    printf("Error.\n");
    printf("Usage: ./a.out [Data Number] [Filename]\n");
    exit(1);
  }
  
  dataNumber = atoi(argv[1]);
  strcpy(filename, argv[2]);

  data = (struct dataset *)malloc(sizeof(struct dataset)*dataNumber);

  inputData(data, dataNumber, filename);

  lsm(data, dataNumber);

  return 0;
}
