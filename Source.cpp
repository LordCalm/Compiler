#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <exception>
#pragma warning(disable : 4996)
using namespace std;


// TOKEN

void load(vector<char> &file)
{
	FILE * text = fopen("text.txt", "r");
	size_t i = 0, n = file.size();
	while (fread(&file[i], sizeof(char), 1, text) != 0) {
		i++;
		if (i == n) {
			n = n * 2;
			file.resize(n);
		}
	}
	file.resize(i);
	fclose(text);
}

void tokenization(vector<char> &file, vector<string> &tokens)
{
	string buf = "";
	int ibuf = 0;
	for (int i = 0; i < file.size(); i++)
	{
		if (file[i] == ' ' || file[i] == '\n') {
			tokens.push_back(buf);
			i++;
			if (buf == "PUSH") {
				while (file[i] != '\n') {
					ibuf += (int)file[i];
					i++;
				}
				buf = (char)ibuf;
				tokens.push_back(buf);
			}
			buf = "";
			ibuf = 0;
		}
		else buf += file[i];
	}
}


//CONVERT

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
	type_table["arg"] = 12, 0);
	
map<string, int> arg_type_table;
int generate_arg_type_table = (
	arg_type_table["null"] = 0,
	arg_type_table["num"] = 1,
	arg_type_table["reg"] = 2, 0);

class convert
{
public:
	int type;
	int cell;
	int arg_type;
};

bool IsNumber(const string &s) {
	return all_of(s.cbegin(), s.cend(), [](const char &c) {
		return isdigit(c);
	});
}

void convertization(vector<string> &tokens, vector<convert> &convert)
{
	for (auto i : tokens) {
		if (func_table.find(i) != func_table.end()) {
			convert.push_back({ type_table["func"], func_table[i], arg_type_table["null"] });
		}
		else if (IsNumber(i)) {
			convert.push_back({ type_table["arg"], atoi(i.c_str()), arg_type_table["num"] });
		}
		else throw runtime_error("Unidentified syntax");
	}
}



int main()
{
	try {
		vector<char> file(2);
		load(file);
		vector<string> tokens;
		tokenization(file, tokens);
		vector<convert> convert;
		convertization(tokens, convert);
		return 0;
	}
	catch (exception &e) {
		cerr << e.what() << endl;
	}
	catch (...) {
		cout << "unknown error" << endl;
	}
	/*
	for (int i = 0; i < file.size(); i++) cout << file[i];
	cout << endl;
	*/
	//for (int i = 0; i < tokens.size(); i++) cout << tokens[i] << endl;
	
}