#include <iostream>
#include <fstream>
#include <cstring>
#include "Player.hpp"
#include "Pack.hpp"
#include "Card.hpp"

using namespace std;

 // Game ADT (private variables + public play function)
 class Game {
    private:
        vector<Player*> Players;
        Pack pack;
        int points_to_win;
        bool shuffle_on;

        int player_to_left(int player_number) {
            if (player_number == 3)
                return 0;
            else {
                return (player_number + 1);
            }
        }

        void shuffle(bool shuffle_on) {
            if (shuffle_on) {
                pack.shuffle();
            }
        }
        
        void deal(int dealer) {
            int current_player = player_to_left(dealer);
            Players[current_player]->add_card(pack.deal_one());
            Players[current_player]->add_card(pack.deal_one());
            Players[current_player]->add_card(pack.deal_one());

            current_player = player_to_left(current_player);
            Players[current_player]->add_card(pack.deal_one());
            Players[current_player]->add_card(pack.deal_one());

            current_player = player_to_left(current_player);
            Players[current_player]->add_card(pack.deal_one());
            Players[current_player]->add_card(pack.deal_one());
            Players[current_player]->add_card(pack.deal_one());

            current_player = player_to_left(current_player);
            Players[current_player]->add_card(pack.deal_one());
            Players[current_player]->add_card(pack.deal_one());

            current_player = player_to_left(current_player);
            Players[current_player]->add_card(pack.deal_one());
            Players[current_player]->add_card(pack.deal_one());

            current_player = player_to_left(current_player);
            Players[current_player]->add_card(pack.deal_one());
            Players[current_player]->add_card(pack.deal_one());
            Players[current_player]->add_card(pack.deal_one());

            current_player = player_to_left(current_player);
            Players[current_player]->add_card(pack.deal_one());
            Players[current_player]->add_card(pack.deal_one());

            current_player = player_to_left(current_player);
            Players[current_player]->add_card(pack.deal_one());
            Players[current_player]->add_card(pack.deal_one());
            Players[current_player]->add_card(pack.deal_one());

        }

        bool make_trump(int round, Suit &order_up_suit, int &player, int dealer, Card upcard) {
            int j = 0;
            for (int i = player_to_left(dealer); j < 4; i = player_to_left(i)) {
                bool is_dealer = j == 3;
                if (Players[i]->make_trump(upcard, is_dealer, round, order_up_suit)) {
                    if (round == 1) {
                        cout << *Players[i] << " orders up " << order_up_suit << endl << endl;
                        Players[dealer]->add_and_discard(upcard);
                    }
                    else {
                        cout << *Players[i] << " orders up " << order_up_suit << endl << endl;
                    }
                    j = 4;
                    player = i;
                    return true;
                }
                cout << *Players[i] << " passes" << endl;
                ++j;
            }
            return false;
        }

        int play_trick(int leader, Suit trump) {
            vector<Card> trick;
            Card led_card = Players[leader]->lead_card(trump);
            trick.push_back(led_card);
            cout << led_card << " led by " << *Players[leader] << endl;
            Card best_card = led_card;
            int j = 0;
            int trick_winner = leader;
            for (int i = player_to_left(leader); j < 3; i = player_to_left(i)) {
                trick.push_back(Players[i]->play_card(trick[0], trump));
                cout << trick[j + 1] << " played by " << *Players[i] << endl;
                if (Card_less(best_card, trick[j + 1], led_card, trump)) {
                    best_card = trick[j + 1];
                    trick_winner = i;
                }
                ++j;
            }

            /* old code here
            Card led_card = Players[current_player]->lead_card(trump);
            trick.push_back(led_card);
            cout << led_card << " led by " << *Players[current_player] << endl;
            
            current_player = player_to_left(current_player);
            Card card2 = Players[current_player]->play_card(led_card, trump);
            trick.push_back(card2);
            cout << card2 << " played by " << *Players[current_player] << endl;

            current_player = player_to_left(current_player);
            Card card3 = Players[current_player]->play_card(led_card, trump);
            trick.push_back(card3);
            cout << card3 << " played by " << *Players[current_player] << endl;

            current_player = player_to_left(current_player);
            Card card4 = Players[current_player]->play_card(led_card, trump);
            trick.push_back(card4);
            cout << card4 << " played by " << *Players[current_player] << endl;


            Card best_card = led_card;
            int j = 0;
            int trick_winner = leader;
            for (int i = leader; j < 3; i = player_to_left(i)) {
                if (Card_less(trick[j], trick[j + 1], trump)) {
                    best_card = trick[j + 1];
                    trick_winner = player_to_left(i);
                }
                ++j;
            }
            */
            
            cout << *Players[trick_winner] << " takes the trick" << endl << endl;
            return trick_winner;
        }

        void score (int team1_tricks, int team2_tricks, int order_up_team, int &score1, int &score2) {
            if (order_up_team == 1) {
                if (team1_tricks == 3 || team1_tricks == 4) {
                    cout << *Players[0] << " and " << *Players[2] << " win the hand" << endl;
                    score1 += 1;
                }
                else if (team1_tricks == 5) {
                    score1 += 2;
                    cout << *Players[0] << " and " << *Players[2] << " win the hand" << endl;
                    cout << "march!" << endl;
                }
                else {
                    score2 += 2;
                    cout << *Players[1] << " and " << *Players[3] << " win the hand" << endl;
                    cout << "euchred!" << endl;
                }
            }
            else if (order_up_team == 2) {
                if (team2_tricks == 3 || team2_tricks == 4) {
                    cout << *Players[1] << " and " << *Players[3] << " win the hand" << endl;
                    score2 += 1;
                }
                else if (team2_tricks == 5) {
                    score2 += 2;
                    cout << *Players[1] << " and " << *Players[3] << " win the hand" << endl;
                    cout << "march!" << endl;
                }
                else {
                    score1 += 2;
                    cout << *Players[0] << " and " << *Players[2] << " win the hand" << endl;
                    cout << "euchred!" << endl;
                }
            }
            
        }


    public:
        Game(vector<Player*> Players_in, Pack pack_in, int points_to_win_in, bool shuffle_on_in) 
            : Players(Players_in), pack(pack_in), points_to_win(points_to_win_in), shuffle_on(shuffle_on_in) {}
        
        void play() {
            int dealer = 0;
            int team1_score = 0;
            int team2_score = 0;
            int hand_count = 0;
            bool game_end = false;
            while(!game_end) { 
                cout << "Hand " << hand_count << endl;
                cout << *Players[dealer] << " deals" << endl;
                shuffle(shuffle_on);
                deal(dealer);
                Suit order_up_suit;
                int order_up_player;
                Card upcard = pack.deal_one();
                cout << upcard << " turned up" << endl;
                if (!make_trump(1, order_up_suit, order_up_player, dealer, upcard)) {
                    make_trump(2, order_up_suit, order_up_player, dealer, upcard);
                }
                int order_up_team;
                if (order_up_player % 2 == 0) {
                    order_up_team = 1;
                }
                else {
                    order_up_team = 2;
                }
                
                Suit &trump_suit = order_up_suit;
                int team1_trickswon = 0;
                int team2_trickswon = 0;
                int leader = player_to_left(dealer);
                for (int i = 0; i < 5; ++i) {
                    leader = play_trick(leader, trump_suit);
                    if (leader % 2 == 0) {
                        ++team1_trickswon;
                    }
                    else {
                       ++team2_trickswon;
                    }
                }
                
                score(team1_trickswon, team2_trickswon, order_up_team, team1_score, team2_score);

                cout << *Players[0] << " and " << *Players[2] << " have " << team1_score << " points" << endl;
                cout << *Players[1] << " and " << *Players[3] << " have " << team2_score << " points" << endl << endl;
                if (team1_score >= points_to_win) {
                    game_end = true;
                    cout << *Players[0] << " and " << *Players[2] << " win!" << endl;
                }
                if (team2_score >= points_to_win) {
                    game_end = true;
                    cout << *Players[1] << " and " << *Players[3] << " win!" << endl;
                }

                dealer = player_to_left(dealer);
                pack.reset();
                hand_count++;
            }

            for (size_t i = 0; i < Players.size(); ++i) {
                delete Players[i];
            }
        }




    

 };

int main(int argc, char* argv[]) {

    if (argc != 12) {
        cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
     << "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 "
     << "NAME4 TYPE4" << endl;
        return 1;
    }
    if (!((strcmp(argv[2], "shuffle") == 0) || (strcmp(argv[2], "noshuffle") == 0))) {
        cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
     << "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 "
     << "NAME4 TYPE4" << endl;
        return 1;
    }
    if (stoi(argv[3]) < 1 || stoi(argv[3]) > 100) {
        cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
     << "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 "
     << "NAME4 TYPE4" << endl;
        return 1;
    }
    if (!((strcmp(argv[5], "Simple") == 0) || (strcmp(argv[5], "Human") == 0))) {
        cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
     << "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 "
     << "NAME4 TYPE4" << endl;
        return 1;
    }
    if (!((strcmp(argv[7], "Simple") == 0) || (strcmp(argv[7], "Human") == 0))) {
        cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
     << "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 "
     << "NAME4 TYPE4" << endl;
        return 1;
    }
    if (!((strcmp(argv[9], "Simple") == 0) || (strcmp(argv[9], "Human") == 0))) {
        cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
     << "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 "
     << "NAME4 TYPE4" << endl;
        return 1;
    }
    if (!((strcmp(argv[11], "Simple") == 0) || (strcmp(argv[11], "Human") == 0))) {
        cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
     << "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 "
     << "NAME4 TYPE4" << endl;
        return 1;
    }

    // play game
    vector<Player*> players;
    for (int i = 0; i < 4; ++i) {
        Player * player = Player_factory(argv[4 + 2 * i], argv[5 + 2 * i]);
        players.push_back(player);
    }
    
    int points_to_win = stoi(argv[3]);
    bool shuffle_on;
    if (strcmp(argv[2], "shuffle") == 0) {
        shuffle_on = true;
    }
    
    else {
        shuffle_on = false;
    }
    
    string filename = argv[1];
    ifstream pack;
    pack.open(filename);
    Pack game_pack = Pack(pack);
    cout << argv[0] << " " << argv[1] << " " << argv[2] << " " << argv[3] << " " << argv[4] 
    << " " << argv[5] << " " << argv[6] << " " << argv[7] << " " << argv[8] << " " << argv[9] << " "
    << argv[10] << " " << argv[11] << " " << endl;
    Game game(players, game_pack, points_to_win, shuffle_on);
    game.play();
}

// issues: players aren't playing the right cards...