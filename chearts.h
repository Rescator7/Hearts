#ifndef CHEARTS_H
#define CHEARTS_H

#include <QObject>

const int DECK_SIZE     = 52;

const int NOT_FOUND     = -1;

const int two_clubs     = 0;
const int queen_spade   = 23;
const int king_spade    = 24;
const int ace_spade     = 25;
const int jack_diamond  = 35;
const int queen_diamond = 36;
const int king_diamond  = 37;
const int ace_diamond   = 38;
const int ace_heart     = 51;
const int empty         = 127;

const int pLEFT         = 0;
const int pRIGHT        = 1;
const int pACCROSS      = 2;
const int pNOPASS       = 3;

const int CLUB          = 0;
const int SPADE         = 1;
const int DIAMOND       = 2;
const int HEART         = 3;
const int FREESUIT      = 4;

const int NOERROR       = 0;
const int ERRHEART      = 1;
const int ERRSUIT       = 2;
const int ERRQUEEN      = 3;

const int OMNIBUS_BONUS        = 1;
const int NO_TRICK_BONUS       = 2;

const char suit_names[4][10]   = {"club", "spade", "diamond", "heart"};

class CHearts : public QObject
{
    Q_OBJECT

public:
    CHearts();
    ~CHearts();

private: // variables
    bool game_over;
    bool shoot_moon;
    bool heart_broken;
    bool jack_diamond_played;
    bool mode_playing;                       // true = playing, false = selecting cards
    bool tram_enabled;

    bool perfect_100;
    bool omnibus;
    bool no_trick_bonus;
    bool queen_spade_break_heart;
    bool new_moon;
    bool moon_add_to_scores;
    bool no_draw;

    bool cards_selected[4][13];
    bool cards_played[DECK_SIZE];

    int user_id;
    int turn;
    int hand_turn;
    int current_suit;
    int passed_to;
    int card_left;
    int hand_score; 
    int best_hand;
    int plr_best_hand;
    int plr_jack_diamond;
    int plr_cards[4][13];
    int plr_score[4];
    int plr_hand_score[4];
    int cpt_plr_cards[4];
    int cards_selected_count[4];
    int cards_left_in_suit[4];
    int plr_cards_in_suit[4][4];
    int plr_has_card[4][52];
    int cpu_cards[3];
    int hand_cards[4];

private: // functions
   bool can_break_heart(int plr);
   bool can_play_qs_first_hand(int plr);
   bool is_only_heart_left(int plr);
   bool is_tram(int plr);
   bool is_first_card();
   bool is_moon_still_possible();
   bool is_card_on_table(int card);

   int  get_cpu_move();
   int  get_lowest_suit(int plr, int suit);
   int  get_lowest_suit_pos(int plr, int suit);
   int  get_card_position(int plr, int card);
   int  get_highest_suit_pos(int plr, int suit);
   int  check_invalid_move(int plr, int card);
   int  freesuit_lead_eval(int card);
   int  spade_lead_eval(int card);
   int  diamond_lead_eval(int card);

   void init_vars();
   void reset_cards_on_table();
   void reset_score();
   void reset_hand_score();
   void reset_cards_played();
   void reset_cards_left_in_suit();
   void reset_plr_has_card();
   void random_deck();
   void process_card(int card);
   void advance_turn();
   void play_hand();
   void process_next_pass(bool skip_moon_check);
   void set_cpu_passing_cards(int plr);
   void reset_plr_cards_in_suit();

public: // functions
   bool is_game_over();
   bool is_it_draw();
   bool is_mode_playing();
   bool is_no_pass();
   bool is_ready_to_pass();
   bool is_it_my_turn();
   bool is_card_selected(int plr, int idx);
   bool is_card_selected(int idx);

   bool select_card(int idx);
   bool unselect_card(int idx);

   int  whoami();
   int  get_current_suit();
   int  play_card(int idx);
   int  get_card(int plr, int idx);
   int  get_card(int idx);
   int  get_my_score();
   int  count_my_cards();
   int  count_plr_cards(int plr);

   void new_game();
   void set_tram_enabled(bool enable);  
   void sort_plr_cards();
   void play_2clubs();
   void select_cpus_cards();
   void pass_cards();
   void reset_cards_passed();

   void set_perfect_100(bool enable);
   void set_omnibus(bool enable);
   void set_no_trick_bonus(bool enable);
   void set_queen_spade_break_heart(bool enable);
   void set_new_moon(bool enable);
   void set_no_draw(bool enable);
   void set_moon_add_to_score(bool enable); 

signals:
   void sig_clear_table();
   void sig_play_card(int card, int idx);
   void sig_refresh_deck(int plr);
   void sig_score(int score, int idx);
   void sig_end_hand(int score1, int score2, int score3, int score4);
   void sig_hand_score(int score, int idx);
   void sig_your_turn(int idx);
   void sig_breaking_heart();
   void sig_shoot_moon(int plr);
   void sig_perfect_100(int plr);
   void sig_tram(int plr);
   void sig_pass_to(int idx);
   void sig_game_over(int score1, int score2, int score3, int score4);
   void sig_bonus(int plr, int bonus, int value);
};

#endif // CHEARTS_H