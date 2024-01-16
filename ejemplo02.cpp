#include <iostream>
#include <mpi.h>

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);

    int rank, nprocs;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

    int data[100];
    int sumaTot = 0;

    if (rank == 0)
    {
        for (int i = 0; i < 100; i++)
        {
            data[i] = i;
        }
        for (int i = 0; i < 25; i++)
        {
            sumaTot = sumaTot + data[i];
        }
        std::printf("SUMA %d \n ", sumaTot);

        for (int i = 1; i < nprocs; i++)
        {
            /* code */
            int start = i * 25;
            int data_rec = 0;

            std::printf("RANK_0 ENVIANDO DATOS A RANK_%d \n", i);
            MPI_Send(&data[start], 25, MPI_INT, i, 0, MPI_COMM_WORLD);

            MPI_Recv(&data_rec, 25, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            sumaTot = sumaTot + data_rec;
        }
        std::printf("Suma Total %d \n ", sumaTot);

        // MPI_Send(datos,cantidad,tip de dato,rank a donde se envia,TAG,grupo);
    }
    else
    {
        // MPI_Recv(datos,cantidad,tip de dato,rank de donde se recive,TAG,grupo);
        std::printf("RANK_%d RECIVIENDO DATOS  \n", rank);

        MPI_Recv(data, 25, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        int suma = 0;
    
        for (int i = 0; i < lim; i++)
        {
            std::printf("Datos suma %d \n ", data[i]);

            suma = suma + data[i];
        }
        std::printf("SUMA %d \n ", suma);

        MPI_Send(&suma, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
    MPI_Finalize();
    return 0;
}