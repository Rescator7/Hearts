#ifndef SPEED_H
#define SPEED_H

#include <QWidget>
#include "config.h"

namespace Ui {
class CSpeed;
}

class CSpeed : public QWidget
{
    Q_OBJECT

public:
    explicit CSpeed(QWidget *parent = 0, CConfig *config = 0);
    ~CSpeed();

private slots:
    void on_pushButton_cancel_clicked();
    void on_horizontalSlider_play_a_card_valueChanged(int value);
    void on_horizontalSlider_play_2_clubs_valueChanged(int value);
    void on_horizontalSlider_shuffle_valueChanged(int value);
    void on_horizontalSlider_clear_table_valueChanged(int value);
    void on_horizontalSlider_your_turn_valueChanged(int value);
    void on_horizontalSlider_pass_cards_valueChanged(int value);
    void on_horizontalSlider_animate_pass_cards_valueChanged(int value);
    void on_horizontalSlider_animate_play_card_valueChanged(int value);
    void on_pushButton_save_clicked();

public:
    void save();
    void reset();
    void popup();
    void Translate();

private:
    Ui::CSpeed *ui;
    CConfig *mConfig;
};

#endif // SPEED_H
