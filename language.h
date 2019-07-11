#ifndef LANGUAGE
#define LANGUAGE


const int MAX_LANGUAGES     = 3;
const int MAX_FIELDS        = 31;
const int MAX_STRLANG_SIZE  = 80;

const int LANG_ENGLISH      = 0;
const int LANG_FRENCH       = 1;
const int LANG_RUSSIAN      = 2;

const int LANG_menuFile            = 0;
const int LANG_menuGame_Variations = 1;
const int LANG_menuOptions         = 2;
const int LANG_menuStats           = 3;
const int LANG_menuHelp            = 4;
const int LANG_menuAbout           = 5;
const int LANG_menuLanguage        = 6;
const int LANG_menuDebug           = 7;

const int LANG_actionNew                      = 8;
const int LANG_actionQuit                     = 9;
const int LANG_actionQueen_Spade_Break_Heart  = 10;
const int LANG_actionPerfect_100              = 11;
const int LANG_actionOmnibus                  = 12;
const int LANG_actionNo_Trick_Bonus           = 13;
const int LANG_actionNew_Moon                 = 14;
const int LANG_actionNo_Draw                  = 15;

const int LANG_actionAuto_Centering = 16;
const int LANG_actionInfo_Channel   = 17;
const int LANG_actionSounds         = 18;
const int LANG_actionTram           = 19;
const int LANG_actionSave_Game_Quit = 20;

const int LANG_actionShow           = 21;
const int LANG_actionReset          = 22;

const int LANG_actionRules          = 23;
const int LANG_actionSettings       = 24;

const int LANG_actionCredits        = 25;

const int LANG_actionEnglish        = 26;
const int LANG_actionFrench         = 27;
const int LANG_actionRussian        = 28;

const int LANG_actionShow_2         = 29;
const int LANG_actionCheat          = 30;


const char LANGUAGES[MAX_LANGUAGES][MAX_FIELDS][MAX_STRLANG_SIZE] = {

    // ENGLISH

  {"File", "Options", "Settings", "Stats", "Help", "About", "Language", "Debug",
   "New", "Quit",
   "Queen Spade Break Heart", "Perfect 100", "Omnibus", "No Trick Bonus", "New Moon", "No Draw",
   "Auto Centering", "Info Channel", "Sounds", "Detect Tram", "Save Game (Quit)",
   "Show", "Reset",
   "Rules", "Settings",
   "Credits",
   "English", "French", "Russian",
   "Cards History", "Cheat Mode"},


   // FRENCH

  {"Fichier", "Variations du jeu", "Options", "Statistiques", "Aide", "À propos", "Langue", "Débuggage",
   "Nouvelle partie", "Quitter",
   "La dame de pique brise le coeur", "Les pointages de 100 sont réduit de moitié", "Omnibus", "Bonus zero pénalité",
        "Nouvelle lune", "Bris d'égalité",
   "Recentrage des cartes", "Canal d'informations", "Activations des sons", "Détection de la fin d'un tour",
        "Sauvegarde automatique",
   "Montrer", "Effacer",
   "Les règles", "Paramètres",
   "Remerciements",
   "Anglais", "Français", "Russe",
   "L'historique des cartes", "Espionner"},

   // RUSSIAN

   {"Файл", "Параметры", "Настройки", "Статистика", "Помощь", "Об игре", "языки", "Отладка",
    "Новая игра", "Выход",
    "Дама пик разбила сердце", "Ровно 100", "Омнибус", "Бонус за ноль взяток", "Новая луна", "Без ничьи",
    "По центру", "Инфоканал", "звуки", "остальное мое", "Сохранить при выходе",
    "Просмотр", "Сброс",
    "Правила", "Настройки",
    "Благодарности",
    "английский", "Французский", "русский",
    "История карт", "Чужие карты"}
  };

#endif // LANGUAGE

