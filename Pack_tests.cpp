#include "Pack.hpp"
#include "unit_test_framework.hpp"

#include <iostream>

using namespace std;

TEST(test_pack_default_ctor) {
    Pack pack;
    Card first = pack.deal_one();
    ASSERT_EQUAL(NINE, first.get_rank());
    ASSERT_EQUAL(SPADES, first.get_suit());
}

// Add more tests here

TEST(test_default_pack) {
    Pack pack;
    for (int i = 0; i < 24; ++i) {
        cout << pack.deal_one() << endl;
    }
}

TEST_MAIN()
