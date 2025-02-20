#include <cassert>
#include <iostream>
#include "Pack.hpp"

using namespace std;

Pack::Pack() {
    
    for (int s = SPADES; s <= DIAMONDS; ++s) {
        Suit suit = static_cast<Suit>(s);
        for (int r = NINE; r <= ACE; ++r) {
            Rank rank = static_cast<Rank>(r);
            Card c(rank, suit);
            int rr = r - 7;
            cards[s * 6 + rr] = c;
        }
    }
    next = 0;
}

Pack::Pack(std::istream& pack_input) {
    for (int i = 0; i < PACK_SIZE; ++i) {
        pack_input >> cards[i];
    }
    next = 0;
}

Card Pack::deal_one() {
    Card card = cards[next];
    ++next;
    return card;
}

void Pack::reset() {
    next = 0;
}

void Pack::shuffle() {
    std::array<Card, PACK_SIZE / 2> halfPack1;
    std::array<Card, PACK_SIZE / 2> halfPack2;
    for (int j = 0; j < 7; j++) {
        for (int k = 0; k < 12; k++) {
            halfPack1[k] = cards[k];
            halfPack2[k] = cards[12 + k];
        }
        std::array<Card, PACK_SIZE> shuffledCards;
        for (int i = 0; i < 12; ++i) {
            shuffledCards[2 * i] = halfPack2[i];
            shuffledCards[2 * i + 1] = halfPack1[i];
        }
        cards = shuffledCards;
        reset();
    }    
}


bool Pack::empty() const {
    if (next == 24) {
        return true;
    }
    else {
        return false;
    }
}



