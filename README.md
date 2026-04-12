# week4_work

MPI_Send makes the order of received messages consistent (receives lowest value to highest in this case) but send messages are not as consistent especially at higher values. At -np 7 the sum of the sys and user time exceed the real time.

MPI_Ssend makes the order of sent messages more likely to be ordered from lowest value to highest value.

MPI_Bsend makes the order of sent messages even more unpredictable than MPI_send but the received messages seem to be more likely to be printed after the sent messages.

MPI_Rsend and MPI_Isend seem to work similarly to MPI_Send but can crash, the code used for this report never crashed due to a low processor limit of 16 making crashes very unlikely. 

Benchmark:
Sending takes a variable amount of time ranging from about 0.00002 seconds and 0.00008 seconds and receiving varies from about 0.00008 seconds and 0.0003 seconds.  

Pingpong:
The code sends a message back and forth between 2 processes (root and client) multiple times and then prints the number of pings, the elapsed time for each process and the average time for each process. For a low number of pings, the elapsed time for both processes is highly variable and the elapsed time for the root process is much higher than for the client process, as the number of pings reaches the thousands then the elapsed times start to converge. However, the elapsed time is still highly variable for each process.

Collective Communications:
The prediction is that transmitting parts of the array would be faster than broadcasting it all at once, but it won’t be noticeable for small arrays. 
MPI_Bcast:
Elapsed time is very short, an array of size 60000 takes about 0.0008 seconds to sum and broadcast. At an array size of about 66000 the sum starts to overflow.

MPI_Send and MPI_Recv:
Transmitting parts of the array with MPI_Send and MPI_Recv can be faster than MPI_Bcast but it seems to be much more variable, an array of size 60000 takes anywhere from 0.00063 seconds to 0.0012 seconds. 
MPI_Scatter: 
Behaves identically to the code above.
