#include "interpreter.h"

void Interpreter::masm(std::string instruction)
{
	//std::cout << bcfile.file.size() << " " << instruction << " " << param << std::endl;
	file.push_back(instruction);
}

void Interpreter::push_idata(std::string name, std::string data)
{
	idata.name.push_back(name);
	idata.data.push_back(data);
}

void Interpreter::pop_idata()
{
	idata.name.pop_back();
	idata.data.pop_back();
}

std::string Interpreter::getType(int instruction) 
{
	return codeFile.type.at(instruction);
}

std::string Interpreter::getToken(int instruction) 
{
	return codeFile.token.at(instruction);
}

void Interpreter::translate()
{
	push_idata("EMPTY", "EMPTY");
	masm("include \\masm32\\include64\\masm64rt.inc");
	masm(".code");

	for (size_t instruction = 0; instruction < codeFile.token.size();) {

		// ARITHEMATIC //

		if (getType(instruction) == "PLUS") {
			if (getType(instruction + 1) == "CONSTANT_VALUE") {
				masm("	add rax, " + getToken(instruction + 1));
				instruction += 2;
			}
		}

		else if (getType(instruction) == "MINUS") {
			if (getType(instruction + 1) == "CONSTANT_VALUE") {
				masm("	sub rax, " + getToken(instruction + 1));
				instruction += 2;
			}
		}

		else if (getType(instruction) == "MULTIPLY") {
			if (getType(instruction + 1) == "CONSTANT_VALUE") {
				masm("	mov rdx," + getToken(instruction + 1));
				masm("	mul rdx, " + getToken(instruction + 1));
				instruction += 2;
			}
		}

		else if (getType(instruction) == "DIVIDE") {
			if (getType(instruction + 1) == "CONSTANT_VALUE") {
				masm("	mov rdx," + getToken(instruction + 1));
				masm("	div rdx, " + getToken(instruction + 1));
				instruction += 2;
			}
		}

		// ----------- //


		// FUNCTIONS //

		else if (getType(instruction) == "FUNCTION" &&
			getType(instruction + 1) == "NAME" &&
			getType(instruction + 2) == "LBRACKET") {
			masm(getToken(instruction + 1) + " PROC ");
			push_idata("FUNCTION_DEF", getToken(instruction + 1));
			push_idata("FUNCTION_DEF_PARAM", "EMPTY");
			instruction += 3;
		}

		else if (getType(instruction) == "RBRACKET" &&
				 getType(instruction + 1) == "LEFT_CURLY_BRACE") {
			pop_idata();
			instruction += 2;
		}

		else if (getType(instruction) == "RIGHT_CURLY_BRACE") {
			if (idata.name.back() == "FUNCTION_DEF") {
				masm("	mov rax, 0");
				masm("	ret");
				masm(idata.data.back() + " ENDP");
				pop_idata();
			}
			instruction += 1;
		}

		else if (getType(instruction) == "RETURN") {
			push_idata("RETURN", "EMPTY");
			instruction += 1;
		}

		// -----------------------------//

		else if (getType(instruction) == "SEMI_COLON") {
			if (idata.name.back() == "RETURN") {
				masm("	ret");
			}
			else if (idata.name.back() == "NAME") {
				masm("	mov " + idata.data.back() + ", rax");
			}
			pop_idata();
			instruction += 1;
		}

		else if (getType(instruction) == "CONSTANT_VALUE") {
			if (idata.name.back() == "RETURN") {
				masm("	mov rax, " + getToken(instruction));
			}
			else if (idata.name.back() == "NAME") {
				masm("	mov rax, " + getToken(instruction));
			}
			instruction += 1;
		}

		else if (getType(instruction) == "NAME") {
			if (idata.name.back() == "RETURN") {
				masm("	mov rax, " + getToken(instruction));
				instruction += 1;
			}
			else if (getType(instruction + 1) == "EQUALS") {
				push_idata("NAME", getToken(instruction));
				instruction += 2;
			}
		}

		else if (getType(instruction) == "INTEGER") {
			if (idata.name.back() == "FUNCTION_DEF_PARAM") {
				file.back() += getToken(instruction + 1) + ":QWORD";
				instruction += 2;
			}
			if (getType(instruction + 1) == "NAME") {
				masm("	LOCAL " + getToken(instruction + 1) + ":QWORD");
				if (codeFile.token.at(instruction + 2) == "=") {
					push_idata("NAME", getToken(instruction + 1));
					instruction += 3;
				}
				else {
					push_idata("EMPTY", "EMPTY");
					instruction += 2;
				}
			}
		}

		else if (getType(instruction) == "COMMA") {
			file.back() += ", ";
			instruction += 1;
		}

	}
	masm("invoke ExitProcess,0");
	masm("end");
}