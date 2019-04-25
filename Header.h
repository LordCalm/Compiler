#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <exception>
#include <set>
#pragma warning(disable : 4996)
using namespace std;


// TOKEN

void load(vector<char>& file)
{
	FILE* text = fopen("text.txt", "r");
	size_t i = 0, n = file.size();
	while (fread(&file[i], sizeof(char), 1, text) != 0) {
		i++;
		if (i == n) {
			n = n * 2;
			file.resize(n);
		}
	}
	file.resize(i);
	file.push_back('\n');
	fclose(text);
}

void tokenization(vector<char>& file, vector<string>& tokens)
{
	string buf = "";
	for (int i = 0; i < file.size(); i++)
	{
		if (file[i] == ';')
			while (file[i] != '\n')
				i++;
		if (file[i] == ' ' || file[i] == '\n' || file[i] == ';') {
			if (buf != "") tokens.push_back(buf);
			buf = "";
		}
		else buf += file[i];
	}
}


void label(vector<string> & tokens, vector<string> & labels)
{
	for (auto i : tokens) {
		if (i.back() == ':') {
			i.resize(i.size() - 1);
			labels.push_back(i);
		}
	}
}


// CONVERT

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
	reg_table["ñx"] = 2,
	reg_table["âx"] = 4, 0);

map<string, int> label_table;
int generate_label_table = (
	label_table["JMP"] = 70, 0);

class convert
{
public:
	int type;
	int cell;
};

bool IsNumber(const string & s) {
	return all_of(s.cbegin(), s.cend(), [](const char& c) {
		return isdigit(c);
		});
}

bool IsRegister(const string & s) {
	return (reg_table.find(s) != reg_table.end()) ? true : false;
}

void corellate_labels(string & i, vector<string> & labels, vector<convert> & converted, vector<string> & l_begin)
{
	string buf = "", type = "";
	int k = 0;
	while (i[k] != ':')
		buf += i[k++];
	type = buf;
	if (label_table.find(type) == label_table.end()) {
		throw logic_error("Unknown label");
	}
	k++;
	buf = "";
	while (k < i.size())
		buf += i[k++];
	l_begin.push_back(buf);
	for (int i = 0; i < labels.size(); i++) {
		if (labels[i] == buf) {
			converted.push_back({ label_table[type], i });
			return;
		}
	}
	throw logic_error("Extra label");
}

void convertization(vector<string> & tokens, vector<string> & labels, vector<convert> & converted)
{
	vector<string> l_begin;
	for (auto i : tokens) {
		if (func_table.find(i) != func_table.end()) {
			converted.push_back({ type_table["func"], func_table[i] });
		}
		else if (IsNumber(i)) {
			converted.push_back({ type_table["arg"], atoi(i.c_str()) });
		}
		else if (IsRegister(i)) {
			converted.push_back({ type_table["reg"], reg_table["ax"] });
		}
		else if (i[0] == 'J') {
			try { corellate_labels(i, labels, converted, l_begin); }
			catch (...) { throw; }
		}
		else if (i.back() == ':') {
			i.resize(i.size() - 1);
			for (int j = 0; j < labels.size(); j++)
				if (labels[j] == i)
					converted.push_back({ type_table["label"], j });
		}
		else throw runtime_error("Unidentified syntax");
	}
	for (auto i : l_begin)
		if (find(labels.begin(), labels.end(), i) == labels.end())
			throw logic_error("Extra label");
	for (auto i : labels)
		if (find(l_begin.begin(), l_begin.end(), i) == l_begin.end())
			throw logic_error("Extra label");
}



// LOGIC

class command {
public:
	int command;
	int arg_type;
	int arg;
};

bool IsLabel(int x) {
	for (auto i : label_table) {
		if (i.second == x)
			return true;
	}
	return false;
}

void logic(vector<convert> & converted, vector<command> & command)
{
	int count = 0;
	map<int, int>  l_begin, l_end;
	vector<convert>::iterator it = converted.begin();
	for (; it < converted.end(); it++) {
		if ((*it).type == type_table["func"]) {
			if ((*it).cell == func_table["PUSH"]) {
				try { it++; }
				catch (...) { throw; }
				if ((*it).type == type_table["arg"]) {
					command.push_back({ (*(it - 1)).cell, arg_type_table["num"], (*it).cell });
					count++;
				}
				else if ((*it).type == type_table["reg"]) {
					command.push_back({ (*(it - 1)).cell, arg_type_table["reg"], (*it).cell });
					count++;
				}
				else
					throw logic_error("No argument for PUSH");

			}
			else if ((*it).cell == func_table["POP"]) {
				try { it++; }
				catch (...) { throw; }
				if ((*it).type == type_table["arg"]) {
					command.push_back({ (*(it - 1)).cell, arg_type_table["num"], (*it).cell });
					count++;
				}
				else if ((*it).type == type_table["reg"]) {
					command.push_back({ (*(it - 1)).cell, arg_type_table["reg"], (*it).cell });
					count++;
				}
				else
					throw logic_error("No argument for POP");
			}
			else if ((*it).cell == func_table["MUL"] ||
				(*it).cell == func_table["ADD"] ||
				(*it).cell == func_table["SUB"] ||
				(*it).cell == func_table["DIV"] ||
				(*it).cell == func_table["OUT"]) {
				command.push_back({ (*it).cell, arg_type_table["null"], 0 });
				count++;
			}
			else
				throw logic_error("Unknown function");
		}
		else if ((*it).type == type_table["label"]) {
			l_end[(*it).cell] = count + 2;
		}
		else if (IsLabel((*it).type)) {
			if (l_end.find((*it).cell) != l_end.end()) {
				command.push_back({ (*it).type, arg_type_table["num"], l_end[(*it).cell] });
				count++;
			}
			else {
				command.push_back({ (*it).type, arg_type_table["num"], 0 });
				l_begin[(*it).cell] = count;
			}

		}
		else
			throw logic_error("Extra argument");
		for (auto i : l_begin) {
			command[i.second].arg = l_end[i.first];
		}
	}
}

