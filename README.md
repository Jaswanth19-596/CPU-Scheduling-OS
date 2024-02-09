# Process Scheduler Simulation

This is a process scheduler simulation program written in C++. It simulates the execution of processes by a computer system with a large memory, one terminal per user, a CPU, and one disk drive. Each process is described by its class (REAL-TIME or INTERACTIVE), start time, and a sequence of resource requests including CPU requests (CPU), disk reads (DISK), and terminal accesses (TTY). Real-time processes have a deadline by which they must complete.

## Features

- Simulates the execution of real-time and interactive processes.
- Tracks CPU and disk utilization.
- Computes statistics such as the number of completed real-time processes, percentage of real-time processes that missed their deadline, number of completed interactive processes, total number of disk accesses, average duration of disk accesses, and total time elapsed since the start of the first process.

## Usage

1. **Clone the Repository:**
    ```
    git clone https://github.com/Jaswanth19-596/CPU-Scheduling-OS.git
    ```

2. **Compile the Code:**
    ```
    g++ main.cpp -o scheduler
    ```

3. **Run the Program:**
    ```
    ./scheduler < input.txt
    ```

    Replace `input.txt` with the input file containing process descriptions.

4. **View Output:**
    The program will output the simulation results to the console and also save them to an `output.txt` file.

## Input Format

The input file should contain descriptions of processes in the following format:

INTERACTIVE 12000\
CPU 100\
TTY 5000\
CPU 100\
DISK 10\
CPU 20\
REAL-TIME 12000\
DEADLINE 13000\
CPU 30




- Each line represents a process, starting with its class (INTERACTIVE or REAL-TIME) followed by its start time and sequence of resource requests.

## Output Format

The program outputs the simulation results including the initial and final states of processes, as well as statistics such as the number of completed processes, percentage of real-time processes that missed their deadline, and CPU and disk utilization.




Initial States of the Processes : 

Key: 1
Value:
    Process ID: 1
    Process Class: REAL-TIME
    Arrival Time: 12000
    Process Status: NEW
Key: 0
Value:
    Process ID: 0
    Process Class: INTERACTIVE
    Arrival Time: 12000
    Process Status: NEW


CPU is idle, executing the Real time process
Assigned the CPU to the Real time process
The Process 1 of class REAL-TIME is going to access the CPU
Proceess 1 of class REAL-TIME has been successfully executed!!



Assigned INTERACTIVE process to the CPU
The Process 0 of class INTERACTIVE is going to access the CPU
The Process 0 of class INTERACTIVE is going to access the TTY
The Process 0 of class INTERACTIVE is going to access the CPU
The Process 0 of class INTERACTIVE is going to access the DISK
The process of class INTERACTIVE came out of Disk queue


Assigned INTERACTIVE process to the CPU
The Process 0 of class INTERACTIVE is going to access the CPU
Proceess 0 of class INTERACTIVE has been successfully executed!!


Final State of the Processes :

Key: 1
Value:
    Process ID: 1
    Process Class: REAL-TIME
    Arrival Time: 12000
    Process Status: EXECUTED
Key: 0
Value:
    Process ID: 0
    Process Class: INTERACTIVE
    Arrival Time: 12000
    Process Status: EXECUTED


Final statistics : 

Number of real-time processes that have been Completed                 : 1
percentage of real-time processes that missed their deadline           : 0%
Total Number of Interactive processes that have been Completed         : 1
Total Number of Disk Accesses                                          : 1
Average Duration of Disk Accesses                                      : 10
Total Time Elapsed since the start of the First Process                : 5261
CPU Utilization                                                        : 4.75195%
Disk Utilization                                                       : 0.190078%




## Contributing

Contributions are welcome! If you find any bugs or have suggestions for improvement, please feel free to open an issue or submit a pull request.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
