#include "Card.hpp"
#include "unit_test_framework.hpp"
#include <iostream>

using namespace std;


TEST(test_card_ctor) {
    Card c(ACE, HEARTS);
    ASSERT_EQUAL(ACE, c.get_rank());
    ASSERT_EQUAL(HEARTS, c.get_suit());
}
TEST(test_card_get_suit_with_trump) {
    Card c(JACK, SPADES);
    Card c2(JACK, CLUBS);
    ASSERT_EQUAL(SPADES, c.get_suit(SPADES));
    ASSERT_EQUAL(SPADES, c2.get_suit(SPADES));
}

// Add more test cases here
TEST(test_card_default_ctor) {
    Card c;
    ASSERT_EQUAL(TWO, c.get_rank());
    ASSERT_EQUAL(SPADES, c.get_suit());   
}

TEST(test_right_bower) {
    Card c(JACK, SPADES);
    ASSERT_TRUE(c.is_right_bower(SPADES));
}

TEST(test_left_bower) {
    Card c(JACK, CLUBS);
    ASSERT_TRUE(c.is_left_bower(SPADES));
}

TEST(test_is_face_or_ace) {
    Card c1(QUEEN,HEARTS);
    Card c2(ACE,SPADES);
    Card c3(THREE, CLUBS);
    ASSERT_TRUE(c1.is_face_or_ace());
    ASSERT_TRUE(c2.is_face_or_ace());
    ASSERT_FALSE(c3.is_face_or_ace());    
}

TEST(test_is_trump) {
    Card c1(JACK, DIAMONDS);
    Card c2(TEN, CLUBS);
    ASSERT_TRUE(c1.is_trump(HEARTS));
    ASSERT_TRUE(c2.is_trump(CLUBS));
    ASSERT_FALSE(c2.is_trump(DIAMONDS));
}

TEST(test_ostream_op) {
    Card c(QUEEN, HEARTS);
    string card;
    ostringstream os(card);
    os << c;
    card = os.str();
    ASSERT_EQUAL(card, "Queen of Hearts");
}
TEST(test_istream_op) {
    Card c;
    
    string card = "Three of Clubs";
    istringstream is(card);
    is >> c;
    
    ASSERT_TRUE(c.get_rank() == THREE);
    ASSERT_TRUE(c.get_suit() == CLUBS);
}

TEST(test_card_inequalties) {
    Card c1(TEN, SPADES);
    Card c2(NINE, CLUBS);
    Card c3(KING, HEARTS);
    Card c4(KING, HEARTS);
    ASSERT_TRUE(c2 < c1);
    ASSERT_TRUE(c3 == c4);
    ASSERT_TRUE(c2 != c4);
    ASSERT_TRUE(c1 <= c4 && c3 <= c4);
    ASSERT_TRUE(c1 > c2);
    ASSERT_TRUE(c1 >= c2 && c3 >= c4);
}

TEST(test_suit_next) {
    Card c1(ACE, DIAMONDS);
    Card c2(QUEEN, CLUBS);
    ASSERT_TRUE(Suit_next(c1.get_suit()) == HEARTS);
    ASSERT_TRUE(Suit_next(c2.get_suit()) == SPADES);
}

TEST(test_card_less) {
    Card c1(ACE, DIAMONDS);
    Card c2(QUEEN, CLUBS);
    Card c3(JACK, HEARTS);
    Card c4(NINE, SPADES);
    Card c5(NINE, DIAMONDS);
    ASSERT_TRUE(Card_less(c1, c2, CLUBS));
    ASSERT_TRUE(Card_less(c2, c1, HEARTS));
    ASSERT_TRUE(Card_less(c2, c3, DIAMONDS));
    ASSERT_TRUE(Card_less(c1, c3, DIAMONDS));
    ASSERT_TRUE(Card_less(c3, c4, SPADES));
    ASSERT_TRUE(Card_less(c1, c3, HEARTS));
    ASSERT_FALSE(Card_less(c4, c5, HEARTS));
}

TEST(test_card_less_led) {
    Card c1(ACE, DIAMONDS);
    Card c2(QUEEN, CLUBS);
    Card c3(JACK, HEARTS);
    Card c4(NINE, SPADES);

    Card c5(THREE, SPADES);

    Card led_card1(NINE, CLUBS);
    Card led_card2(NINE, SPADES);
    
    ASSERT_TRUE(Card_less(c1, c2, led_card1, HEARTS));
    ASSERT_FALSE(Card_less(c2, c1, led_card1, HEARTS));
    ASSERT_TRUE(Card_less(c2, c3, led_card1, HEARTS));
    ASSERT_FALSE(Card_less(c2, c3, led_card1, CLUBS));
    ASSERT_TRUE(Card_less(c5, c4, led_card2, SPADES)); 
    ASSERT_FALSE(Card_less(Card(ACE,DIAMONDS),Card(ACE,DIAMONDS), led_card2, SPADES));
    ASSERT_TRUE(Card_less(Card(JACK,CLUBS),Card(JACK,SPADES), led_card2, HEARTS));
}

TEST_MAIN()
