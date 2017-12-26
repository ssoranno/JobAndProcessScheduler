//Steven Soranno
// Evan Deangelis

// Cisc361 Final Project

#include "SLL.hpp"
#include "Job.hpp"

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
using namespace std;

// Declare Constant Variables

int Time = 0;
int MainMemory;
int TotalDevices;
int quantum;
int availableMem;
int availableDev;
int quit = 0;
int read = 1;
int CPUtime = 0;

// Instantiate running process job
Job* runProc = NULL;

// Instantiate hold queues, ready queue, wait queue 
SLL* hq1 = new SLL;
SLL* hq2 = new SLL;
SLL* rq = new SLL;
SLL* wq = new SLL;
SLL* devList = new SLL; // temporary list to help with the banker's algorithm

SLL* jList = new SLL; // List of jobs to be printed in the D command

// Declare functions
int checkReq(int d, int j);
void run();
void deviceInterrupt(int d, int j, Job *jah);
void Lcommand(char* line);
void deviceRelease(int d, int j);

// This function breaks up the c command input line and stores the correct input values into the constants.  
// It also sets the available memory variable to the value in the main memory variable and the available devices variable to the total devices memory. 

void Ccommand(char* line)
{
    int time = 0;
    int i = 2;
    int num = line[2] - '0';
    int mult = 10;
    while (num <= 9 && num >= 0) // Read the time input from the line and store it in time
    {
        time = mult * time + num;
        i++;
        num = line[i] - '0';

    } // while
    if (time == Time) // If the system time is the same as the command runtime then run the command
    {

        i = i + 3;
        num = line[i] - '0';
        while (num <= 9 && num >= 0) // Read the main memory input and store it in the Main Memory variable
        {
            MainMemory = mult * MainMemory + num;
            i++;
            num = line[i] - '0';

        } // while
        i = i + 3;
        num = line[i] - '0';
        while (num <= 9 && num >= 0) // Read the devices input and store it in TotalDevices
        {
            TotalDevices = mult * TotalDevices + num;
            i++;
            num = line[i] - '0';
        } // while
        i = i + 3;
        num = line[i] - '0';
        while (num <= 9 && num >= 0) // Read the quantum input and store it in increment
        {
            quantum = mult * quantum + num;
            i++;
            num = line[i] - '0';
        } // while
        availableMem = MainMemory; // Set available Memory to main memory
        availableDev = TotalDevices; // Set available Devices to total devices
        read = 1;
    } else
    {
        read = 0;
    } // else

} // void Ccommand(char* line)


// This function breaks up the A command input line and stores the correct input values into the temp variables
// It then creates a job with these input values and places it in the hold queue, ready queue, or rejects the 
// job based on the memory and devices it requests.
void Acommand(const char* line)
{
    int time = 0;
    int j = 0;
    int m = 0;
    int s = 0;
    int r = 0;
    int p = 0;

    int i = 2;
    int num = line[2] - '0';
    int mult = 10;
    while (num <= 9 && num >= 0) // Read the time input from the line and store it in time
    {
        time = mult * time + num;
        i++;
        num = line[i] - '0';
    } // while
    if (time == Time) // If the system time is the same as the command runtime then run the command
    {
        i = i + 3;
        num = line[i] - '0';
        while (num <= 9 && num >= 0) // Read the job number input from the line and store it j
        {
            j = mult * j + num;
            i++;
            num = line[i] - '0';

        } // while
        i = i + 3;
        num = line[i] - '0';
        while (num <= 9 && num >= 0) // Read the memory input from the line and store it m
        {
            m = mult * m + num;
            i++;
            num = line[i] - '0';
        } // while
        i = i + 3;
        num = line[i] - '0';
        while (num <= 9 && num >= 0) // Read the device input from the line and store it s
        {
            s = mult * s + num;
            i++;
            num = line[i] - '0';
        } // while
        i = i + 3;
        num = line[i] - '0';
        while (num <= 9 && num >= 0) // Read the runtime input from the line and store it r
        {
            r = mult * r + num;
            i++;
            num = line[i] - '0';
        } // while
        i = i + 3;
        num = line[i] - '0';
        while (num <= 9 && num >= 0) // Read the priority input from the line and store it p
        {
            p = mult * p + num;
            i++;
            num = line[i] - '0';
        } // while
        // If the input memory is less than the total memory and input device is less than the total devices then allow the job to run
        if (m < MainMemory && s < TotalDevices) 
        {
            Job *job = new Job(j, time, m, s, r, p); // create new job
            job->setWeight(quantum);
            jList->push3(job); // Place it on the job list for printing
            if (m < availableMem) // If there is enough available memory place the value in the job in the ready queue
            {
                job->setState(1);
                rq->push2(job);
                availableMem-=m;

            } else // If there is not enough available memory place the value in the job in one of the hold queues based on priority
            {
                if (job->getPriority() == 1)
                {
                    hq1->push1(job);

                } else
                {
                    hq2->push1(job);
                }  //  if (job->getPriority() == 1)
            } //if (m < availableMem)

        } else{ // Job is rejected place it on the jList for printing in the D command
            Job *job = new Job(j, time, m, s, r, p); // create new job
            job->setWeight(quantum);
            job->setState(5);
            jList->push3(job);
        } // if (m < MainMemory && s < TotalDevices)
        read = 1;
    } else
        read = 0;
} // void Acommand(const char* line)

// This function breaks up the A command input line and stores the correct input values into the temp variables
// This function then handles to devices request of a job when it is running in the CPU
void Qcommand(char* line)
{
    int time = 0;
    int j = 0;
    int d = 0;

    int i = 2;
    int num = line[2] - '0';
    int mult = 10;
    while (num <= 9 && num >= 0) // Read the time input from the line and store it in time
    {
        time = mult * time + num;
        i++;
        num = line[i] - '0';
    } // while
    if (time == Time) // If the system time is the same as the command runtime then run the command
    {
        i = i + 3;
        num = line[i] - '0';
        while (num <= 9 && num >= 0) // Read the job input from the line and store it in job
        {
            j = mult * j + num;
            i++;
            num = line[i] - '0';

        } // while
        i = i + 3;
        num = line[i] - '0';
        while (num <= 9 && num >= 0) // Read the devices input from the line and store it in d
        {
            d = mult * d + num;
            i++;
            num = line[i] - '0';
        } // while

        read = 1;
        // if devices requested is less than the total devices and the process is currently in the CPU, try and grant the request
        if (d <= TotalDevices && runProc->getNum() == j)
        {
            Job *temp = rq->getFirst();
            // Loop through the ready queue and make sure there are no other jobs in the ready queue where devices are requested first.
            int i=0;
            while (temp != NULL|| i <rq->getSize())
            {
                if (temp->getWaitQ() == 1) // if a Q request is waiting, jump to the device interrupt function to process the request first
                {
                    runProc->setWaitQ(1); 
                }
                temp = temp->getNext();
                i++;
            } // while
            if(runProc->getWaitQ() == 0){
                deviceInterrupt(d, j, runProc); // If no other Q request, process the current Q request
            } // if
        } else
        {
            // If the process is not in the CPU then set the variable waitQ to true and save the request until it get on the CPU
            if (rq->findK(j) != -1)
            {
                rq->findJob(j)->setWaitQ(1);
                rq->findJob(j)->setRequest(d);
            } else
            {
                wq->findJob(j)->setWaitQ(1);
                wq->findJob(j)->setRequest(d);
            } // if (rq->findK(j) != -1)
        } // if (d <= TotalDevices && runProc->getNum() == j)

    } else
        read = 0;
}

void deviceInterrupt(int d, int j, Job *jah)
{
    //This function takes a process and checks requests to see where devices can be given
    if (d <= TotalDevices)
    {
        if (devList->findK(j) == -1)//if the process is not on the device list
        {
            devList->push3(jah);

        }//if (devList->findK(j) == -1)
        else
        {
            devList->findJob(j)->setRequest(d);
        }//else
        if (checkReq(d, j)) //calls the bankers algorithm to see if request can be granted
        {
            availableDev -= d;
            jah->setDev(d);
            devList->findJob(j)->setDev(d);
            devList->findJob(j)->setRequest(0);
            // added to runtime subtracted cputime
            jah->setRuntime(jah->getRuntime() + 1);
            CPUtime--;
            jah->setState(1);
            jah->setRequest(0); //runProc
            rq->push2(jah);
            jah = NULL;
            runProc = NULL;
        }//if (checkReq(d, j))
        else
        {
            
            jah->setState(4);
            wq->push2(jah);  // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
            jah = NULL;
            runProc = NULL;  // added this !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        }//else
    }// if (d <= TotalDevices ) 
}// deviceInterrupt(int d, int j, Job *jah)

void Lcommand(char* line)
{
    //This function reads in a request to release devices and handles the process’ devices 
    int time = 0;
    int j = 0;
    int d = 0;

    int i = 2;
    int num = line[2] - '0';
    int mult = 10;
    while (num <= 9 && num >= 0)
    {
        time = mult * time + num;
        i++;
        num = line[i] - '0';
    }//while (num <= 9 && num >= 0)
    if (time == Time)
    {
        i = i + 3;
        num = line[i] - '0';
        while (num <= 9 && num >= 0)
        {
            j = mult * j + num;
            i++;
            num = line[i] - '0';

        }//while (num <= 9 && num >= 0)
        i = i + 3;
        num = line[i] - '0';
        while (num <= 9 && num >= 0)
        {
            d = mult * d + num;
            i++;
            num = line[i] - '0';
        }//while (num <= 9 && num >= 0)
        if (j == runProc->getNum()) //checks to see if the process is on the CPU
        {
            deviceRelease(d, j);
        }//if (j == runProc->getNum())
        else
        {
            if (rq->findK(j) != -1) //find the process on the wait queue or ready queue
            {
                rq->findJob(j)->setWaitL(1);
                rq->findJob(j)->setRelease(d);
            }// if (rq->findK(j) != -1)
            else
            {
                wq->findJob(j)->setWaitL(1);
                wq->findJob(j)->setRelease(d);
            }//else
        }//else
        read = 1;
    }// if (time == Time)
    else
    {
        read = 0;
    }
}//Lcommand(char* line)

void deviceRelease(int d, int j)
{
    //This function puts devices back into the system to be used and handles wait queues requests
    if (runProc->getDev() != 0) //checks to see that run process has devices
    {
        availableDev += d;
        devList->findJob(j)->setDev(-d);
        runProc->setDev(-d);
        if (devList->findJob(j)->getDev() == 0) //checks to see if device is already in the device list
        {
            devList->remKth(j);
        }// if (devList->findJob(j)->getDev() == 0)

        Job *tmp = wq->getFirst();
        int i=0;
        
        while (tmp != NULL || i<wq->getSize()) //loops through wait queue to check request
        {
            if (checkReq(tmp->getRequest(), tmp->getNum()))
            {
                availableDev -= tmp->getRequest();
                tmp->setDev(tmp->getRequest());
                devList->findJob(tmp->getNum())->setDev(tmp->getRequest());
                devList->findJob(tmp->getNum())->setRequest(0);
                tmp->setRequest(0);

                rq->push3(wq->findJob(tmp->getNum()));
                wq->remKth(tmp->getNum());
                
            }// if (checkReq(tmp->getRequest(), tmp->getNum()))
            tmp = tmp->getNext();
            i++;
        }// while (tmp != NULL)
    } //if (runProc->getDev() != 0)
}//deviceRelease(int d, int j)

void Dcommand(char* line)
{
    //Prints the current state of the system
    int time = 0;
    int i = 2;
    int num = line[2] - '0';
    int mult = 10;
    while (num <= 9 && num >= 0)
    {
        time = mult * time + num;
        i++;
        num = line[i] - '0';
    }
    Job *tmp = jList->getFirst();
    while (tmp != NULL) {
        if (rq->findK(tmp->getNum()) != -1) {
            tmp->setState(rq->findJob(tmp->getNum())->getState());
            tmp->setRuntime(rq->findJob(tmp->getNum())->getRuntime());
            tmp->setDev2(rq->findJob(tmp->getNum())->getDev());
            tmp->setWeight(quantum);
        } else if (wq->findK(tmp->getNum()) != -1) {
            tmp->setState(wq->findJob(tmp->getNum())->getState());
            tmp->setRuntime(wq->findJob(tmp->getNum())->getRuntime());
            tmp->setDev2(wq->findJob(tmp->getNum())->getDev());
            tmp->setWeight(quantum);
        } else if (hq1->findK(tmp->getNum()) != -1) {
            tmp->setState(hq1->findJob(tmp->getNum())->getState());
            tmp->setRuntime(hq1->findJob(tmp->getNum())->getRuntime());
            tmp->setDev2(hq1->findJob(tmp->getNum())->getDev());
            tmp->setWeight(quantum);
        } else if (hq2->findK(tmp->getNum()) != -1) {
            tmp->setState(hq2->findJob(tmp->getNum())->getState());
            tmp->setRuntime(hq2->findJob(tmp->getNum())->getRuntime());
            tmp->setDev2(hq2->findJob(tmp->getNum())->getDev());
            tmp->setWeight(quantum);
        } else if (runProc!=NULL  && runProc->getNum() == tmp->getNum()) // changed this !!!!!!!!!!!!!!!!!!!!!
        {
            tmp->setState(runProc->getState());
            tmp->setDev2(runProc->getDev());
            tmp->setRuntime(runProc->getRuntime());
            tmp->setWeight(quantum);
        }
        tmp = tmp->getNext();
    }
    if (time == Time)
    {
        cout << "D" << endl;
        cout << "Time:" << time << endl;
        
        cout<< "Job List: "<<endl;
        jList->printSLL();
        cout << "Ready Queue:" << endl;
        rq->printSLL();
        cout << "Wait Queue: " << endl;
        wq->printSLL();
        cout << "Hold Queue 1: " << endl;
        hq1->printSLL();
        cout << "Hole Queue 2: " << endl;
        hq2->printSLL();
        cout <<"---------------" <<endl;
        read = 1;
    } else if (time == 9999)
    {
        cout << "D" << endl;
        cout << "System Turnaround Time:" << Time << endl;
        cout << "System Weighted Turnaround Time: " << Time / quantum << endl;

        cout<< "Job List: "<<endl;
        jList->printSLL();
        cout << "Ready Queue:" << endl;
        rq->printSLL();
        cout << "Wait Queue: " << endl;
        wq->printSLL();
        cout << "Hold Queue 1: " << endl;
        hq1->printSLL();
        cout << "Hole Queue 2: " << endl;
        hq2->printSLL();
        cout <<"---------------" <<endl;
        cout << endl;
        read = 1;
        quit = 1;
    }
    else
    {
        read = 0;
    }//else

}//Dcommand(char* line)

void complete(Job *j)
{
    //This function clears memory and checks requests from newly released devices
    if (j->getRequest() == 0 && !j->getWaitQ())
    {
        j->setState(3);
        jList->findJob(j->getNum())->setState(3);
        jList->findJob(j->getNum())->setfinishTime(Time);
        jList->findJob(j->getNum())->setRuntime(0);
        jList->findJob(j->getNum())->setWeight(quantum);
        availableMem += j->getM();
        availableDev += j->getDev();
        devList->remKth(j->getNum());
        Job *tmp = wq->getFirst();
        int i=0;
        wq->getSize();
        while (tmp != NULL|| i<wq->getSize())
        {
            if (checkReq(tmp->getRequest(), tmp->getNum()))
            {
                
                availableDev -= tmp->getRequest();
                tmp->setDev(tmp->getRequest());
                devList->findJob(tmp->getNum())->setDev(tmp->getRequest());
                devList->findJob(tmp->getNum())->setRequest(0);
                tmp->setRequest(0);
                rq->push3(wq->findJob(tmp->getNum()));
                wq->remKth(tmp->getNum());
            } // if (checkReq(tmp->getRequest(), tmp->getNum()))
            tmp = tmp->getNext();
            i++;
        }// if (while (tmp != NULL)       
    }//  if (j->getRequest() == 0 && !j->getWaitQ())
    
    if (hq1->getSize() > 0 && hq1->getFirst()->getM() <= availableMem) { // Check to 
        rq->push3(hq1->getFirst());
        hq1->remFirst();
    } else if (hq1->getSize() > 0 && hq1->getFirst()->getM() <= availableMem) {
        rq->push3(hq2->getFirst());
        hq2->remFirst();
    }
    
}// complete(Job *j)

void run()
{
    // This function handles the cpu processes of each time cycle 
    if (rq->getSize() != 0)
    {
        if (runProc == NULL) //if there is nothing on CPU 
        {
            runProc = rq->remFirst();
            runProc->setState(2);
        }//if (runProc == NULL)
        else if (CPUtime == quantum - 1) //if the quantum has been reached
        {
            runProc->setState(1);
            rq->push2(runProc);
            runProc = rq->remFirst();
            runProc->setState(2);
            CPUtime = 0;
        }//else if (CPUtime == increment-1)
        else if (runProc->getWaitQ() == 1) //if the process had requested a device
        {
            runProc->setWaitQ(0);
            deviceInterrupt(runProc->getRequest(), runProc->getNum(), runProc);
            runProc = rq->remFirst();
            runProc->setState(2);
            CPUtime = 0;
        }//else if(runProc->getWaitQ()==1)
        else if (runProc->getWaitL() == 1) //if the device had tried to release a device
        {
            runProc->setWaitL(0);
            deviceRelease(runProc->getRelease(), runProc->getNum());
        }// else if (runProc->getWaitL()==1)

        runProc->setRuntime(runProc->getRuntime() - 1);
        CPUtime++;
        
        if (runProc->getRuntime() == 0 && !runProc->getWaitQ()) //handle a job that has completed
        {
            complete(runProc);
            runProc = rq->remFirst();
            runProc->setState(2);
            CPUtime = 0;
        }//if (runProc->getRuntime()==0 && !runProc->getWaitQ())

    } else if(rq->getSize() == 0 && runProc!=NULL){  // Changed this !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        runProc->setRuntime(runProc->getRuntime() - 1);
         if (runProc->getRuntime() == 0 && !runProc->getWaitQ()) //handle a job that has completed
        {
            complete(runProc);
            runProc->setState(2);
            runProc = NULL;
        }//if (runProc->getRuntime()==0 && !runProc->getWaitQ())
    }//   if(rq->getSize()!=0)
}//run()

int checkReq(int d, int j)
{
    // This function runs the bankers algorithm to check to see requests could be granted
    int work = availableDev;
    int finish[devList->getSize()];
    int i = 0;
    int change = 1;
    SLL *tmpList = new SLL;
    Job *tmp = devList->getFirst();
    for (int i = 0; i < devList->getSize(); i++) //create an array for processes that finish
    {
        finish[i] = 0;
        tmpList->push3(tmp);
        tmp = tmp->getNext();
    }//  for(int i =0; i<devList->getSize(); i++)
    i = 0;
    while (change) //run until there is no more change in the system
    {
        change = 0;
        tmp = devList->getFirst();
        while (i < devList->getSize()) //run through a the device list
        {
            if (!finish[i] && tmp->getRequest() <= work) //check to see if job is done or request is less than //available
            {
                finish[i] = 1;
                work += tmp->getDev();
                change = 1;

            }//if (!finish[i] && tmp->getRequest() <= work)
            tmp = tmp->getNext();
            i++;
        }//while(i<devList->getSize())

    }//while (change)
    int safe = 1;
    for (i = 0; i < devList->getSize(); i++) //check to see if system is safe
    {
        if (finish[i] == 0)
        {
            safe = 0;
        }//if (finish[i] == 0)
    }//for(i =0; i<devList->getSize(); i++)
    if (safe) //if safe process the request in a temporary list to see results
    {
        tmp = tmpList->findJob(j);
        work = availableDev;
        if (d <= availableDev)
        {
            if (d + tmp->getDev() <= tmp->getDevMax()) //if the new allocation is less than the process max
            {
                work -= d;
                tmp->setDev(d);
                tmp->setRequest(-d);
            }// if (d + tmp->getDev() <= tmp->getDevMax())


            tmp = tmpList->getFirst();
            for (i = 0; i < tmpList->getSize(); i++) //reset the finish array to test a safe state again
            {
                finish[i] = 0;
            }// for (i = 0; i < tmpList->getSize(); i++)
            i = 0;
            change = 1;
            while (change) //check for safe state again
            {
                change = 0;
                tmp = tmpList->getFirst();
                while (i < tmpList->getSize())
                {
                    if (!finish[i] && tmp->getRequest() <= work)
                    {
                        finish[i] = 1;
                        work += tmp->getDev();
                        change = 1;
                    }//if (!finish[i] && tmp->getRequest() <= work)
                    tmp = tmp->getNext();
                    i++;
                }//while (i < tmpList->getSize())

            }//while (change)
            safe = 1;
            for (i = 0; i < tmpList->getSize(); i++)
            {
                if (finish[i] == 0)
                {
                    safe = 0;
                }//if (finish[i] == 0)
            }// for (i = 0; i < tmpList->getSize(); i++)
        }//if (d <= availableDev)
        else
        {
            safe = 0;
        }//else

    }//if (safe)

    return safe;
}// checkReq(int d, int j)

int main(int argc, char** argv)
{
    // loop through file
    // if line starts with C
    // Send line to C method
    // if line starts with A
    // Send line to A method
    // if line starts with Q
    // Send line to Q method
    // if line starts with L
    // Send line to L method
    // if line starts with D
    // Send line to D method

    FILE *pToFile = fopen("input2.txt", "r");
    int line = 0;
    char input[512];
    char file[40][512];
    while (fgets(input, 512, pToFile) && read)
    {
        for (int i = 0; i < 512; i++)
        {
            file[line][i] = input[i];
        }
        line++;
    }

    int line2 = 0;

    while (line2 < line && quit == 0)
    {
        run();
        if (file[line2][0] == 'C')
        {
            Ccommand(file[line2]);
        } else if (file[line2][0] == 'Q')
        {
            Qcommand(file[line2]);
        } else if (file[line2][0] == 'A')
        {
            Acommand(file[line2]);
        } else if (file[line2][0] == 'L')
        {
            Lcommand(file[line2]);
        } else if (file[line2][0] == 'D')
        {
            Dcommand(file[line2]);
        }
        if (read)
        {
            line2++;
        }
        Time++;
        
    }
    return (EXIT_SUCCESS);
}