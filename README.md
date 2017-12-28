# JobAndProcessScheduler
CPU Job and Process Scheduler Simulation Created For Operating Systems Class

Input:

A text file named "input.txt".  The input file describes a set of arriving jobs and their actions.  Each line in the file will contain one of the commands listed below. Each command consists of a letter in column one followed by a set of parameters. Each text file contains multiple type "C" (system configuration) commands. There will always be exactly one blank after each number in the input file.

Input File Command Examples:

System Configuration: C 9 M=45 S=12 Q=1
  The example above states that the system to be simulated starts at time 9, and that the system has a main memory consisting of 45; 12   serial devices; and a time quantum or time slice of 1.
Job Arrival: A 10 J=1 M=5 S=4 R=3 P=1
  The example above states that job number 1 with priority 1 arrives at time 10, requires 5 units of main memory, holds no more than 4     devices at any point during execution, and runs for 3 units of time.
Request for Devices: Q 10 J=3 D=4
  The example above states that at time 10, job number 3 requests for 4 devices. A job only requests devices when it is running on the     CPU. The Quantum is interrupted to process request. If request is granted process goes to the end of the ready queue, else it goes     to the device wait state.
Release of Devices: L 10 J=5 D=1
  The example above states that at time 10, job number 5 releases one device. A job only releases devices when it is running on the CPU.   Quantum is interrupted. One or more jobs may be taken off the Device Wait queue due to this.
Display Current System Status: D 11
  The example above states that at time 11 an external event is generated and the output should be printed.  If "D 9999" is executed       then then simulation terminated the output is printed.

Design Approach:

Our scheduling simulator contains 2 hold queues, a ready queue, a CPU or run function, and a wait
queue. Our program begins by reading in input from a file called input.txt. The program reads in the
input file line by line and stores each line into a 2-dimensional array. When it finishes reading in all the
input, each line or command in the 2-dimensional array is executed at the correct time specified by the
instruction. We implemented the clock by creating a while loop around the main program and then
incrementing a time variable after running the CPU function once and reading one instruction from the
input 2D array. Next, we implemented the command executions. The C command initializes the main
memory and total device size and the quantum number for round robin. When the C command is
executed it stores these the values as global variables at the top of the program. The A command
initializes a job and places it on to a Hold Queue if there is not enough available memory, or places it
into the ready queue if there is enough available memory. A job is a class containing all the necessary
information needed to run that job/process. The hold queues are linked lists of jobs that are based on
priority. A job with a priority of 1 goes into hold queue 1 and a job with a priority of 2 goes into hold
queue 2. The ready queue is another linked list of jobs that is not based on priority. The first job in the
ready is moved into the CPU and the CPU/ run function begins processing that job for a time equal to
the quantum. Also, when a job is moved from the input or the hold queue to the ready queue it is
placed on the end of the ready queue. The Q command prompts the program to give a certain job
some devices when it is running in the CPU. When the Q command is executed and the job is running in
the CPU function, then the program executes the banker's algorithm to check if giving that program
devices will allow the system to maintain a safe state. If the job is not running in the CPU when the Q
command is executed, then the request for devices is saved until the time when the job is running on
the CPU. We wrote the q command in this way because the sample input that was provided did not
always execute the Q command when the correct job was in the CPU. The L command prompts the
program release devices when a certain job is in the CPU. We wrote this command in a similar way to
the Q command because sometimes the correct job is not running in the CPU when the L command is
executed. We also check the release of the devices with the banker’s algorithm. Finally, the D
command prints the current state of the entire system.

Output:

A list of each job that has entered the system; for each job, print the state of the job, the remaining service time for unfinished jobs and the turnaround time and weighted turnaround time for finished jobs.  The contents of each queue, the system turnaround time, and system weighted turnaround are also printed.
