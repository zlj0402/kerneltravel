#include <array>
#include <iostream>
#include <ctime> 
#include <cstdlib> //qsort, bsearch, NULL
#include <algorithm>	// sort
#include <vector>

#include "Util.h"
#include "UtilClass.h"
#include "TestArray.h"
#include "TestVector.h"
#include "TestList.h"
#include "TestForwardList.h"
#include "TestDeque.h"
#include "TestStack.h"
#include "TestQueue.h"
#include "TestMultiset.h"
#include "TestMultimap.h"
#include "TestUnorderedMultiset.h"
#include "TestUnorderedMultimap.h"
#include "TestSet.h"
#include "TestMap.h"
#include "TestUnorderedSet.h"
#include "TestUnorderedMap.h"

#define TEST(func) (func((long &)ASIZE))

int main() {
	
	jj01::test_array();
	jj02::test_vector((long &)ASIZE);
	jj03::test_list((long &)ASIZE);
	jj04::test_forward_list((long &)ASIZE);
	jj05::test_deque((long &)ASIZE);
	jj06::test_stack((long &)ASIZE);
	jj07::test_queue((long &)ASIZE);
	jj08::test_multiset((long &)ASIZE);
	jj09::test_multimap((long &)ASIZE);
	jj10::test_unordered_multiset((long &)ASIZE);
	jj11::test_unordered_multimap((long &)ASIZE);
	jj12::test_set((long &)ASIZE);
	TEST(jj13::test_map);
	TEST(jj14::test_unordered_set);
	TEST(jj15::test_unordered_map);
}

