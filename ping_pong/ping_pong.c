#include <mpi.h>

#include <stdio.h>
#include <assert.h>

int main()
{
        MPI_Init(NULL, NULL);         
        
        int ret_val = 1;

        int world_rank = 0;
        MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
        
        int world_size = 0;
        MPI_Comm_size(MPI_COMM_WORLD, &world_size);
        if (world_size != 2)
                {
                printf("Wrong number of proccesses. Number of processes must be 2\n");
                goto exit_error;
                }

        int ball = 0;

        if (world_rank == 0)
                {
                ball = 42;
                while (1)
                        {
                        MPI_Send(&ball, 1, MPI_INT, world_rank + 1, 0, MPI_COMM_WORLD);  

                        MPI_Recv(&ball, 1, MPI_INT, world_rank + 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                        printf("Process %d received ball (%d) from process %d\n", world_rank, ball, world_rank + 1);

                        sleep(1);
                        }
                } 
        else if (world_rank == 1)  
                {
                while (1)
                        {
                        MPI_Recv(&ball, 1, MPI_INT, world_rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                        printf("Process %d received ball (%d) from process %d\n", world_rank, ball, world_rank - 1);

                        sleep(1);

                        MPI_Send(&ball, 1, MPI_INT, world_rank - 1, 0, MPI_COMM_WORLD);  
                        }
                }
        else
                {
                // Can't be here. Maybe.
                assert(0);
                }

        ret_val = 0; // Success
        
        exit_error:

        MPI_Finalize();
        
        return ret_val; 
}
