#ifndef DEFINE
#define DEFINE

#include <QtGlobal>

const char version[80] = "Hearts 1.8.10";

const int adjust_pos[14] = {0, 6, 6, 5, 4, 4, 3, 3, 2, 2, 1, 1, 0, 0};

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
  #define EOL Qt::endl
#else
  #define EOL endl
#endif

#define NOBODY        -1
#define PLAYER_SOUTH   0
#define PLAYER_WEST    1
#define PLAYER_NORTH   2
#define PLAYER_EAST    3

#define QUEEN_SPADE_f    1
#define PERFECT_100_f    1 << 1
#define OMNIBUS_f        1 << 2
#define NO_TRICK_BONUS_f 1 << 3
#define NEW_MOON_f       1 << 4
#define NO_DRAW_f        1 << 5

const int LANG_ENGLISH      = 0;
const int LANG_FRENCH       = 1;
const int LANG_RUSSIAN      = 2;

const int FNOERR                         = 0;
const int ERROPENRO                      = 1;
const int ERROPENWO                      = 2;
const int FCORRUPTED                     = 3;

const int MAX_PLR_NAMES                  = 44;

const int AI_flags_pass_hearts_zero      = 0x1;
const int AI_flags_pass_hearts_high      = 0x2;
const int AI_flags_pass_hearts_low       = 0x4;
const int AI_flags_try_moon              = 0x8;
const int AI_flags_pass_elim_suit        = 0x10;
const int AI_flags_safe_keep_qs          = 0x20;
const int AI_flags_count_spade           = 0x40;
const int AI_flags_friendly              = 0x80;


const char names[MAX_PLR_NAMES][10] = {"You", "Aina", "Airi", "Alex", "Charles", "Christian", "Christine", "Cindy",
                                       "Danny", "David", "Denis", "Elena", "Erica", "Gabriel", "Grace", "Karine",
                                       "Karl", "Jason", "Jennifer", "John", "Linda", "Mai", "Maimi", "Marc", "Mary",
                                       "Masaki", "Michael", "Michelle", "Myriam", "Nadia", "Patricia", "Paul", "Reina",
                                       "Rick", "Riho", "Robert", "Sam", "Sandy", "Sara", "Sayuki", "Sayumi", "Sonia",
                                       "Sophie", "Tomoko"};

// These combinations of flags determine the playing style of the [cpus] players.
const int AI_CPU_flags[MAX_PLR_NAMES] = {
    0,                                                          // You
    AI_flags_pass_hearts_low  | AI_flags_try_moon |
    AI_flags_count_spade,                                       // Aina
    AI_flags_pass_hearts_high | AI_flags_pass_elim_suit |
    AI_flags_count_spade | AI_flags_friendly,                   // Airi
    AI_flags_pass_hearts_low  | AI_flags_safe_keep_qs,          // Alex
    AI_flags_pass_hearts_zero | AI_flags_try_moon,              // Charles
    AI_flags_pass_hearts_high | AI_flags_count_spade,           // Christian
    AI_flags_pass_hearts_zero | AI_flags_try_moon |
    AI_flags_pass_elim_suit | AI_flags_friendly,                // Christine
    AI_flags_pass_hearts_low,                                   // Cindy
    AI_flags_pass_hearts_zero | AI_flags_pass_elim_suit,        // Dany
    AI_flags_pass_hearts_high | AI_flags_count_spade |
    AI_flags_friendly,                                          // David
    AI_flags_pass_hearts_low  | AI_flags_safe_keep_qs,          // Denis
    AI_flags_pass_hearts_high | AI_flags_try_moon,              // Elena
    AI_flags_pass_hearts_low | AI_flags_count_spade |
    AI_flags_friendly,                                          // Erica
    AI_flags_pass_hearts_zero | AI_flags_pass_elim_suit,        // Gabriel
    AI_flags_pass_hearts_zero | AI_flags_safe_keep_qs |
    AI_flags_friendly,                                          // Grace
    AI_flags_pass_hearts_low  | AI_flags_pass_hearts_high |
    AI_flags_try_moon | AI_flags_count_spade,                   // Karine
    AI_flags_pass_hearts_high | AI_flags_friendly,              // Karl
    AI_flags_pass_hearts_low  | AI_flags_pass_hearts_high,      // Jason
    AI_flags_pass_hearts_high | AI_flags_pass_elim_suit,        // Jennifer
    AI_flags_pass_hearts_zero | AI_flags_try_moon |
    AI_flags_count_spade | AI_flags_friendly,                   // John
    AI_flags_pass_hearts_low,                                   // Linda
    AI_flags_pass_hearts_zero,                                  // Mai
    AI_flags_pass_hearts_low | AI_flags_count_spade |
    AI_flags_friendly,                                          // Maimi
    AI_flags_pass_hearts_high | AI_flags_try_moon,              // Marc
    AI_flags_pass_hearts_low  | AI_flags_pass_elim_suit,        // Mary
    AI_flags_pass_hearts_low  | AI_flags_pass_hearts_high,      // Masaki
    AI_flags_pass_hearts_zero | AI_flags_count_spade |
    AI_flags_friendly,                                          // Michael
    AI_flags_pass_hearts_high | AI_flags_safe_keep_qs,          // Michelle
    AI_flags_pass_hearts_low  | AI_flags_try_moon |
    AI_flags_pass_elim_suit | AI_flags_friendly,                // Myriam
    AI_flags_pass_hearts_zero | AI_flags_try_moon,              // Nadia
    AI_flags_pass_hearts_high | AI_flags_count_spade,           // Patricia
    AI_flags_pass_hearts_zero,                                  // Paul
    AI_flags_pass_hearts_low  | AI_flags_try_moon,              // Reina
    AI_flags_pass_hearts_high | AI_flags_safe_keep_qs |
    AI_flags_friendly,                                          // Rick
    AI_flags_pass_hearts_low  | AI_flags_pass_hearts_high |
    AI_flags_count_spade  | AI_flags_friendly,                  // Riho
    AI_flags_pass_hearts_zero | AI_flags_pass_elim_suit,        // Robert
    AI_flags_pass_hearts_low,                                   // Sam
    AI_flags_pass_hearts_zero | AI_flags_safe_keep_qs |
    AI_flags_count_spade,                                       // Sandy
    AI_flags_pass_hearts_zero,                                  // Sara
    AI_flags_pass_hearts_high | AI_flags_try_moon |
    AI_flags_count_spade | AI_flags_friendly,                   // Sayuki
    AI_flags_pass_hearts_low  | AI_flags_pass_elim_suit |
    AI_flags_safe_keep_qs,                                      // Sayumi
    AI_flags_pass_hearts_low  | AI_flags_try_moon,              // Sonia
    AI_flags_pass_hearts_high | AI_flags_count_spade |
    AI_flags_friendly,                                          // Sophie
    AI_flags_pass_hearts_low  | AI_flags_safe_keep_qs           // Tomoko
};
#endif // DEFINE
