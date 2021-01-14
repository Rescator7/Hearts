#include <QFile>
#include <QDir>
#include <QString>
#include <QTextStream>
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

      if (cpt > 22) break; // too many lines ?? corrupted file ??
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
    out << "Username = " << username << endl;
    out << "Password = " << password << endl;
    out << "Warning = " << (warning ? "true" : "false") << endl;
  }

  switch (language) {
    case LANG_ENGLISH: out << "Language = english" << endl; break;
    case LANG_FRENCH:  out << "Language = french"  << endl; break;
    case LANG_RUSSIAN: out << "Language = russian" << endl; break;
  }

  switch (deck_style) {
    case STANDARD_DECK:   out << "Deck_Style = standard" << endl; break;
    case ENGLISH_DECK:    out << "Deck_Style = english" << endl; break;
    case RUSSIAN_DECK:    out << "Deck_Style = russian" << endl; break;
    case NICU_WHITE_DECK: out << "Deck_Style = nicu_white" << endl; break;
  }

  switch (background) {
    case BACKGROUND_NONE:     out << "Background = None" << endl; break;
    case BACKGROUND_UNIVERSE: out << "Background = Universe" << endl; break;
    case BACKGROUND_OCEAN:    out << "Background = Ocean" << endl; break;
    case BACKGROUND_EVEREST:  out << "Background = Everest" << endl; break;
    case BACKGROUND_MT_FUJI:  out << "Background = Mt_Fuji" << endl; break;
    case BACKGROUND_DESERT:   out << "Background = Desert" << endl; break;
  }

  out << "Animated_Play = " << (animated_play ? "true" : "false") << endl;
  out << "Auto_Centering = " << (auto_centering ? "true" : "false") << endl;
  out << "Cheat_Mode = " << (cheat_mode ? "true" : "false") << endl;
  out << "Info_Channel = " << (info_channel ? "true" : "false") << endl;
  out << "Sounds = " << (sounds ? "true" : "false") << endl;
  out << "Detect_Tram = " << (detect_tram ? "true" : "false") << endl;
  out << "Easy_Card_Selection = " << (easy_card_selection ? "true" : "false") << endl;
  out << "Auto_Start = " << (auto_start ? "true" : "false") << endl;

  switch (speed) {
    case SPEED_SLOW : out << "Speed = slow" << endl; break;
    case SPEED_FAST : out << "Speed = fast" << endl; break;
    default :         out << "Speed = normal" << endl;
  }

  out << "Perfect_100 = " << (perfect_100 ? "true" : "false") << endl;
  out << "Omnibus = " << (omnibus ? "true" : "false") << endl;
  out << "Queen_Spade_Break_Heart = " << (queen_spade_break_heart ? "true" : "false") << endl;
  out << "No_Trick_Bonus = " << (no_trick_bonus ? "true" : "false") << endl;
  out << "New_Moon = " << (new_moon ? "true" : "false") << endl;
  out << "No_Draw = " << (no_draw ? "true" : "false") << endl;
  out << "Save_Game = " << (save_game ? "true" : "false") << endl;

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
