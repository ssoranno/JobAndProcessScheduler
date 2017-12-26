//Steven Soranno
// Evan Deangelis

#ifndef SLL_HPP_
#define SLL_HPP_

#include "Job.hpp"

class SLL {
	Job *first;
	Job *last;
	int size;
public:
	SLL();
	~SLL();
	void printSLL();
	void push2(Job *j);
        void push1(Job *j);
        int getSize();
	int pop();
	Job *remFirst();
        int findK(int k);
        void remKth(int k);
        Job* getFirst();
        Job *findJob(int k);
        void addAtK(Job* x, int k);
        void push3(Job* j);
};

#endif