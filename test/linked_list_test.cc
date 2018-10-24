#include <functional>
#include <time.h>
#include <stdlib.h>
#include <deque>

#include "gtest/gtest.h"
#include "linked_list.h"

void show_q(linked_list_t *q) {
	list_el_t *cur;
	int i = 0;
	for(cur = q->head; i < q->size ; cur = cur->next, i++) {
		printf("[ %10d %s %s ] -> ", cur->value, q->head == cur? "HEAD": "", q->tail == cur? "TAIL": "");
	}
	printf("end\n");
}

#define match_el(ground_truth, q) {									\
	list_el_t *cur = q->head;										\
	for (int i = 0; i < ground_truth.size(); i++, cur=cur->next)	\
		EXPECT_EQ(cur->value, ground_truth.at(i));					\
}

TEST(LinkedListTest, Creation) {
	linked_list_t *q = create();
	EXPECT_EQ(q->size, 0);
	EXPECT_EQ(q->head, nullptr);
	EXPECT_EQ(q->tail, nullptr);
	clean(q);
}

TEST(LinkedListTest, Insert) {
	linked_list_t *q = create();
	insert(q, 0, 100);
	EXPECT_EQ(q->size, 1);
	EXPECT_EQ(q->head, q->tail);
	EXPECT_EQ(q->head->next, nullptr);
	EXPECT_EQ(q->head->value, 100);
	clean(q);
}

TEST(LinkedListTest, Deletion) {
	int node_val;
	EXPECT_FALSE(del(NULL, 0, &node_val));
	EXPECT_FALSE(del(NULL, -5, &node_val));
}

class LinkedListSqTest: public testing::Test {
	protected:
		void SetUp() {
			start_time = time(nullptr);
			q = create();
		}
		void TearDown() {
			//clean(q);
			time_t end_time = time(nullptr);
			EXPECT_TRUE(end_time - start_time <= 10) << "the test spend more than 10 seconds";
		}
		time_t start_time;
		linked_list_t *q;
		std::deque<int> ground_truth;
};

TEST_F(LinkedListSqTest, RandomUnshiftShift) {
	for (int n_op = 0; n_op < 10000; n_op++) {
		int operation = rand() % 2;
		switch (operation) {
			case 0: {
				int val = random();
				bool r =  unshift(q, val);
				EXPECT_TRUE(r);
				EXPECT_EQ(q->tail->next, nullptr);
				EXPECT_EQ(q->head->value, val);
				ground_truth.push_front(val);
				EXPECT_EQ(ground_truth.size(), q->size);
				match_el(ground_truth, q);
				break;
			}
			case 1: {
				int val = 0;
				bool r = shift(q, &val);
				if (ground_truth.size() == 0) {
					EXPECT_FALSE(r);
				}
				else {
					ground_truth.pop_front();
					EXPECT_EQ(ground_truth.size(), q->size);
				}
				break;
			}
		}
	}
}

TEST_F(LinkedListSqTest, RandomPushPop) {
	for (int n_op = 0; n_op < 10000; n_op++) {
		int operation = rand() % 2;
		switch (operation) {
			case 0: {
				int val = random();
				bool r =  push(q, val);
				EXPECT_TRUE(r);
				EXPECT_EQ(q->tail->next, nullptr);
				ground_truth.push_back(val);
				EXPECT_EQ(ground_truth.size(), q->size);
				match_el(ground_truth, q);
				break;
			}
			case 1: {
				int val = 0;
				bool r = pop(q, &val);
				if (ground_truth.size() == 0) {
					EXPECT_FALSE(r);
				}
				else {
					EXPECT_EQ(ground_truth.back(), val);
					ground_truth.pop_back();
					EXPECT_EQ(ground_truth.size(), q->size);
					match_el(ground_truth, q);
				}
				break;
			}
		}
	}
}

TEST_F(LinkedListSqTest, RandomInsertDel) {
	for (int n_op = 0; n_op < 10000; n_op++) {
		int operation = rand() % 2;
		switch (operation) {
			case 0: {
				int val = random() % 100;
				int index = random() % 3;
				if (0 <= index && index <= q->size) {
					// legal input
					bool r = insert(q, index, val);
					EXPECT_TRUE(r);
					ground_truth.insert(ground_truth.begin() + index, val);
					EXPECT_EQ(ground_truth.size(), q->size);
					match_el(ground_truth, q);
				} else {
					bool r = insert(q, index, val);
					EXPECT_FALSE(r);
				}
				break;
			}
			case 1: {
				int val = 0;
				int index = random() % 100;
				bool r = del(q, index, &val);
				if (0 <= index && index < ground_truth.size()) {
					// legal deletion
					EXPECT_TRUE(r);
					EXPECT_EQ(ground_truth.at(index), val);
					ground_truth.erase(ground_truth.begin() + index);
					EXPECT_EQ(ground_truth.size(), q->size);
					match_el(ground_truth, q);
				}
				else {
					EXPECT_FALSE(r);
				}
			}
		}
	}
}

int main(int argc, char** argv) {
	srand(time(NULL));
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
