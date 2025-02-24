#include "Player.hpp"
#include "unit_test_framework.hpp"
#include <iostream>

using namespace std;

//Simple Player Tests
TEST(test_player_get_name) {
    Player * alice = Player_factory("Alice", "Simple");
    ASSERT_EQUAL("Alice", alice->get_name());
    delete alice;
}

TEST(test_simple_make_trump) {
    Player * billy = Player_factory("Billy", "Simple");
    Card c(TEN, DIAMONDS);
    billy->add_card(c);
    billy->add_card(Card(TEN, HEARTS));
    billy->add_card(Card(JACK, SPADES));
    billy->add_card(Card(ACE, DIAMONDS));
    billy->add_card(Card(QUEEN, HEARTS));
    Card upcard1(NINE, HEARTS);
    Card upcard2(JACK, CLUBS);
    Suit order_up_suit;
    ASSERT_FALSE(billy->make_trump(upcard1, false, 1, order_up_suit));
    ASSERT_FALSE(billy->make_trump(upcard2, false, 1, order_up_suit));
    ASSERT_TRUE(billy->make_trump(upcard2, false, 2, order_up_suit));
    ASSERT_TRUE(order_up_suit == SPADES);
    ASSERT_TRUE(billy->make_trump(upcard1, true, 2, order_up_suit));
    ASSERT_TRUE(order_up_suit == DIAMONDS);
    delete billy;
}

TEST(test_simple_make_trump_round_1_success) {
    Player * billy = Player_factory("Billy", "Simple");
    billy->add_card(Card(JACK, HEARTS));
    billy->add_card(Card(QUEEN, HEARTS));
    billy->add_card(Card(KING, SPADES));
    billy->add_card(Card(NINE, DIAMONDS));
    billy->add_card(Card(TEN, CLUBS));
    
    Card upcard(ACE, HEARTS);
    Suit order_up_suit;
    
    ASSERT_TRUE(billy->make_trump(upcard, false, 1, order_up_suit));
    ASSERT_EQUAL(order_up_suit, HEARTS);
    delete billy;
}

TEST(test_simple_play_card) {
    Player * billy = Player_factory("Billy", "Simple");
    Card c(TEN, DIAMONDS);
    Card led_card(QUEEN, DIAMONDS);
    billy->add_card(c);
    billy->add_card(Card(TEN, HEARTS));
    billy->add_card(Card(JACK, SPADES));
    billy->add_card(Card(ACE, DIAMONDS));
    billy->add_card(Card(QUEEN, HEARTS));
    Card card1 = billy->play_card(led_card, SPADES);
    ASSERT_TRUE(card1 == Card(ACE, DIAMONDS));
    delete billy;
}

TEST(test_simple_add_discard) {
    Player * billy = Player_factory("Billy", "Simple");
    Card led_card(TEN, CLUBS);
    Card upcard(KING, SPADES);
    billy->add_card(Card(TEN, DIAMONDS));
    billy->add_card(Card(NINE, HEARTS));
    billy->add_card(Card(JACK, SPADES));
    billy->add_card(Card(ACE, DIAMONDS));
    billy->add_card(Card(QUEEN, HEARTS));
    billy->add_and_discard(upcard);
    ASSERT_TRUE((billy->play_card(led_card, DIAMONDS)) == (Card(TEN, DIAMONDS)));
    ASSERT_TRUE((billy->play_card(led_card, DIAMONDS)) == (Card(JACK, SPADES)));
    ASSERT_TRUE((billy->play_card(led_card, DIAMONDS)) == (Card(QUEEN, HEARTS)));
    delete billy;
}

TEST(test_simple_lead_card) {
    Player * billy = Player_factory("Billy", "Simple");
    Card c(TEN, DIAMONDS);
    billy->add_card(c);
    billy->add_card(Card(TEN, HEARTS));
    billy->add_card(Card(JACK, SPADES));
    billy->add_card(Card(ACE, DIAMONDS));
    billy->add_card(Card(QUEEN, HEARTS));
    Card card1 = billy->lead_card(DIAMONDS);
    ASSERT_TRUE(card1 == Card(QUEEN, HEARTS));
    delete billy;
}
TEST_MAIN()
