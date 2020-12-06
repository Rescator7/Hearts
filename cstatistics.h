#ifndef CSTATISTICS_H
#define CSTATISTICS_H

#include <QWidget>
#include <QTableWidgetItem>

#include "define.h"

const char STATS_FILENAME[20] = "/.hearts.stats";
const char STATS_BACKUP_FILE[20] = "/.hearts.stats.bak";

#define STATS_GAME_STARTED  0
#define STATS_GAME_FINISHED 1
#define STATS_HANDS_PLAYED  2
#define STATS_SCORES        3
#define STATS_FIRST_PLACE   4
#define STATS_SECOND_PLACE  5
#define STATS_THIRD_PLACE   6
#define STATS_FOURTH_PLACE  7
#define STATS_SHOOT_MOON    8
#define STATS_QUEEN_SPADE   9
#define STATS_OMNIBUS       10
#define STATS_NO_TRICKS     11
#define STATS_PERFECT_100   12
#define STATS_UNDO          13

namespace Ui {
class CStatistics;
}

class CStatistics : public QWidget
{
    Q_OBJECT

public:
    explicit CStatistics(QWidget *parent = nullptr);
    ~CStatistics();

private:
    Ui::CStatistics *ui;
    bool file_corrupted;

    QTableWidgetItem *item_names[MAX_PLR_NAMES];
    QTableWidgetItem *item_first_place[MAX_PLR_NAMES];
    QTableWidgetItem *item_second_place[MAX_PLR_NAMES];
    QTableWidgetItem *item_third_place[MAX_PLR_NAMES];
    QTableWidgetItem *item_fourth_place[MAX_PLR_NAMES];
    QTableWidgetItem *item_avg_score[MAX_PLR_NAMES];
    QTableWidgetItem *item_best_score[MAX_PLR_NAMES];
    QTableWidgetItem *item_worst_score[MAX_PLR_NAMES];
    QTableWidgetItem *item_shoot_moon[MAX_PLR_NAMES];
    QTableWidgetItem *item_queen_spade[MAX_PLR_NAMES];
    QTableWidgetItem *item_omnibus[MAX_PLR_NAMES];
    QTableWidgetItem *item_no_tricks[MAX_PLR_NAMES];
    QTableWidgetItem *item_perfect_100[MAX_PLR_NAMES];

    int game_started;
    int game_finished;
    int hands_played;
    int undo;

    int first_place[MAX_PLR_NAMES];
    int second_place[MAX_PLR_NAMES];
    int third_place[MAX_PLR_NAMES];
    int fourth_place[MAX_PLR_NAMES];

    int total_score[MAX_PLR_NAMES];
    int best_score[MAX_PLR_NAMES];
    int worst_score[MAX_PLR_NAMES];

    int shoot_moon[MAX_PLR_NAMES];
    int queen_spade_taken[MAX_PLR_NAMES];
    int omnibus_bonus_taken[MAX_PLR_NAMES];
    int no_tricks_bonus_taken[MAX_PLR_NAMES];
    int perfect_100_bonus_taken[MAX_PLR_NAMES];

private: // functions
    void init_vars();
    void create_window();
    void update_window(int plr, int stats);

public: // functions
    bool is_file_corrupted();

    int  load_stats_file();
    int  save_stats_file();

    void increase_stats(int plr, int stats);
    void set_score(int plr, int score);
    void reset();
    void show_stats();
    void Translate();
};

#endif // CSTATISTICS_H
