#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <cardspos.h>
#include <QLabel>
#include <QFile>
#include <QDir>
#include <QPushButton>
#include <QImage>
#include <QString>
#include <QMessageBox>
#include <QTime>
#include <QResource>
#include "time.h"
#include "assert.h"
#include "rules.h"
#include "credits.h"
#include "settings.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    srand(time(0));

    ui->textEdit->setTextColor((Qt::yellow));
    ui->textEdit->setTextBackgroundColor(Qt::black);

    stop_delay = false;

    setWindowTitle(version);

    init_pointers();

    load_sounds();

    config = new CConfig;

    hearts = new CHearts;

    stats = new CStats;

    set_options();
    set_hearts_options();

    connect(hearts, SIGNAL(sig_refresh_deck(int)), this, SLOT(refresh_deck(int)));
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

    connect(qApp, SIGNAL(aboutToQuit()), this, SLOT(save_files()));

    message("Welcome to " + QString(version));

    if (stats->is_file_corrupted())
      message("[Error]: The statistics file is corrupted!");

    int errno;

    errno = hearts->load_saved_game();
    if (errno == NOERROR)
      load_saved_game();
    else {
      if (errno == FCORRUPTED) {
        message("[Error]: The saved game file is corrupted! Deleted!");

        QFile file(QDir::homePath() + SAVEDGAME_FILENAME);
        file.remove();
      }
      stats->increase_stats(0, STATS_GAME_STARTED);
      hearts->new_game();
      set_plr_names();
    }

    set_cheat_mode_enabled(config->is_cheat_mode());
}

MainWindow::~MainWindow()
{
    delete ui;
    delete hearts;
    delete config;
    delete stats;

    delete img_empty_card;
    delete img_your_turn;
    delete img_back_card;

    delete img_pass[pLEFT];
    delete img_pass[pRIGHT];
    delete img_pass[pACCROSS];
    delete img_pass[pNOPASS];

    for (int i=0; i < DECK_SIZE; i++)
      delete img_cards[i];

    destroy_sounds();
}

void MainWindow::load_sounds()
{
#ifdef __al_included_allegro5_allegro_audio_h
    al_install_audio();
    al_init_acodec_addon();
    al_reserve_samples(11);

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

    ALLEGRO_FILE *fp = al_open_memfile((void*)resource_snd1.data(), resource_snd1.size(), "rb");
    snd_breaking_heart = al_load_sample_f(fp, ".wav");
    al_fclose(fp);

    fp = al_open_memfile((void*)resource_snd2.data(), resource_snd2.size(), "rb");
    snd_dealing_card = al_load_sample_f(fp, ".wav");
    al_fclose(fp);

    fp = al_open_memfile((void*)resource_snd3.data(), resource_snd3.size(), "rb");
    snd_error = al_load_sample_f(fp, ".wav");
    al_fclose(fp);

    fp = al_open_memfile((void*)resource_snd4.data(), resource_snd4.size(), "rb");
    snd_game_over = al_load_sample_f(fp, ".wav");
    al_fclose(fp);

    fp = al_open_memfile((void*)resource_snd5.data(), resource_snd5.size(), "rb");
    snd_shoot_moon = al_load_sample_f(fp, ".wav");
    al_fclose(fp);

    fp = al_open_memfile((void*)resource_snd6.data(), resource_snd6.size(), "rb");
    snd_your_turn = al_load_sample_f(fp, ".wav");
    al_fclose(fp);

    fp = al_open_memfile((void*)resource_snd7.data(), resource_snd7.size(), "rb");
    snd_queen_spade = al_load_sample_f(fp, ".wav");
    al_fclose(fp);

    fp = al_open_memfile((void*)resource_snd8.data(), resource_snd8.size(), "rb");
    snd_contact = al_load_sample_f(fp, ".wav");
    al_fclose(fp);

    fp = al_open_memfile((void*)resource_snd9.data(), resource_snd9.size(), "rb");
    snd_passing_cards = al_load_sample_f(fp, ".wav");
    al_fclose(fp);

    fp = al_open_memfile((void*)resource_snd10.data(), resource_snd10.size(), "rb");
    snd_shuffling_cards = al_load_sample_f(fp, ".wav");
    al_fclose(fp);

    fp = al_open_memfile((void*)resource_snd11.data(), resource_snd11.size(), "rb");
    snd_perfect_100 = al_load_sample_f(fp, ".wav");
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
#endif
}

void MainWindow::init_pointers()
{
    img_empty_card = new QImage(":/SVG-cards/empty.png", "PNG");     // NEW
    img_your_turn = new QImage(":/SVG-cards/card-base2.png", "PNG"); // NEW
    img_back_card = new QImage(":/SVG-cards/back.png", "PNG");

    img_pass[pLEFT] = new QImage(":/icons/left-icon.png", "PNG");
    img_pass[pRIGHT] = new QImage(":/icons/right-icon.png", "PNG");
    img_pass[pACCROSS] = new QImage(":/icons/up-icon.png", "PNG");
    img_pass[pNOPASS] = new QImage(":/icons/no_pass-icon.png", "PNG");

    img_cards[0] = new QImage(":/SVG-cards/club_2.png", "PNG");
    img_cards[1] = new QImage(":/SVG-cards/club_3.png", "PNG");
    img_cards[2] = new QImage(":/SVG-cards/club_4.png", "PNG");
    img_cards[3] = new QImage(":/SVG-cards/club_5.png", "PNG");
    img_cards[4] = new QImage(":/SVG-cards/club_6.png", "PNG");
    img_cards[5] = new QImage(":/SVG-cards/club_7.png", "PNG");
    img_cards[6] = new QImage(":/SVG-cards/club_8.png", "PNG");
    img_cards[7] = new QImage(":/SVG-cards/club_9.png", "PNG");
    img_cards[8] = new QImage(":/SVG-cards/club_10.png", "PNG");
    img_cards[9] = new QImage(":/SVG-cards/club_jack.png", "PNG");
    img_cards[10] = new QImage(":/SVG-cards/club_queen.png", "PNG");
    img_cards[11] = new QImage(":/SVG-cards/club_king.png", "PNG");
    img_cards[12] = new QImage(":/SVG-cards/club_1.png", "PNG");

    img_cards[13] = new QImage(":/SVG-cards/spade_2.png", "PNG");
    img_cards[14] = new QImage(":/SVG-cards/spade_3.png", "PNG");
    img_cards[15] = new QImage(":/SVG-cards/spade_4.png", "PNG");
    img_cards[16] = new QImage(":/SVG-cards/spade_5.png", "PNG");
    img_cards[17] = new QImage(":/SVG-cards/spade_6.png", "PNG");
    img_cards[18] = new QImage(":/SVG-cards/spade_7.png", "PNG");
    img_cards[19] = new QImage(":/SVG-cards/spade_8.png", "PNG");
    img_cards[20] = new QImage(":/SVG-cards/spade_9.png", "PNG");
    img_cards[21] = new QImage(":/SVG-cards/spade_10.png", "PNG");
    img_cards[22] = new QImage(":/SVG-cards/spade_jack.png", "PNG");
    img_cards[23] = new QImage(":/SVG-cards/spade_queen.png", "PNG");
    img_cards[24] = new QImage(":/SVG-cards/spade_king.png", "PNG");
    img_cards[25] = new QImage(":/SVG-cards/spade_1.png", "PNG");

    img_cards[26] = new QImage(":/SVG-cards/diamond_2.png", "PNG");
    img_cards[27] = new QImage(":/SVG-cards/diamond_3.png", "PNG");
    img_cards[28] = new QImage(":/SVG-cards/diamond_4.png", "PNG");
    img_cards[29] = new QImage(":/SVG-cards/diamond_5.png", "PNG");
    img_cards[30] = new QImage(":/SVG-cards/diamond_6.png", "PNG");
    img_cards[31] = new QImage(":/SVG-cards/diamond_7.png", "PNG");
    img_cards[32] = new QImage(":/SVG-cards/diamond_8.png", "PNG");
    img_cards[33] = new QImage(":/SVG-cards/diamond_9.png", "PNG");
    img_cards[34] = new QImage(":/SVG-cards/diamond_10.png", "PNG");
    img_cards[35] = new QImage(":/SVG-cards/diamond_jack.png", "PNG");
    img_cards[36] = new QImage(":/SVG-cards/diamond_queen.png", "PNG");
    img_cards[37] = new QImage(":/SVG-cards/diamond_king.png", "PNG");
    img_cards[38] = new QImage(":/SVG-cards/diamond_1.png", "PNG");

    img_cards[39] = new QImage(":/SVG-cards/heart_2.png", "PNG");
    img_cards[40] = new QImage(":/SVG-cards/heart_3.png", "PNG");
    img_cards[41] = new QImage(":/SVG-cards/heart_4.png", "PNG");
    img_cards[42] = new QImage(":/SVG-cards/heart_5.png", "PNG");
    img_cards[43] = new QImage(":/SVG-cards/heart_6.png", "PNG");
    img_cards[44] = new QImage(":/SVG-cards/heart_7.png", "PNG");
    img_cards[45] = new QImage(":/SVG-cards/heart_8.png", "PNG");
    img_cards[46] = new QImage(":/SVG-cards/heart_9.png", "PNG");
    img_cards[47] = new QImage(":/SVG-cards/heart_10.png", "PNG");
    img_cards[48] = new QImage(":/SVG-cards/heart_jack.png", "PNG");
    img_cards[49] = new QImage(":/SVG-cards/heart_queen.png", "PNG");
    img_cards[50] = new QImage(":/SVG-cards/heart_king.png", "PNG");
    img_cards[51] = new QImage(":/SVG-cards/heart_1.png", "PNG");

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

    cheat_radio_button[0] = ui->radioButton;
    cheat_radio_button[1] = ui->radioButton_2;
    cheat_radio_button[2] = ui->radioButton_3;
    cheat_radio_button[3] = ui->radioButton_4;
}

void MainWindow::save_files()
{
 stats->save_stats_file();
 stats->quit();

 if (!ui->actionSave_Game_Quit->isChecked())
   return;

 if (hearts->is_game_over())
   return;

 hearts->save_game(plr_names_idx[0], plr_names_idx[1], plr_names_idx[2], plr_names_idx[3]);
}

void MainWindow::load_saved_game()
{
  message("[Info]: Previous saved game has been loaded!");
  for (int i=0; i<4; i++) {
    int name_id = hearts->get_plr_name_id(i);

    plr_names_idx[i] = name_id;

    if (name_id == -1)
      label[18 + i]->setText("You");
    else
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

      label[13 + cpt]->setPixmap(QPixmap::fromImage(img_cards[card]->scaledToHeight(100)));
    }
  }
}

void MainWindow::set_options()
{
  ui->actionAuto_Centering->setChecked(config->is_auto_centering());
  ui->actionCheat->setChecked(config->is_cheat_mode());
  ui->actionInfo_Channel->setChecked(config->is_info_channel());
  ui->actionSounds->setChecked(config->is_sounds());
  ui->actionTram->setChecked(config->is_detect_tram());

  ui->actionPerfect_100->setChecked(config->is_perfect_100());
  ui->actionOmnibus->setChecked(config->is_omnibus());
  ui->actionQueen_Spade_Break_Heart->setChecked(config->is_queen_spade_break_heart());
  ui->actionNo_Trick_Bonus->setChecked(config->is_no_trick_bonus());
  ui->actionNew_Moon->setChecked(config->is_new_moon());
  ui->actionNo_Draw->setChecked(config->is_no_draw());
  ui->actionSave_Game_Quit->setChecked(config->is_save_game());

  set_info_channel_enabled(config->is_info_channel());
}

void MainWindow::set_hearts_options()
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
  }

  label[18 + whoami]->setText("You");
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
     case OMNIBUS_BONUS  : msg_bonus = "omnibus ";
                           stats->increase_stats(plr_names_idx[plr], STATS_OMNIBUS);
                           break;
     case NO_TRICK_BONUS : msg_bonus = "no trick bonus ";
                           stats->increase_stats(plr_names_idx[plr], STATS_NO_TRICKS);
                           break;
     default: msg_bonus = "<unknown> ";
  }

  if (plr == hearts->whoami())
    mesg = "[Info]: You receive the bonus: " + msg_bonus + QString::number(value) + " point(s)";
  else
    mesg = "[Info]: Player '" + label[18 + plr]->text() + "' receive the bonus: " + msg_bonus +
            QString::number(value) + " point(s)";

  message(mesg);
}

void MainWindow::perfect_100(int plr)
{
 stats->increase_stats(plr_names_idx[plr], STATS_PERFECT_100);

 QString mesg;

 if (plr == hearts->whoami())
   mesg = "[Info]: You got the perfect 100!\n[Info]: Your score has been set to 50.";
 else
   mesg = "[Info]: Player '" + label[18 + plr]->text() + "' got the perfect 100!\n[Info]: Player '" +
                               label[18 + plr]->text() + "' score has been set to 50.";

 message(mesg);

#ifdef __al_included_allegro5_allegro_audio_h
  if (ui->actionSounds->isChecked())
    al_play_sample(snd_perfect_100, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
#endif
}

void MainWindow::breaking_hearts()
{
  message("[Info]: Hearts has been broken!");

#ifdef __al_included_allegro5_allegro_audio_h
  if (ui->actionSounds->isChecked())
    al_play_sample(snd_breaking_heart, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
#endif
}

void MainWindow::game_over(int score1, int score2, int score3, int score4)
{
  QString mesg, mesg2;

  stats->increase_stats(0, STATS_GAME_FINISHED);

  clear_deck();

  if (!hearts->is_it_draw())
    mesg2 = "Won !";
  else
    mesg2 = "Drew !";

 int lowest;
 int unsorted[4];

 unsorted[0] = score1;
 unsorted[1] = score2;
 unsorted[2] = score3;
 unsorted[3] = score4;

 for (int i=0; i<4; i++) {
   int cpt = 0;
   for (int i2=0; i2<4; i2++)
     if (unsorted[i] >= unsorted[i2])
       cpt++;

   switch(cpt) {
     case 1 : stats->increase_stats(plr_names_idx[i], STATS_FIRST_PLACE);
              lowest = unsorted[i];
              break;
     case 2 : stats->increase_stats(plr_names_idx[i], STATS_SECOND_PLACE);
              break;
     case 3 : stats->increase_stats(plr_names_idx[i], STATS_THIRD_PLACE);
              break;
     case 4 : stats->increase_stats(plr_names_idx[i], STATS_FOURTH_PLACE);
              break;
   }
 }

 stats->set_score(plr_names_idx[0], score1);
 stats->set_score(plr_names_idx[1], score2);
 stats->set_score(plr_names_idx[2], score3);
 stats->set_score(plr_names_idx[3], score4);

 mesg = "[Info]: GAME OVER!\n[Info]: Player '" + label[18]->text() + "': " +
         QString::number(score1) + " point(s) " + (score1 == lowest ? mesg2 : "") +
        "\n[Info]: Player '"                  + label[19]->text() + "': " +
         QString::number(score2) + " point(s) " + (score2 == lowest ? mesg2 : "") +
        "\n[Info]: Player '"                  + label[20]->text() + "': " +
         QString::number(score3) + " point(s) " + (score3 == lowest ? mesg2 : "") +
        "\n[Info]: Player '"                  + label[21]->text() + "': " +
         QString::number(score4) + " point(s) " + (score4 == lowest ? mesg2 : "");

 message(mesg);

  if (!ui->actionInfo_Channel->isChecked())
    QMessageBox::information(this, "information", mesg);

#ifdef __al_included_allegro5_allegro_audio_h
  if (ui->actionSounds->isChecked())
    al_play_sample(snd_game_over, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
#endif

  ui->actionNew->setDisabled(false);
  ui->actionSave->setDisabled(true);
}

void MainWindow::tram(int plr)
{
  QString mesg;

  if (plr == hearts->whoami())
    mesg = "[Info]: You takes the rest!";
  else
    mesg = "[Info]: Player '" + label[18 + plr]->text() + "' takes the rest!";

  message(mesg);
  clear_deck();
}

void MainWindow::shoot_moon(int plr)
{

 stats->increase_stats(plr_names_idx[plr], STATS_SHOOT_MOON);

#ifdef __al_included_allegro5_allegro_audio_h
  if (ui->actionSounds->isChecked())
    al_play_sample(snd_shoot_moon, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
#endif

  QString mesg;

  if (plr == hearts->whoami()) {
    mesg = "[Info]: You shoot the moon!";

    if (ui->actionNew_Moon->isChecked() && (hearts->get_my_score() >= 26)) {
      message(mesg);

      QMessageBox msgBox(this);
      msgBox.setWindowTitle("You shoot the moon!");
      msgBox.setText("What is your choice ?");
      QPushButton *button_add = msgBox.addButton(QString("Add"), QMessageBox::YesRole);
      msgBox.addButton(QString("Subtract"), QMessageBox::NoRole);
      msgBox.setDefaultButton(button_add);
      msgBox.exec();
      if (msgBox.clickedButton() == button_add) {
         message("[Info]: You added 26 pts to everyone's scores!");
         hearts->set_moon_add_to_score(true);
      } else {
         message("[Info]: You substracted 26 pts to your score!");
         hearts->set_moon_add_to_score(false);
        }
      return;
    }
  }
  else {
    mesg = "[Info]: Player '" + label[18 + plr]->text() + "' shoot the moon!";
  }

  message(mesg);

  if (!ui->actionInfo_Channel->isChecked())
    QMessageBox::information(this, "information", mesg);
}

void MainWindow::pass_to(int pass_to)
{        
  ui->actionNew->setDisabled(false);
  ui->actionSave->setDisabled(false);
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

  lcd_hand_score[0]->display(score1);
  lcd_hand_score[1]->display(score2);
  lcd_hand_score[2]->display(score3);
  lcd_hand_score[3]->display(score4);

  QString mesg;

  mesg = "[Info]: New scores: '" + label[18]->text() + ": " +
          QString::number(lcd_score[0]->value()) + " (" + QString::number(score1) + ")', '" +
                                   label[19]->text() + ": " +
          QString::number(lcd_score[1]->value()) + " (" + QString::number(score2) + ")', '" +
                                   label[20]->text() + ": " +
          QString::number(lcd_score[2]->value()) + " (" + QString::number(score3) + ")', '" +
                                   label[21]->text() + ": " +
          QString::number(lcd_score[3]->value()) + " (" + QString::number(score4) + ")'";

  message(mesg);


#ifdef __al_included_allegro5_allegro_audio_h
  if (ui->actionSounds->isChecked() && !hearts->is_game_over()) {
    al_play_sample(snd_shuffling_cards, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
    delay(1500);
  }
#endif
}

void MainWindow::show_deck(int plr)
{
  active_deck = plr;

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
    if (hearts->is_card_selected(plr, i))
      label[pos]->setPixmap(QPixmap::fromImage(img_back_card->scaledToHeight(100)));
    else
      label[pos]->setPixmap(QPixmap::fromImage(img_cards[card]->scaledToHeight(100)));
  }
}

void MainWindow::set_info_channel_enabled(bool enable)
{
 int new_height = height();

 if (enable) {
    new_height += ui->textEdit->height();
    if (new_height > max_mainwindow_height)
      new_height = max_mainwindow_height;

    setFixedHeight(new_height);
    ui->textEdit->show();
  } else {
      ui->textEdit->hide();

      new_height -= ui->textEdit->height();
      setFixedHeight(new_height);
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
      cheat_radio_button[0]->hide();
      cheat_radio_button[1]->hide();
      cheat_radio_button[2]->hide();
      cheat_radio_button[3]->hide();

      show_deck(hearts->whoami());
    }

  cheat_radio_button[hearts->whoami()]->setChecked(true);
}

void MainWindow::refresh_deck(int plr)
{
  cheat_radio_button[plr]->setChecked(true);
  show_deck(plr);
}

void MainWindow::clear_deck()
{
  for (int i=0; i<13; i++)
    label[i]->hide();
}

void MainWindow::clear_table()
{
  delay(200);
  for (int i=0; i<4; i++)
    label[i+13]->setPixmap(QPixmap::fromImage(img_empty_card->scaledToHeight(100)));
}

void MainWindow::select_card(int num)
{
  int card_id;
  int count = hearts->count_my_cards() - 1;

  if (ui->actionAuto_Centering->isChecked())
    card_id = rev_cards_position[count][num];
  else
    card_id = num;

  assert((card_id >= 0) && (card_id <= 12));

 if ((active_deck != hearts->whoami()) || wait_delay) return;

 if (hearts->is_mode_playing()) {

   if (!hearts->is_it_my_turn()) return;     // wait_delay seem to do the job, but maybe it's just by luck...

   ui->actionNew->setDisabled(true);
   ui->actionSave->setDisabled(true);

   int error;

   if ((error = hearts->play_card(card_id))) {
     switch (error) {
        case ERRHEART: message("[Error]: You can't break hearts yet!");
                       break;
        case ERRSUIT:  message("[Error]: You must follow the suit! The lead is in " +
                               QString(suit_names[hearts->get_current_suit()]) + "!");
                       break;
        case ERRQUEEN: message("[Error]: You can't play the queen of spade on the first hand!");
                       break;
     }
#ifdef __al_included_allegro5_allegro_audio_h
     if (ui->actionSounds->isChecked())
       al_play_sample(snd_error, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
#endif
      ui->actionNew->setDisabled(false);
      ui->actionSave->setDisabled(false);
   }  
   return;
 }

 bool selected = hearts->is_card_selected(card_id);

 if (selected) {
   if (hearts->unselect_card(card_id)) {
     int card = hearts->get_card(card_id);
     label[card_id]->setPixmap(QPixmap::fromImage((img_cards[card]->scaledToHeight(100))));

   }
 } else
     if (hearts->select_card(card_id)) {
       label[card_id]->setPixmap(QPixmap::fromImage(img_back_card->scaledToHeight(100)));

#ifdef __al_included_allegro5_allegro_audio_h
       if (ui->actionSounds->isChecked())
         al_play_sample(snd_contact, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
#endif
     }
}

void MainWindow::play_card(int card, int idx)
{
 if (card == two_clubs)
   delay(700);
 else
   delay(400);

  label[idx+13]->setPixmap(QPixmap::fromImage(img_cards[card]->scaledToHeight(100)));

#ifdef __al_included_allegro5_allegro_audio_h
  if (ui->actionSounds->isChecked()) {
     al_play_sample(snd_dealing_card, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);

    if (card == queen_spade)
      al_play_sample(snd_queen_spade, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
  }
#endif
}

void MainWindow::show_your_turn(int idx)
{
  delay(200);

  label[idx+13]->setPixmap(QPixmap::fromImage(img_your_turn->scaledToHeight(100)));

#ifdef __al_included_allegro5_allegro_audio_h
  if (ui->actionSounds->isChecked())
    al_play_sample(snd_your_turn, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
#endif

  ui->actionNew->setDisabled(false);
  ui->actionSave->setDisabled(false);
}

void MainWindow::on_label_18_clicked() // pass 3 cards
{
  if (hearts->is_no_pass() ||
      (active_deck != hearts->whoami())) return;

  if (!hearts->is_ready_to_pass()) {
    message("[Error]: You needs to select 3 cards to pass!");

 #ifdef __al_included_allegro5_allegro_audio_h
  if (ui->actionSounds->isChecked())
    al_play_sample(snd_error, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
 #endif

    return;
  }

  label[17]->setDisabled(true);

  ui->actionNew->setDisabled(true);
  ui->actionSave->setDisabled(true);

  hearts->select_cpus_cards();

  hearts->pass_cards();

  int ptr = 0;
  for (int i=0; i<13; i++)
    if (hearts->is_card_selected(i))
      cards_received[ptr++] = hearts->get_card(i);

  hearts->reset_cards_passed();

  reverse_cards_rgb();
  show_deck(hearts->whoami());

#ifdef __al_included_allegro5_allegro_audio_h
  if (ui->actionSounds->isChecked())
    al_play_sample(snd_passing_cards, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
#endif

  delay(2000);

  hearts->sort_plr_cards();

  reverse_cards_rgb();

  show_deck(hearts->whoami());

  hearts->play_2clubs();
}

void MainWindow::reverse_cards_rgb()
{
  img_cards[cards_received[0]]->invertPixels();
  img_cards[cards_received[1]]->invertPixels();
  img_cards[cards_received[2]]->invertPixels();
}

void MainWindow::message(QString mesg)
{
  ui->textEdit->append(mesg);
}

void MainWindow::delay(int n)
{
    wait_delay = true;
    QTime dieTime= QTime::currentTime().addMSecs(n);
    while ((QTime::currentTime() < dieTime) && !stop_delay)
      QCoreApplication::processEvents(QEventLoop::AllEvents, 2000);
    wait_delay = false;
}

void MainWindow::on_radioButton_clicked()
{
  show_deck(0);
}

void MainWindow::on_radioButton_2_clicked()
{
  show_deck(1);
}

void MainWindow::on_radioButton_3_clicked()
{
  show_deck(2);
}

void MainWindow::on_radioButton_4_clicked()
{
  show_deck(3);
}

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

  message("[Info]: Starting a new game.");

  stop_delay = true;
  hearts->new_game();
  stop_delay = false;
  set_plr_names();
}

void MainWindow::on_actionCheat_triggered()
{
  bool checked = ui->actionCheat->isChecked();

  set_cheat_mode_enabled(checked);
  config->set_config_file(CONFIG_CHEAT_MODE, checked);
}

void MainWindow::on_actionAuto_Centering_triggered()
{
  show_deck(active_deck);

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

void MainWindow::on_actionReset_triggered()
{
  QMessageBox msgBox(this);
  msgBox.setText("Do you want to reset statistics?");
  msgBox.setInformativeText("Are you sure?");
  msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
  msgBox.setDefaultButton(QMessageBox::No);
  int ret = msgBox.exec();
  if (ret == QMessageBox::Yes) {
     stats->reset();
     message("[Info]: You resetted the statistics!");
  }
}

void MainWindow::on_actionShow_triggered()
{
  stats->show_stats();
}

void MainWindow::on_actionQuit_triggered()
{
  save_files();

  close();
}
