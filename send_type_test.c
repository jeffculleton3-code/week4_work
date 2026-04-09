#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <time.h> 

void client_task(int my_rank, int uni_size);
int root_task(int uni_size);
void check_task(int uni_size, int my_rank);

int main(int argc, char **argv)
{
        // declare and initialise error handling variable
        int ierror = 0;

        // declare and initialise rank and size varibles
        int my_rank, uni_size;
        my_rank = uni_size = 0;

        // intitalise MPI
        ierror = MPI_Init(&argc, &argv);

        // gets the rank and world size
        ierror = MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
        ierror = MPI_Comm_size(MPI_COMM_WORLD,&uni_size);

		//create buffer for Bsend
		int s;
		MPI_Pack_size( 1, MPI_INT, MPI_COMM_WORLD, &s);
     	int buffer_size = s + MPI_BSEND_OVERHEAD;

		void * buffer = malloc(buffer_size);
		MPI_Buffer_attach(buffer, buffer_size);
	
		//check universe size
        if (uni_size > 1)
        {
                check_task(uni_size, my_rank);
			
        } // end if (uni_size > 1)
        else // i.e. uni_size <=1
        {
                // prints a warning
                printf("Unable to communicate with less than 2 processes. MPI communicator size = %d\n", uni_size);
        }

		//detach buffer
		MPI_Buffer_detach(&buffer, &buffer_size);
		free(buffer);
			
        // finalise MPI
        ierror = MPI_Finalize();
        return 0;
}

void client_task(int my_rank, int uni_size)
{
        // creates and initialies transmission variables
        int send_message, count, dest, tag;
        send_message = dest = tag = 0;
        count = 1;
		
		MPI_Request request;

        // sets the destination for the message
        dest = 0; // destination is root

        // creates the message
        send_message = my_rank * 10;

		// get start time
		clock_t t; 
    	t = clock();
	
        // sends the message
        MPI_Send(&send_message, count, MPI_INT, dest, tag, MPI_COMM_WORLD);
		
		
        // prints the message from the sender
        printf("Hello, I am %d of %d. Sent %d to Rank %d\n",
                         my_rank, uni_size, send_message, dest);
		//find end time
		t = clock() - t;
		double time_taken = ((double)t)/CLOCKS_PER_SEC;
		printf("%d took %fs to send\n", 
						my_rank, time_taken);
        
}

int root_task(int uni_size)
{
    // creates and initialies transmission variables
	int my_rank;
	my_rank = 0;
	int recv_message, count, source, tag;
	recv_message = source = tag = 0;
	count = 1;
	MPI_Status status;

	
        // iterates through all the other ranks
        for (int their_rank = 1; their_rank < uni_size; their_rank++)
        {
                // sets the source argument to the rank of the sender
                source = their_rank;

				// get start time
				clock_t t; 
    			t = clock();

                // receives the messages
                MPI_Recv(&recv_message, count, MPI_INT, source, tag, MPI_COMM_WORLD, &status);
				

                // prints the message from the sender
                printf("Hello, I am %d of %d. Received %d from Rank %d\n",
                                my_rank, uni_size, recv_message, source);

				//find end time
				t = clock() - t;
				double time_taken = ((double)t)/CLOCKS_PER_SEC;
				printf("%d took %fs to send\n", 
					my_rank, time_taken);
        }// end for (int their_rank = 1; their_rank < uni_size; their_rank++)
	return 0;
}

void check_task(int uni_size, int my_rank)
{
	// checks which process is running and calls the appropriate task
	if (0 == my_rank)
	{
		root_task(uni_size);
	} // end if (0 == my_rank)
	else // i.e. (0 != my_rank)
	{
		client_task(my_rank, uni_size);
		
	} // end else // i.e. (0 != my_rank)
}
