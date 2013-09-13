//============================================================================
// Name        : imFilt.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "readWriteFile.h"

using namespace std;


int main(int argc, char** argv) {

	ReadWriteFile *w = new ReadWriteFile(argv[0], argv[1], argv[2]);

	return 0;
}
