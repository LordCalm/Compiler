#include "Stack.h"
#include <fstream>
#include <string>
#include <vector>
#include <map>

class command {
public:
	int command;
	int arg_type;
	int arg;
};

int main()
{
	vector<command> instruction;
	try
	{
		ifstream input("C:\\Users\\ƒмитрий\\source\\repos\\Compiler\\instruct.txt");
		if (!input)
			throw runtime_error("Missing executable file");
		else {
			int count = 1;
			string s = "";
			while (getline(input, s)) {
				if (count % 3 == 0) {
					instruction.back().arg = atoi(s.c_str());
					count = 1;
				}
				else if (count % 2 == 0) {
					instruction.back().arg_type = atoi(s.c_str());
					count++;
				}
				else {
					instruction.push_back({ atoi(s.c_str()), 0, 0 });
					count++;
				}
			}
		}
	}
	catch (exception exe)
	{
		cout << exe.what() << endl;
	}
	
	map<string, int> func_table;
	int generate_func_table = (
		func_table["PUSH"] = 17,
		func_table["POP"] = 18,
		func_table["MUL"] = 19,
		func_table["ADD"] = 20,
		func_table["SUB"] = 21,
		func_table["DIV"] = 22,
		func_table["OUT"] = 23, 0);

	map<string, int> type_table;
	int generate_type_table = (
		type_table["func"] = 11,
		type_table["arg"] = 12,
		type_table["label"] = 13,
		type_table["reg"] = 14, 0);

	map<string, int> arg_type_table;
	int generate_arg_type_table = (
		arg_type_table["null"] = 5,
		arg_type_table["num"] = 6,
		arg_type_table["reg"] = 7, 0);

	map<string, int> reg_table;
	int generate_reg_table = (
		reg_table["ax"] = 0,
		reg_table["bx"] = 1,
		reg_table["сx"] = 2,
		reg_table["вx"] = 4, 0);

	map<string, int> label_table;
	int generate_label_table = (
		label_table["JMP"] = 70, 0);

	Stack stack(instruction.size());
	int ax = 0, bx = 0, cx = 0, dx = 0;

	try {
		for (int i = 0; i < instruction.size(); i++) {
			if (instruction[i].command == func_table["PUSH"]) {
				if (instruction[i].arg_type == arg_type_table["num"]) {
					stack.Push(instruction[i].arg);
				}
				else if (instruction[i].arg_type == arg_type_table["reg"]) {
					if (instruction[i].arg == reg_table["ax"]) {
						stack.Push(ax);
					}
					else if (instruction[i].arg == reg_table["bx"]) {
						stack.Push(bx);
					}
					else if (instruction[i].arg == reg_table["cx"]) {
						stack.Push(cx);
					}
					else if (instruction[i].arg == reg_table["dx"]) {
						stack.Push(dx);
					}
				}
			}
			else if (instruction[i].command == func_table["POP"]) {
				if (instruction[i].arg_type == arg_type_table["null"]) {
					stack.Pop();
				}
				else if (instruction[i].arg_type == arg_type_table["reg"]) {
					if (instruction[i].arg == reg_table["ax"]) {
						ax = stack.Pop();
					}
					else if (instruction[i].arg == reg_table["bx"]) {
						bx = stack.Pop();
					}
					else if (instruction[i].arg == reg_table["cx"]) {
						cx = stack.Pop();
					}
					else if (instruction[i].arg == reg_table["dx"]) {
						dx = stack.Pop();
					}
				}
			}
			else if (instruction[i].command == func_table["MUL"]) {
				stack.mul();
			}
			else if (instruction[i].command == func_table["ADD"]) {
				stack.add();
			}
			else if (instruction[i].command == func_table["SUB"]) {
				stack.sub();
			}
			else if (instruction[i].command == func_table["DIV"]) {
				stack.div();
			}
			else if (instruction[i].command == func_table["OUT"]) {
				cout << stack.Pop() << endl;
				return 0;
			}

			else if (instruction[i].command == label_table["JMP"]) {
				int buf = instruction[i].arg - 1;
				instruction.erase(instruction.begin() + i);
				i = buf;
			}
		}
	}
	catch (exception& e) {
		cout << e.what() << endl;
	}
	catch (...) {
		cout << "unknown error" << endl;
	}




	return 0;
}