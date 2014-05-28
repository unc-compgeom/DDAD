#include <iostream>
#include <queue>
#include "Point_2.h"
#include "Segment_2.h"

using namespace std;

template <class T>
class Queue
{
	queue<T> Elements;
	int head;
	int tail;
public:
	Queue(){}
	void enqueue(T &e)
	{
			
		Elements.push(e);
	}
	T& dequeue()
	{
		T &temp = Elements.front();
		Elements.pop();
		return temp;
	}
	bool isEmpty()
	{
		return Elements.empty();
	}
};