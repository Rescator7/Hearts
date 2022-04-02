#ifndef CONFIG_H
#define CONFIG_H

#include <QString>

const int CONFIG_AUTO_CENTERING          = 0;
const int CONFIG_CHEAT_MODE              = 1;
const int CONFIG_INFO_CHANNEL            = 2;
const int CONFIG_SOUNDS                  = 3;
const int CONFIG_DETECT_TRAM             = 4;
const int CONFIG_PERFECT_100             = 5;
const int CONFIG_OMNIBUS                 = 6;
const int CONFIG_QUEEN_SPADE_BREAK_HEART = 7;
const int CONFIG_NO_TRICK_BONUS          = 8;
const int CONFIG_NEW_MOON                = 9;
const int CONFIG_NO_DRAW                 = 10;
const int CONFIG_SAVE_GAME               = 11;
const int CONFIG_LANGUAGE                = 12;
const int CONFIG_EASY_CARD_SELECTION     = 13;
const int CONFIG_AUTO_START              = 14;
const int CONFIG_DECK_STYLE              = 15;
const int CONFIG_ANIMATED_PLAY           = 16;
const int CONFIG_USERNAME                = 17;
const int CONFIG_PASSWORD                = 18;
const int CONFIG_WARNING                 = 19;

const int BACKGROUND_UNSET               = -1;
const int BACKGROUND_NONE                = 0;
const int BACKGROUND_UNIVERSE            = 1;
const int BACKGROUND_OCEAN               = 2;
const int BACKGROUND_MT_FUJI             = 3;
const int BACKGROUND_EVEREST             = 4;
const int BACKGROUND_DESERT              = 5;

const int SPEED_SLOW                     = 0;
const int SPEED_NORMAL                   = 1;
const int SPEED_FAST                     = 2;
const int SPEED_EXPERT                   = 3;

const int SPEED_PLAY_CARD                = 0;
const int SPEED_PLAY_TWO_CLUBS           = 1;
const int SPEED_SHUFFLE                  = 2;
const int SPEED_CLEAR_TABLE              = 3;
const int SPEED_YOUR_TURN                = 4;
const int SPEED_PASS_CARDS               = 5;
const int SPEED_ANIMATE_PASS_CARDS       = 6;
const int SPEED_ANIMATE_PLAY_CARD        = 7;
const int MAX_SPEEDS                     = 8;

const int SPEED_PLAY_CARD_VALUES[3]      = {550, 400, 200};
const int SPEED_PLAY_TWO_CLUBS_VALUES[3] = {1000, 700, 350};
const int SPEED_SHUFFLE_VALUES[3]        = {1550, 1200, 1050};
const int SPEED_CLEAR_TABLE_VALUES[3]    = {350, 200, 100};
const int SPEED_YOUR_TURN_VALUES[3]      = {300, 200, 100};
const int SPEED_PASS_CARDS_VALUES[3]     = {2500, 2000, 1000};
const int SPEED_ANIMATE_PASS_CARDS_VALUES[3] = {22, 10, 4};
const int SPEED_ANIMATE_PLAY_CARD_VALUES[3]  = {200, 170, 130};

const char CONFIG_FILENAME[10]  = "/.hearts";

class CConfig
{
public:
    CConfig();
    ~CConfig();

private:
    void init_vars();
    int save_config_file();
    int load_config_file();

    int language;
    int deck_style;
    int speed;
    int background;
    int expert_speeds[MAX_SPEEDS];

    // game variants
    bool perfect_100;
    bool omnibus;
    bool queen_spade_break_heart;
    bool no_trick_bonus;
    bool new_moon;
    bool no_draw;
    bool auto_start;

    // settings
    bool animated_play;
    bool auto_centering;
    bool cheat_mode;
    bool info_channel;
    bool sounds;
    bool detect_tram;
    bool easy_card_selection;
    bool save_game;

    // online
    QString username;
    QString password;
    bool warning;

public:
    QString &Username();
    QString &Password();
    bool Warning();

    void set_online(QString u, QString p);

    int set_config_file(int param, bool enable);
    int set_language(int lang);
    int set_deck_style(int style);
    int set_speed(int s);
    int set_expert_speeds(int v1, int v2, int v3, int v4, int v5, int v6, int v7, int v8);
    int set_background(int b);
    int get_language();
    int get_deck_style();
    int get_speed();
    int get_speed(int type);
    int get_background();

    bool is_auto_centering();
    bool is_cheat_mode();
    bool is_info_channel();
    bool is_sounds();
    bool is_detect_tram();

    bool is_perfect_100();
    bool is_omnibus();
    bool is_queen_spade_break_heart();
    bool is_no_trick_bonus();
    bool is_new_moon();
    bool is_no_draw();
    bool is_save_game();
    bool is_easy_card_selection();
    bool is_auto_start();
    bool is_animated_play();
};

#endif // CONFIG_H
