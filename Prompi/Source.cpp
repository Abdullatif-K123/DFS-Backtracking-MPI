#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include<cmath>
#include "mpi.h"

int rank,size, sum, n, m, step, ans, tmp;
bool mat[100][100], vis[100][100];

int dfs(int ix, int jx){
    if (ix >= n || jx >= (rank * step) + step + tmp || jx >= m || vis[ix][jx]) return 0;
    vis[ix][jx] = true;
    return (dfs(ix + 1, jx) + dfs(ix, jx + 1)) + mat[ix][jx];
}

int main(int argc , char *argv[])
{
	printf("Hello\n");
	fflush(stdout);
    ans = sum = n = m = 0;
    MPI_Init(&argc,&argv);
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );
    MPI_Comm_size( MPI_COMM_WORLD, &size );
    if (rank == 0){
        printf("Enter Dim of matrix : {n, m} :\n");
		fflush(stdout);
        scanf("%d%d", &n, &m);
        for (int i = 0; i < n; i++)
            for (int j = 0; j < m; j++){
				int x = rand() % 10;
				if (x <= 5) mat[i][j] = 0;
				else mat[i][j] = 1;
			}
    }
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&m, 1, MPI_INT, 0, MPI_COMM_WORLD);
	for(int i=0;i<n;i++)
		MPI_Bcast(mat[i], n, MPI_INT, 0, MPI_COMM_WORLD);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            vis[i][j]=0;
    if (size>=m ) step=1;
    else step=m/(size);

	if (rank == size - 1) tmp += (m % size);
	else tmp = 0;

    sum = dfs(0,rank*step);

    MPI_Barrier( MPI_COMM_WORLD);

    MPI_Reduce( &sum , &ans , 1 , MPI_INT , MPI_SUM , 0 , MPI_COMM_WORLD);	
    printf("rank = %d\tsum = %d\n", rank, sum);
	//fflush(stdout);
	
	if (rank == 0) {
        for (int i = 0; i < n; i++)
            for (int j = 0; j < m; j++)
                printf("%d%c", mat[i][j], " \n"[j == m - 1]);
		printf("Sum of all ones in the matrix : %d\n", ans);
	}
    MPI_Finalize();
    return 0;
 }
// mpiexec -n 5 Prompi