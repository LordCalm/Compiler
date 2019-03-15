#include<iostream>
#include<vector>
#include<string>
#pragma warning(disable : 4996)
using namespace std;

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

int main()
{
	vector<char> file(2);
	load(file);
	vector<string> tokens;
	tokenization(file, tokens);
	/*
	for (int i = 0; i < file.size(); i++) cout << file[i];
	cout << endl;
	*/
	for (int i = 0; i < tokens.size(); i++) cout << tokens[i] << endl;
	return 0;
}