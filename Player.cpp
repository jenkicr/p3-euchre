#include "Player.hpp"
#include <cassert>
#include <algorithm>


using namespace std;

class SimplePlayer : public Player {
private:
    string name;
    vector<Card> hand;
public: 
    SimplePlayer (const string &name_in) : name(name_in) {}
    
    const std::string & get_name() const {
        return name;
    }
    
    void add_card(const Card &c) {
        assert(hand.size() < MAX_HAND_SIZE);
        hand.push_back(c);
        sort(hand.begin(), hand.end());
    }

    bool make_trump(const Card &upcard, bool is_dealer, int round, Suit &order_up_suit) const {
        assert(round == 1 || round == 2);
        if (round == 1) {
            int count = 0;
            for (int i = 0; i < hand.size(); ++i) {
                if (hand[i].is_face_or_ace() && hand[i].is_trump(upcard.get_suit())) {
                    count++;
                }
            }
            if (count >= 2) {
                order_up_suit = upcard.get_suit();
                return true;
            }
            else {
                return false;
            }
        }
        else if (round == 2) {
            if (is_dealer) {
                order_up_suit = Suit_next(upcard.get_suit());
                return true;
            }
            else {
                int count = 0;
                for (int i = 0; i < hand.size(); ++i) {
                    if (hand[i].is_face_or_ace() 
                    && hand[i].is_trump(Suit_next(upcard.get_suit()))) {
                        count++;
                    }
                }
                if (count >= 1) {
                    order_up_suit = Suit_next(upcard.get_suit());
                    return true;
                }
                else {
                    return false;
                }
            }
        }
        return false;
    }

    Card play_card(const Card &led_card, Suit trump) {
        assert(hand.size() >= 1);
        int count_same_suit = 0;
        for (int i = 0; i < hand.size(); i++) {
            if (hand[i].get_suit(trump) == led_card.get_suit(trump)) {
                count_same_suit++;
            }
        }
        int index = 0;
        if (count_same_suit > 0) {
            Card highest_card;
            for (int i = 0; i < hand.size(); i++) {
                if (hand[i].get_suit(trump) == led_card.get_suit(trump)) {
                    if (hand[i].is_right_bower(trump)) {
                        highest_card = hand[i];
                        index = i;
                    }
                    else if (hand[i].is_left_bower(trump) && !highest_card.is_right_bower(trump)) {
                        highest_card = hand[i];
                        index = i;
                    }
                    else if (hand[i] > highest_card && !highest_card.is_left_bower(trump) && 
                    !highest_card.is_right_bower(trump)) {
                        highest_card = hand[i];
                        index = i;
                    }
                }
            }
            hand.erase(hand.begin() + index);
            return highest_card;
        }
        else {
            Card lowest_card = hand[0];
            int index = 0;
            for (int i = 0; i < hand.size()-1; i++) {
                if (Card_less(hand[i+1], lowest_card, trump)) {
                    lowest_card = hand[i+1];
                    index = i + 1;
                }
            }
            hand.erase(hand.begin() + index);
            return lowest_card;
        }
        
    }

    void add_and_discard(const Card &upcard) {
        assert(hand.size() > 1);
        hand.push_back(upcard); // adds upcard to hand
        sort(hand.begin(), hand.end());
      
        Card min = hand[0];
        int index = 0;
        for (int i = 1; i < hand.size(); ++i) {
            if (Card_less(hand[i], min, upcard.get_suit())) {
                min = hand[i];
                index = i;
            }
        }
        hand.erase(hand.begin() + index); // discards lowest value card
    }

    Card lead_card(Suit trump) {
        assert(hand.size() >= 1);
        
        sort(hand.begin(), hand.end());
        for (int i = hand.size() - 1; i >= 0; i--) {
            if (!(hand[i].is_trump(trump))) {
                Card led_card = hand[i];
                hand.erase(hand.begin() + i);
                return led_card;
            }
        }
        Card highest_card = hand[0];
        int index = 0;
        for (int i = 0; i < hand.size() - 1; ++i) {
            if (Card_less(highest_card, hand[i + 1], trump)) {
                highest_card = hand[i+1];
                index = i + 1;
            }
        }
        hand.erase(hand.begin() + index);
        return highest_card;
    }
};

class HumanPlayer : public Player {
    private:
        string name;
        vector<Card> hand;

        void print_hand() const {
            for (size_t i=0; i < hand.size(); ++i) {
              cout << "Human player " << name << "'s hand: "
                   << "[" << i << "] " << hand[i] << "\n";
            }
        }
    public: 
        HumanPlayer(const string &name_in) : name(name_in) {}

        const std::string & get_name() const {
            return name;
        }

        void add_card(const Card &c){
            assert(hand.size() < MAX_HAND_SIZE);
            hand.push_back(c);
            sort(hand.begin(), hand.end());
        }
        void add_and_discard(const Card &upcard) {
            sort(hand.begin(), hand.end());
            print_hand();
            hand.push_back(upcard);
            cout << "Discard upcard: [-1]\n";
            cout << "Human player " << name << ", please select a card to discard:\n" << endl;
            int card_int;
            cin >> card_int;
            if (card_int == -1) {
                hand.pop_back();  // Use pop_back instead of erase
            }
            else if (card_int >= 0 && card_int < hand.size()) {
                hand.erase(hand.begin() + card_int);  // Remove the off-by-one error
                sort(hand.begin(), hand.end());
            }
        }
        
        bool make_trump(const Card &upcard, bool is_dealer,
            int round, Suit &order_up_suit) const {
            print_hand();
            cout << "Human player " << name << ", please enter a suit, or \"pass\":" << endl;
            string suit_string;
            cin >> suit_string;
            if (suit_string == "pass") {
                return false;
            }
            else {
                order_up_suit = string_to_suit(suit_string);
                return true;
            }
        }
        
        Card lead_card(Suit trump) {
            print_hand();
            cout << "Human player " << name << ", please select a card:" << endl;
            int card_int;
            cin >> card_int;
            Card card = hand[card_int];
            hand.erase(hand.begin() + card_int);
            return card;
        }

        Card play_card(const Card &led_card, Suit trump) {
            print_hand();
            cout << "Human player " << name << ", please select a card:" << endl;
            int card_int;
            cin >> card_int;
            Card card = hand[card_int];
            hand.erase(hand.begin() + card_int);
            return card;
        }
};

Player * Player_factory(const std::string &name, const std::string &strategy) {
    if (strategy == "Human") {
        return new HumanPlayer(name);
    }
    else if (strategy == "Simple") {
        return new SimplePlayer(name);
    }
    assert(false);
    return nullptr;
}

std::ostream & operator<<(std::ostream &os, const Player &p) {
    os << p.get_name();
    return os;
}


























