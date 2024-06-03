#include <iostream>
#include <fstream>
using namespace std;

int main() {
	ofstream file;
    file.open("aaa.txt", ios::out | ios::app );
    // ofstream file("aaa.txt");
	if( file )
	{
		cout << "if( file ) " << endl;
	}

	if(file.is_open())
	{
		cout << "file.is_open()" << endl;
        file << "hello" << endl;
	}

	file.close();//
    return 0;
}
