#include <iostream>
#include <mpi.h>
#include <vector>
#include <cmath>
#define MAX_ITEMS 25

void matrix_mult(double *A, double *b, double *c, int rows, int cols)
{
    for (int i = 0; i < rows; i++)
    {
        double tmp = 0;
        for (int j = 0; j < cols; j++)
        {
            tmp = tmp + A[i * cols + j] * b[j];
        }
        c[i] = tmp;
    }
}
int sumar(int *A, int size)
{   
    int suma=0;
    for (int i = 0; i < size; i++)
    {
        suma=suma+A[i];
    }
    return suma;
}
int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);

    int rank, nprocs;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
    int block_size;
    int real_size;
    int padding = 0;
    if (MAX_ITEMS % nprocs != 0)
    {
        real_size = std::ceil((double)MAX_ITEMS / nprocs) * nprocs;
        block_size = real_size / nprocs;
        padding = real_size - MAX_ITEMS;
    }
    std::vector<int> data;
    std::vector<int> data_local(block_size);
    // Inicializar
    if (rank == 0)
    {
        data.resize(real_size);
            std::printf("Dimension: %d, real_size: %d, block_size: %d,padding: %d\n",
                        MAX_ITEMS, real_size, block_size, padding);
        for (int i = 0; i < MAX_ITEMS; i++)
        {
            /* code */
            data[i] = i;
        }
    }
    // Enviar los datos
    MPI_Scatter(data.data(), block_size, MPI_INT, data_local.data(), block_size, MPI_INT, 0, MPI_COMM_WORLD);
    if (rank == nprocs - 1)
    {
        block_size = block_size - padding;
    }
    int suma_parcial = sumar(data_local.data(), block_size);

    // Calcular la suma parcial
    std::printf("RAND %d: suma parcial = %d \n", rank, suma_parcial);
    // enviar la suma al rank 0
    int suma_total = 0;

    MPI_Reduce(&suma_parcial, &suma_total, 1, MPI_INT,
               MPI_SUM, 0, MPI_COMM_WORLD);
    // imprimir el resultado
    if (rank == 0)
    {
        std::printf("Suma total: %d\n", suma_total);
    }
    MPI_Finalize();
}