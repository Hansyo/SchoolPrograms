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

// おそらくこれがinverse_matrixになる
// void inverse_matrix(const int N,double **matrix,double **matrix_inverse)
// 何故か上のやつだとうまく以下ない...
// 永遠の0ならぬ、永遠の謎...
void inverse_matrix(const int N,double matrix[N+1][N+1],double matrix_inverse[N+1][N+1])
{
  int i,j,k;
  double cache;
  // 単位行列を生成
  for(i = 0;i < N+1;i++){
    for(j = 0;j < N+1;j++){
      if(i == j) matrix_inverse[i][i] = 1;
      else matrix_inverse[i][j] = 0;
    }
  }
  // 行基本変形を用いた逆行列の計算
  for(i = 0;i < N+1;i++){
    // 単位行列に当たる部分を1にする処理
    cache = matrix[i][i];
    for(j = 0;j < N+1;j++){
      if(matrix[i][j] != 0) matrix[i][j] /= cache;
      if(matrix_inverse[i][j] != 0) matrix_inverse[i][j] /= cache;
    }
    // ある行に対する行基本変形
    for(j = 0;j < N+1;j++){
      if(j != i){ // 今いる行が、変形を行う基礎となる行で無いことの確認
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

void lsm(struct dataset data[], int dataNumber, const int N)
{
  dispData(data, dataNumber);
  /*
  printf("\n");
  printf("わかりやすく出力\n");
  printf("(%lf, %lf)\n", data[0].x, data[0].y);
  printf("(%lf, %lf)\n", data[1].x, data[1].y);
  printf("(%lf, %lf)\n", data[2].x, data[2].y);
  */
  
  /* ここから最小二乗法のプログラムを書く */
  double matrix_X[N+1],matrix_B[N+1];
  double matrix_A[N+1][N+1],matrix_invers_A[N+1][N+1];
  double data_y[dataNumber];
  int i,j;

  // sum_pow用に、data_yを1で初期化
  for(i = 0;i < dataNumber;i++) data_y[i] = 1;
  // matrix_Aに最小二乗法の例の行列を代入
  // 連立方程式の係数に当たる部分を作成している
  for(i = 0;i < N+1;i++) {
    for(j = 0;j < N+1;j++) {
      matrix_A[i][j] = sum_pow(data,data_y,dataNumber,2 * N - (i+j));
      // printf("A[%d][%d] = %f\n",i,j,matrix_A[i][j]);
    }
  }
  // sum_pow用に、data_yをdata.yで初期化(初期化という意味があっているかどうかわからない)
  // あくまでもニュアンス(イメージ)
  for(i = 0;i < dataNumber;i++) data_y[i] = data[i].y;
  // 連立方程式の答えに当たる部分の作成
  for(i = 0;i < N+1;i++){
    matrix_B[i] = sum_pow(data,data_y,dataNumber,N - i);
    // printf("B[%d] = %f\n",i,matrix_B[i]);
  }
  // 逆行列の計算
  // データの型が違うと言われたが、何が違うのかよくわかってない
  inverse_matrix(N,matrix_A , matrix_invers_A);
  for(i = 0;i < N+1;i++) {
    for(j = 0;j < N+1;j++) {
      // printf("in_A[%d][%d] = %f\n",i,j,matrix_invers_A[i][j]);
    }
  }

  // 表示用の部分
  // 後々関数化する予定
  for(i = 0;i < N+1;i++){
    printf("(a%d * x^%d)",i,N - i);
    if(i != N) printf(" + ");
    else printf("\n");
  }
  // 連立方程式を計算し、結果を代入
  for(i = 0;i < N+1;i++){
    matrix_X[i] = 0;
    for(j = 0;j < N+1;j++){
      matrix_X[i] += matrix_B[j] * matrix_invers_A[i][j];
    }
    // 実際の係数を表示
    printf("a%d = %f\n",i,matrix_X[i]);
  }

}


int main(int argc, char *argv[])
{
  struct dataset *data;
  int dataNumber;
  int N;
  char filename[256];

  if(argc-1 != 3) {
    printf("Error.\n");
    printf("Usage: ./a.out [Data Number] [N] [Filename]\n");
    exit(1);
  }
  
  dataNumber = atoi(argv[1]);
  N = atoi(argv[2]);
  strcpy(filename, argv[3]);

  data = (struct dataset *)malloc(sizeof(struct dataset)*dataNumber);

  inputData(data, dataNumber, filename);

  lsm(data, dataNumber, N);

  return 0;
}
