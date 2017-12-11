#include "gtest\gtest.h"
#include "IHeap.h"
#include "BinomialHeap.h"
#include "SimpleHeap.h"
#include "LeftistHeap.h"
#include "SkewHeap.h"
#include <iostream>
#include <random>
#include <algorithm>
#include <vector>
#include <string>

struct returning {
	std::vector<std::string> cmd;
	int cnt_heaps;
};
returning generate_test() {
	returning k;
	int size = 20 + rand() % 100;
	k.cmd.push_back("AddHeap 0");
	k.cnt_heaps = 1;
	int val, val1, val2, num;
	std::string cur_cmd;
	while (k.cmd.size() != size) {
		int mode = rand() % 5;
		switch (mode) {
		case 0:
			cur_cmd = "AddHeap ";
			val = rand();
			cur_cmd += std::to_string(val);
			k.cmd.push_back(cur_cmd);
			k.cnt_heaps++;
			break;
		case 1:
			if (k.cnt_heaps <= 1)
				break;
			cur_cmd = "Meld ";
			val1 = rand() % k.cnt_heaps;
			val2 = rand() % k.cnt_heaps;
			while (val1 == val2) {
				val2 = rand() % k.cnt_heaps;
			}
			cur_cmd += std::to_string(val1) + " " + std::to_string(val2);
			k.cmd.push_back(cur_cmd);
			break;
		case 2:
			cur_cmd = "Insert ";
			num = rand() % k.cnt_heaps;
			val = rand();
			cur_cmd += std::to_string(num) + " " + std::to_string(val);
			k.cmd.push_back(cur_cmd);
			break;
		case 3:
			cur_cmd = "GetMin ";
			num = rand() % k.cnt_heaps;
			cur_cmd += std::to_string(num);
			k.cmd.push_back(cur_cmd);
			break;
		case 4:
			cur_cmd = "ExtractMin ";
			num = rand() % k.cnt_heaps;
			cur_cmd += std::to_string(num);
			k.cmd.push_back(cur_cmd);
			break;
		}
	}
	return k;
}
void test_heap(std::vector<std::string> &cmd, std::vector<IHeap*> &testing_heap, std::vector<SimpleHeap> &primal_heap) {
	int cur_heap = 0;
	for (int i = 0; i < cmd.size(); i++) {
		if (cmd[i][0] == 'A') {//AddHeap (7 symb + 1 space)
			std::string num = cmd[i].substr(8);
			int t = atoi(num.c_str());
			testing_heap[cur_heap]->Add(t);
			primal_heap[cur_heap].Add(t);
			cur_heap++;
		}
		if (cmd[i][0] == 'I') {//Insert (6 symb + 1 space)
			std::string index = "";
			int pos = 7;
			while (cmd[i][pos] != ' ') {
				index += cmd[i][pos];
				pos++;
			}
			pos++;
			std::string keystr = cmd[i].substr(pos);
			int ind = atoi(index.c_str());
			if (ind > cur_heap)
				continue;
			int key = atoi(keystr.c_str());
			testing_heap[ind]->Add(key);
			primal_heap[ind].Add(key);
		}
		if (cmd[i][0] == 'G') {//GetMin (6 symb + 1 space)
			std::string index = cmd[i].substr(7);
			int ind = atoi(index.c_str());
			if (ind > cur_heap)
				continue;
			if (primal_heap[ind].empty())
				continue;
			else
				ASSERT_EQ(testing_heap[ind]->GetMin(), primal_heap[ind].GetMin());
		}
		if (cmd[i][0] == 'E') {//ExtractMin (10 symb + 1 space)
			std::string index = cmd[i].substr(11);
			int ind = atoi(index.c_str());
			if (ind > cur_heap)
				continue;
			if (primal_heap[ind].empty())
				continue;
			else
				ASSERT_EQ(testing_heap[ind]->ExtractMin(), primal_heap[ind].ExtractMin());
		}
		if (cmd[i][0] == 'M') {//Meld (4 symb + 1 space)
			std::string first = "";
			int pos = 5;
			while (cmd[i][pos] != ' ') {
				first += cmd[i][pos];
				pos++;
			}
			pos++;
			std::string secondstr = cmd[i].substr(pos);
			int fir = atoi(first.c_str());
			int sec = atoi(secondstr.c_str());
			if (fir > cur_heap || sec > cur_heap)
				continue;
			testing_heap[fir]->Meld(testing_heap[sec]);
			primal_heap[fir].Meld(&primal_heap[sec]);
		}
	}
}

template <class heap_type>
void main_test(returning &k, const int &cnt_tests) {
	for (int i = 0; i < cnt_tests; i++) {
		std::vector <SimpleHeap> primal_heap(k.cnt_heaps);
		std::vector <IHeap*> casted_heap(k.cnt_heaps);
		for (int j = 0; j < k.cnt_heaps; j++) {
			casted_heap[j] = dynamic_cast <IHeap*> (new heap_type());
		}
		test_heap(k.cmd, casted_heap, primal_heap);
	}
}
TEST(testHeap, BHeap) {
	int cnt_tests = 20;
	main_test<BinomialHeap>(generate_test(), cnt_tests);
}
TEST(testHeap, LHeap) {
	int cnt_tests = 20;
	main_test<LeftistHeap>(generate_test(), cnt_tests);
}
TEST(testHeap, SHeap) {
	int cnt_tests = 20;
	main_test<SkewHeap>(generate_test(), cnt_tests);
}
int main(int argc, char *argv[]) {

	srand(time(nullptr));
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}