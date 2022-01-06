#pragma once

#include "lexer.h"

#include <fstream>
#include <vector>
#include <string>

enum statement {
	IF_STATEMENT,
	WHILE_STATEMENT,
	FUNCTION_STATEMENT,
	ELSE_STATEMENT,
	ELIF_STATEMENT,
	SWITCH_STATEMENT,
	AND_STATEMENT,
	UNKNOWN,
};

class Interpreter : public Lexer {
public:
	std::vector<std::string> file;
	struct InstructionData {
		std::vector<std::string> name;
		std::vector<std::string> data;
	};
	InstructionData idata;
	std::string getType(int instruction);
	std::string getToken(int instruction);
	void push_idata(std::string name, std::string data);
	void pop_idata();
	void masm(std::string instruction);
	void translate();
};