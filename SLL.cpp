//Steven Soranno
// Evan Deangelis

#include "SLL.hpp"
#include "Job.hpp"
#include <iostream>
#include <stdlib.h>
using namespace std;

// SLL constructor
SLL::SLL()
{
    first = NULL;
    last = NULL;
    size = 0;
}

// SLL destructor
SLL::~SLL() 
{
    Job *tmp = first;
    while (first != NULL)
    {
        tmp = first->next;
        delete first;
        first = tmp;
        size=getSize();
    }
    first = NULL;
    last = NULL;
}

void SLL::printSLL() //prints the list
{
    Job *tmp = first;
    while (tmp != NULL)
    {
        tmp->printJob();
        tmp = tmp->next;
    }

    cout << endl;
}

void SLL::push2(Job *j) //puts job at the end of a list
{
    if (first == NULL)//|| first->priority > j->priority)
    {
        size=getSize();
        j->next=first;
        first = j;
    } else
    {
        Job *tmp= first;
        while (tmp->next!=NULL)// && tmp->next->priority <= j->priority)
        {
            tmp = tmp->next;
        }
        j->next = tmp->next;
        tmp->next = j;
        size=getSize();
    }
}

void SLL::push1(Job *j) //puts job on queue based on its runtime
{ 
    if (first == NULL|| first->runtime > j->runtime)
    {
        size=getSize();
        j->next=first;
        first = j;
    } else
    {
        Job *tmp= first;
        while (tmp->next!=NULL && tmp->next->runtime < j->runtime)
        {
             tmp = tmp->next;
        }
        j->next = tmp->next;
        tmp->next = j;
        size=getSize();
    }
    
}


Job *SLL::remFirst() //removes first item in list
{ 
    Job *j=first;
    if (size==1)
    {
        first = NULL;
        size = 0;
    } else
    {
        Job *tmp = first->next;
        j = first;
        first = tmp;
        size=getSize();
    }
    return j;
}

int SLL::getSize() //returns size of list
{
    int i=0;
    Job *tmp = first;

    while(tmp!=NULL)
    {
        i++;
        tmp=tmp->next;
        //cout<<"v"<<endl;
    }
    return i;
}

int SLL::findK(int k) //finds the index of a certain job in the list
{
	Job *tmp = first;
	int ind = 0;
	while (tmp != NULL && tmp->jobNum != k) {
		tmp = tmp->next;
		ind++;
	}
        size = getSize();
	if (ind < size) {
		return ind;
	}
	else {
		return -1;
	}
}

void SLL::remKth(int x) //removes the job at a certain index
{
    int k = findK(x);
    if (k < size && k > 0)
    {
        Job *tmp = first;
        for (int i = 0; i < k - 1; i++)
        {
            tmp = tmp->next;
        }
        Job *tmp2 = tmp->next;
        tmp->next = tmp->next->next;
        //delete tmp2;
        size=getSize();
    }
    else if (k==0)
    {
        remFirst();
    }
}

Job *SLL::findJob(int k) //returns the job found based off job number
{
    Job *tmp = first;
    int i=0;
    while(tmp->jobNum != k && i<size-1)
    {
        tmp = tmp->next;
        i++;
    }
    return tmp;
}

Job* SLL::getFirst()//returns the first item in the list
{
    return first;
}

void SLL::addAtK(Job* x, int k) //adds job at certain index
{
    Job *tmp = first;
    if (k == 0)
    {
        push2(x);
    }
    if (k < size && k >= 0)
    {
        for (int i = 0; i < k - 1; i++)
        {
            tmp = tmp->next;
        }
        Job *tmp2 = tmp->next;
        tmp->next = x;
        tmp->next->next = tmp2;
        size=getSize();
    }
}

void SLL::push3(Job* j) //puts a deep copy of a job on the end of a list
{
    Job* newJ = new Job (j->jobNum, j->arrTime, j->m, j->dev, j->runtime, j->priority);
    newJ->devHold=j->devHold;
    newJ->devTemp=j->devTemp;
    newJ->state = j->state;
     if (first == NULL)//|| first->priority > j->priority)
    {
        size=getSize();
        newJ->next=first;
        first = newJ;
    } else
    {
        Job *tmp= first;
        while (tmp->next!=NULL)// && tmp->next->priority <= j->priority)
        {
            tmp = tmp->next;
        }
        newJ->next = tmp->next;
        tmp->next = newJ;
        size=getSize();
    }
}