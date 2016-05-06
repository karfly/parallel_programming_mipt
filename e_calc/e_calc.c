#include <mpi.h>

#include <stdio.h>
#include <assert.h>

#define TRUE_E ((long double)2.7182818284590452353602874713527)

int factorial(int n)
        {
        if (n < 0)
                {
                printf("factorial: Wrong arguments. Number must be not negative\n");
                return -1;
                }

        if ((n == 0) || (n == 1))
                return 1;

        return n * factorial(n - 1);
        }

int main(int argc, char * argv[])
{
        if (argc != 2)
                {
                printf("Wrong number of arguments. Usage: $mpirun -np [NP] ./e_calc [N] - where N is number of members in series\n");
                return 1;
                }

        MPI_Init(&argc, &argv);         
        
        int ret_val = 1;

        int world_rank = 0;
        MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
        
        int world_size = 0;
        MPI_Comm_size(MPI_COMM_WORLD, &world_size);

        int N = atoi(argv[1]); // Supposing that given argument is valid integer number
        if (N <= 0)
                {
                printf("Wrong N. N must be positive.\n");
                goto exit_error;
                }

        long double result = 0;
        long double my_sum = 0;
        
        int i = 0;
        long double factorial_tmp = 0;
        for (i = world_rank; i <= N; i += world_size)
                {
                factorial_tmp = factorial(i);
                if (factorial_tmp != 0)
                        my_sum += 1 / factorial_tmp;
                }

        //printf("%d my_sum = %Lf\n", world_rank, my_sum);

        if (world_rank == 0)
                {
                result += my_sum;

                if (world_size != 1)
                        {
                        long double received_sum = 0;
                        i = 0;
                        for (i = 1; i < world_size; i++)
                                {
                                MPI_Recv(&received_sum, 1, MPI_LONG_DOUBLE, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                                //printf("received %Lf from %d\n", received_sum, i);
                                result += received_sum;
                                }
                        }

                printf("%.19Lf\n", result);
                //printf("e \t= %.19Lf - true\n", TRUE_E);
                //printf("error \t= %.19Lf\n", TRUE_E - result);
                }
        else
                {
                MPI_Send(&my_sum, 1, MPI_LONG_DOUBLE, 0, 0, MPI_COMM_WORLD);  
                }

        ret_val = 0; // Success
        
        exit_error:

        MPI_Finalize();
        
        return ret_val; 
}

