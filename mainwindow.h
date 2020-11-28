#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QLabel>
#include <QLCDNumber>
#include <QPushButton>
#include <QProgressBar>
#include "define.h"

#ifdef USE_LIBALLEGRO5
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_memfile.h>
#include <allegro5/file.h>
#endif

#include "chearts.h"
#include "config.h"
#include "cstatistics.h"
#include "cdeck.h"
#include "client.h"
#include "ctable.h"

#ifdef DEBUG
#include "debug.h"
#endif

const int mode_button_command   = 1;
const int mode_button_help      = 2;
const int mode_button_says      = 3;

const int mode_button_first     = 1;
const int mode_button_last      = 3;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_label_card_s1_clicked();
    void on_label_card_s2_clicked();
    void on_label_card_s3_clicked();
    void on_label_card_s4_clicked();
    void on_label_card_s5_clicked();
    void on_label_card_s6_clicked();
    void on_label_card_s7_clicked();
    void on_label_card_s8_clicked();
    void on_label_card_s9_clicked();
    void on_label_card_s10_clicked();
    void on_label_card_s11_clicked();
    void on_label_card_s12_clicked();
    void on_label_card_s13_clicked();
    void on_label_18_clicked();

    void on_actionNew_triggered();
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
    void on_actionSave_Game_Quit_triggered();
    void on_actionReset_triggered();
    void on_actionShow_triggered();

#ifdef DEBUG
    void on_actionShow_2_triggered();
    void on_actionCheat_triggered();
    void on_actionCards_Played_triggered();
#endif

    void on_actionEnglish_triggered(bool checked);
    void on_actionRussian_triggered(bool checked);
    void on_actionFrench_triggered(bool checked);
    void on_actionEasy_card_selection_triggered();
    void on_actionStandard_triggered();
    void on_actionRussian_2_triggered();
    void on_actionEnglish_2_triggered();
    void on_lineEdit_returnPressed();
    void on_actionConnect_triggered();
    void on_actionCreate_Table_triggered();
    void on_label_played_s_clicked();
    void on_label_played_w_clicked();
    void on_label_played_n_clicked();
    void on_label_played_e_clicked();
    void on_actionTables_triggered();
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_5_clicked();
    void on_pushButton_6_clicked();
    void on_pushButton_7_clicked();
    void on_actionOnline_triggered();
    void on_actionAuto_Start_triggered();
    void on_actionSlow_triggered();
    void on_actionNormal_triggered();
    void on_actionFast_triggered();
    void on_actionAnimate_Play_triggered();
    void on_label_heart_s_clicked();
    void on_label_heart_w_clicked();
    void on_label_heart_n_clicked();
    void on_label_heart_e_clicked();
    void on_label_score_s_clicked();
    void on_label_name_s_clicked();
    void on_label_score_w_clicked();
    void on_label_name_w_clicked();
    void on_label_score_n_clicked();
    void on_label_name_n_clicked();
    void on_label_score_e_clicked();
    void on_label_name_e_clicked();

private:
    Ui::MainWindow *ui;
    QImage *img_pass[4];
    QImage *img_connected;
    QImage *img_disconnected;
    QLabel *label_cards[4][13];
    QLabel *label_scores[4];
    QLabel *label_player_name[4];
    QLabel *label_card_played[4];
    QTranslator translator;
    QProgressBar *progress_bar_time[5];

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
    ALLEGRO_SAMPLE *snd_connected;
    ALLEGRO_SAMPLE *snd_disconnected;
    ALLEGRO_SAMPLE *snd_announcement;
#endif

private:
    CHearts *hearts;
    CConfig *config;
    CStatistics *stats;
    CDeck *deck;
    CClient *client;
    CTable *table_list;
    QTimer *timer;

#ifdef DEBUG
    CDebug  *debug;
#endif

    bool wait_delay;
    bool stop_delay;

    int cards_received[3];
    int plr_names_idx[4];
    int card_height;
    int card_played[4];
    int original_pos_cards[4];
    int scores[4];
    int hand_scores[4];

// Online variables
    int  online_table_id;
    int  online_passto;
    int  online_num_cards[4];
    int  online_num_selected;
    int  online_myCards[13];
    int  online_cards_received_pos[3];
    int  online_my_turn;
    int  online_button_mode;
    int  online_bar;

    bool online_selected[13];
    bool online_playing;
    bool online_connected;
    bool online_can_sit;
    bool online_heart_broken;
    bool online_game_started;

    char online_names[4][20];

    int convert_chair(int chair);

    void clear_deck();
    void show_deck(bool animate, bool replace);

    void set_info_channel_enabled(bool enable);
    void set_language(int lang);
    void set_cards_disabled(bool d);
    void refresh_cards_played();
    void init_vars();
    void load_sounds();
    void destroy_sounds();
    void init_pointers();
    void set_settings();
    void set_options();
    void load_saved_game();
    void start_game(bool booting);
    void disable_cheat();
    void check_easy_cards();
    void online_set_settings(bool disable);
    void online_rotate_players_name();

public:
    int get_name_label(QString p);

    void delay(int n);
    void set_plr_names();
    void select_card(int num);
    void init_online_game();
    void shoot_moon(int plr, int delay);
    void light_connected(bool connected);
    void set_online_buttons_styles();

    void online_end_hand(int north, int south, int west, int east);
    void online_game_over(int north, int south, int west, int east);
    void online_receive_bonus(int plr, int bonus, int value);
    void online_perfect_100(int plr);
    void online_select_card(int num);
    void online_pass_cards();
    void online_show_buttons(bool enable);
    void online_show_lineedit(bool enable);
    void online_show_deck();
    void online_hide_progress_bar();

public slots:
    void online_action(unsigned int action, QString param);
    void message(QString mesg);
    void error(QString mesg);
    void refresh_deck(bool animate);
    void play_card(int card, int plr);
    void clear_table();
    void refresh_score(int plr, int score);
    void end_of_hand(int score1, int score2, int score3, int score4);
    void refresh_hand_score(int plr, int score);
    void show_your_turn(int idx);
    void pass_to(int pass_to);
    void tram(int plr);
    void shoot_moon(int plr);
    void game_over(int score1, int score2, int score3, int score4);
    void reverse_cards_rgb();
    void breaking_hearts();
    void perfect_100(int plr);
    void receive_bonus(int plr, int bonus, int value);
    void got_queen_spade(int plr);
    void save_files();
    void join_game(int id, char chair);
    void update_bar();
    void activate_timer(int cs, int bar, int max);
    void remove_timer();
    void aboutToQuit();
    void animate_pass_cards(int w1, int w2, int w3, int n1, int n2, int n3, int e1, int e2, int e3);
    void handle_new_game();
};

#endif // MAINWINDOW_H
