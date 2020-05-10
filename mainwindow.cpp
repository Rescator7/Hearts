#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QLabel>
#include <QFile>
#include <QDir>
#include <QPushButton>
#include <QImage>
#include <QString>
#include <QMessageBox>
#include <QTime>
#include <QResource>
#include <string.h>

#include "time.h"
#include "assert.h"
#include "rules.h"
#include "credits.h"
#include "connect.h"
#include "settings.h"
#include "online.h"
#include "cardspos.h"
#include "cgame.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    srand(static_cast<unsigned int>(time(nullptr)));

    ui->textEdit->setTextColor(Qt::yellow);
    ui->textEdit->setTextBackgroundColor(Qt::black);

    stop_delay = false;

    setWindowTitle(version);

    init_pointers();

    load_sounds();

    config = new CConfig;

    hearts = new CHearts;

    stats = new CStatistics;

    deck = new CDeck(config->get_deck_style());

    table_list = new CTable();

    client = new CClient();

#ifdef DEBUG
    debug = new CDebug(deck->get_img_card(empty));
#endif
    ui->progressBar->hide();

    timer = new QTimer();

    init_vars();
    set_settings();
    set_options();

    light_connected(false);
    online_show_buttons(false);

    connect(timer, SIGNAL(timeout()), this, SLOT(update_bar()));

    connect(table_list, SIGNAL(clicked(int, char)), this, SLOT(join_game(int, char)));

    connect(client, SIGNAL(sig_message(QString)), this, SLOT(message(QString)));
    connect(client, SIGNAL(sig_action(unsigned int, QString)), this, SLOT(online_action(unsigned int, QString)));

    connect(hearts, SIGNAL(sig_refresh_deck(int,bool)), this, SLOT(refresh_deck(int,bool)));
    connect(hearts, SIGNAL(sig_play_card(int,int)), this, SLOT(play_card(int,int)));
    connect(hearts, SIGNAL(sig_clear_table()), this, SLOT(clear_table()));
    connect(hearts, SIGNAL(sig_score(int,int)), this, SLOT(refresh_score(int,int)));
    connect(hearts, SIGNAL(sig_hand_score(int,int)), this, SLOT(refresh_hand_score(int,int)));
    connect(hearts, SIGNAL(sig_end_hand(int,int,int,int)), this, SLOT(end_of_hand(int,int,int,int)));
    connect(hearts, SIGNAL(sig_your_turn(int)), this, SLOT(show_your_turn(int)));
    connect(hearts, SIGNAL(sig_breaking_heart()), this, SLOT(breaking_hearts()));
    connect(hearts, SIGNAL(sig_game_over(int,int,int,int)), this, SLOT(game_over(int,int,int,int)));
    connect(hearts, SIGNAL(sig_tram(int)), this, SLOT(tram(int)));
    connect(hearts, SIGNAL(sig_shoot_moon(int)), this, SLOT(shoot_moon(int)));
    connect(hearts, SIGNAL(sig_perfect_100(int)), this, SLOT(perfect_100(int)));
    connect(hearts, SIGNAL(sig_bonus(int,int,int)), this, SLOT(receive_bonus(int,int,int)));
    connect(hearts, SIGNAL(sig_pass_to(int)), this, SLOT(pass_to(int)));
    connect(hearts, SIGNAL(sig_got_queen_spade(int)), this, SLOT(got_queen_spade(int)));

    connect(qApp, SIGNAL(aboutToQuit()), this, SLOT(aboutToQuit()));
    connect(ui->actionQuit, SIGNAL(triggered(bool)), this, SLOT(close()));

    set_language(config->get_language());

    message(tr("Welcome to ") + QString(version));

    if (stats->is_file_corrupted())
      message(tr("[Error]: The statistics file is corrupted!"));

    start_game();

#ifdef DEBUG
    set_cheat_mode_enabled(config->is_cheat_mode());
#else
    ui->radioButton->hide();
    ui->radioButton_2->hide();
    ui->radioButton_3->hide();
    ui->radioButton_4->hide();
#endif
}

MainWindow::~MainWindow()
{
  delete ui;
  delete client;
  delete hearts;
  delete config;
  delete stats;
  delete deck;
  delete table_list;
  delete timer;

#ifdef DEBUG
  delete debug;
#endif

  delete img_connected;
  delete img_disconnected;

  delete img_pass[pLEFT];
  delete img_pass[pRIGHT];
  delete img_pass[pACCROSS];
  delete img_pass[pNOPASS];

  destroy_sounds();
}

void MainWindow::init_vars()
{
  // online variables
  online_table_id = 0;
  online_passto = pLEFT;
  online_connected = false;
  online_can_sit = false;
  online_game_started = false;
  online_button_mode = mode_button_command;

  card_played[0] = empty;
  card_played[1] = empty;
  card_played[2] = empty;
  card_played[3] = empty;

  card_height = 130;
  def_card_posy = ui->label->y();

  switch (config->get_deck_style()) {
    case STANDARD_DECK: ui->actionStandard->setChecked(true); break;
    case ENGLISH_DECK:  ui->actionEnglish_2->setChecked(true); break;
    case RUSSIAN_DECK:  ui->actionRussian_2->setChecked(true); break;
  }
}

void MainWindow::load_sounds()
{
#ifdef __al_included_allegro5_allegro_audio_h
    al_install_audio();
    al_init_acodec_addon();
    al_reserve_samples(14);

    QResource resource_snd1(":/sounds/34201__themfish__glass-house1.wav");
    QResource resource_snd2(":/sounds/240777__f4ngy__dealing-card.wav");
    QResource resource_snd3(":/sounds/253886__themusicalnomad__negative-beeps.wav");
    QResource resource_snd4(":/sounds/133283__fins__game-over.wav");
    QResource resource_snd5(":/sounds/171671__fins__success-1.wav");
    QResource resource_snd6(":/sounds/Soundwhich - Correct (LittleRainySeasons) [public].wav");
    QResource resource_snd7(":/sounds/344013__reitanna__high-pitched-ah2.wav");
    QResource resource_snd8(":/sounds/96127__bmaczero__contact1.wav");
    QResource resource_snd9(":/sounds/434472__dersuperanton__taking-card.wav");
    QResource resource_snd10(":/sounds/423767__someonecool14__card-shuffling.wav");
    QResource resource_snd11(":/sounds/400163__vitovsky1__fanfare-short.wav");
    QResource resource_snd12(":/sounds/322897__rhodesmas__connected-01.wav");
    QResource resource_snd13(":/sounds/322895__rhodesmas__disconnected-01.wav");
    QResource resource_snd14(":/sounds/493696__stib__bingbong.wav");

    ALLEGRO_FILE *fp = al_open_memfile(reinterpret_cast<void *>(const_cast<unsigned char *>(resource_snd1.data())), resource_snd1.size(), "rb");
    snd_breaking_heart = al_load_sample_f(fp, ".wav");
    al_fclose(fp);

    fp = al_open_memfile(reinterpret_cast<void *>(const_cast<unsigned char *>(resource_snd2.data())), resource_snd2.size(), "rb");
    snd_dealing_card = al_load_sample_f(fp, ".wav");
    al_fclose(fp);

    fp = al_open_memfile(reinterpret_cast<void *>(const_cast<unsigned char *>(resource_snd3.data())), resource_snd3.size(), "rb");
    snd_error = al_load_sample_f(fp, ".wav");
    al_fclose(fp);

    fp = al_open_memfile(reinterpret_cast<void *>(const_cast<unsigned char *>(resource_snd4.data())), resource_snd4.size(), "rb");
    snd_game_over = al_load_sample_f(fp, ".wav");
    al_fclose(fp);

    fp = al_open_memfile(reinterpret_cast<void *>(const_cast<unsigned char *>(resource_snd5.data())), resource_snd5.size(), "rb");
    snd_shoot_moon = al_load_sample_f(fp, ".wav");
    al_fclose(fp);

    fp = al_open_memfile(reinterpret_cast<void *>(const_cast<unsigned char *>(resource_snd6.data())), resource_snd6.size(), "rb");
    snd_your_turn = al_load_sample_f(fp, ".wav");
    al_fclose(fp);

    fp = al_open_memfile(reinterpret_cast<void *>(const_cast<unsigned char *>(resource_snd7.data())), resource_snd7.size(), "rb");
    snd_queen_spade = al_load_sample_f(fp, ".wav");
    al_fclose(fp);

    fp = al_open_memfile(reinterpret_cast<void *>(const_cast<unsigned char *>(resource_snd8.data())), resource_snd8.size(), "rb");
    snd_contact = al_load_sample_f(fp, ".wav");
    al_fclose(fp);

    fp = al_open_memfile(reinterpret_cast<void *>(const_cast<unsigned char *>(resource_snd9.data())), resource_snd9.size(), "rb");
    snd_passing_cards = al_load_sample_f(fp, ".wav");
    al_fclose(fp);

    fp = al_open_memfile(reinterpret_cast<void *>(const_cast<unsigned char *>(resource_snd10.data())), resource_snd10.size(), "rb");
    snd_shuffling_cards = al_load_sample_f(fp, ".wav");
    al_fclose(fp);

    fp = al_open_memfile(reinterpret_cast<void *>(const_cast<unsigned char *>(resource_snd11.data())), resource_snd11.size(), "rb");
    snd_perfect_100 = al_load_sample_f(fp, ".wav");
    al_fclose(fp);

    fp = al_open_memfile(reinterpret_cast<void *>(const_cast<unsigned char *>(resource_snd12.data())), resource_snd12.size(), "rb");
    snd_connected = al_load_sample_f(fp, ".wav");
    al_fclose(fp);

    fp = al_open_memfile(reinterpret_cast<void *>(const_cast<unsigned char *>(resource_snd13.data())), resource_snd13.size(), "rb");
    snd_disconnected = al_load_sample_f(fp, ".wav");
    al_fclose(fp);

    fp = al_open_memfile(reinterpret_cast<void *>(const_cast<unsigned char *>(resource_snd14.data())), resource_snd14.size(), "rb");
    snd_announcement = al_load_sample_f(fp, ".wav");
    al_fclose(fp);

    assert(snd_breaking_heart);
    assert(snd_dealing_card);
    assert(snd_error);
    assert(snd_game_over);
    assert(snd_shoot_moon);
    assert(snd_your_turn);
    assert(snd_queen_spade);
    assert(snd_contact);
    assert(snd_passing_cards);
    assert(snd_shuffling_cards);
    assert(snd_perfect_100);
    assert(snd_connected);
    assert(snd_disconnected);
    assert(snd_announcement);
#else
    ui->actionSounds->setDisabled(true);
#endif
}

void MainWindow::destroy_sounds()
{
#ifdef __al_included_allegro5_allegro_audio_h
    al_destroy_sample(snd_shoot_moon);
    al_destroy_sample(snd_your_turn);
    al_destroy_sample(snd_game_over);
    al_destroy_sample(snd_error);
    al_destroy_sample(snd_dealing_card);
    al_destroy_sample(snd_breaking_heart);
    al_destroy_sample(snd_queen_spade);
    al_destroy_sample(snd_passing_cards);
    al_destroy_sample(snd_shuffling_cards);
    al_destroy_sample(snd_connected);
    al_destroy_sample(snd_disconnected);
    al_destroy_sample(snd_announcement);
#endif
}

void MainWindow::init_pointers()
{
    img_connected = new QImage(":/icons/connected.png", "PNG");
    img_disconnected = new QImage(":/icons/disconnected.png", "PNG");
    img_pass[pLEFT] = new QImage(":/icons/left-icon.png", "PNG");
    img_pass[pRIGHT] = new QImage(":/icons/right-icon.png", "PNG");
    img_pass[pACCROSS] = new QImage(":/icons/up-icon.png", "PNG");
    img_pass[pNOPASS] = new QImage(":/icons/no_pass-icon.png", "PNG");

    label[0] = ui->label;
    label[1] = ui->label_2;
    label[2] = ui->label_3;
    label[3] = ui->label_4;
    label[4] = ui->label_5;
    label[5] = ui->label_6;
    label[6] = ui->label_7;
    label[7] = ui->label_8;
    label[8] = ui->label_9;
    label[9] = ui->label_10;
    label[10] = ui->label_11;
    label[11] = ui->label_12;
    label[12] = ui->label_13;
    label[13] = ui->label_14;
    label[14] = ui->label_15;
    label[15] = ui->label_16;
    label[16] = ui->label_17;
    label[17] = ui->label_18;
    label[18] = ui->label_19;
    label[19] = ui->label_20;
    label[20] = ui->label_21;
    label[21] = ui->label_22;

    lcd_score[0] = ui->lcdNumber;
    lcd_score[1] = ui->lcdNumber_2;
    lcd_score[2] = ui->lcdNumber_3;
    lcd_score[3] = ui->lcdNumber_4;

    lcd_hand_score[0] = ui->lcdNumber_10;
    lcd_hand_score[1] = ui->lcdNumber_11;
    lcd_hand_score[2] = ui->lcdNumber_12;
    lcd_hand_score[3] = ui->lcdNumber_13;

 #ifdef DEBUG
    cheat_radio_button[0] = ui->radioButton;
    cheat_radio_button[1] = ui->radioButton_2;
    cheat_radio_button[2] = ui->radioButton_3;
    cheat_radio_button[3] = ui->radioButton_4;
#endif
}

void MainWindow::aboutToQuit()
{
  client->disconnect();
  save_files();
}

void MainWindow::save_files()
{    
 stats->save_stats_file();

 if (!ui->actionSave_Game_Quit->isChecked())
   return;

 if (hearts->is_game_over())
   return;

 if (hearts->is_fresh_game())
   return;

 hearts->save_game(plr_names_idx[0], plr_names_idx[1], plr_names_idx[2], plr_names_idx[3]);
}

void MainWindow::load_saved_game()
{
  message(tr("[Info]: Previous saved game has been loaded!"));
  for (int i=0; i<4; i++) {
    int name_id = hearts->get_plr_name_id(i);

    plr_names_idx[i] = name_id;
    label[18 + i]->setText(names[name_id]);
  }

  if (hearts->is_mode_playing()) {
    label[17]->setDisabled(true);

    int cpt = hearts->whoami();
    for (int i=0; i<3; i++) {
      int card = hearts->get_plr_hand_card(2-i);

      if (card == empty) continue;

      if (--cpt < 0)
        cpt = 3;

      label[13 + cpt]->setPixmap(QPixmap::fromImage(deck->get_img_card(card)->scaledToHeight(card_height)));
      card_played[cpt] = card;

#ifdef DEBUG
      debug->save_card(names[plr_names_idx[cpt]], deck->get_img_card(card));
    }
    debug->reverse_order();
#else
    }
#endif
  }
}

void MainWindow::start_game()
{
  int error = hearts->load_saved_game();

  if (error == NOERROR)
    load_saved_game();
  else {
    if (error == FCORRUPTED) {
      message(tr("[Error]: The saved game file is corrupted! Deleted!"));

      QFile file(QDir::homePath() + SAVEDGAME_FILENAME);
      file.remove();
    }
    stats->increase_stats(0, STATS_GAME_STARTED);
    hearts->new_game();
    set_plr_names();
  }
}

void MainWindow::light_connected(bool connected)
{
  if (connected)
    ui->label_24->setPixmap(QPixmap::fromImage(img_connected->scaledToHeight(15)));
  else
    ui->label_24->setPixmap(QPixmap::fromImage(img_disconnected->scaledToHeight(15)));
}

void MainWindow::set_settings()
{
#ifdef DEBUG
  ui->actionCheat->setChecked(config->is_cheat_mode());
#endif

  ui->actionAuto_Centering->setChecked(config->is_auto_centering());
  ui->actionInfo_Channel->setChecked(config->is_info_channel());
  ui->actionSounds->setChecked(config->is_sounds());
  ui->actionTram->setChecked(config->is_detect_tram());
  ui->actionEasy_card_selection->setChecked(config->is_easy_card_selection());

  ui->actionPerfect_100->setChecked(config->is_perfect_100());
  ui->actionOmnibus->setChecked(config->is_omnibus());
  ui->actionQueen_Spade_Break_Heart->setChecked(config->is_queen_spade_break_heart());
  ui->actionNo_Trick_Bonus->setChecked(config->is_no_trick_bonus());
  ui->actionNew_Moon->setChecked(config->is_new_moon());
  ui->actionNo_Draw->setChecked(config->is_no_draw());
  ui->actionSave_Game_Quit->setChecked(config->is_save_game());

  set_info_channel_enabled(config->is_info_channel());
}

void MainWindow::set_options()
{
  hearts->set_tram_enabled(config->is_detect_tram());
  hearts->set_perfect_100(config->is_perfect_100());
  hearts->set_omnibus(config->is_omnibus());
  hearts->set_queen_spade_break_heart(config->is_queen_spade_break_heart());
  hearts->set_no_trick_bonus(config->is_no_trick_bonus());
  hearts->set_new_moon(config->is_new_moon());
  hearts->set_no_draw(config->is_no_draw());
}

void MainWindow::set_plr_names()
{
  bool name_taken[MAX_PLR_NAMES];

  for (int i=0; i<MAX_PLR_NAMES; i++)
     name_taken[i] = false;

  int whoami = hearts->whoami();

  for (int i=0; i<4; i++) {
     if (i == whoami) continue;

    int x = rand() % MAX_PLR_NAMES;
    while (!x || name_taken[x])
        x = rand() % MAX_PLR_NAMES;

    name_taken[x] = true;;
    label[18 + i]->setText(names[x]);
    plr_names_idx[i] = x;

    hearts->AI_set_cpu_flags(i, AI_CPU_flags[x]);
  }

  label[18 + whoami]->setText(tr("You"));
  plr_names_idx[whoami] = 0;
}

void MainWindow::got_queen_spade(int plr)
{
  stats->increase_stats(plr_names_idx[plr], STATS_QUEEN_SPADE);
}

void MainWindow::receive_bonus(int plr, int bonus, int value)
{
  QString mesg, msg_bonus;

  switch (bonus) {
     case OMNIBUS_BONUS  : msg_bonus = tr("omnibus ");
                           stats->increase_stats(plr_names_idx[plr], STATS_OMNIBUS);
                           break;
     case NO_TRICK_BONUS : msg_bonus = tr("no trick bonus ");
                           stats->increase_stats(plr_names_idx[plr], STATS_NO_TRICKS);
                           break;
     default: msg_bonus = tr("<unknown> ");
  }

  if (plr == hearts->whoami())
    mesg = tr("[Info]: You receive the bonus: ") + msg_bonus + QString::number(value) + tr(" point(s)");
  else
    mesg = tr("[Info]: Player '") + label[18 + plr]->text() + tr("' receive the bonus: ") + msg_bonus +
            QString::number(value) + tr(" point(s)");

  message(mesg);
}

void MainWindow::online_receive_bonus(int plr, int bonus, int value)
{
  QString mesg, msg_bonus;

  switch (bonus) {
     case OMNIBUS_BONUS  : msg_bonus = tr("omnibus ");
                           break;
     case NO_TRICK_BONUS : msg_bonus = tr("no trick bonus ");
                           break;
     default: msg_bonus = tr("<unknown> ");
  }

  if (plr == online_my_turn)
    mesg = tr("[Info]: You receive the bonus: ") + msg_bonus + QString::number(value) + tr(" point(s)");
  else
    mesg = tr("[Info]: Player '") + label[18 + plr]->text() + tr("' receive the bonus: ") + msg_bonus +
            QString::number(value) + tr(" point(s)");

  message(mesg);
}

void MainWindow::perfect_100(int plr)
{
 stats->increase_stats(plr_names_idx[plr], STATS_PERFECT_100);

 QString mesg;

 if (plr == hearts->whoami())
   mesg = tr("[Info]: You got the perfect 100!\n[Info]: Your score has been set to 50.");
 else
   mesg = tr("[Info]: Player '") + label[18 + plr]->text() + tr("' got the perfect 100!\n[Info]: Player '") +
                                   label[18 + plr]->text() + tr("' score has been set to 50.");

 message(mesg);

#ifdef __al_included_allegro5_allegro_audio_h
  if (ui->actionSounds->isChecked())
    al_play_sample(snd_perfect_100, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, nullptr);
#endif
}

void MainWindow::online_perfect_100(int plr)
{
  QString mesg;

  if (plr == online_my_turn)
    mesg = tr("[Info]: You got the perfect 100!\n[Info]: Your score has been set to 50.");
  else
    mesg = tr("[Info]: Player '") + label[18 + plr]->text() + tr("' got the perfect 100!\n[Info]: Player '") +
                                    label[18 + plr]->text() + tr("' score has been set to 50.");

  message(mesg);

#ifdef __al_included_allegro5_allegro_audio_h
  if (ui->actionSounds->isChecked())
    al_play_sample(snd_perfect_100, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, nullptr);
#endif
}

void MainWindow::breaking_hearts()
{
  message(tr("[Info]: Hearts has been broken!"));

#ifdef __al_included_allegro5_allegro_audio_h
  if (ui->actionSounds->isChecked())
    al_play_sample(snd_breaking_heart, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, nullptr);
#endif
}

void MainWindow::game_over(int score1, int score2, int score3, int score4)
{
  QString mesg, mesg2;

  stats->increase_stats(0, STATS_GAME_FINISHED);

  clear_deck();

  if (!hearts->is_it_draw())
    mesg2 = tr("Won !");
  else
    mesg2 = tr("Drew !");

 int lowest = 0;
 int unsorted[4];

 unsorted[0] = score1;
 unsorted[1] = score2;
 unsorted[2] = score3;
 unsorted[3] = score4;

 for (int i=0; i<4; i++) {
   int cpt = 0;
   for (int i2=0; i2<4; i2++)
     if (unsorted[i] > unsorted[i2])
       cpt++;

   switch(cpt) {
     case 0 : stats->increase_stats(plr_names_idx[i], STATS_FIRST_PLACE);
              lowest = unsorted[i];
              break;
     case 1 : stats->increase_stats(plr_names_idx[i], STATS_SECOND_PLACE);
              break;
     case 2 : stats->increase_stats(plr_names_idx[i], STATS_THIRD_PLACE);
              break;
     case 3 : stats->increase_stats(plr_names_idx[i], STATS_FOURTH_PLACE);
              break;
   }
 }

 stats->set_score(plr_names_idx[0], score1);
 stats->set_score(plr_names_idx[1], score2);
 stats->set_score(plr_names_idx[2], score3);
 stats->set_score(plr_names_idx[3], score4);

 mesg = tr("[Info]: GAME OVER!\n[Info]: Player '")  + label[18]->text() + "': " +
         QString::number(score1) + tr(" point(s) ") + (score1 == lowest ? mesg2 : "") +
        tr("\n[Info]: Player '")                    + label[19]->text() + "': " +
         QString::number(score2) + tr(" point(s) ") + (score2 == lowest ? mesg2 : "") +
        tr("\n[Info]: Player '")                    + label[20]->text() + "': " +
         QString::number(score3) + tr(" point(s) ") + (score3 == lowest ? mesg2 : "") +
        tr("\n[Info]: Player '")                    + label[21]->text() + "': " +
         QString::number(score4) + tr(" point(s) ") + (score4 == lowest ? mesg2 : "");

 message(mesg);

#ifdef __al_included_allegro5_allegro_audio_h
  if (ui->actionSounds->isChecked())
    al_play_sample(snd_game_over, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, nullptr);
#endif

  if (!ui->actionInfo_Channel->isChecked())
    QMessageBox::information(this, tr("Information"), mesg);

  ui->actionNew->setDisabled(false);
}

void MainWindow::online_game_over(int north, int south, int west, int east)
{
  int lowest = south;
  bool drew = false;

  if ((north <= south) && (north <= west) && (north <= east)) {
    lowest = north;
  if ((north == south) || (north == west) || (north == east))
    drew = true;
  } else
      if ((south <= north) && (south <= west) && (south <= east)) {
        lowest = south;
        if ((south == north) || (south == west) || (south == east))
          drew = true;
      } else
          if ((west <= north) && (west <= south) && (west <= east)) {
            lowest = west;
            if ((west == north) || (west == south) || (west == east))
              drew = true;
          } else
              if ((east <= north) && (east <= south) && (east <= west)) {
                lowest = east;
                if ((east == north) || (east == south) || (east == west))
                  drew = true;
              }

  QString mesg, mesg2;

  if (drew)
    mesg2 = tr("Drew !");
  else
    mesg2 = tr("Won !");

  mesg = tr("[Info]: GAME OVER!\n[Info]: Player '") + label[18]->text() + "': " +
          QString::number(south) + tr(" point(s) ") + (south == lowest ? mesg2 : "") +
         tr("\n[Info]: Player '")                   + label[19]->text() + "': " +
          QString::number(west) + tr(" point(s) ")  + (west == lowest ? mesg2 : "") +
         tr("\n[Info]: Player '")                   + label[20]->text() + "': " +
          QString::number(north) + tr(" point(s) ") + (north == lowest ? mesg2 : "") +
         tr("\n[Info]: Player '")                   + label[21]->text() + "': " +
          QString::number(east) + tr(" point(s) ")  + (east == lowest ? mesg2 : "");

  message(mesg);

 #ifdef __al_included_allegro5_allegro_audio_h
   if (ui->actionSounds->isChecked())
     al_play_sample(snd_game_over, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, nullptr);
 #endif

   if (!ui->actionInfo_Channel->isChecked())
     QMessageBox::information(this, tr("Information"), mesg);
}

void MainWindow::tram(int plr)
{
  QString mesg;

  if (plr == hearts->whoami())
    mesg = tr("[Info]: You takes the rest!");
  else
    mesg = tr("[Info]: Player '") + label[18 + plr]->text() + tr("' takes the rest!");

  message(mesg);
  clear_deck();
}

void MainWindow::shoot_moon(int plr)
{
  stats->increase_stats(plr_names_idx[plr], STATS_SHOOT_MOON);

#ifdef __al_included_allegro5_allegro_audio_h
  if (ui->actionSounds->isChecked())
    al_play_sample(snd_shoot_moon, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, nullptr);
#endif

  QString mesg;

  if (plr == hearts->whoami()) {
    mesg = tr("[Info]: You shoot the moon!");

    if (ui->actionNew_Moon->isChecked() && (hearts->get_my_score() >= 26)) {
      message(mesg);

      QMessageBox msgBox(this);
      msgBox.setWindowTitle(tr("You shoot the moon!"));
      msgBox.setText(tr("What is your choice ?"));
      QPushButton *button_add = msgBox.addButton(tr("Add"), QMessageBox::YesRole);
      msgBox.addButton(tr("Subtract"), QMessageBox::NoRole);
      msgBox.setDefaultButton(button_add);
      msgBox.exec();
      if (msgBox.clickedButton() == button_add) {
         message(tr("[Info]: You added 26 pts to everyone's scores!"));
         hearts->set_moon_add_to_score(true);
      } else {
          message(tr("[Info]: You substracted 26 pts to your score!"));
          hearts->set_moon_add_to_score(false);
         }
      return;
    }
  } else {
      mesg = tr("[Info]: Player '") + label[18 + plr]->text() + tr("' shoot the moon!");
    }

  message(mesg);

  if (!ui->actionInfo_Channel->isChecked())
    QMessageBox::information(this, tr("information"), mesg);
}

// this is the online version of shoot_moon()
void MainWindow::shoot_moon(int plr, int delay)
{
#ifdef __al_included_allegro5_allegro_audio_h
  if (ui->actionSounds->isChecked())
    al_play_sample(snd_shoot_moon, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, nullptr);
#endif

  QString mesg;

  if (plr == online_my_turn) {
    mesg = tr("[Info]: You shoot the moon!");

    if (online_new_moon) {
      message(mesg);

      QMessageBox msgBox(this);
      msgBox.setWindowTitle(tr("You shoot the moon!"));
      msgBox.setText(tr("What is your choice ?"));
      QPushButton *button_add = msgBox.addButton(tr("Add"), QMessageBox::YesRole);
      if (delay)
        button_add->animateClick(delay * 1000);
      msgBox.addButton(tr("Subtract"), QMessageBox::NoRole);
      msgBox.setDefaultButton(button_add);
      msgBox.exec();
      if (msgBox.clickedButton() == button_add) {
         message(tr("[Info]: You added 26 pts to everyone's scores!"));

         client->send("moon +");
         remove_timer();
      } else {
         message(tr("[Info]: You substracted 26 pts to your score!"));

         client->send("moon -");
         remove_timer();
        }
      return;
    }
  }
  else {
    mesg = tr("[Info]: Player '") + label[18 + plr]->text() + tr("' shoot the moon!");
  }

  message(mesg);

  if (!ui->actionInfo_Channel->isChecked())
    QMessageBox::information(this, tr("information"), mesg);
}

void MainWindow::pass_to(int pass_to)
{
  if (!online_connected) {
    ui->actionNew->setDisabled(false);
    ui->actionConnect->setDisabled(false);
  }
  label[17]->setPixmap(QPixmap::fromImage(img_pass[pass_to]->scaledToHeight(80)));
  label[17]->setDisabled(pass_to == pNOPASS);
}

void MainWindow::refresh_score(int score, int idx)
{
  lcd_score[idx]->display(score);
}

void MainWindow::refresh_hand_score(int score, int idx)
{
  lcd_hand_score[idx]->display(score);
}

void MainWindow::end_of_hand(int score1, int score2, int score3, int score4)
{
  stats->increase_stats(0, STATS_HANDS_PLAYED);

#ifdef DEBUG
  debug->save_card(nullptr, deck->get_img_card(empty));
#endif

  lcd_hand_score[0]->display(score1);
  lcd_hand_score[1]->display(score2);
  lcd_hand_score[2]->display(score3);
  lcd_hand_score[3]->display(score4);

  QString mesg;

  mesg = tr("[Info]: New scores: '") + label[18]->text() + ": " +
          QString::number(lcd_score[0]->value()) + " (" + QString::number(lcd_hand_score[0]->value()) + ")', '" +
                                   label[19]->text() + ": " +
          QString::number(lcd_score[1]->value()) + " (" + QString::number(lcd_hand_score[1]->value()) + ")', '" +
                                   label[20]->text() + ": " +
          QString::number(lcd_score[2]->value()) + " (" + QString::number(lcd_hand_score[2]->value()) + ")', '" +
                                   label[21]->text() + ": " +
          QString::number(lcd_score[3]->value()) + " (" + QString::number(lcd_hand_score[3]->value()) + ")'";

  message(mesg);

#ifdef __al_included_allegro5_allegro_audio_h
  if (ui->actionSounds->isChecked() && !hearts->is_game_over()) {
    al_play_sample(snd_shuffling_cards, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, nullptr);
    delay(1500);
  }
#endif
}

void MainWindow::online_end_hand(int north, int south, int west, int east) {

#ifdef DEBUG
  debug->save_card(nullptr, deck->get_img_card(empty));
#endif

  lcd_score[0]->display(south);
  lcd_score[1]->display(west);
  lcd_score[2]->display(north);
  lcd_score[3]->display(east);

  QString mesg;

  mesg = tr("[Info]: New scores: '") + label[18]->text() + ": " +
          QString::number(lcd_score[0]->value()) + " (" + QString::number(lcd_hand_score[0]->value()) + ")', '" +
                                   label[19]->text() + ": " +
          QString::number(lcd_score[1]->value()) + " (" + QString::number(lcd_hand_score[1]->value()) + ")', '" +
                                   label[20]->text() + ": " +
          QString::number(lcd_score[2]->value()) + " (" + QString::number(lcd_hand_score[2]->value()) + ")', '" +
                                   label[21]->text() + ": " +
          QString::number(lcd_score[3]->value()) + " (" + QString::number(lcd_hand_score[3]->value()) + ")'";

  message(mesg);

  for (int i=0; i<4; i++)
    lcd_hand_score[i]->display(0);
}

void MainWindow::show_deck(int plr, bool refresh)
{
  active_deck = plr;

  if (online_connected) {
    online_show_deck();
    return;
  }

  int count = hearts->count_plr_cards(plr) - 1;

  for (int i=0; i<13; i++)
    label[i]->hide();

  for (int i=0; i<13; i++) {
    int card = hearts->get_card(plr, i);

    if (card == empty) break;

    int pos;
    if (ui->actionAuto_Centering->isChecked())
      pos = cards_position[count][i];
    else
      pos = i;

    assert ((pos >= 0) && (pos <= 12));

    label[pos]->show();

    label[pos]->setPixmap(QPixmap::fromImage(deck->get_img_card(card)->scaledToHeight(card_height)));
    if (hearts->is_card_selected(plr, i))
      label[pos]->move(label[pos]->x(),def_card_posy - 20);
    else
      label[pos]->move(label[pos]->x(),def_card_posy);

    if (ui->actionEasy_card_selection->isChecked()) {
      if (plr != hearts->whoami())
        label[pos]->setDisabled(false);
      else
      if (refresh)
        label[pos]->setDisabled(!hearts->is_card_selectable(card));
    }
  }
}

void MainWindow::online_show_deck()
{
  int count = online_num_cards - 1;

  for (int i=0; i<13; i++)
    label[i]->hide();

  for (int i=0; i<13; i++) {
    int card = online_myCards[i];

    if (card == empty) break;

    int pos;
    if (ui->actionAuto_Centering->isChecked())
      pos = cards_position[count][i];
    else
      pos = i;

    assert ((pos >= 0) && (pos <= 12));

    label[pos]->show();
    label[pos]->setPixmap(QPixmap::fromImage(deck->get_img_card(card)->scaledToHeight(card_height)));

    if (online_selected[i])
      label[pos]->move(label[pos]->x(),def_card_posy - 20);
    else
      label[pos]->move(label[pos]->x(),def_card_posy);
  }
}

void MainWindow::set_info_channel_enabled(bool enable)
{
 int new_height = height();

 if (enable) {
    new_height += ui->textEdit->height() + ui->lineEdit->height();
    if (new_height > max_mainwindow_height)
      new_height = max_mainwindow_height;

    setFixedHeight(new_height);
    ui->textEdit->show();
  } else {
      ui->textEdit->hide();

      new_height -= ui->textEdit->height() + ui->lineEdit->height();
      setFixedHeight(new_height);
    }
}

#ifdef DEBUG
void MainWindow::disable_cheat(bool full)
{
  cheat_radio_button[0]->hide();
  cheat_radio_button[1]->hide();
  cheat_radio_button[2]->hide();
  cheat_radio_button[3]->hide();

  if (full) {
    ui->actionCheat->setChecked(false);
    ui->actionCheat->setDisabled(true);
  }
}

void MainWindow::set_cheat_mode_enabled(bool enable)
{
  assert(hearts);

  if (enable) {
    cheat_radio_button[0]->show();
    cheat_radio_button[1]->show();
    cheat_radio_button[2]->show();
    cheat_radio_button[3]->show();
  } else {      
      disable_cheat(false);
      show_deck(hearts->whoami(), true);
    }

  cheat_radio_button[hearts->whoami()]->setChecked(true);
}
#endif

void MainWindow::refresh_deck(int plr, bool d)
{
#ifdef DEBUG
  cheat_radio_button[plr]->setChecked(true);
#endif

  show_deck(plr, d);
}

void MainWindow::clear_deck()
{
  for (int i=0; i<13; i++)
    label[i]->hide();
}

void MainWindow::clear_table()
{
  delay(200);
  for (int i=0; i<4; i++) {
    label[i+13]->setPixmap(QPixmap::fromImage(deck->get_img_card(empty)->scaledToHeight(card_height)));
    card_played[i] = empty;
  }
}

void MainWindow::online_select_card(int num)
{
  int card_id;

  if (ui->actionAuto_Centering->isChecked())
    card_id = rev_cards_position[online_num_cards - 1][num];
  else
    card_id = num;

  assert((card_id >= 0) && (card_id <= 12));

 if (online_playing) {
   QString p = "play " + QString::number(online_myCards[card_id]);

   client->send(p);

   return;
 }

 if (online_selected[card_id]) {
   online_selected[card_id] = false;
   online_num_selected--;
   label[card_id]->move(label[card_id]->x(), def_card_posy);
 } else
     if (!online_selected[card_id] && (online_num_selected < 3)) {
       online_selected[card_id] = true;
       online_num_selected++;
       label[card_id]->move(label[card_id]->x(), def_card_posy - 20);

#ifdef __al_included_allegro5_allegro_audio_h
       if (ui->actionSounds->isChecked())
         al_play_sample(snd_contact, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, nullptr);
#endif
     }
}

void MainWindow::select_card(int num)
{
  if (wait_delay) return;

  if (online_connected) {
    online_select_card(num);
    return;
  }

  if (active_deck != hearts->whoami()) return;

  int card_id;

  if (ui->actionAuto_Centering->isChecked())
    card_id = rev_cards_position[hearts->count_my_cards() - 1][num];
  else
    card_id = num;

  assert((card_id >= 0) && (card_id <= 12));

  if (hearts->is_mode_playing()) {
    if (!hearts->is_it_my_turn()) return;     // wait_delay seem to do the job, but maybe it's just by luck...

    ui->actionNew->setDisabled(true);
    ui->actionConnect->setDisabled(true);

    if (ui->actionEasy_card_selection->isChecked())
      set_cards_disabled(false);

    int error;

    if ((error = hearts->play_card(card_id))) {
      switch (error) {
         case ERRHEART: message(tr("[Error]: You can't break hearts yet!"));
                        break;
         case ERRSUIT:  message(tr("[Error]: You must follow the suit! The lead is in ") +
                                QString(suit_names[hearts->get_current_suit()]) + "!");
                        break;
         case ERRQUEEN: message(tr("[Error]: You can't play the queen of spade on the first hand!"));
                        break;
      }

#ifdef __al_included_allegro5_allegro_audio_h
      if (error && ui->actionSounds->isChecked())
        al_play_sample(snd_error, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, nullptr);
#endif  
    }

    ui->actionNew->setDisabled(false);
    ui->actionConnect->setDisabled(false);
    return;
 }

 if (hearts->is_card_selected(card_id) && hearts->unselect_card(card_id))
   label[card_id]->move(label[card_id]->x(), def_card_posy);
 else {
    if (!hearts->is_card_selected(card_id) && hearts->select_card(card_id)) {
      label[card_id]->move(label[card_id]->x(), def_card_posy - 20);

#ifdef __al_included_allegro5_allegro_audio_h
    if (ui->actionSounds->isChecked())
      al_play_sample(snd_contact, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, nullptr);
#endif
    }
 }
}

void MainWindow::play_card(int card, int idx)
{
  if (card == two_clubs)
    delay(700);
  else
    delay(400);

#ifdef DEBUG
 if (online_connected)
   debug->save_card(reinterpret_cast<const char *>(&online_names[idx]), deck->get_img_card(card));
 else
   debug->save_card(names[plr_names_idx[idx]], deck->get_img_card(card));
#endif

 card_played[idx] = card;

 label[idx+13]->setPixmap(QPixmap::fromImage(deck->get_img_card(card)->scaledToHeight(card_height)));

#ifdef __al_included_allegro5_allegro_audio_h
 if (ui->actionSounds->isChecked()) {
    al_play_sample(snd_dealing_card, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, nullptr);

    if (card == queen_spade)
      al_play_sample(snd_queen_spade, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, nullptr);
 }
#endif
}

void MainWindow::show_your_turn(int idx)
{
  delay(200);

  show_deck(active_deck, true);

  label[idx+13]->setPixmap(QPixmap::fromImage(deck->get_img_card(your_turn)->scaledToHeight(card_height)));
  card_played[idx] = your_turn;

#ifdef __al_included_allegro5_allegro_audio_h
  if (ui->actionSounds->isChecked())
    al_play_sample(snd_your_turn, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, nullptr);
#endif

  if (!online_connected) {
    ui->actionNew->setDisabled(false);
    ui->actionConnect->setDisabled(false);
  }
}

void MainWindow::online_pass_cards()
{
  if (online_passto == pNOPASS) return;

  if (online_num_selected != 3) {
    message(tr("[Error]: You needs to select 3 cards to pass!"));

#ifdef __al_included_allegro5_allegro_audio_h
    if (ui->actionSounds->isChecked())
      al_play_sample(snd_error, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, nullptr);
#endif
      return;
  }

  label[17]->setDisabled(true);

  int n = 0;
  QString s = "pass ";

  for (int i=0; i<13; i++)
    if (online_selected[i]) {
      assert(n < 3);
      s.append(QString::number(i) + " ");
      online_cards_received_pos[n] = i;
      n++;
    }

  client->send(s);
  remove_timer();
  wait_delay = true;  // this will disable cards select, while waiting to receive more from the server
}

// This function is called when the pass icon is clicked()
void MainWindow::on_label_18_clicked()
{  
  if (online_connected) {
    online_pass_cards();
    return;
  }

  if (hearts->is_no_pass() || (active_deck != hearts->whoami()))
    return;

  if (!hearts->is_ready_to_pass()) {
    message(tr("[Error]: You needs to select 3 cards to pass!"));

 #ifdef __al_included_allegro5_allegro_audio_h
    if (ui->actionSounds->isChecked())
      al_play_sample(snd_error, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, nullptr);
 #endif

    return;
  }

  label[17]->setDisabled(true);

  ui->actionNew->setDisabled(true);
  ui->actionConnect->setDisabled(true);

  hearts->AI_pass_cpus_cards();
  hearts->pass_cards();

  int ptr = 0;
  for (int i=0; i<13; i++)
    if (hearts->is_card_selected(i))
      cards_received[ptr++] = hearts->get_card(i);

  hearts->reset_cards_passed();

  reverse_cards_rgb();
  show_deck(hearts->whoami(), true);

#ifdef __al_included_allegro5_allegro_audio_h
  if (ui->actionSounds->isChecked())
    al_play_sample(snd_passing_cards, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, nullptr);
#endif

  delay(2000);

  hearts->sort_plr_cards();

  reverse_cards_rgb();

  show_deck(hearts->whoami(), true);

  hearts->play_2clubs();
}

void MainWindow::set_cards_disabled(bool d)
{
 for (int i=0; i<13; i++)
   label[i]->setDisabled(d);
}

void MainWindow::reverse_cards_rgb()
{
  deck->reverse_card_rgb(cards_received[0]);
  deck->reverse_card_rgb(cards_received[1]);
  deck->reverse_card_rgb(cards_received[2]);
}

void MainWindow::flush_deck()
{
  for (int i=0; i<13; i++)
    label[i]->setPixmap(QPixmap::fromImage(deck->get_img_card(empty)->scaledToHeight(card_height)));

  for (int i=0; i<4; i++) {
    label[i + 13]->setPixmap(QPixmap::fromImage(deck->get_img_card(empty)->scaledToHeight(card_height)));
  }
}

void MainWindow::refresh_cards_played()
{
  for (int i=0; i<4; i++) {
    int card = card_played[i];

    label[13 + i]->setPixmap(QPixmap::fromImage(deck->get_img_card(card)->scaledToHeight(card_height)));
  }
}

void MainWindow::message(QString mesg)
{
  ui->textEdit->append(mesg);
}

void MainWindow::delay(int n)
{
  if (online_connected)
    return;

  wait_delay = true;
  QTime dieTime= QTime::currentTime().addMSecs(n);
  while ((QTime::currentTime() < dieTime) && !stop_delay)
    QCoreApplication::processEvents(QEventLoop::AllEvents, 2000);
  wait_delay = false;
}

#ifdef DEBUG
void MainWindow::on_radioButton_clicked()
{
  show_deck(0, true);
}

void MainWindow::on_radioButton_2_clicked()
{
  show_deck(1, true);
}

void MainWindow::on_radioButton_3_clicked()
{
  show_deck(2, true);
}

void MainWindow::on_radioButton_4_clicked()
{
  show_deck(3, true);
}
#endif

void MainWindow::on_label_clicked()
{
  select_card(0);
}

void MainWindow::on_label_2_clicked()
{
  select_card(1);
}

void MainWindow::on_label_3_clicked()
{
  select_card(2);
}

void MainWindow::on_label_4_clicked()
{
  select_card(3);
}

void MainWindow::on_label_5_clicked()
{
  select_card(4);
}

void MainWindow::on_label_6_clicked()
{
  select_card(5);
}

void MainWindow::on_label_7_clicked()
{
  select_card(6);
}

void MainWindow::on_label_8_clicked()
{
  select_card(7);
}

void MainWindow::on_label_9_clicked()
{
  select_card(8);
}

void MainWindow::on_label_10_clicked()
{
  select_card(9);
}

void MainWindow::on_label_11_clicked()
{
  select_card(10);
}

void MainWindow::on_label_12_clicked()
{
  select_card(11);
}

void MainWindow::on_label_13_clicked()
{
  select_card(12);
}

void MainWindow::on_actionNew_triggered()
{
  stats->increase_stats(0, STATS_GAME_STARTED);

#ifdef DEBUG
  debug->reset();
#endif

  message(tr("[Info]: Starting a new game."));

  stop_delay = true;
  hearts->new_game();
  stop_delay = false;
  set_plr_names();
  set_cards_disabled(false);
}

#ifdef DEBUG
void MainWindow::on_actionCheat_triggered()
{
  bool checked = ui->actionCheat->isChecked();

  set_cheat_mode_enabled(checked);
  config->set_config_file(CONFIG_CHEAT_MODE, checked);
}
#endif

void MainWindow::on_actionAuto_Centering_triggered()
{
  show_deck(active_deck, false);

  config->set_config_file(CONFIG_AUTO_CENTERING, ui->actionAuto_Centering->isChecked());
}

void MainWindow::on_actionTram_triggered()
{
  bool checked = ui->actionTram->isChecked();

  hearts->set_tram_enabled(checked);
  config->set_config_file(CONFIG_DETECT_TRAM, checked);
}

void MainWindow::on_actionInfo_Channel_triggered()
{
  bool checked = ui->actionInfo_Channel->isChecked();

  set_info_channel_enabled(checked);
  config->set_config_file(CONFIG_INFO_CHANNEL, checked);
}

void MainWindow::on_actionSounds_triggered()
{
  config->set_config_file(CONFIG_SOUNDS, ui->actionSounds->isChecked());
}

void MainWindow::on_actionPerfect_100_triggered()
{
  bool checked = ui->actionPerfect_100->isChecked();

  hearts->set_perfect_100(checked);
  config->set_config_file(CONFIG_PERFECT_100, checked);
}

void MainWindow::on_actionOmnibus_triggered()
{
  bool checked = ui->actionOmnibus->isChecked();

  hearts->set_omnibus(checked);
  config->set_config_file(CONFIG_OMNIBUS, checked);
}

void MainWindow::on_actionQueen_Spade_Break_Heart_triggered()
{
  bool checked = ui->actionQueen_Spade_Break_Heart->isChecked();

  hearts->set_queen_spade_break_heart(checked);
  config->set_config_file(CONFIG_QUEEN_SPADE_BREAK_HEART, checked);
}

void MainWindow::on_actionNo_Trick_Bonus_triggered()
{
  bool checked = ui->actionNo_Trick_Bonus->isChecked();

  hearts->set_no_trick_bonus(checked);
  config->set_config_file(CONFIG_NO_TRICK_BONUS, checked);
}

void MainWindow::on_actionNew_Moon_triggered()
{
  bool checked = ui->actionNew_Moon->isChecked();

  hearts->set_new_moon(checked);
  config->set_config_file(CONFIG_NEW_MOON, checked);
}

void MainWindow::on_actionNo_Draw_triggered()
{
  bool checked = ui->actionNo_Draw->isChecked();

  hearts->set_no_draw(checked);
  config->set_config_file(CONFIG_NO_DRAW, checked);
}

void MainWindow::on_actionSave_Game_Quit_triggered()
{
  config->set_config_file(CONFIG_SAVE_GAME, ui->actionSave_Game_Quit->isChecked());
}

void MainWindow::on_actionRules_triggered()
{
  Rules rules_diag(this);
  rules_diag.setModal(true);
  rules_diag.exec();
}

void MainWindow::on_actionCredits_triggered()
{
  Credits credits_diag(this);
  credits_diag.setModal(true);
  credits_diag.exec();
}

void MainWindow::on_actionSettings_triggered()
{
  Settings settings_diag(this);
  settings_diag.setModal(true);
  settings_diag.exec();
}

void MainWindow::on_actionOnline_triggered()
{
 online online_diag(this);
 online_diag.setModal(true);
 online_diag.exec();
}

void MainWindow::on_actionReset_triggered()
{
  QMessageBox msgBox(this);
  msgBox.setText(tr("Do you want to reset statistics?"));
  msgBox.setInformativeText(tr("Are you sure?"));
  msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
  msgBox.setDefaultButton(QMessageBox::No);
  int ret = msgBox.exec();
  if (ret == QMessageBox::Yes) {
     stats->reset();
     message(tr("[Info]: You resetted the statistics!"));
  }
}

void MainWindow::on_actionShow_triggered()
{
  stats->show();
}

#ifdef DEBUG
void MainWindow::on_actionShow_2_triggered()
{
  debug->show();
}
#endif


void MainWindow::fit_button(QPushButton *button, int x, int width)
{
  int y = ui->pushButton->y();

  button->setFixedWidth(width);
  button->move(x, y);
}

void MainWindow::set_language(int lang)
{
 ui->actionEnglish->setChecked(false);
 ui->actionFrench->setChecked(false);
 ui->actionRussian->setChecked(false);

 qApp->removeTranslator(&translator);

 switch (lang) {
    case LANG_ENGLISH: if (translator.load(QLocale(QLocale::English), QLatin1String("translation"), QLatin1String("_"), QLatin1String(":/languages"))) {
                         qApp->installTranslator(&translator);

                         ui->actionEnglish->setChecked(true);
                         fit_button(ui->pushButton, 10, 40);
                         fit_button(ui->pushButton_2, 50, 55);
                         fit_button(ui->pushButton_3, 105, 55);
                         fit_button(ui->pushButton_4, 160, 100);
                         fit_button(ui->pushButton_5, 260, 50);
                         fit_button(ui->pushButton_7, 310, 50);
                       }
                       break;
    case LANG_FRENCH:  if (translator.load(QLocale(QLocale::French), QLatin1String("translation"), QLatin1String("_"), QLatin1String(":/languages"))) {
                         qApp->installTranslator(&translator);

                         ui->actionFrench->setChecked(true);
                         fit_button(ui->pushButton, 10, 60);
                         fit_button(ui->pushButton_2, 70, 65);
                         fit_button(ui->pushButton_3, 135, 55);
                         fit_button(ui->pushButton_4, 190, 90);
                         fit_button(ui->pushButton_5, 280, 75);
                         fit_button(ui->pushButton_7, 355, 80);
                       }
                       break;
    case LANG_RUSSIAN: if (translator.load(QLocale(QLocale::Russian), QLatin1String("translation"), QLatin1String("_"), QLatin1String(":/languages"))) {
                         qApp->installTranslator(&translator);

                         ui->actionRussian->setChecked(true);
                         fit_button(ui->pushButton, 10, 60);
                         fit_button(ui->pushButton_2, 70, 55);
                         fit_button(ui->pushButton_3, 125, 80);
                         fit_button(ui->pushButton_4, 205, 135);
                         fit_button(ui->pushButton_5, 340, 80);
                         fit_button(ui->pushButton_7, 420, 65);
                       }
                       break;
 }

 ui->retranslateUi(this);
 table_list->Translate();
 stats->Translate();

#ifndef DEBUG
  ui->menuDebug->setTitle("");
  ui->menuDebug->setEnabled(false);
#endif

}

void MainWindow::on_actionEnglish_triggered(bool checked)
{
  if (!checked) {
    ui->actionEnglish->setChecked(true);
    return;
  }

  set_language(LANG_ENGLISH);

  config->set_language(LANG_ENGLISH);
}

void MainWindow::on_actionFrench_triggered(bool checked)
{
  if (!checked) {
    ui->actionFrench->setChecked(true);
    return;
  }

  set_language(LANG_FRENCH);

  config->set_language(LANG_FRENCH);
}

void MainWindow::on_actionRussian_triggered(bool checked)
{
  if (!checked) {
    ui->actionRussian->setChecked(true);
    return;
  }

  set_language(LANG_RUSSIAN);

  config->set_language(LANG_RUSSIAN);
}

void MainWindow::on_actionEasy_card_selection_triggered()
{
 bool checked = ui->actionEasy_card_selection->isChecked();

 if (checked)
   show_deck(active_deck, true);
 else
   set_cards_disabled(false);

 config->set_config_file(CONFIG_EASY_CARD_SELECTION, checked);
}

void MainWindow::on_actionStandard_triggered()
{
  ui->actionStandard->setChecked(true);
  ui->actionEnglish_2->setChecked(false);
  ui->actionRussian_2->setChecked(false);

  if (config->get_deck_style() == STANDARD_DECK)
    return;

#ifdef DEBUG
  debug->reset();
#endif

  flush_deck();

  deck->set_deck(STANDARD_DECK);
  show_deck(active_deck, true);
  refresh_cards_played();
  config->set_deck_style(STANDARD_DECK);
}

void MainWindow::on_actionEnglish_2_triggered()
{
  ui->actionStandard->setChecked(false);
  ui->actionEnglish_2->setChecked(true);
  ui->actionRussian_2->setChecked(false);

  if (config->get_deck_style() == ENGLISH_DECK)
    return;

#ifdef DEBUG
  debug->reset();
#endif

  flush_deck();

  deck->set_deck(ENGLISH_DECK);
  show_deck(active_deck, true);
  refresh_cards_played();
  config->set_deck_style(ENGLISH_DECK);
}

void MainWindow::on_actionRussian_2_triggered()
{
  ui->actionStandard->setChecked(false);
  ui->actionEnglish_2->setChecked(false);
  ui->actionRussian_2->setChecked(true);

  if (config->get_deck_style() == RUSSIAN_DECK)
    return;

#ifdef DEBUG
  debug->reset();
#endif

  flush_deck();

  deck->set_deck(RUSSIAN_DECK);
  show_deck(active_deck, true);
  refresh_cards_played();
  config->set_deck_style(RUSSIAN_DECK);
}

void MainWindow::on_lineEdit_returnPressed()
{
  QString data;

  switch (online_button_mode) {
     case mode_button_command: break;
     case mode_button_help: data = "help "; break;
     case mode_button_says: data = "say "; break;
  }

  data += ui->lineEdit->text();

  client->send(data);
  message(data);

  ui->lineEdit->clear();
}

void MainWindow::on_actionConnect_triggered()
{
  bool warning = config->Warning();

  Connect connect_diag(this);
  connect_diag.setModal(true);
  connect_diag.config(config->Username(), config->Password(), warning);

  connect_diag.exec();

  client->setHandle(connect_diag.getHandle());
  client->setPassword(connect_diag.getPassword());

  config->set_online(connect_diag.getHandle(), connect_diag.getPassword());

  if (warning && connect_diag.WarningDisabled())
    config->set_config_file(CONFIG_WARNING, false);

  if (connect_diag.WarningAccepted())
    switch (connect_diag.result()) {
      case QDialog::Accepted: client->socketConnect(true, connect_diag.getHost(), connect_diag.getPort());
                              break;
      case QDialog::Rejected:
                              break;
      default: client->socketConnect(false, connect_diag.getHost(), connect_diag.getPort());
    }
}

void MainWindow::on_actionCreate_Table_triggered()
{
  unsigned int flags = 0;

  CGame table(this);
  table.setModal(true);

  if (ui->actionQueen_Spade_Break_Heart->isChecked())
    flags = QUEEN_SPADE_f;

  if (ui->actionPerfect_100->isChecked())
    flags |= PERFECT_100_f;

  if (ui->actionOmnibus->isChecked())
    flags |= OMNIBUS_f;

  if (ui->actionNo_Trick_Bonus->isChecked())
    flags |= NO_TRICK_BONUS_f;

  if (ui->actionNew_Moon->isChecked())
    flags |= NEW_MOON_f;

  if (ui->actionNo_Draw->isChecked())
    flags |= NO_DRAW_f;

  table.set_flags(flags);

  table.exec();

  if (table.result() == QDialog::Accepted) {
    QString m;

    m.sprintf("new %d", table.get_flags());
    client->send(m);
  }
}

// South
void MainWindow::on_label_14_clicked()
{
  if (online_can_sit)
    client->send("sit s");
}

// North
void MainWindow::on_label_16_clicked()
{
  if (online_can_sit)
    client->send("sit n");
}

// West
void MainWindow::on_label_15_clicked()
{
  if (online_can_sit)
    client->send("sit w");
}

// East
void MainWindow::on_label_17_clicked()
{
  if (online_can_sit)
    client->send("sit e");
}

void MainWindow::init_online_game()
{
  online_table_id = 0;

  for (int i=0; i<4; i++)
    card_played[i] = empty;

  for (int i=0; i<13; i++) {
     online_myCards[i] = back_card;
     online_selected[i] = false;
     online_num_cards = 13;
  }

  // Clear players's name
  label[18]->setText("");
  label[19]->setText("");
  label[20]->setText("");
  label[21]->setText("");

  // Clear score and hand score
  for (int i=0; i<4; i++) {
    lcd_hand_score[i]->display(0);
    lcd_score[i]->display(0);
  }

  pass_to(pNOPASS);
  flush_deck();

  // set all the 13 cards to back cards
  for (int i=0; i<13; i++) {
    label[i]->setPixmap(QPixmap::fromImage(deck->get_img_card(back_card)->scaledToHeight(card_height)));
    label[i]->setEnabled(true);
    label[i]->show();
  }

  reset_cards_pos();
}

int MainWindow::get_name_label(QString p)
{
  if (p == "n")
    return 20;
  if (p == "s")
    return 18;
  if (p == "w")
    return 19;
  if (p == "e")
    return 21;

  return 0;
}

void MainWindow::online_action(unsigned int action, QString param)
{
  QString m, name;
  QStringList pList = param.split(" ");

  int wait, c1, c2, c3, p1, p2, p3;
  int north, south, west, east;
  long int s1, s2, s3, s4;

#ifdef DEBUG_DATAGRAMS
  m = "Action: " + QString::number(action) + " Param : " + param;
  message(m);
#endif

  switch (action) {
    case ACTION_CONNECTED:
           // Save the current game
           save_files();

           online_connected = true;
           wait_delay = true; // disable cards select

           // Disable New & Connect Buttons
           ui->actionConnect->setDisabled(true);
           ui->actionNew->setDisabled(true);

#ifdef DEBUG
           debug->reset();
           disable_cheat(true);
#endif

           light_connected(true);
           online_show_buttons(true);

           init_online_game();

#ifdef __al_included_allegro5_allegro_audio_h
            if (ui->actionSounds->isChecked())
              al_play_sample(snd_connected, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, nullptr);
#endif
           break;
    case ACTION_CHOOSE_CHAIR:
            if (pList.size() != 1) {
              message("ERROR: ACTION_CHOOSE_CHAIR");
              break;
            }
            online_can_sit = true;
            online_table_id = pList.at(0).toInt();

            for (int i=0; i<4; i++) {
              card_played[i] = sit_here;
              label[i + 13]->setPixmap(QPixmap::fromImage(deck->get_img_card(sit_here)->scaledToHeight(card_height)));
            }

            break;
    case ACTION_SIT_CHAIR:
            // 0 = table_id   1 = n,s,w,e   2 = bool muted, 3 = player name
           if (pList.size() != 4) {
             message("ERROR: ACTION_SIT_CHAIR");
             break;
           }

           // update the game list window
           table_list->SetPlayer(pList.at(0), pList.at(3), pList.at(1));

           // not my table --> break
           if (pList.at(0).toInt() != online_table_id) break;

           ui->pushButton_5->setDisabled(pList.at(2).toInt());

           c1 = get_name_label(pList.at(1));
           if (c1) {
             QByteArray ba = pList.at(3).toLocal8Bit();
             strncpy(online_names[c1-18], ba.data(), 20);
             label[c1]->setText(pList.at(3));
           }
           break;
    case ACTION_MY_CHAIR:
            if (pList.size() != 1) {
              message("ERROR: ACTION_MY_CHAIR");
              break;
             }
            online_my_turn = pList.at(0).toInt();
            online_game_started = false;
            break;
    case ACTION_STAND_CHAIR:
            if (pList.size() != 2) {
              message("ERROR: ACTION_STAND_CHAIR");
              break;
            }

            // update the game list window
            table_list->SetPlayer(pList.at(0), "", pList.at(1));

            // not my table --> break
            if (pList.at(0).toInt() != online_table_id) break;

            c1 = get_name_label(pList.at(1));

            if (online_game_started)
              name = "(" + label[c1]->text() + ")";
            else
              name = "";

            label[c1]->setText(name);

            if (!online_game_started)
              label[c1-5]->setPixmap(QPixmap::fromImage(deck->get_img_card(sit_here)->scaledToHeight(card_height)));

            break;
    case ACTION_CREATE_TABLE:
            if (pList.size() != 2) {
               message("ERROR: ACTION_CREATE_TABLE");
               break;
             }
             table_list->AddRow(pList.at(0), pList.at(1));
            break;
    case ACTION_DELETE_TABLE:
            table_list->RemoveRow(param);
            if (online_table_id == param.toInt()) {
              init_online_game();
            }
            break;
    case ACTION_DISCONNECTED:
            // If you connect to a wrong server. You won't receive the datagram for ACTION_CONNECTED.
            // online_connected will be false, and the current game unsaved.
            // If after that you connect to a heart server. The previous connection will trigger
            // ACTION_DISCONNECTED that would call start_game(), and because it was unsaved it will start a
            // new game. On the next disconnection the original unsaved game will be lost and replaced.
            // So, if online_connected is false we must break.
            if (!online_connected)
              break;

            remove_timer();

            table_list->Empty();

            light_connected(false);
            online_show_buttons(false);

            ui->actionConnect->setDisabled(false);
            ui->actionNew->setDisabled(false);
            ui->actionCheat->setDisabled(false);

#ifdef __al_included_allegro5_allegro_audio_h
            if (ui->actionSounds->isChecked())
              al_play_sample(snd_disconnected, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, nullptr);
#endif
            online_connected = false;

            reset_cards_pos();

            start_game();
            break;
    case ACTION_SET_CARDS:
            if (pList.size() != 15) {
              message("ERROR: ACTION_SET_CARDS");
              break;
            }

            for (int i=0; i<13;i++)
               online_selected[i] = false;

            online_passto = pList.at(0).toInt();
            wait = pList.at(1).toInt();

            for (int i=2; i<pList.size(); i++)
              online_myCards[i-2] = pList.at(i).toInt();

            online_game_started = true;
            online_can_sit = false;
            online_playing = false;
            online_num_cards = pList.size() - 2;
            online_num_selected = 0;

            pass_to(online_passto);
            show_deck(0, false);

            wait_delay = online_passto == pNOPASS;

            activate_timer(wait);
            break;
    case ACTION_FORCE_PASS:
            remove_timer();
            if (pList.size() != 3) {
              message("ERROR: ACTION_FORCE_PASS(1)");
              return;
            }
            c1 = pList.at(0).toInt();
            c2 = pList.at(1).toInt();
            c3 = pList.at(2).toInt();
            if ((c1 < 0) || (c2 < 0) || (c3 < 0) || (c1 > 12) || (c2 > 12) || (c3 > 12)) {
              message("ERROR: ACTION_FORCE_PASS(2)");
              break;
            }
            online_cards_received_pos[0] = c1;
            online_cards_received_pos[1] = c2;
            online_cards_received_pos[2] = c3;
            wait_delay = true;
            label[17]->setDisabled(true);
            reset_cards_pos();
            label[c1]->move(label[c1]->x(), def_card_posy - 20);
            label[c2]->move(label[c2]->x(), def_card_posy - 20);
            label[c3]->move(label[c3]->x(), def_card_posy - 20);
            break;
    case ACTION_RECEIVE_CARDS:
            if (pList.size() != 3) {
               message("ERRORE: ACTION_RECEIVE_CARDS");
               break;
            }
            pList.sort();
#ifdef __al_included_allegro5_allegro_audio_h
            if (ui->actionSounds->isChecked())
              al_play_sample(snd_passing_cards, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, nullptr);
#endif
            online_heart_broken = false;
            p1 = online_cards_received_pos[0];
            p2 = online_cards_received_pos[1];
            p3 = online_cards_received_pos[2];
            c1 = pList.at(0).toInt();
            c2 = pList.at(1).toInt();
            c3 = pList.at(2).toInt();
            cards_received[0] = c1;
            cards_received[1] = c2;
            cards_received[2] = c3;
            online_myCards[p1] = c1;
            online_myCards[p2] = c2;
            online_myCards[p3] = c3;
            std::sort(online_myCards, online_myCards+13);
            reset_cards_pos();
            reverse_cards_rgb();
            label[p1]->setPixmap(QPixmap::fromImage(deck->get_img_card(c1)->scaledToHeight(card_height)));
            label[p2]->setPixmap(QPixmap::fromImage(deck->get_img_card(c2)->scaledToHeight(card_height)));
            label[p3]->setPixmap(QPixmap::fromImage(deck->get_img_card(c3)->scaledToHeight(card_height)));
            reverse_cards_rgb();
            for (int i=0; i<13; i++)
              online_selected[i] = false;
            show_deck(0, 0);
            break;
    case ACTION_YOUR_TURN:
            if (pList.size() != 1) {
              message("ERROR: ACTION_YOUR_TURN");
            } else
                activate_timer(pList.at(0).toInt());
            wait_delay = false;                        // enable card playing
            show_your_turn(online_my_turn);
            online_playing = true;
            break;
    case ACTION_PLAY:
            wait_delay = true;                         // disable card playing
            if (pList.size() != 2) {
              message("ERROR: ACTION PLAY");
              break;
            }
            c1 = pList.at(0).toInt();
            if ((c1 < 0) || (c1 > 3)) {
              message("ERROR: ACTION PLAY(2)");
              break;
            }
            c2 = pList.at(1).toInt();
            if ((c2 < 0) || (c2 > DECK_SIZE - 1)) {
              message("ERROR: ACTION PLAY(3)");
              break;
            }
            if (!online_heart_broken && (c2 / 13 == HEART)) {
              online_heart_broken = true;
              breaking_hearts();
            }
            play_card(c2, c1);
            for (int i=0; i<13; i++)
                if (online_myCards[i] == c2) {
                  online_myCards[i] = empty;
                  online_num_cards--;
                  break;
                }
            std::sort(online_myCards, online_myCards+13);
            remove_timer();
            show_deck(0, 0);
            break;
    case ACTION_HAND_SCORE:
            if (pList.size() != 2) {
              message("ERROR: ACTION_HAND_SCORE");
              break;
            }
            c1 = pList.at(0).toInt();
            c2 = pList.at(1).toInt();
            refresh_hand_score(c2, c1);
            break;
    case ACTION_SCORE:
            clear_table();

            if (pList.size() != 4) {
              message("ERROR: ACTION_SCORE");
              break;
            }
            north = pList.at(0).toInt(); // score NORTH
            south = pList.at(1).toInt(); // score SOUTH
            west = pList.at(2).toInt();  // score WEST
            east = pList.at(3).toInt();  // score EAST

            online_end_hand(north, south, west, east);
            break;
    case ACTION_SHOOT_MOON:
            if (pList.size() != 2) {
              message("ERROR: ACTION_SHOOT_MOON");
              break;
            }
            c1 = pList.at(0).toInt();
            c2 = pList.at(1).toInt();
            assert((c1 >= 0) && (c1 <= 3));
            if (c2) {
              activate_timer(c2);
              online_new_moon = true;
            } else
                online_new_moon = false;
            shoot_moon(c1, c2);
            break;
    case ACTION_BONUS_NO_TRICKS:
            if (pList.size() != 2) {
              message("ERROR: ACTION_BONUS_NO_TRICKS");
              break;
            }
            c1 = pList.at(0).toInt();
            c2 = pList.at(1).toInt();

            c3 = lcd_hand_score[c1]->intValue() - c2;
            refresh_hand_score(c3, c1);
            online_receive_bonus(c1, NO_TRICK_BONUS, c2);
            break;
    case ACTION_BONUS_OMNIBUS:
            if (pList.size() != 2) {
              message("ERROR: ACTION_OMNIBUS");
              break;
            }
            c1 = pList.at(0).toInt();
            c2 = pList.at(1).toInt();

            c3 = lcd_hand_score[c1]->intValue() - c2;
            refresh_hand_score(c3, c1);
            online_receive_bonus(c1, OMNIBUS_BONUS, c2);
            break;
    case ACTION_PERFECT_100:
            if (pList.size() != 2) {
              message("ERROR: ACTION_PERFECT_100");
              break;
            }
            online_perfect_100(pList.at(0).toInt());
            break;
    case ACTION_GAMEOVER:
            if (pList.size() != 4) {
              message("ERROR: ACTION_SCORE");
              break;
            }
            remove_timer();
            north = pList.at(0).toInt(); // score NORTH
            south = pList.at(1).toInt(); // score SOUTH
            west = pList.at(2).toInt();  // score WEST
            east = pList.at(3).toInt();  // score EAST
            online_game_over(north, south, west, east);
            break;
    case ACTION_ANNOUNCEMENT:
#ifdef __al_included_allegro5_allegro_audio_h
            if (ui->actionSounds->isChecked())
              al_play_sample(snd_announcement, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, nullptr);
#endif
            if (param.isEmpty()) {
              message("ERROR: ACTION_ANNOUNCEMENT");
              break;
            }
            m = tr("**** ANNOUNCEMENT from ") + param + " ****";
            message(m);
            break;
    case ACTION_PLAYER_MOON:
            clear_table();
            if (pList.size() != 2) {
              message("ERROR: ACTION_PLAYER_MOON");
              return;
            }
            c1 = pList.at(0).toInt(); // player
            c2 = pList.at(1).toInt(); // bool 1=add, 0=subs
            if (c2) {
              if (c1 != 0) refresh_hand_score(26, 0);
              if (c1 != 1) refresh_hand_score(26, 1);
              if (c1 != 2) refresh_hand_score(26, 2);
              if (c1 != 3) refresh_hand_score(26, 3);
              refresh_hand_score(0, c1);
            } else {
                 if (c1 != 0) refresh_hand_score(0, 0);
                 if (c1 != 1) refresh_hand_score(0, 1);
                 if (c1 != 2) refresh_hand_score(0, 2);
                 if (c1 != 3) refresh_hand_score(0, 3);
                 refresh_hand_score(-26, c1);
              }
            break;
    case ACTION_LEAVE_TABLE:
            remove_timer();
            init_online_game();
            break;
    case ACTION_CLEAR_TABLE:
            clear_table();
            break;
    case ACTION_TABLE_MUTED:
            ui->pushButton_5->setDisabled(true);
            message(param);
            break;
    case ACTION_SHUFFLE:
#ifdef __al_included_allegro5_allegro_audio_h
            if (ui->actionSounds->isChecked() && !hearts->is_game_over())
              al_play_sample(snd_shuffling_cards, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, nullptr);
#endif
            break;
    case ACTION_STATS:
            if (pList.size() != 4) {
               message("ERROR: ACTION_STATS");
               return;
            }
            s1 = pList.at(0).toLong();
            s2 = pList.at(1).toLong();
            s3 = pList.at(2).toLong();
            s4 = pList.at(3).toLong();
            m = tr("Your online statistics:\r\n") +
                tr("Games  : ")     + QString::number(s1+s2+s3+s4) +
                tr("\r\nFirst  : ") + QString::number(s1) +
                tr("\r\nSecond : ") + QString::number(s2) +
                tr("\r\nThird  : ") + QString::number(s3) +
                tr("\r\nFourth : ") + QString::number(s4);
            message(m);
            break;
    case ACTION_GAME_STARTED:
            if (pList.size() != 1) {
              message("ERROR: ACTION_GAME_STARTED");
              return;
            }
            clear_table();
            table_list->RemoveRow(param);
            break;
    case ACTION_RECONNECTED:
      // 0   = table id
      // 1   = score S
      // 2   = score W
      // 3   = score N
      // 4   = score E
      // 5   = hand score S
      // 6   = hand score W
      // 7   = hand score N
      // 8   = hand score E
      // 9   = card played S
      // 10  = card played W
      // 11  = card played N
      // 12  = card played E
      // 13  = my chair
      // 14  = Status
      // 15  = Time left
      // 16  = game passto
      // 17  = bool heart broken
      // 18 19 20 21 22 23 24 25 26 27 28 29 30 = my cards (empty card included)
            if (pList.size() != 31) {
              message("ERROR: ACTION_RECONNECTED");
              return;
            }
            online_table_id = pList.at(0).toInt();

            south = pList.at(1).toInt();
            west  = pList.at(2).toInt();
            north = pList.at(3).toInt();
            east  = pList.at(4).toInt();
            lcd_score[PLAYER_SOUTH]->display(south);
            lcd_score[PLAYER_WEST]->display(west);
            lcd_score[PLAYER_NORTH]->display(north);
            lcd_score[PLAYER_EAST]->display(east);

            south = pList.at(5).toInt();
            west  = pList.at(6).toInt();
            north = pList.at(7).toInt();
            east  = pList.at(8).toInt();
            lcd_hand_score[PLAYER_SOUTH]->display(south);
            lcd_hand_score[PLAYER_WEST]->display(west);
            lcd_hand_score[PLAYER_NORTH]->display(north);
            lcd_hand_score[PLAYER_EAST]->display(east);

            card_played[PLAYER_SOUTH] = pList.at(9).toInt();
            card_played[PLAYER_WEST]  = pList.at(10).toInt();
            card_played[PLAYER_NORTH] = pList.at(11).toInt();
            card_played[PLAYER_EAST]  = pList.at(12).toInt();
            refresh_cards_played();

            online_my_turn = pList.at(13).toInt();
            c1 = pList.at(14).toInt(); // status
            c2 = pList.at(15).toInt(); // timeleft

            wait_delay = true;
            online_playing = true;

            if (c1 == STATUS_YOUR_TURN) {
              wait_delay = false;
              show_your_turn(online_my_turn);
              if (c2) activate_timer(c2);
            } else
                if (c1 == STATUS_PASSING) {
                  online_playing = false;
                  wait_delay = false;
                 if (c2) activate_timer(c2);
                }

            online_passto = pList.at(16).toInt();

            pass_to(online_passto);
            if ((c1 == STATUS_PASSED) || (c1 != STATUS_PASSING))
              label[17]->setDisabled(true);

            // TODO: check to disable passto button
            online_heart_broken = pList.at(17).toInt();
            online_num_cards = 0;
            online_can_sit = false;
            table_list->RemoveRow(pList.at(0));
            for (int i=0; i<13; i++) {
               c1 = pList.at(18+i).toInt();
               online_myCards[i] = c1;
               online_selected[i] = false;
               if (c1 != empty)
                 online_num_cards++;
            }
            online_game_started = true;
            show_deck(0, false);
            break;
    case ACTION_WRONG_VALUE:
            if (pList.size() != 2) {
              message("ERROR: ACTION_WRONG_VALUE");
              return;
            }
            m = tr("Wrong value ! The range is: [") + QString::number(pList.at(0).toInt()) + ", " + QString::number(pList.at(1).toInt()) + "]";
            message(m);
            break;
  }
}

void MainWindow::reset_cards_pos()
{
  for (int i=0; i<13; i++)
    label[i]->move(label[i]->x(), def_card_posy);
}

void MainWindow::join_game(int id, char chair)
{
  QString m;

  if (id == online_table_id) {
    if (chair != ' ')
      m.sprintf("sit %c", chair);
    else
      return;
  } else {
      if (chair == ' ')
        m.sprintf("join %d", id);
      else
        m.sprintf("join %d %c", id, chair);
    }
  client->send(m);
}

void MainWindow::on_actionTables_triggered()
{
  table_list->show();
}

void MainWindow::activate_timer(int cs)
{
  ui->progressBar->setMaximum(cs * 10);
  ui->progressBar->setValue(cs * 10);
  ui->progressBar->show();
  timer->setInterval(100);
  timer->start();
}

void MainWindow::remove_timer()
{
  timer->stop();
  ui->progressBar->hide();
}

void MainWindow::update_bar()
{
  int value = ui->progressBar->value();

  value -= 100;
  if (value <= 0) {
    value = 0;
    remove_timer();
  }
  ui->progressBar->setValue(value);
}

// online button exit
void MainWindow::on_pushButton_clicked()
{
  QMessageBox msgBox(this);

  msgBox.setText(tr("Do you want to leave the server?"));
  msgBox.setInformativeText(tr("Are you sure?"));
  msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
  msgBox.setDefaultButton(QMessageBox::No);
  int ret = msgBox.exec();
  if (ret == QMessageBox::Yes) {
    client->send("exit");
  }
}

// online button leave
void MainWindow::on_pushButton_2_clicked()
{
  client->send("leave");
}

// online button table
void MainWindow::on_pushButton_3_clicked()
{
  on_actionTables_triggered();
}

// online button create table
void MainWindow::on_pushButton_4_clicked()
{
  on_actionCreate_Table_triggered();
}

// online mute
void MainWindow::on_pushButton_5_clicked()
{
  client->send("mute");
}


// start
void MainWindow::on_pushButton_7_clicked()
{
  client->send("start");
}

// online button action: command, help, says
void MainWindow::on_pushButton_6_clicked()
{
  online_button_mode++;
  if (online_button_mode > mode_button_last)
    online_button_mode = mode_button_first;

  switch (online_button_mode) {
     case mode_button_command : ui->pushButton_6->setText(">"); break;
     case mode_button_help :    ui->pushButton_6->setText("?");
                                client->send("help");
                                break;
     case mode_button_says :    ui->pushButton_6->setText("@"); break;
  }
}

// Show online buttons when connected.
// Hide them when disconnecting.
void MainWindow::online_show_buttons(bool enable)
{
  if (enable) {
    ui->pushButton->show();
    ui->pushButton_2->show();
    ui->pushButton_3->show();
    ui->pushButton_4->show();
    ui->pushButton_5->show();
    ui->pushButton_7->show();
  } else {
      ui->pushButton->hide();
      ui->pushButton_2->hide();
      ui->pushButton_3->hide();
      ui->pushButton_4->hide();
      ui->pushButton_5->hide();
      ui->pushButton_7->hide();
    }
}
