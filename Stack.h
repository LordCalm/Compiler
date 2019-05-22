#pragma once
#include <iostream>
#include <stdexcept>
using namespace std;

typedef class Stack {
private:
	int *_stack_ptr;
	const int _size;
	int _top;
public:
	Stack(int csize);
	~Stack();

	void Push(int data);
	int Pop();
	int Top();
	void Dup();
	void Clear();
	bool Empty();

	void mul();
	void add();
	void sub();
	void div();
}stack;
