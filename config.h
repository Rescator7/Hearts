#ifndef CONFIG_H
#define CONFIG_H

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
const int CONFIG_DECK_STYLE              = 14;

const char CONFIG_FILENAME[10]  = "/.hearts";

class CConfig
{
public:
    CConfig();

private:
    void init_vars();
    int save_config_file();
    int load_config_file();

    int language;
    int deck_style;

    // game variants
    bool perfect_100;
    bool omnibus;
    bool queen_spade_break_heart;
    bool no_trick_bonus;
    bool new_moon;
    bool no_draw;

    // settings
    bool auto_centering;
    bool cheat_mode;
    bool info_channel;
    bool sounds;
    bool detect_tram;
    bool easy_card_selection;
    bool save_game;

public:
    int set_config_file(int param, bool enable);
    int set_language(int lang);
    int set_deck_style(int style);
    int get_language();
    int get_deck_style();

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
};

#endif // CONFIG_H
