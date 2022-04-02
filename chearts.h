#ifndef CHEARTS_H
#define CHEARTS_H

#include <QObject>

#define FIRST_CARD   0
#define SECOND_CARD  1
#define THIRD_CARD   2
#define FOURTH_CARD  3

const int DECK_SIZE     = 52;

const int NOT_FOUND     = -1;

const int two_clubs     = 0;
const int ace_clubs     = 12;
const int two_spade     = 13;
const int queen_spade   = 23;
const int king_spade    = 24;
const int ace_spade     = 25;
const int two_diamond   = 26;
const int jack_diamond  = 35;
const int queen_diamond = 36;
const int king_diamond  = 37;
const int ace_diamond   = 38;
const int two_heart     = 39;
const int jack_heart    = 48;
const int queen_heart   = 49;
const int king_heart    = 50;
const int ace_heart     = 51;
const int back_card     = 100;
const int empty         = 127;
const int your_turn     = 255;

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
const int ERR2CLUBS     = 4;

const int OMNIBUS_BONUS        = 1;
const int NO_TRICK_BONUS       = 2;

const char suit_names[4][10]   = {"club", "spade", "diamond", "heart"};
const char SAVEDGAME_FILENAME[20]  = "/.hearts.saved";
const char SAVEDGAME_CORRUPTED[20] = "/.hearts.saved.bak";

const int first_card_suit[4] = {two_clubs, two_spade, two_diamond, two_heart};
const int last_card_suit[4]  = {ace_clubs, ace_spade, ace_diamond, ace_heart};

class CHearts : public QObject
{
    Q_OBJECT

public:
    CHearts();
    ~CHearts();

private: // variables
    struct struct_undo_data {
       bool undo_available;
       bool heart_broken;
       bool shoot_moon;
       bool jack_diamond_played;
       bool moon_add_to_scores;
       bool cards_played[DECK_SIZE];

       int passed_to;
       int hand_score;
       int best_hand;
       int hand_turn;
       int current_suit;
       int card_left;
       int plr_best_hand;
       int plr_jack_diamond;
       int plr_cards[4][13];
       int plr_score[4];
       int plr_hand_score[4];
       int cpt_plr_cards[4];
       int cards_left_in_suit[4];
       int plr_cards_in_suit[4][4];
       int plr_has_card[4][DECK_SIZE];
       int hand_cards[4];
    } undo_data;

    bool game_over;
    bool shoot_moon;
    bool heart_broken;
    bool jack_diamond_played;
    bool mode_playing;                       // true = playing, false = selecting cards
    bool tram_enabled;
    bool fresh_game;
    bool auto_start;
    bool moon_wait;

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
    int plr_name_id[4];
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
    int plr_has_card[4][DECK_SIZE];
    int cpu_cards[3];
    int hand_cards[4];
    int AI_cpu_flags[4];

private: // functions
   bool can_break_heart(int plr);
   bool can_play_qs_first_hand(int plr);
   bool is_only_heart_left(int plr);
   bool is_tram(int plr);
   bool is_prepass_to_moon();
   bool is_moon_an_option();
   bool is_card_on_table(int card);
   bool get_cpu_moon_add(int plr);

   int  get_lowest_suit(int plr, int suit);
   int  get_lowest_suit_pos(int plr, int suit);
   int  get_highest_suit(int plr, int suit);
   int  get_highest_suit_pos(int plr, int suit);
   int  get_highest_card_table();
   int  check_invalid_move(int plr, int card);
   int  eval_card_strength(int plr, int card);

   int  AI_get_cpu_move();
   int  AI_eval_lead_spade(int card);
   int  AI_eval_lead_diamond(int card);
   int  AI_eval_lead_hearts(int card);
   int  AI_eval_lead_freesuit(int card);

   bool AI_pass_friendly();
   void AI_pass_hearts(int cpu, int &cpt);
   void AI_pass_spades(int cpu, int &cpt);
   void AI_pass_clubs(int cpu, int &cpt);
   void AI_pass_diamonds(int cpu, int &cpt);
   void AI_pass_random(int cpu, int &cpt);
   void AI_pass_to_moon(int cpu, int &cpt);
   void AI_pass_remove_suit(int cpu, int suit, int &cpt);
   void AI_pass_save_card(int card_id, int &cpt);

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
   void save_undo();

public: // functions
   bool is_game_over();
   bool is_it_draw();
   bool is_mode_playing();
   bool is_no_pass();
   bool is_ready_to_pass();
   bool is_it_my_turn();
   bool is_card_selected(int plr, int idx);
   bool is_card_selected(int idx);
   bool is_card_selectable(int card);
   bool is_card_played(int card);
   bool is_starting();
   bool is_undo_available();
   bool is_moon_wait();

   bool select_card(int idx);
   bool unselect_card(int idx);

   int  undo();
   int  get_turn();
   int  get_current_suit();
   int  play_card(int idx);
   int  get_card(int plr, int idx);
   int  get_card(int idx);
   int  get_my_score();
   int  count_my_cards();
   int  count_plr_cards(int plr);
   int  save_game(int plr1, int plr2, int pl3, int pl4);
   int  load_saved_game();
   int  get_plr_name_id(int plr);
   int  get_plr_hand_card(int plr);
   int  is_fresh_game();
   int  get_lowest_score();
   int  get_highest_score();
   int  get_plr_pass_to(int plr_from);
   int  get_plr_num_cards(int plr);
   int  get_card_position(int plr, int card);

   void new_game(bool booting);
   void set_tram_enabled(bool enable);  
   void sort_plr_cards();
   void play_2clubs();
   void pass_cards();
   void reset_cards_passed();

   void AI_set_cpu_flags(int cpu, int flags);
   void AI_pass_cpus_cards();

   void set_perfect_100(bool enable);
   void set_omnibus(bool enable);
   void set_no_trick_bonus(bool enable);
   void set_queen_spade_break_heart(bool enable);
   void set_new_moon(bool enable);
   void set_no_draw(bool enable);
   void set_auto_start(bool enable);
   void set_moon_add_to_score(bool enable); 

signals:
   void sig_clear_table();
   void sig_play_card(int card, int idx);
   void sig_refresh_deck(bool animate);
   void sig_score(int plr, int score);
   void sig_end_hand(int score1, int score2, int score3, int score4);
   void sig_hand_score(int plr, int score);
   void sig_your_turn(int idx);
   void sig_breaking_heart();
   void sig_shoot_moon(int plr);
   void sig_perfect_100(int plr);
   void sig_tram(int plr);
   void sig_pass_to(int idx);
   void sig_pass_cards(int w1, int w2, int w3, int n1, int n2, int n3, int e1, int e2, int e3);
   void sig_game_over(int score_s, int score_w, int score_n, int score_e);
   void sig_bonus(int plr, int bonus, int value);
   void sig_got_queen_spade(int plr);
   void sig_new_game();
};

#endif // CHEARTS_H
