#include "speed.h"
#include "ui_speed.h"
#include "config.h"

CSpeed::CSpeed(QWidget *parent, CConfig *config) :
    QWidget(parent),
    ui(new Ui::CSpeed)
{
    ui->setupUi(this);

    mConfig = config;
}

CSpeed::~CSpeed()
{
    delete ui;
}

void CSpeed::popup()
{
  reset();
  show();
}

void CSpeed::reset()
{
  ui->horizontalSlider_play_a_card->setValue(mConfig->get_speed(SPEED_PLAY_CARD));
  ui->horizontalSlider_play_2_clubs->setValue(mConfig->get_speed(SPEED_PLAY_TWO_CLUBS));
  ui->horizontalSlider_shuffle->setValue(mConfig->get_speed(SPEED_SHUFFLE));
  ui->horizontalSlider_clear_table->setValue(mConfig->get_speed(SPEED_CLEAR_TABLE));
  ui->horizontalSlider_your_turn->setValue(mConfig->get_speed(SPEED_YOUR_TURN));
  ui->horizontalSlider_pass_cards->setValue(mConfig->get_speed(SPEED_PASS_CARDS));
  ui->horizontalSlider_animate_pass_cards->setValue(mConfig->get_speed(SPEED_ANIMATE_PASS_CARDS));
  ui->horizontalSlider_animate_play_card->setValue(mConfig->get_speed(SPEED_ANIMATE_PLAY_CARD));
}

void CSpeed::on_pushButton_cancel_clicked()
{
  hide();
}

void CSpeed::Translate()
{
  ui->retranslateUi(this);
}

void CSpeed::on_horizontalSlider_play_a_card_valueChanged(int value)
{
  ui->lcdNumber_play_a_card->display(value);
}

void CSpeed::on_horizontalSlider_play_2_clubs_valueChanged(int value)
{
  ui->lcdNumber_play_2_clubs->display(value);
}

void CSpeed::on_horizontalSlider_shuffle_valueChanged(int value)
{
  ui->lcdNumber_shuffle->display(value);
}

void CSpeed::on_horizontalSlider_clear_table_valueChanged(int value)
{
  ui->lcdNumber_clear_table->display(value);
}

void CSpeed::on_horizontalSlider_your_turn_valueChanged(int value)
{
  ui->lcdNumber_your_turn->display(value);
}

void CSpeed::on_horizontalSlider_pass_cards_valueChanged(int value)
{
  ui->lcdNumber_pass_cards->display(value);
}

void CSpeed::on_horizontalSlider_animate_pass_cards_valueChanged(int value)
{
  ui->lcdNumber_animate_pass_cards->display(value);
}

void CSpeed::on_horizontalSlider_animate_play_card_valueChanged(int value)
{
  ui->lcdNumber_animate_play_card->display(value);
}

void CSpeed::on_pushButton_save_clicked()
{
  mConfig->set_expert_speeds(ui->horizontalSlider_play_a_card->value(),
                             ui->horizontalSlider_play_2_clubs->value(),
                             ui->horizontalSlider_shuffle->value(),
                             ui->horizontalSlider_clear_table->value(),
                             ui->horizontalSlider_your_turn->value(),
                             ui->horizontalSlider_pass_cards->value(),
                             ui->horizontalSlider_animate_pass_cards->value(),
                             ui->horizontalSlider_animate_play_card->value());
  hide();
}
