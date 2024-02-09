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

*/
#include<iostream>
#include<string.h>
#include<queue>
#include<utility>
#include <string>
#include <sstream>
#include <unordered_map>

using namespace std;

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


    Process(int pid, string pclass, queue<pair<string,int>> tasksQueue, int arrivalTime, int deadline, int diskEnterTime, int ioTime){
        this->pid = pid;
        this->pclass = pclass;
        this->tasksQueue = tasksQueue;
        this->diskEnterTime = diskEnterTime;
        this->ioTime = ioTime;
        this->arrivalTime = arrivalTime;
        this->deadline = deadline;
    }

    Process(const Process& other) : pid(other.pid), pclass(other.pclass), tasksQueue(other.tasksQueue), arrivalTime(other.arrivalTime), deadline(other.deadline), diskEnterTime(other.diskEnterTime), ioTime(other.ioTime) {}


    void print(){
        cout<<this->pid<<"\n";
        cout<<this->pclass<<"\n";
        cout<<this->arrivalTime<<"\n";
        cout<<this->diskEnterTime<<"\n";
        cout<<this->ioTime<<"\n";

        printTasks();
    }

    void printTasks(){
        while(tasksQueue.size() > 0){
            cout << "(" << tasksQueue.front().first << ", " << tasksQueue.front().second << ")" << endl;
            tasksQueue.pop();
        }
    }

};

struct CompareProcesses{

    bool operator()(const Process& p1, const Process & p2){
        return p1.arrivalTime > p2.arrivalTime;
    }

};

struct Process_State{
    
    public:
    int processId;
    string processClass;
    int arrivalTime;
    string processStatus;
    
     Process_State() {}
    
    Process_State(int processId, string processClass, int arrivalTime, string processStatus){
        this->processId = processId;
        this->processClass = processClass;
        this->arrivalTime = arrivalTime;
        this->processStatus = processStatus;
    }
    
};





int counter = 11999;
Process *CPU = NULL;
queue<Process> intqueue;
queue<Process> rtqueue;
queue<Process> diskqueue;
unordered_map<int, Process_State> map;

void execute(){
    cout<<"COminginto execute method"<<endl;
    queue<pair<string, int>> tasks = CPU->tasksQueue;
    cout<<CPU<<endl;
    cout<<"Executing the "<<CPU->pclass<<" "<<CPU->pid<<" process"<<endl;
    while(tasks.size() > 0){
        
        pair<string, int> task = tasks.front();
        cout<<task.first<<" "<<task.second<<endl;
        tasks.pop();

        cout<<"The task is "<<task.first<<endl;

        // If the process requires CPU counter, just give it
        if(task.first == "CPU"){
            cout<<"Hey Process "<<CPU->pid<<" Go and access the CPU\n";
            if(map.find(CPU->pid) != map.end()){
                map.find(CPU->pid)->second.processStatus = "EXECUTING";
            }
            int currentTime = counter;
            while(counter <= currentTime+task.second){
                counter++;
            }
        }
        else if(task.first == "TTY"){
            cout<<"Hey Process "<<CPU->pid<<" Go and access the Terminal\n";
            if(map.find(CPU->pid) != map.end()){
                map.find(CPU->pid)->second.processStatus = "ACCESSING TERMINAL";
            }
            int currentTime = counter;
            while(counter <= currentTime + task.second){
                counter++;
            }
            // counter += task.second;
        }
        else if(task.first == "DISK"){
            cout<<"So you want DISK, that means you don't want CPU, ok Go back to DISK and when you arrive go to ready queue\n";
            // If the process needs a disk add it to the disk queue
            CPU->diskEnterTime = counter;
            CPU->ioTime = task.second;
            if(map.find(CPU->pid) != map.end()){
                map.find(CPU->pid)->second.processStatus = "WAITING FOR DISK ACCESS";
            }
            CPU->tasksQueue = tasks;
            
            queue<pair<string, int>> temp;
            
            temp.push({"", 0});
            
            while(tasks.size()>0){
                temp.push({tasks.front().first, tasks.front().second});
                tasks.pop();
            }
            
            CPU->tasksQueue = temp;
            
            diskqueue.push(*CPU);
            // Mark the current executing process as null
            CPU = NULL;
            return;
        }
    }
    cout<<"Proceess "<<CPU->pid<<" has been successfully executed!!";
    if(map.find(CPU->pid) != map.end()){
        map.find(CPU->pid)->second.processStatus = "EXECUTED";
    }
    CPU = NULL;
}


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



int main(){

    priority_queue<Process, vector<Process>, CompareProcesses> pq;

    string line;
    int finalProcessArrival = 0;

    int count = 0;


    string processtype = "";
    int temparrivaltime = 0;

    int processID=0;

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

        // cout<<"THis type"<<type<<endl;

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
                // if(!getline(cin, taskstring) || taskstring.empty()){
                //     Process p1 = Process(processID++, "INTERACTIVE", tasksQueue1, arrivalTime, -1, -1, -1);
                //     pq.push(p1);
                //     break;
                // }

                stringstream sss(taskstring);
                string tasktype;
                sss>>tasktype;
                int timereq = 0;
                sss>>timereq;
                // cout<<tasktype<<endl;

                if(tasktype == "INTERACTIVE" || tasktype == "REAL-TIME"){
                    processtype = tasktype;
                    temparrivaltime = timereq;
                    cout<<"A neew process came so saving the last process"<<endl;
                    map[processID] = Process_State(processID, "INTERACTIVE", arrivalTime, "NEW");
                    Process p1 = Process(processID++, "INTERACTIVE", tasksQueue1, arrivalTime, -1, -1, -1);
                    pq.push(p1);
                    cout<<"Hehes"<<endl;
                    break;
                }
                

                tasksQueue1.push({tasktype, timereq});
            }
        }
        else{
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

                    // cout<<"Reyyyyyy"<<tasktype<<endl;
                    if(tasktype == "INTERACTIVE" || tasktype == "REAL-TIME"){
                        processtype = tasktype;
                        temparrivaltime = timereq;
                        map[processID] = Process_State(processID, "REAL-TIME", arrivalTime, "NEW");
                        Process p1 = Process(processID++, "REAL-TIME", myqueue, arrivalTime, -1, -1, -1);
                        pq.push(p1);
                        break;
                    }
                
                    cout<<endl<<endl<<endl;
                    cout<<tasktype<<" "<<timereq<<endl<<endl;
                    myqueue.push({tasktype, timereq});
                    cout<<myqueue.size()<<endl<<endl;
                }
            }
        }
    
   
    endLoop:

    printTable();

    cout<<"Priotiry queue"<<pq.size()<<endl;
    while(CPU!=NULL || pq.size() > 0 || rtqueue.size() > 0 || intqueue.size() > 0 || diskqueue.size()>0) {

        // cout<<"About to add new processes into ready queue"<<endl;
        
        // If there are any processes present and the process arrival counter is already crossed, then
        while(pq.size() > 0 && pq.top().arrivalTime <= counter){
            cout<<"Came in to add processes to ready queue"<<endl;
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
            cout<<"CPU is not equal to null and already executing a real time process"<<endl;
            execute();
            cout<<"Came back";
        }
        // If there is a real counter process in the queue
        else if(rtqueue.size() > 0){
            
            // If Cpu is IDLE
            if(CPU == NULL){
                cout<<"CPU is IDLE and Real time process queue has some processes"<<endl;

                // cout<<endl<<rtqueue.size()<<endl;
                cout<<"CPU is idle, you can execute the Real time process"<<endl;
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
            cout<<"COming here to add the INTERACTIVE process"<<endl;
            // If CPU is IDLE
            if(CPU == NULL){
                cout<<"CPU is idle, you can execute the INTERACTIVE process"<<endl;
                Process p = intqueue.front();

                intqueue.pop();
                CPU = &p;

                cout<<"Assigned INTERACTIVE process to the CPU"<<endl;
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

        // Add all the processes that completed their disk utilization to their respective queues
        // cout<<"About to enter disk queue loop"<<endl;

        while(diskqueue.size() > 0 && diskqueue.front().diskEnterTime + diskqueue.front().ioTime >= counter){
            cout<<"Entered the loop of Disk queue"<<endl;
            Process p = diskqueue.front();
            diskqueue.pop();
            
            if(p.pclass == "INTERACTIVE"){
                cout<<"Intereactive process came out of Disk queue"<<endl;
                intqueue.push(p);
            }
            else{
                cout<<"Real time process came out of Disk queue"<<endl;
                rtqueue.push(p);
            }
            if(map.find(p.pid) != map.end()){
                map.find(p.pid)->second.processStatus = "WAITING IN READY QUEUE";
            }
        }

        
        counter++;
        if(counter >= 50000) break;
        
    }
    
    printTable();
    cout<<intqueue.size()<<endl;
    cout<<"Completed";
}


