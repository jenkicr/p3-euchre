#include "Player.hpp"
#include "unit_test_framework.hpp"

#include <iostream>

using namespace std;

TEST(test_player_get_name) {
    Player * alice = Player_factory("Alice", "Simple");
    ASSERT_EQUAL("Alice", alice->get_name());

    delete alice;
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

TEST(test_human_play_card) {
    Player * charlie = Player_factory("Charlie", "Human");
    Card c1(TEN, DIAMONDS);
    Card c2(JACK, HEARTS);
    Card c3(QUEEN, SPADES);
    charlie->add_card(c1);
    charlie->add_card(c2);
    charlie->add_card(c3);
    
    Card led_card(ACE, DIAMONDS);
    Card played = charlie->play_card(led_card, SPADES);
    // human inserts 0
    ASSERT_EQUAL(played, c1);
    
    played = charlie->play_card(led_card, SPADES);
    // human inserts 0
    ASSERT_EQUAL(played, c2);
    
    delete charlie;
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

TEST(test_human_make_trump) {
    Player * charlie = Player_factory("Charlie", "Human");
    Card upcard1(NINE, HEARTS);
    Suit order_up_suit;  // Remove initialization to SPADES
    ASSERT_FALSE(charlie->make_trump(upcard1, false, 1, order_up_suit));
    // human inserts pass
    ASSERT_TRUE(charlie->make_trump(upcard1, true, 1, order_up_suit));
    // human inserts Diamonds
    ASSERT_EQUAL(order_up_suit, DIAMONDS);
    delete charlie;
}

TEST(test_simple_add_discard) {
    Player * billy = Player_factory("Billy", "Simple");
    Card c(TEN, DIAMONDS);
    Card led_card(TEN, CLUBS);
    billy->add_card(c);
    billy->add_card(Card(NINE, HEARTS));
    billy->add_card(Card(JACK, SPADES));
    billy->add_card(Card(ACE, DIAMONDS));
    billy->add_card(Card(QUEEN, HEARTS));
    Card upcard(KING, SPADES);
    billy->add_and_discard(upcard);
    ASSERT_TRUE((billy->play_card(led_card, DIAMONDS)) == (Card(TEN, DIAMONDS)));
    ASSERT_TRUE((billy->play_card(led_card, DIAMONDS)) == (Card(JACK, SPADES)));
    ASSERT_TRUE((billy->play_card(led_card, DIAMONDS)) == (Card(QUEEN, HEARTS)));

    delete billy;
}

TEST(test_human_add_discard) {
    Player * charlie = Player_factory("Charlie", "Human");
    Card c1(TEN, DIAMONDS);
    Card c2(JACK, SPADES);
    Card upcard(KING, CLUBS);
    
    charlie->add_card(c1);
    charlie->add_card(c2);
    charlie->add_and_discard(upcard);
    // human inserts -1
    
    // Verify the upcard was properly discarded by checking remaining cards
    Card led_card(ACE, CLUBS);
    Card played1 = charlie->play_card(led_card, SPADES);
    // human inserts 0
    Card played2 = charlie->play_card(led_card, SPADES);
    // human inserts 0
    
    ASSERT_TRUE(played1 == c1 || played1 == c2);
    ASSERT_TRUE(played2 == c1 || played2 == c2);
    ASSERT_FALSE(played1 == played2);
    
    delete charlie;
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

TEST(test_human_lead_card) {
    Player * charlie = Player_factory("Charlie", "Human");
    Card c(TEN, DIAMONDS);
    charlie->add_card(c);
    Card card = charlie->lead_card(DIAMONDS);
    // human inserts 0
    ASSERT_TRUE(c == card);
    
    delete charlie;
}

// Add more tests here

TEST_MAIN()
