#ifndef __GL_EXAMPLE_UTILITY_CPP__
#define __GL_EXAMPLE_UTILITY_CPP__

#include <iostream>
#include <fstream>

using namespace std;

string getShaderSource(const string path)
{
	cout << "shader path : " << path << endl;
	// string contents;
	// read File
	// std::ifstream fs(path, std::ios::in | std::ios::ate);

	std::ifstream sourceFile(path, std::fstream::in);
	std::string sourceCode;
	if (sourceFile.is_open())
		sourceCode = std::string(std::istreambuf_iterator<char>(sourceFile), 
								std::istreambuf_iterator<char>());
	return sourceCode;
}

#endif

