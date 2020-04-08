#include <string>
#include <iostream>
#include "JSONRoot.h"
#include "util.h"
int main() {
	std::string jsonPath = "init_arguments.json";
	JSONRoot root(jsonPath);
	root.generateMDDoc();
	std::ofstream ofs;
	ofs.open("init_arguments.md");
	ofs << root << std::endl;
	ofs.clear();
	ofs.close();
	return 0;
}