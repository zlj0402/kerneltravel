#ifndef TESTUNORDEREDSET_H
#define TESTUNORDEREDSET_H

#include <unordered_set>
//#include <stdexcept>
#include <string>
#include <cstdlib> //abort()
#include <cstdio>  //snprintf()
#include <iostream>
#include <ctime> 
#include "Util.h"

using std::cout;
using std::endl;
using std::unordered_set;
using std::string;
using std::exception;

namespace jj14
{
	void test_unordered_set(long& value)
	{
		cout << "\ntest_unordered_set().......... \n";

		unordered_set<string> c;
		char buf[10];

		clock_t timeStart = clock();
		for (long i = 0; i < value; ++i)
		{
			try {
				snprintf(buf, 10, "%d", rand());
				c.insert(string(buf));
			}
			catch (exception& p) {
				cout << "i=" << i << " " << p.what() << endl;
				abort();
			}
		}
		cout << "milli-seconds : " << (clock() - timeStart) << endl;
		cout << "unordered_set.size()= " << c.size() << endl;
		cout << "unordered_set.max_size()= " << c.max_size() << endl;  //357913941
		cout << "unordered_set.bucket_count()= " << c.bucket_count() << endl;
		cout << "unordered_set.load_factor()= " << c.load_factor() << endl;
		cout << "unordered_set.max_load_factor()= " << c.max_load_factor() << endl;
		cout << "unordered_set.max_bucket_count()= " << c.max_bucket_count() << endl;
		for (unsigned i = 0; i < 20; ++i) {
			cout << "bucket #" << i << " has " << c.bucket_size(i) << " elements.\n";
		}

		string target = get_a_target_string();
		{
			timeStart = clock();
			auto pItem = find(c.begin(), c.end(), target);	//比 c.find(...) 慢很多	
			cout << "std::find(), milli-seconds : " << (clock() - timeStart) << endl;
			if (pItem != c.end())
				cout << "found, " << *pItem << endl;
			else
				cout << "not found! " << endl;
		}

		{
			timeStart = clock();
			auto pItem = c.find(target);		//比 std::find(...) 快很多							
			cout << "c.find(), milli-seconds : " << (clock() - timeStart) << endl;
			if (pItem != c.end())
				cout << "found, " << *pItem << endl;
			else
				cout << "not found! " << endl;
		}
	}
}

#endif	//TESTUNORDEREDSET_H
