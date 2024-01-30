#include <iostream>
#include <mpi.h>
#include <vector>
#include <cmath>

void mult(int *A, int filas_a, int columnas_a,
          int *B, int filas_b, int columnas_b, int *C)
{

    for (int i = 0; i < filas_a; i++)
    {
        for (int j = 0; j < columnas_b; j++)
        {
            for (int k = 0; k < columnas_a; k++)
            {
                C[i * columnas_b + j] += A[i * columnas_a + k] * B[k * columnas_b + j];
            }
        }
    }
   
}
int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);

    int rank, nprocs;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

    if (rank == 0)
    {
        std::vector<int> A = {1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4};
        std::vector<int> B = {1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4};
        std::vector<int> C(16);

        MPI_Bcast(B.data(), B.size(), MPI_INT, 0, MPI_COMM_WORLD);
        int paso = A.size() / nprocs;

        MPI_Scatter(A.data(), paso, MPI_INT, MPI_IN_PLACE,
                    paso, MPI_INT, 0, MPI_COMM_WORLD);

        // MPI_Scatter( const void* sendbuf , MPI_Count sendcount , MPI_Datatype sendtype , void* recvbuf , MPI_Count recvcount , MPI_Datatype recvtype , int root , MPI_Comm comm);
         mult(A.data(), 1, 4, B.data(), 4, 4, C.data());

        MPI_Gather(MPI_IN_PLACE, 0, MPI_INT, C.data(), 4, MPI_INT, 0, MPI_COMM_WORLD);
        for (int i = 0; i < C.size(); i++)
        {
            /* code */
            std::printf("Vals Res: %d\n", C[i]);
        }
    }
    else
    {
        std::vector<int> a_local(4);
        std::vector<int> b_local(16);
        std::vector<int> c_local(4);
        MPI_Scatter(nullptr, 0, MPI_INT,
                    a_local.data(), 4, MPI_INT,
                    0, MPI_COMM_WORLD);
        MPI_Bcast(b_local.data(), b_local.size(), MPI_INT, 0, MPI_COMM_WORLD);

        mult(a_local.data(), 1, 4, b_local.data(), 4, 4, c_local.data());
       
        MPI_Gather(c_local.data(), 4, MPI_INT, nullptr, 4, MPI_INT, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();
}