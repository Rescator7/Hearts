#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QLabel>
#include <QLCDNumber>
#include <QRadioButton>
#include "define.h"

#ifdef USE_LIBALLEGRO5
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_memfile.h>
#include <allegro5/file.h>
#endif

#include "chearts.h"
#include "config.h"

const int max_mainwindow_height = 915;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_radioButton_clicked();
    void on_radioButton_2_clicked();
    void on_radioButton_3_clicked();
    void on_radioButton_4_clicked();
    void on_label_clicked();
    void on_label_2_clicked();
    void on_label_3_clicked();
    void on_label_4_clicked();
    void on_label_5_clicked();
    void on_label_6_clicked();
    void on_label_7_clicked();
    void on_label_8_clicked();
    void on_label_9_clicked();
    void on_label_10_clicked();
    void on_label_11_clicked();
    void on_label_12_clicked();
    void on_label_13_clicked();
    void on_label_18_clicked();

    void on_actionNew_triggered();
    void on_actionCheat_triggered();
    void on_actionAuto_Centering_triggered();
    void on_actionTram_triggered();

    void on_actionInfo_Channel_triggered();

    void on_actionSounds_triggered();

    void on_actionPerfect_100_triggered();

    void on_actionOmnibus_triggered();

    void on_actionQueen_Spade_Break_Heart_triggered();

    void on_actionNo_Trick_Bonus_triggered();

    void on_actionNew_Moon_triggered();

    void on_actionNo_Draw_triggered();

    void on_actionRules_triggered();

    void on_actionCredits_triggered();

    void on_actionSettings_triggered();

private:
    Ui::MainWindow *ui;
    QImage *img_cards[52];
    QImage *img_pass[4];
    QImage *img_empty_card;
    QImage *img_back_card;
    QImage *img_your_turn;
    QLabel *label[22];
    QLCDNumber *lcd_hand_score[4];
    QLCDNumber *lcd_score[4];
    QRadioButton *cheat_radio_button[4];

#ifdef __al_included_allegro5_allegro_audio_h
    ALLEGRO_SAMPLE *snd_shoot_moon;
    ALLEGRO_SAMPLE *snd_your_turn;
    ALLEGRO_SAMPLE *snd_game_over;
    ALLEGRO_SAMPLE *snd_error;
    ALLEGRO_SAMPLE *snd_dealing_card;
    ALLEGRO_SAMPLE *snd_breaking_heart;
    ALLEGRO_SAMPLE *snd_queen_spade;
    ALLEGRO_SAMPLE *snd_contact;
    ALLEGRO_SAMPLE *snd_passing_cards;
    ALLEGRO_SAMPLE *snd_shuffling_cards;
    ALLEGRO_SAMPLE *snd_perfect_100;
#endif

private:
    CHearts *hearts;
    CConfig *config;

    bool wait_delay;
    bool stop_delay;

    int active_deck;
    int cards_received[3];

    void clear_deck();
    void show_deck(int plr);
    void set_info_channel_enabled(bool enable);
    void set_cheat_mode_enabled(bool enable);
    void load_sounds();
    void destroy_sounds();
    void init_pointers();
    void set_options();
    void set_hearts_options();

public:
    void message(QString mesg);
    void delay(int n);
    void set_plr_names();
    void select_card(int num);

public slots:
    void refresh_deck(int plr);
    void play_card(int card, int idx);
    void clear_table();
    void refresh_score(int score, int idx);
    void end_of_hand(int score1, int score2, int score3, int score4);
    void refresh_hand_score(int score, int idx);
    void show_your_turn(int idx);
    void pass_to(int pass_to);
    void tram(int plr);
    void shoot_moon(int plr);
    void game_over(int score1, int score2, int score3, int score4);
    void reverse_cards_rgb();
    void breaking_hearts();
    void perfect_100(int plr);
    void receive_bonus(int plr, int bonus, int value);
};

#endif // MAINWINDOW_H
