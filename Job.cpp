//Steven Soranno
// Evan Deangelis

// Job class

#include "Job.hpp"
#include <iostream>
#include <stdlib.h>

using namespace std;

// constructor
Job::Job(int n, int a, int w, int d, int r, int p)
{
    next = NULL;
    jobNum = n;
    arrTime = a;
    m = w;
    dev = d;
    runtime = r;
    priority = p;
    state = 0;
    devHold=0;
    finishTime=0;
    waitQ=0;
    waitL=0;
    releaseWait=0;
    devTemp=0;
    weight= 0;
}

Job::~Job()
{

}

// Setter and Getters
int Job::getM()
{
    return m;
}

void Job::setM(int s)
{
    m=s;
}

int Job::getDev()
{
    return devHold;
}

int Job::getNeed()
{
    return (dev-devHold);
}

void Job::setDev(int d)
{
    devHold+=d;
    if (devHold>dev)
    {
        devHold=dev;
    }
}

int Job::getNum()
{
    return jobNum;
}

void Job::setNum(int n)
{
    jobNum =n;
}

int Job::getPriority()
{
    return priority;
}

void Job::setPriority(int p)
{
    priority=p;
}

void Job::setState(int s)
{
    state=s;
}

int Job::getState()
{
    return state;
}

void Job::setRuntime(int r)
{
    runtime=r;
}

int Job::getRuntime()
{
    return runtime;
}


void Job::printJob()
{
    cout << "job:" << jobNum << endl;
    
    cout << "Devices:" << devHold << endl;
    
    cout << "Remaining Time:" << runtime << endl;
    if (state==0)
    {
        cout<<"State: Holding"<<endl;
    }
    else if (state==1)
    {
        cout<<"State: Ready"<<endl;
    }
    else if (state == 2)
    {
        cout<< "State: Running"<<endl;
    }
    else if (state == 3)
    {
        cout<< "State: Completed at time "<< finishTime <<endl;
        cout << "Turnaround time: "<< finishTime-arrTime<<endl;
        cout << "Weighted Turnaround time: "<< (int)((finishTime-arrTime)/weight)<<endl;
    }
    else if (state == 4)
    {
        cout<<"State: Waiting for Devices"<<endl;
    } else if(state == 5){
        cout<<"State: Rejected"<<endl;
    }
    cout << endl;
}

Job* Job::getNext()
{
    return next;
}

int Job::getFinishTime(){
    return finishTime;
}

void Job::setfinishTime(int i)
{
    finishTime=i;
}

int Job::getRequest(){
    return devTemp;
}

void Job::setRequest(int r)
{
    devTemp=r;
}

int Job::getWaitQ(){
    return waitQ;
}

void Job::setWaitQ(int q)
{
    waitQ=q;
}

int Job::getWaitL(){
    return waitL;
}

void Job::setWaitL(int q)
{
    waitL=q;
}

int Job::getRelease(){
    return releaseWait;
}

void Job::setRelease(int r)
{
    releaseWait=r;
}

int Job::getDevMax()
{
    return dev;
}
void Job::setWeight(int w){
    weight = w;
}

void Job::setDev2(int d){
    devHold = d;
}