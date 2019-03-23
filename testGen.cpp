/**
	Testcase generator for:
	@author Huynh Hoang Kha
	@version 1.0
*/
#pragma (warning: disable 4996)
#include<iostream>
#include<iomanip>
#include<time.h>
using namespace std;

int main() {
	for (int i = 1; i <= 20; i++) {
		cout << "//=======================" << endl;
		cout << "testcase" << i << ":" << endl;
		//-----------------------------------
		//Section: Generate the testcase
		
		//Endsection: Generate the testcase
		//-----------------------------------
		cout << "expect:" << endl;
		//-----------------------------------
		//Section: Generate the expect
		
		//Endsection: Generate the expect
		//-----------------------------------
		cout << "#end" << endl;
		cout << "//=======================";
		if (i < 20) cout << endl;
	}
	return 0;
}