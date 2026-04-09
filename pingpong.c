#include <stdio.h>
#include <mpi.h>
#include <time.h> 
#include <stdlib.h>

int check_args(int argc, char **argv);
void root_task(int num_pings);
void client_task(int num_pings);

int main(int argc, char **argv)
{
    int ierror = 0;

	int num_pings = check_args(argc, argv);

    // declare and initialise rank and size varibles
    int my_rank, uni_size;

    // intitalise MPI
    ierror = MPI_Init(&argc, &argv);

    // gets the rank and world size
    ierror = MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
    ierror = MPI_Comm_size(MPI_COMM_WORLD,&uni_size);
	
	// initialise start and end time
	double start_time, end_time;
    start_time = MPI_Wtime();
	
	if (0 == my_rank)
	{
		root_task(num_pings);
	} // end if (0 == my_rank)
	else // i.e. (0 != my_rank)
	{
		client_task(num_pings);
		
	} // end else // i.e. (0 != my_rank)
	
	//find end time
	end_time = MPI_Wtime();
	double time_taken = end_time - start_time;
	printf("elasped time of process %d: %fs\n", 
					my_rank, time_taken);
	double average_time = time_taken/num_pings;
	printf("average time of process %d: %fs\n",
					my_rank, average_time);
	
	ierror = MPI_Finalize();
    return 0;
}

void root_task(int num_pings)
{
    // creates and initialies transmission variables
	int counter, count, source, dest, tag;
	counter = tag = 0;
	count = dest = source = 1;
	MPI_Status status;

        // sends and recieves from client until desired number of processes is reached
        while (counter < num_pings)
        {
				//sends message to client
				MPI_Send(&counter, count, MPI_INT, dest, tag, MPI_COMM_WORLD);
                // receives the messages from client
                MPI_Recv(&counter, count, MPI_INT, source, tag, MPI_COMM_WORLD, &status);
			
        }// end for (counter = num_pings}
	
		printf("number of pingpongs: %d\n", counter);
}


void client_task(int num_pings)
{
        // creates and initialies transmission variables
        int counter, count, source, dest, tag;
        dest = tag = source = 0;
        count = 1;
		MPI_Status status;

		while(1)
		{
			// recieves message from root
			MPI_Recv(&counter, count, MPI_INT, source, tag, MPI_COMM_WORLD, &status);

			//increment the counter by 1
			counter++;
			
        	// sends the message to root
        	MPI_Send(&counter, count, MPI_INT, dest, tag, MPI_COMM_WORLD);
				
        	if (counter >= num_pings)
				break;
		}
        
}

int check_args(int argc, char **argv)
{
	// declare and initialise the numerical argument
	int num_pings = 0;

	// check the number of arguments
	if (argc == 2) // program name and numerical argument
	{
		// declare and initialise the numerical argument
		num_pings = atoi(argv[1]);
	}
	else // the number of arguments is incorrect
	{
		// raise an error
		fprintf(stderr, "ERROR: You did not provide a numerical argument!\n");
		fprintf(stderr, "Correct use: mpicc -n 2 ./program [NUMBER]\n");

		// and exit COMPLETELY
		exit (-1);
	}
	return num_pings;
}
