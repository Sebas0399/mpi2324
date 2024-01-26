#include <iostream>
#include <mpi.h>
#include <vector>
#include <cmath>
#define MAX_ITEMS 25

int sumar(int *A, int size)
{
    int suma = 0;
    for (int i = 0; i < size; i++)
    {
        suma = suma + A[i];
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
        MPI_Scatter(data.data(), block_size, MPI_INT, MPI_IN_PLACE, block_size, MPI_INT, 0, MPI_COMM_WORLD);
    }
    else
    {
        MPI_Scatter(MPI_IN_PLACE, block_size, MPI_INT, data_local.data(), block_size, MPI_INT, 0, MPI_COMM_WORLD);
    }

    // Enviar los datos
    if (rank == nprocs - 1)
    {
        block_size = block_size - padding;
    }
    int suma_parcial = sumar(data_local.data(), block_size);

    // Calcular la suma parcial
    std::printf("RANK %d: suma parcial = %d \n", rank, suma_parcial);
    // enviar la suma al rank 0
    int suma_total = 0;

    MPI_Reduce(&suma_parcial, &suma_total, 1, MPI_INT,
               MPI_SUM, 0, MPI_COMM_WORLD);
    // imprimir el resultado
    if (rank == 0)
    {
        int suma_parcial = sumar(data.data(), block_size);
        suma_total=suma_total+suma_parcial;
        std::printf("Suma total: %d\n", suma_total);
    }
    MPI_Finalize();
}