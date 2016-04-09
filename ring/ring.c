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
        if (world_size < 2)
                {
                printf("Too few processes. Number of processes must >= 2\n");
                goto exit_error;
                }

        int message_number = 0;
        int received_message_number = 0;

        if (world_rank == 0)
                {
                message_number = 42;
                MPI_Send(&message_number, 1, MPI_INT, world_rank + 1, 0, MPI_COMM_WORLD);  
                printf("Process %d have sent number %d to process %d\n", world_rank, message_number, world_rank + 1);

                MPI_Recv(&received_message_number, 1, MPI_INT, world_size - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                printf("Process %d received number %d from process %d\n", world_rank, received_message_number, world_size - 1);
                } 
        else if (world_rank != (world_size - 1))  
                {
                MPI_Recv(&received_message_number, 1, MPI_INT, world_rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                printf("Process %d received number %d from process %d\n", world_rank, received_message_number, world_rank - 1);

                MPI_Send(&received_message_number, 1, MPI_INT, world_rank + 1, 0, MPI_COMM_WORLD);
                printf("Process %d have sent number %d to process %d\n", world_rank, received_message_number, world_rank + 1);
                }
        else if (world_rank == (world_size - 1))
                {
                MPI_Recv(&received_message_number, 1, MPI_INT, world_rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                printf("Process %d received number %d from process %d\n", world_rank, received_message_number, world_rank - 1);

                MPI_Send(&received_message_number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
                printf("Process %d have sent number %d to process %d\n", world_rank, received_message_number, 0);
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
