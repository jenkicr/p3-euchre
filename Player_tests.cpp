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
    // Original test
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
    ASSERT_FALSE(billy->make_trump(upcard2, true, 1, order_up_suit));
    ASSERT_TRUE(billy->make_trump(upcard2, false, 2, order_up_suit));
    ASSERT_TRUE(order_up_suit == SPADES);
    ASSERT_TRUE(billy->make_trump(upcard1, true, 2, order_up_suit));
    ASSERT_TRUE(order_up_suit == DIAMONDS);
    delete billy;

    // Test case for exactly 2 face cards of trump suit in round 1
    billy = Player_factory("Billy", "Simple");
    billy->add_card(Card(JACK, HEARTS));
    billy->add_card(Card(QUEEN, HEARTS));
    billy->add_card(Card(NINE, SPADES));
    billy->add_card(Card(TEN, DIAMONDS));
    billy->add_card(Card(NINE, CLUBS));
    
    Card upcard3(NINE, HEARTS);
    ASSERT_TRUE(billy->make_trump(upcard3, false, 1, order_up_suit));
    ASSERT_EQUAL(order_up_suit, HEARTS);
    delete billy;

    // Test case for right bower counting as trump suit in round 1
    billy = Player_factory("Billy", "Simple");
    billy->add_card(Card(JACK, HEARTS));  // Right bower
    billy->add_card(Card(JACK, DIAMONDS)); // Left bower
    billy->add_card(Card(NINE, SPADES));
    billy->add_card(Card(TEN, CLUBS));
    billy->add_card(Card(NINE, CLUBS));
    
    Card upcard4(NINE, HEARTS);
    ASSERT_TRUE(billy->make_trump(upcard4, false, 1, order_up_suit));
    ASSERT_EQUAL(order_up_suit, HEARTS);
    delete billy;

    // Test case for round 2 with exactly 1 face card of next suit
    billy = Player_factory("Billy", "Simple");
    billy->add_card(Card(KING, DIAMONDS)); // Next suit face card
    billy->add_card(Card(NINE, HEARTS));
    billy->add_card(Card(TEN, SPADES));
    billy->add_card(Card(NINE, CLUBS));
    billy->add_card(Card(TEN, CLUBS));
    
    Card upcard5(NINE, HEARTS);
    ASSERT_TRUE(billy->make_trump(upcard5, false, 2, order_up_suit));
    ASSERT_EQUAL(order_up_suit, DIAMONDS);
    delete billy;

    // Test case for round 2 with no face cards but is dealer
    billy = Player_factory("Billy", "Simple");
    billy->add_card(Card(NINE, DIAMONDS));
    billy->add_card(Card(TEN, HEARTS));
    billy->add_card(Card(NINE, SPADES));
    billy->add_card(Card(TEN, CLUBS));
    billy->add_card(Card(NINE, CLUBS));
    
    Card upcard6(NINE, HEARTS);
    ASSERT_TRUE(billy->make_trump(upcard6, true, 2, order_up_suit));
    ASSERT_EQUAL(order_up_suit, DIAMONDS);
    delete billy;

    // Test case for testing if left bower is seen as trump in r2
    billy = Player_factory("Billy", "Simple");
    billy->add_card(Card(NINE, DIAMONDS));
    billy->add_card(Card(TEN, HEARTS));
    billy->add_card(Card(NINE, SPADES));
    billy->add_card(Card(TEN, CLUBS));
    billy->add_card(Card(JACK, DIAMONDS));

    Card upcard7(KING, DIAMONDS);
    ASSERT_TRUE(billy->make_trump(upcard7, false, 2, order_up_suit));
    ASSERT_EQUAL(order_up_suit, HEARTS);

    // Test case for testing if left bower is seen as trump in r1
    billy = Player_factory("Billy", "Simple");
    billy->add_card(Card(QUEEN, DIAMONDS));
    billy->add_card(Card(TEN, HEARTS));
    billy->add_card(Card(NINE, SPADES));
    billy->add_card(Card(TEN, CLUBS));
    billy->add_card(Card(JACK, HEARTS));

    ASSERT_TRUE(billy->make_trump(upcard7, false, 1, order_up_suit));
    ASSERT_EQUAL(order_up_suit, DIAMONDS);
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
    // Test 1: Basic discard functionality
    Player * billy = Player_factory("Billy", "Simple");
    Card led_card(TEN, CLUBS);
    Card upcard(KING, SPADES);

    billy->add_card(Card(NINE, HEARTS));
    billy->add_card(Card(TEN, DIAMONDS));
    billy->add_card(Card(JACK, SPADES));
    billy->add_card(Card(ACE, DIAMONDS));
    billy->add_card(Card(QUEEN, HEARTS));

    billy->add_and_discard(upcard);

    ASSERT_TRUE((billy->play_card(led_card, SPADES)) == (Card(TEN, DIAMONDS)));
    ASSERT_TRUE((billy->play_card(led_card, SPADES)) == (Card(JACK, SPADES)));
    ASSERT_TRUE((billy->play_card(led_card, SPADES)) == (Card(QUEEN, HEARTS)));
    ASSERT_TRUE((billy->play_card(led_card, SPADES)) == (Card(KING, SPADES)));
    ASSERT_TRUE((billy->play_card(led_card, SPADES)) == (Card(ACE, DIAMONDS)));
    
    delete billy;

    // Test 2: Upcard is higher than lowest trump
    billy = Player_factory("Billy", "Simple");
    Card upcard2(ACE, HEARTS);
    
    billy->add_card(Card(NINE, HEARTS));  // Queen of Clubs should be discarded
    billy->add_card(Card(TEN, HEARTS));
    billy->add_card(Card(JACK, DIAMONDS));
    billy->add_card(Card(QUEEN, CLUBS));
    billy->add_card(Card(KING, SPADES));

    billy->add_and_discard(upcard2);
    
    Card played = billy->play_card(led_card, HEARTS);
    ASSERT_TRUE(played == Card(NINE, HEARTS));  // Queen of Clubs should be discarded  
    delete billy;

    // Test 3: Upcard is lowest trump
    billy = Player_factory("Billy", "Simple");
    Card upcard3(NINE, DIAMONDS);
    
    billy->add_card(Card(JACK, DIAMONDS));
    billy->add_card(Card(QUEEN, DIAMONDS));
    billy->add_card(Card(KING, HEARTS));
    billy->add_card(Card(ACE, CLUBS));
    billy->add_card(Card(TEN, SPADES));

    billy->add_and_discard(upcard3); //Ten of Spades should be discarded
    
    ASSERT_TRUE((billy->play_card(led_card, DIAMONDS)) == (Card(ACE, CLUBS)));    
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
