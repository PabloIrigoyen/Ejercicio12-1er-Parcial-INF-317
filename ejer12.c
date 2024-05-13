#include "stdio.h"
#include "stdlib.h"
#include "mpi.h"

int fibonacci(int n) {
    if (n <= 1) {
        return n;
    } else {
        int a = 0, b = 1, t;
        for (int i = 2; i <= n; ++i) {
            t = a + b;
            a = b;
            b = t;
        }
        return b;
    }
}

int main(int argc, char *argv[]) {
    int ran, tam, n;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &ran);
    MPI_Comm_size(MPI_COMM_WORLD, &tam);

    if (size < 2) {
        printf("Se requieren al menos 2 procesos para este programa.\n");
        MPI_Finalize();
        return 1;
    }

    if (rank == 0) {
        printf("Ingrese el valor de n para calcular el término n-ésimo de Fibonacci: ");
        scanf("%d", &n);
        for (int i = 1; i < tam; ++i) {
            MPI_Send(&n, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
    } else {
        MPI_Recv(&n, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        int resultado = fibonacci(n);
        MPI_Send(&resultado, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
    if (ran == 0) {
        int resultado;
        for (int i = 1; i < tam; ++i) {
            MPI_Recv(&resultado, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
            printf("El término %d de Fibonacci es: %d\n", n, resultado);
        }
    }

    MPI_Finalize();
    return 0;
}
