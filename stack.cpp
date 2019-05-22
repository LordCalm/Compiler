#include "Stack.h"

Stack::Stack(int size) :
	_size(size)
{
	_stack_ptr = new int [_size];
	_top = 0;
}

Stack::~Stack()
{
	delete[] _stack_ptr;
}

void Stack::Push(int data)
{
	if (_top < _size) *(_stack_ptr + _top++) = data;
	else
	{
		throw out_of_range("stack overflow");
	}
}

int Stack::Pop()
{
	if (_top > 0) return *(_stack_ptr + --_top);
	else
	{
		throw out_of_range("stack is empty");
		return 0;
	}
}

int Stack::Top()
{
	if (_top > 0) return *(_stack_ptr + _top - 1);
	else
	{
		throw out_of_range("stack is empty");
		return 0;
	}
}

void Stack::Dup()
{
	if (_top < _size) *(_stack_ptr + _top++) = *(_stack_ptr + _top - 1);
	else
	{
		throw out_of_range("stack overflow");
	}
}

void Stack::Clear()
{
	_top = 0;
}

bool Stack::Empty()
{
	if (_top != 0) return true;
	return false;
}

void Stack::mul()
{
	if (_top > 1)
	{
		int x = Pop(), y = Pop();
		Push(x * y);
	}
	else if (_top == 1) throw logic_error("Not enough arguments");
	else throw out_of_range("stack is empty");
}

void Stack::add()
{
	if (_top > 1)
	{
		int x = Pop(), y = Pop();
		Push(x + y);
	}
	else if (_top == 1) throw logic_error("Not enough arguments");
	else throw out_of_range("stack is empty");
}

void Stack::sub()
{
	if (_top > 1)
	{
		int x = Pop(), y = Pop();
		Push(y - x);
	}
	else if (_top == 1) throw logic_error("Not enough arguments");
	else throw out_of_range("stack is empty");
}

void Stack::div()
{
	if (_top > 1)
	{
		int x = Pop(), y = Pop();
		Push(y / x);
	}
	else if (_top == 1) throw logic_error("Not enough arguments");
	else throw out_of_range("stack is empty");
}

