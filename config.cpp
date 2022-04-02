#include <QFile>
#include <QDir>
#include <QString>
#include <QTextStream>
#include <QtGlobal>
#include "config.h"
#include "define.h"
#include "cdeck.h"

CConfig::CConfig()
{
 init_vars();

 QFile file(QDir::homePath() + CONFIG_FILENAME);

 if (!file.exists())
   save_config_file();      // create the file by saving the default values.
 else
   load_config_file();
}

CConfig::~CConfig()
{
}

void CConfig::init_vars()
{
  animated_play = true;
  auto_centering = true;
  cheat_mode = false;
  info_channel = true;
  sounds = true;
  detect_tram = true;
  easy_card_selection = true;
  save_game = true;
  auto_start = false;

  perfect_100 = false;
  omnibus = false;
  queen_spade_break_heart = false;
  no_trick_bonus = false;
  new_moon = false;
  no_draw = false;

  expert_speeds[SPEED_PLAY_CARD] = 0;
  expert_speeds[SPEED_PLAY_TWO_CLUBS] = 0;
  expert_speeds[SPEED_SHUFFLE] = 0;
  expert_speeds[SPEED_CLEAR_TABLE] = 0;
  expert_speeds[SPEED_YOUR_TURN] = 0;
  expert_speeds[SPEED_PASS_CARDS] = 0;
  expert_speeds[SPEED_ANIMATE_PASS_CARDS] = 0;
  expert_speeds[SPEED_ANIMATE_PLAY_CARD] = 0;

  speed = SPEED_NORMAL;

  username = "";
  password = "";
  warning = true;

  language = LANG_ENGLISH;
  deck_style = STANDARD_DECK;
  background = BACKGROUND_NONE;
}

int CConfig::load_config_file() {
  QFile file(QDir::homePath() + CONFIG_FILENAME);

  int cpt = 0;

  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    return ERROPENRO;
  }

  while (!file.atEnd()) {
      cpt++;
      QString line = file.readLine();
      QString param = line.section(" ", 0, 0);
      QString value = line.section(" ", -1, -1).simplified();

      if (param == "Language") {
        if (value == "english")
          language = LANG_ENGLISH;
        else
        if (value == "french")
          language = LANG_FRENCH;
        else
        if (value == "russian")
          language = LANG_RUSSIAN;

        continue;
      }

      if (param == "Deck_Style") {
        if (value == "standard")
          deck_style = STANDARD_DECK;
        else
        if (value == "english")
          deck_style = ENGLISH_DECK;
        else
        if (value == "russian")
          deck_style = RUSSIAN_DECK;
        else
        if (value == "nicu_white")
          deck_style = NICU_WHITE_DECK;
        else
        if (value == "tigullio_modern")
          deck_style = TIGULLIO_MODERN_DECK;

        continue;
      }

      if (param == "Speed") {
        if (value == "slow")
          speed = SPEED_SLOW;
        else
        if (value == "normal")
          speed = SPEED_NORMAL;
        else
        if (value == "fast")
          speed = SPEED_FAST;
        else
        if (value == "expert")
          speed = SPEED_EXPERT;

        continue;
      }

      if (param == "Speed_play_card") {
        expert_speeds[SPEED_PLAY_CARD] = value.toInt();

        continue;
      }

      if (param == "Speed_play_two_clubs") {
        expert_speeds[SPEED_PLAY_TWO_CLUBS] = value.toInt();

        continue;
      }

      if (param == "Speed_shuffle") {
        expert_speeds[SPEED_SHUFFLE] = value.toInt();

        continue;
      }

      if (param == "Speed_clear_table") {
        expert_speeds[SPEED_CLEAR_TABLE] = value.toInt();

        continue;
      }

      if (param == "Speed_your_turn") {
        expert_speeds[SPEED_YOUR_TURN] = value.toInt();

        continue;
      }

      if (param == "Speed_pass_cards") {
        expert_speeds[SPEED_PASS_CARDS] = value.toInt();

        continue;
      }

      if (param == "Speed_animate_pass_cards") {
        expert_speeds[SPEED_ANIMATE_PASS_CARDS] = value.toInt();

        continue;
      }

      if (param == "Speed_animate_play_card") {
        expert_speeds[SPEED_ANIMATE_PLAY_CARD] = value.toInt();

        continue;
      }

      if (param == "Background") {
        if (value == "None")
          background = BACKGROUND_NONE;
        else
        if (value == "Universe")
          background = BACKGROUND_UNIVERSE;
        else
        if (value == "Ocean")
          background = BACKGROUND_OCEAN;
        else
        if (value == "Mt_Fuji")
          background = BACKGROUND_MT_FUJI;
        else
        if (value == "Everest")
          background = BACKGROUND_EVEREST;
        else
        if (value == "Desert")
          background = BACKGROUND_DESERT;

        continue;
      }

      if (param == "Username") {
        username  = value;
        continue;
      }

      if (param == "Password") {
        password = value;
        continue;
      }

      bool enable;

      if (value == "true")
        enable = true;
      else
        enable = false;

      if (param == "Animated_Play")
        animated_play = enable;
      else
      if (param == "Auto_Centering")
        auto_centering = enable;
      else
      if (param == "Cheat_Mode")
        cheat_mode = enable;
      else
      if (param == "Info_Channel")
        info_channel = enable;
      else
      if (param == "Sounds")
        sounds = enable;
      else
      if (param == "Detect_Tram")
        detect_tram = enable;
      else
      if (param == "Easy_Card_Selection")
        easy_card_selection = enable;
      else
      if (param == "Perfect_100")
        perfect_100 = enable;
      else
      if (param == "Omnibus")
        omnibus = enable;
      else
      if (param == "Queen_Spade_Break_Heart")
        queen_spade_break_heart = enable;
      else
      if (param == "No_Trick_Bonus")
        no_trick_bonus = enable;
      else
      if (param == "New_Moon")
         new_moon = enable;
      else
      if (param == "No_Draw")
        no_draw = enable;
      else
      if (param == "Save_Game")
        save_game = enable;
      else
      if (param == "Warning")
        warning = enable;
      else
      if (param == "Auto_Start")
        auto_start = enable;
      else {
          // unknown param
      }

      if (cpt > 30) break; // too many lines ?? corrupted file ??
  }
  file.close();

  return FNOERR;
}

int CConfig::set_language(int lang) {
  language = lang;

  return save_config_file();
}

int CConfig::set_deck_style(int style) {
  deck_style = style;

  return save_config_file();
}

int CConfig::set_speed(int s) {
  speed = s;

  return save_config_file();
}

int CConfig::set_expert_speeds(int v1, int v2, int v3, int v4, int v5, int v6, int v7, int v8)
{
  speed = SPEED_EXPERT;

  expert_speeds[SPEED_PLAY_CARD] = v1;
  expert_speeds[SPEED_PLAY_TWO_CLUBS] = v2;
  expert_speeds[SPEED_SHUFFLE] = v3;
  expert_speeds[SPEED_CLEAR_TABLE] = v4;
  expert_speeds[SPEED_YOUR_TURN] = v5;
  expert_speeds[SPEED_PASS_CARDS] = v6;
  expert_speeds[SPEED_ANIMATE_PASS_CARDS] = v7;
  expert_speeds[SPEED_ANIMATE_PLAY_CARD] = v8;

  return save_config_file();
}

int CConfig::set_background(int b)
{
  background = b;

  return save_config_file();
}

int CConfig::set_config_file(int param, bool enable)
{
  switch (param) {
    case CONFIG_AUTO_CENTERING :          auto_centering = enable; break;
    case CONFIG_CHEAT_MODE :              cheat_mode = enable; break;
    case CONFIG_INFO_CHANNEL :            info_channel = enable; break;
    case CONFIG_SOUNDS :                  sounds = enable; break;
    case CONFIG_DETECT_TRAM :             detect_tram = enable; break;
    case CONFIG_PERFECT_100 :             perfect_100 = enable; break;
    case CONFIG_OMNIBUS :                 omnibus = enable; break;
    case CONFIG_QUEEN_SPADE_BREAK_HEART : queen_spade_break_heart = enable; break;
    case CONFIG_NO_TRICK_BONUS :          no_trick_bonus = enable; break;
    case CONFIG_NEW_MOON :                new_moon = enable; break;
    case CONFIG_NO_DRAW :                 no_draw = enable; break;
    case CONFIG_SAVE_GAME :               save_game = enable; break;
    case CONFIG_EASY_CARD_SELECTION :     easy_card_selection = enable; break;
    case CONFIG_WARNING :                 warning = enable; break;
    case CONFIG_AUTO_START :              auto_start = enable; break;
    case CONFIG_ANIMATED_PLAY :           animated_play = enable; break;
  }

  return save_config_file();
}

int CConfig::save_config_file()
{
  QFile file(QDir::homePath() + CONFIG_FILENAME);

  if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
     return ERROPENWO;
  }

  QTextStream out(&file);

  if (username.size() && password.size()) {
    out << "Username = " << username << EOL;
    out << "Password = " << password << EOL;
    out << "Warning = " << (warning ? "true" : "false") << EOL;
  }

  switch (language) {
    case LANG_ENGLISH: out << "Language = english" << EOL; break;
    case LANG_FRENCH:  out << "Language = french"  << EOL; break;
    case LANG_RUSSIAN: out << "Language = russian" << EOL; break;
  }

  switch (deck_style) {
    case STANDARD_DECK:        out << "Deck_Style = standard" << EOL; break;
    case ENGLISH_DECK:         out << "Deck_Style = english" << EOL; break;
    case RUSSIAN_DECK:         out << "Deck_Style = russian" << EOL; break;
    case NICU_WHITE_DECK:      out << "Deck_Style = nicu_white" << EOL; break;
    case TIGULLIO_MODERN_DECK: out << "Deck_Style = tigullio_modern" << EOL; break;
  }

  switch (background) {
    case BACKGROUND_NONE:     out << "Background = None" << EOL; break;
    case BACKGROUND_UNIVERSE: out << "Background = Universe" << EOL; break;
    case BACKGROUND_OCEAN:    out << "Background = Ocean" << EOL; break;
    case BACKGROUND_EVEREST:  out << "Background = Everest" << EOL; break;
    case BACKGROUND_MT_FUJI:  out << "Background = Mt_Fuji" << EOL; break;
    case BACKGROUND_DESERT:   out << "Background = Desert" << EOL; break;
  }

  out << "Animated_Play = " << (animated_play ? "true" : "false") << EOL;
  out << "Auto_Centering = " << (auto_centering ? "true" : "false") << EOL;
  out << "Cheat_Mode = " << (cheat_mode ? "true" : "false") << EOL;
  out << "Info_Channel = " << (info_channel ? "true" : "false") << EOL;
  out << "Sounds = " << (sounds ? "true" : "false") << EOL;
  out << "Detect_Tram = " << (detect_tram ? "true" : "false") << EOL;
  out << "Easy_Card_Selection = " << (easy_card_selection ? "true" : "false") << EOL;
  out << "Auto_Start = " << (auto_start ? "true" : "false") << EOL;

  switch (speed) {
    case SPEED_SLOW : out << "Speed = slow" << EOL; break;
    case SPEED_FAST : out << "Speed = fast" << EOL; break;
    case SPEED_EXPERT : out << "Speed = expert" << EOL;

                        out << "Speed_play_card = " << expert_speeds[SPEED_PLAY_CARD] << EOL;
                        out << "Speed_play_two_clubs = " << expert_speeds[SPEED_PLAY_TWO_CLUBS] << EOL;
                        out << "Speed_shuffle = " << expert_speeds[SPEED_SHUFFLE] << EOL;
                        out << "Speed_clear_table = " << expert_speeds[SPEED_CLEAR_TABLE] << EOL;
                        out << "Speed_your_turn = " << expert_speeds[SPEED_YOUR_TURN] << EOL;
                        out << "Speed_pass_cards = " << expert_speeds[SPEED_PASS_CARDS] << EOL;
                        out << "Speed_animate_pass_cards = " << expert_speeds[SPEED_ANIMATE_PASS_CARDS] << EOL;
                        out << "Speed_animate_play_card = " << expert_speeds[SPEED_ANIMATE_PLAY_CARD] << EOL;

                        break;
    default :           out << "Speed = normal" << EOL;
  }

  out << "Perfect_100 = " << (perfect_100 ? "true" : "false") << EOL;
  out << "Omnibus = " << (omnibus ? "true" : "false") << EOL;
  out << "Queen_Spade_Break_Heart = " << (queen_spade_break_heart ? "true" : "false") << EOL;
  out << "No_Trick_Bonus = " << (no_trick_bonus ? "true" : "false") << EOL;
  out << "New_Moon = " << (new_moon ? "true" : "false") << EOL;
  out << "No_Draw = " << (no_draw ? "true" : "false") << EOL;
  out << "Save_Game = " << (save_game ? "true" : "false") << EOL;

  file.close();
  return FNOERR;
}

void CConfig::set_online(QString u, QString p)
{
  username = u;
  password = p;

  save_config_file();
}

QString &CConfig::Username()
{
  return username;
}

QString &CConfig::Password()
{
  return password;
}

bool CConfig::Warning()
{
  return warning;
}

bool CConfig::is_auto_centering() {
  return auto_centering;
}

bool CConfig::is_cheat_mode() {
  return cheat_mode;
}

bool CConfig::is_info_channel() {
  return info_channel;
}

bool CConfig::is_sounds() {
  return sounds;
}

bool CConfig::is_detect_tram() {
  return detect_tram;
}

bool CConfig::is_perfect_100() {
  return perfect_100;
}

bool CConfig::is_omnibus() {
  return omnibus;
}

bool CConfig::is_queen_spade_break_heart() {
  return queen_spade_break_heart;
}

bool CConfig::is_no_trick_bonus() {
  return no_trick_bonus;
}

bool CConfig::is_new_moon() {
  return new_moon;
}

bool CConfig::is_no_draw() {
  return no_draw;
}

bool CConfig::is_save_game() {
  return save_game;
}

bool CConfig::is_easy_card_selection() {
  return easy_card_selection;
}

bool CConfig::is_auto_start() {
  return auto_start;
}

bool CConfig::is_animated_play() {
  return animated_play;
}

int CConfig::get_language() {
  return language;
}

int CConfig::get_deck_style() {
  return deck_style;
}

int CConfig::get_speed() {
  return speed;
}

int CConfig::get_speed(int type) {
  if (speed == SPEED_EXPERT)
    return expert_speeds[type];

  switch (type) {
    case SPEED_CLEAR_TABLE: return SPEED_CLEAR_TABLE_VALUES[speed]; break;
    case SPEED_SHUFFLE:     return SPEED_SHUFFLE_VALUES[speed]; break;
    case SPEED_PASS_CARDS:  return SPEED_PASS_CARDS_VALUES[speed]; break;
    case SPEED_PLAY_CARD:   return SPEED_PLAY_CARD_VALUES[speed]; break;
    case SPEED_PLAY_TWO_CLUBS: return SPEED_PLAY_TWO_CLUBS_VALUES[speed]; break;
    case SPEED_YOUR_TURN:   return SPEED_YOUR_TURN_VALUES[speed]; break;
    case SPEED_ANIMATE_PASS_CARDS: return SPEED_ANIMATE_PASS_CARDS_VALUES[speed]; break;
    case SPEED_ANIMATE_PLAY_CARD: return SPEED_ANIMATE_PLAY_CARD_VALUES[speed]; break;
  }
  return 0;
}

int CConfig::get_background()
{
  return background;
}
