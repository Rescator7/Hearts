#include <QFile>
#include <QDir>
#include <QString>
#include <QTextStream>
#include "config.h"

//#include <QMessageBox>

CConfig::CConfig()
{
 init_vars();

 QFile file(QDir::homePath() + CONFIG_FILENAME);

 if (!file.exists())
   save_config_file();      // create the file by saving the default values.
 else
   load_config_file();
}

void CConfig::init_vars()
{
  auto_centering = true;
  cheat_mode = false;
  info_channel = true;
  sounds = true;
  detect_tram = true;

  perfect_100 = false;
  omnibus = false;
  queen_spade_break_heart = false;
  no_trick_bonus = false;
  new_moon = false;
  no_draw = false;
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
      QString value = line.section(" ", -1, -1);

      bool enable;

     //  QMessageBox::warning(0, "warning", param+"='"+value+"'"+QString::number(value.length()));

      if (value == "true\n")
        enable = true;
      else
        enable = false;

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
      else {
          // unknown param
      }

      if (cpt > 11) break; // too many lines ?? corrupted file ??
  }
  file.close();

  return FNOERR;
}

int CConfig::set_config_file(int param, bool enable)
{
  switch (param) {
    case CONFIG_AUTO_CENTERING :          auto_centering = enable; break;
    case CONFIG_CHEAT_MODE :              cheat_mode = enable; break;
    case CONFIG_DETECT_TRAM :             detect_tram = enable; break;
    case CONFIG_INFO_CHANNEL :            info_channel = enable; break;
    case CONFIG_SOUNDS :                  sounds = enable; break;

    case CONFIG_PERFECT_100 :             perfect_100 = enable; break;
    case CONFIG_OMNIBUS :                 omnibus = enable; break;
    case CONFIG_QUEEN_SPADE_BREAK_HEART : queen_spade_break_heart = enable; break;
    case CONFIG_NO_TRICK_BONUS :          no_trick_bonus = enable; break;
    case CONFIG_NEW_MOON :                new_moon = enable; break;
    case CONFIG_NO_DRAW :                 no_draw = enable; break;
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

  out << "Auto_Centering = " << (auto_centering ? "true" : "false") << endl;
  out << "Cheat_Mode = " << (cheat_mode ? "true" : "false") << endl;
  out << "Info_Channel = " << (info_channel ? "true" : "false") << endl;
  out << "Sounds = " << (sounds ? "true" : "false") << endl;
  out << "Detect_Tram = " << (detect_tram ? "true" : "false") << endl;

  out << "Perfect_100 = " << (perfect_100 ? "true" : "false") << endl;
  out << "Omnibus = " << (omnibus ? "true" : "false") << endl;
  out << "Queen_Spade_Break_Heart = " << (queen_spade_break_heart ? "true" : "false") << endl;
  out << "No_Trick_Bonus = " << (no_trick_bonus ? "true" : "false") << endl;
  out << "New_Moon = " << (new_moon ? "true" : "false") << endl;
  out << "No_Draw = " << (no_draw ? "true" : "false") << endl;

  file.close();
  return FNOERR;
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