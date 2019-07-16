#ifndef LANGUAGE
#define LANGUAGE


const int MAX_LANGUAGES     = 3;
const int MAX_FIELDS        = 36;
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

const int LANG_actionAuto_Centering      = 16;
const int LANG_actionInfo_Channel        = 17;
const int LANG_actionSounds              = 18;
const int LANG_actionTram                = 19;
const int LANG_actionEasy_Card_Selection = 20;
const int LANG_actionSave_Game_Quit      = 21;
const int LANG_MenuDeck                  = 22;

const int LANG_actionStandard       = 23;
const int LANG_actionEnglish_2      = 24;
const int LANG_actionRussian_2      = 25;

const int LANG_actionShow           = 26;
const int LANG_actionReset          = 27;

const int LANG_actionRules          = 28;
const int LANG_actionSettings       = 29;

const int LANG_actionCredits        = 30;

const int LANG_actionEnglish        = 31;
const int LANG_actionFrench         = 32;
const int LANG_actionRussian        = 33;

const int LANG_actionShow_2         = 34;
const int LANG_actionCheat          = 35;


const char LANGUAGES[MAX_LANGUAGES][MAX_FIELDS][MAX_STRLANG_SIZE] = {

    // ENGLISH

  {"File", "Options", "Settings", "Stats", "Help", "About", "Language", "Debug",
   "New", "Quit",
   "Queen Spade Break Heart", "Perfect 100", "Omnibus", "No Trick Bonus", "New Moon", "No Draw",
   "Auto Centering", "Info Channel", "Sounds", "Detect Tram", "Easy card selection", "Save Game (Quit)", "Card deck",
   "Standard", "English", "Russian",
   "Show", "Reset",
   "Rules", "Settings",
   "Credits",
   "English", "French", "Russian",
   "Cards History", "Cheat Mode"},


   // FRENCH

  {"Fichier", "Variations du jeu", "Options", "Statistiques", "Aide", "À propos", "Langue", "Débuggage",
   "Nouvelle partie", "Quitter",
   "La dame de pique brise le coeur", "Les pointages de 100 sont réduit de moitié", "Omnibus", "Bonus zéro pénalité",
        "Nouvelle lune", "Bris d'égalité",
   "Recentrage des cartes", "Canal d'informations", "Activations des sons", "Détection de la fin d'un tour",
        "Sélection de carte assistée", "Sauvegarde automatique", "Jeu de cartes",
   "Standard", "Anglais", "Russe",
   "Montrer", "Effacer",
   "Les règles", "Paramètres",
   "Remerciements",
   "Anglais", "Français", "Russe",
   "L'historique des cartes", "Espionner"},

   // RUSSIAN

   {"Файл", "Параметры", "Настройки", "Статистика", "Помощь", "Об игре", "Язык", "Отладка",
    "Новая игра", "Выход",
    "Дама пик разбила сердце", "Ровно 100", "Омнибус", "Бонус за ноль взяток", "Новая луна", "Без ничьи", 
    "По центру", "Инфоканал", "Звуки", "Остальное моё", "Простой выбор карты", "Сохранить при выходе", "Колода карт",
    "Стандартная", "Английская", "Русская",
    "Просмотр", "Сброс",
    "Правила", "Настройки",
    "Благодарности",
    "Английский", "Французский", "Русский",
    "История карт", "Чужие карты"}
  };

#endif // LANGUAGE

