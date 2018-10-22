#include <functional>
#include <time.h>
#include <stdlib.h>
#include <deque>

#include "gtest/gtest.h"
#include "linked_list.h"


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
			clean(q);
			time_t end_time = time(nullptr);
			EXPECT_TRUE(end_time - start_time <= 10) << "the test spend more than 10 seconds";
		}
		time_t start_time;
		linked_list_t *q;
		std::deque<int> ground_truth;
};

TEST_F(LinkedListSqTest, RandomUnshiftShift) {
	int ops[] = {0, 0, 1, 1};
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

int main(int argc, char** argv) {
	srand(time(NULL));
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
