#include "Header.h"
#include <fstream>
int main()
{
	try {
		vector<char> file(2);
		try {
			load(file);
		}
		catch (exception & e) {
			cerr << "Load error: " << e.what() << endl;
		}
		vector<string> tokens;
		vector<string> labels;
		try {
			tokenization(file, tokens);
			label(tokens, labels);
		}
		catch (exception & e) {
			cerr << "Token error: " << e.what() << endl;
		}
		//for (int i = 0; i < tokens.size(); i++) cout << tokens[i] << endl;
		vector<convert> converted;
		try {
			convertization(tokens, labels, converted);
		}
		catch (exception & e) {
			cerr << "Convertion error: " << e.what() << endl;
		}
		vector<command> instruction;
		try {
			logic(converted, instruction);
		}
		catch (exception & e) {
			cerr << "Logic error: " << e.what() << endl;
		}
		ofstream output("instruct.txt");
		for (auto i : instruction)
			output << i.command << " " << i.arg_type << " " << i.arg << endl;
		return 0;
	}
	catch (exception & e) {
		cerr << e.what() << endl;
	}
	catch (...) {
		cout << "unknown error" << endl;
	}
}

