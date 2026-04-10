#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

// declares the functions that will be called within main
// note how declaration lines are similar to the initial line
// of a function definition, but with a semicolon at the end;
int check_args(int argc, char **argv);
void initialise_vector(int vector[], int size, int initial);
void print_vector(int vector[], int size);
int sum_vector(int vector[], int size);
int vector_sum_p(int *array, int size, int rank, int num_proc);

int main(int argc, char **argv)
{
        MPI_Init(&argc, &argv);

        int rank, num_proc;
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        MPI_Comm_size(MPI_COMM_WORLD, &num_proc);
  
        // declare and initialise the numerical argument variable
        int num_arg = check_args(argc, argv);

        // creates a vector variable
        // int my_vector[num_arg]; // suffers issues for large vectors
        int* my_vector = malloc (num_arg * sizeof(int));
        // and initialises every element to zero
        initialise_vector(my_vector, num_arg, 0);


        //makes a vector with all natural numbers up to num_arg - 1
        for (int i = 0; i < num_arg; i++) 
        {
                my_vector[i] = i;
        }

        // sums the vector
        int my_sum = vector_sum_p(my_vector, num_arg, rank, num_proc);

        // prints the sum
        if (rank ==0)
        {
                printf("Sum: %d\n", my_sum);
        }

        // if we use malloc, must free when done!
        free(my_vector);

        MPI_Finalize();
        return 0;
}

// defines a function to sum a vector of ints into another int
int sum_vector(int vector[], int size)
{
        // creates a variable to hold the sum
        int sum = 0;

        // iterates through the vector
        for (int i = 0; i < size; i++)
        {
                // sets the elements of the vector to the initial value
                sum += vector[i];
        }

        // returns the sum
        return sum;
}

// defines a function to initialise all values in a vector to a given inital value
void initialise_vector(int vector[], int size, int initial)
{
        // iterates through the vector
        for (int i = 0; i < size; i++)
        {
                // sets the elements of the vector to the initial value
                vector[i] = initial;
        }
}

// defines a function to print a vector of ints
void print_vector(int vector[], int size)
{
        // iterates through the vector
        for (int i = 0; i < size; i++)
        {
                // prints the elements of the vector to the screen
                printf("%d\n", vector[i]);
        }
}

// defines a function that checks your arguments to make sure they'll do what you need
int check_args(int argc, char **argv)
{
        // declare and initialise the numerical argument
        int num_arg = 0;

        // check the number of arguments
        if (argc == 2) // program name and numerical argument
        {
                // declare and initialise the numerical argument
                num_arg = atoi(argv[1]);
        }
        else // the number of arguments is incorrect
        {
                // raise an error
                fprintf(stderr, "ERROR: You did not provide a numerical argument!\n");
                fprintf(stderr, "Correct use: %s [NUMBER]\n", argv[0]);

                // and exit COMPLETELY
                exit (-1);
        }
        return num_arg;
}

int vector_sum_p(int *array, int size, int rank, int num_proc)
{  
        int sum = 0;
        int root = 0;
  
        // First, determine the size of each job
        int chunk = size/num_proc;
  
        // Next, work out what work to do
        int start = rank * chunk;
  
        int stop = (rank + 1)* chunk;
  
        // now, do the sum just as before
        for (int i = start; i < stop; i++) 
        {
          
                 sum += array[i];
          
        }
        // sum is the partial sum from start to end
        // need to send this to the root

        int final_sum = 0;
        int temp;
        
        if (rank != root) 
        {
          
                 MPI_Send(&sum, 1, MPI_INT, root, 0, MPI_COMM_WORLD);
          
        }
        else //(rank == root)
        {
          
            final_sum = sum;

            for (int i = 1; i < num_proc; i++) 
            {
                MPI_Recv(&temp, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                final_sum += temp;
            }
         }

        return final_sum;
}
