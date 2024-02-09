/*
    Name : Jaswanth Mada
    Course Number : 51520-001 002
    Assignment Description : This is a CPU Scheduling Simulation program 
    written in C++. This program simulates the execution of processes by a  
    computer system with a large memory, one terminal per user, a CPU 
    and one disk drive. Each Process described by its class (REAL-TIME)
    or (INTERACTIVE), start time and a sequence of resource requests 
    including CPU requests(CPU), Disk Reads(DISK), and terminal Accesses
    (TTY). Real time processes have a deadline and they must be given higher
    priority over INTERACTIVE processes. When a REAL TIME process arrives, 
    the INTERACTIVE process should come out of CPU and the REAL TIME process
    must be given the CPU.
    Deadline : 02-08-2024
*/
// Including ALl the required Header files
#include<iostream>
#include<string.h>
#include<queue>
#include<utility>
#include <string>
#include <sstream>
#include <unordered_map>
#include <fstream> 

using namespace std;



// Creating a structure for the Process
struct Process{
    public : 
    int pid;
    string pclass;
    queue<pair<string,int>> tasksQueue;
    int diskEnterTime;
    int ioTime;
    int arrivalTime;
    int deadline;
    bool isNull;


    // Constructor to create the Process
    Process(int pid, string pclass, queue<pair<string,int>> tasksQueue, int arrivalTime, int deadline, int diskEnterTime, int ioTime){
        this->pid = pid;
        this->pclass = pclass;
        this->tasksQueue = tasksQueue;
        this->diskEnterTime = diskEnterTime;
        this->ioTime = ioTime;
        this->arrivalTime = arrivalTime;
        this->deadline = deadline;
    }
    
    // Copy Constructor
    Process(const Process& other) : pid(other.pid), pclass(other.pclass), tasksQueue(other.tasksQueue), arrivalTime(other.arrivalTime), deadline(other.deadline), diskEnterTime(other.diskEnterTime), ioTime(other.ioTime) {}


    // Used to print the Process
    void print(){
        cout<<this->pid<<"\n";
        cout<<this->pclass<<"\n";
        cout<<this->arrivalTime<<"\n";
        cout<<this->diskEnterTime<<"\n";
        cout<<this->ioTime<<"\n";

        printTasks();
    }
    
    // Used to print the Tasks
    void printTasks(){
        while(tasksQueue.size() > 0){
            cout << "(" << tasksQueue.front().first << ", " << tasksQueue.front().second << ")" << endl;
            tasksQueue.pop();
        }
    }

};

// Used to CompareProcesses and Sort Processes
struct CompareProcesses{

    bool operator()(const Process& p1, const Process & p2){
        return p1.arrivalTime > p2.arrivalTime;
    }

};


// Used to keep track of State of each Process
struct Process_State{
    
    public:
    int processId;
    string processClass;
    int arrivalTime;
    string processStatus;

    // Default Constructor 
    Process_State() {}
    
    // Parameterized Constructor
    Process_State(int processId, string processClass, int arrivalTime, string processStatus){
        this->processId = processId;
        this->processClass = processClass;
        this->arrivalTime = arrivalTime;
        this->processStatus = processStatus;
    }
};




// Defining the Global Variables



// Counter : keeps track of current point of time
int counter = 0;

// CPU : Represents the Current executing Process
Process *CPU = NULL;

// intQueue : Contains the Interactive Processes
queue<Process> intqueue;

// rtQueue : Contains the RealTIme Processes 
queue<Process> rtqueue;

// diskqueue : Contains the processes that are waiting to enter into Disk
queue<Process> diskqueue;

// Map : Keeps track of the Process states
unordered_map<int, Process_State> map;

// noOfRealTimeProcesses : Keeps track of Number of Real Time Processes
int noOfReamTimeProcesses = 0;

// noOfRealTimeProcessesCompleted : Keeps track of Number of Real Time Processes that are executed
int noOfRealTimeProcessesCompleted = 0;

// noOfRealTimeProcessesCompleted : Keeps track of Number of Real Time Processes that missed the deadline
int noOfRealTimeProcessesMissedDeadline = 0;

// noOfInteractiveProcesses : Keeps track of Number of Interactive Time Processes that are executed
int noOfInteractiveProcesses = 0;

// totalNoOfDiskAccesses : Keeps track of Number of Disk accesses
int totalNoOfDiskAccesses = 0;

// totalNoOfDiskAccesses : Keeps track of Total Disk access time
int totalDiskAccessTime = 0;

// intitialProcessArrivalTime : Stores the First process arrival time 
int intitialProcessArrivalTime = 0;

// CPUBusyTime : Keeps track of the amount of time that CPU is executing
int CPUBusyTime = 0;

// diskBusyTime : Keeps track of the amount of time the Disk is getting used
int diskBusyTime = 0;



// This function is used to execute a process
void execute(){
    
    // Tasks : Keeps track of all the tasks that the process needs to perform
    queue<pair<string, int>> tasks = CPU->tasksQueue;
    
    // If there are any more tasks
    while(tasks.size() > 0){

        // Take the particular task        
        pair<string, int> task = tasks.front();
        tasks.pop();
        
        if(task.first != ""){
            cout<<"The Process "<<CPU->pid<<" of class "<<CPU->pclass<<" is going to access the "<<task.first<<endl;
        }
        
        // If the process requires CPU
        if(task.first == "CPU"){
            
            // Change the current state of process
            if(map.find(CPU->pid) != map.end()){
                map.find(CPU->pid)->second.processStatus = "EXECUTING";
            }
            
            
            int currentTime = counter;
            CPUBusyTime += task.second;
            
            // The process is being executed in the CPU.
            while(counter <= currentTime+task.second){
                counter++;
            }
        }
        // If the process requires Terminal
        else if(task.first == "TTY"){
            
            // Change the status of the process
            if(map.find(CPU->pid) != map.end()){
                map.find(CPU->pid)->second.processStatus = "ACCESSING TERMINAL";
            }
            
            // The process is accessing the Terminal
            int currentTime = counter;
            while(counter <= currentTime + task.second){
                counter++;
            }
        }
        
        // If the process wants to Perform IO
        else if(task.first == "DISK"){
            totalNoOfDiskAccesses++;
            
            // If the process needs a disk add it to the disk queue
            CPU->diskEnterTime = counter;
            CPU->ioTime = task.second;
            
            // Change the status of the process
            if(map.find(CPU->pid) != map.end()){
                map.find(CPU->pid)->second.processStatus = "WAITING FOR DISK ACCESS";
            }
            
            // Store the remaining tasks
            queue<pair<string, int>> temp;
            temp.push({"", 0});
            while(tasks.size()>0){
                temp.push({tasks.front().first, tasks.front().second});
                tasks.pop();
            }
            
            CPU->tasksQueue = temp;
            
            // Add the process to the diskqueue
            diskqueue.push(*CPU);
            
            // Mark the current executing process as null
            CPU = NULL;
            return;
        }
    }
    
    // When the process gets out of the loop, all the tasks of that particular process are executed
    cout<<"Proceess "<<CPU->pid<<" of class "<<CPU->pclass <<" has been successfully executed!!"<<endl<<endl;
    
    // Change the status of the process
    if(map.find(CPU->pid) != map.end()){
        if(map.find(CPU->pid)->second.processClass == "REAL-TIME" && CPU->deadline < counter){
            map.find(CPU->pid)->second.processStatus = "MISSED DEADLINE";    
        }
        else{
            map.find(CPU->pid)->second.processStatus = "EXECUTED";
        }
    }
    
    // Mark the CPU as NULL as it needs to assigned to another process
    CPU = NULL;
}


// It is used to view the current status of all the processes
void printTable(){
    for (const auto& pair : map) {
        cout << "Key: " << pair.first << endl;
        cout << "Value:" << endl;
        cout << "    Process ID: " << pair.second.processId << endl;
        cout << "    Process Class: " << pair.second.processClass << endl;
        cout << "    Arrival Time: " << pair.second.arrivalTime << endl;
        cout << "    Process Status: " << pair.second.processStatus << endl;
    }
}

// It is used to print the statistics of the processes
void printStatistics(){
    for (const auto& pair : map) {
            if(pair.second.processClass == "REAL-TIME"){
            if(pair.second.processStatus == "EXECUTED"){
                noOfRealTimeProcessesCompleted++;
            }
            else{
                noOfRealTimeProcessesMissedDeadline++;
            }
        }
        else{
            noOfInteractiveProcesses++;
        }
    }
    
    cout<<"Number of real-time processes that have been Completed                 : "<<noOfRealTimeProcessesCompleted<<endl;
    cout<<"percentage of real-time processes that missed their deadline           : "<< (noOfRealTimeProcessesMissedDeadline/noOfReamTimeProcesses) * 100<<"%"<<endl;
    cout<<"Total Number of Interactive processes that have been Completed         : "<< noOfInteractiveProcesses<<endl;
    cout<<"Total Number of Disk Accesses                                          : "<< totalNoOfDiskAccesses<<endl;
    cout<<"Average Duration of Disk Accesses                                      : "<< totalDiskAccessTime/totalNoOfDiskAccesses<<endl;
    cout<<"Total Time Elapsed since the start of the First Process                : "<<counter - intitialProcessArrivalTime<<endl;
    cout<<"CPU Utilization                                                        : "<<((double)CPUBusyTime / ((double)counter - (double)intitialProcessArrivalTime)) * 100<<"%"<<endl;
    cout<<"Disk Utilization                                                       : "<< ((double)totalDiskAccessTime / ((double)counter - (double)intitialProcessArrivalTime)) * 100<<"%"<<endl;
}


// Main Function
int main(){
    
    std::ofstream outputFile("output.txt");

    // Redirect cout to the output file stream
    auto coutBuffer = std::cout.rdbuf(outputFile.rdbuf());

    // Stores the processes when they are in New state
    priority_queue<Process, vector<Process>, CompareProcesses> pq;

    string line;
    int finalProcessArrival = 0;
    int count = 0;

    
    // Used to store the process type and arrivalTime
    string processtype = "";
    int temparrivaltime = 0;

    int processID=0;

    
    // Reading the Input from the file.
    while(true){
        int arrivalTime;
        string type;

        if(processtype == ""){
            getline(cin, line);
            stringstream ss(line);
            ss>>type;
            ss>>arrivalTime;
        }else{
            type = processtype;
            arrivalTime = temparrivaltime;
        }

        finalProcessArrival = max(arrivalTime, finalProcessArrival);

        // If the current process is of type INTERACTIVE
        if(type=="INTERACTIVE"){     
            string taskstring;
            queue<pair<string, int>> tasksQueue1;
            tasksQueue1.push({"",0});

            while(true){
                
                getline(cin, taskstring);

                if(taskstring.empty()){
                    map[processID] = Process_State(processID, "INTERACTIVE", arrivalTime, "NEW");
                    Process p1 = Process(processID++, "INTERACTIVE", tasksQueue1, arrivalTime, -1, -1, -1);
                    pq.push(p1);
                    break;
                }

                stringstream sss(taskstring);
                string tasktype;
                sss>>tasktype;
                int timereq = 0;
                sss>>timereq;

                if(tasktype == "INTERACTIVE" || tasktype == "REAL-TIME"){
                    processtype = tasktype;
                    temparrivaltime = timereq;
                    map[processID] = Process_State(processID, "INTERACTIVE", arrivalTime, "NEW");
                    Process p1 = Process(processID++, "INTERACTIVE", tasksQueue1, arrivalTime, -1, -1, -1);
                    pq.push(p1);
                    break;
                }
                tasksQueue1.push({tasktype, timereq});
            }
        }
        // If the process is of type REAL-TIME
        else{
            noOfReamTimeProcesses++;
            
            string taskstring;
            queue<pair<string, int>> myqueue;
            int deadline = 0;
            myqueue.push({"",0});
            while(true){

                getline(cin, taskstring);
                if(taskstring.empty()){
                    map[processID] = Process_State(processID, "REAL-TIME", arrivalTime, "NEW");
                    Process p1 = Process(processID++, "REAL-TIME", myqueue, arrivalTime, deadline, -1, -1);
                    pq.push(p1);
                    goto endLoop;
                }

                    stringstream sss(taskstring);
                    string tasktype;
                    sss>>tasktype;
                    int timereq = 0;
                    sss>>timereq;
                    
                    if(tasktype == "DEADLINE"){
                        deadline = timereq;
                        continue;
                    }

                    if(tasktype == "INTERACTIVE" || tasktype == "REAL-TIME"){
                        processtype = tasktype;
                        temparrivaltime = timereq;
                        map[processID] = Process_State(processID, "REAL-TIME", arrivalTime, "NEW");
                        Process p1 = Process(processID++, "REAL-TIME", myqueue, arrivalTime, -1, -1, -1);
                        pq.push(p1);
                        break;
                    }
                    myqueue.push({tasktype, timereq});
                }
            }
        }
    
   
    // label to come out of the loop
    endLoop:

    
    // Prints the Inital States of all the processes
    cout<<endl<<endl<<"Initial States of the Processes : "<<endl<<endl;
    printTable();
    cout<<endl<<endl;

    // Storing the inital arrivalTime of the process    
    intitialProcessArrivalTime = pq.top().arrivalTime;

    
    while(CPU!=NULL || pq.size() > 0 || rtqueue.size() > 0 || intqueue.size() > 0 || diskqueue.size()>0) {

        
        
        // If there are any new processes that arrived
        while(pq.size() > 0 && pq.top().arrivalTime <= counter){
    
            // If INTERACTIVE, add it to a INTERACTIVE queue
            if(pq.top().pclass == "INTERACTIVE"){
                intqueue.push(pq.top());
            }else{
            // If Realtime, add it to a Real counter queue
                rtqueue.push(pq.top());
            }
            
            pq.pop();
        }

        if(CPU != NULL && CPU->pclass == "REAL-TIME"){
            execute();
        }
        // If there is a real counter process in the queue
        else if(rtqueue.size() > 0){
            
            // If Cpu is IDLE
            if(CPU == NULL){
                cout<<"CPU is idle, executing the Real time process"<<endl;
                Process p = rtqueue.front();

                rtqueue.pop();

                CPU = &p;
                
                cout<<"Assigned the CPU to the Real time process"<<endl;
            }
            else if(CPU->pclass == "REAL-TIME"){
                execute();
                cout<<"Execute the Realtime process";
            }
            else if(CPU->pclass == "INTERACTIVE"){
                cout<<"INTERACTIVE process is being stored in ready queue and assigning CPU to Real time process"<<endl;
                intqueue.push(*CPU);
                CPU =  &rtqueue.front();
                rtqueue.pop();
                cout<<"Added INTERACTIVE process to the end of ready state";
            }
        }
        else if(intqueue.size() > 0){
            // If CPU is IDLE
            if(CPU == NULL){
                Process p = intqueue.front();

                intqueue.pop();
                CPU = &p;

                cout<<endl<<endl<<"Assigned INTERACTIVE process to the CPU"<<endl;
            }
            else if(CPU->pclass == "REAL-TIME"){
                cout<<"Execute the Real Time process";
                execute();
            }
            else if(CPU->pclass == "INTERACTIVE"){
                cout<<"Execute the INTERACTIVE process";
                execute();
            }
        }
        else if(CPU != NULL){
            execute();
        }


        // Check the DIsk Queue to see if there are any processes that completed the disk access
        while(diskqueue.size() > 0 && diskqueue.front().diskEnterTime + diskqueue.front().ioTime >= counter){
            
            Process p = diskqueue.front();
            diskqueue.pop();
            
            if(p.pclass == "INTERACTIVE"){
                cout<<"The process of class "<<p.pclass<<" came out of Disk queue"<<endl;
                intqueue.push(p);
            }
            else{
                cout<<"The process of class "<<p.pclass<<" came out of Disk queue"<<endl;
                rtqueue.push(p);
            }
            totalDiskAccessTime += p.ioTime;
            
            // Change the status of the process
            if(map.find(p.pid) != map.end()){
                map.find(p.pid)->second.processStatus = "WAITING IN READY QUEUE";
            }
        }
        counter++;
        if(counter >= 50000) break;
    }
    
    // Prints the final state of all the processes
    cout<<endl<<"Final State of the Processes :"<<endl<<endl;
    printTable();
    
    cout<<endl<<endl<<"Final statistics : "<<endl<<endl;
    // Prints the statistics
    printStatistics();
    
    
    // Close the file
    outputFile.close();

    // Restore cout back to its original buffer
    std::cout.rdbuf(coutBuffer);
    
    return 0;
}


