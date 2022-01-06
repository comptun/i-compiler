#include "lynx.h"

#include "main.h"
#include <random>
#include <ctime>

Lynx lynx;

int main()
{
	std::srand(std::random_device{}());
	lynx.readCode(std::ifstream("main.txt"));

	/*lynx.translate();
	for (size_t i = 0; i < bcfile.file.size(); i += 2) {
		std::cout << i << " " << bcfile.file.at(i) << " " << bcfile.file.at(i + 1) << std::endl;
	}
	std::cin.get();*/
	
	/*lynx.translate();
	for (size_t i = 0; i < lynx.codeFile.token.size(); ++i) {
		std::cout << "\"" << lynx.codeFile.token.at(i) << "\"" << " : " << lynx.codeFile.type.at(i) << std::endl;
	}
	std::cin.get();*/

	lynx.translate();
	for (size_t i = 0; i < lynx.file.size(); ++i) {
		std::cout << lynx.file.at(i) << "\n";
	}

	std::cin.get();
}