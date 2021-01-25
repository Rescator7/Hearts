#ifdef ONLINE_PLAY

#include <QPushButton>

#include "cgame.h"
#include "ui_cgame.h"
#include "define.h"

CGame::CGame(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CGame)
{
  ui->setupUi(this);

  ui->buttonBox->button(QDialogButtonBox::Cancel)->setText(tr("Cancel"));
  ui->buttonBox->button(QDialogButtonBox::Ok)->setText(tr("Ok"));

  flags = 0;
}

CGame::~CGame()
{
  delete ui;
}

void CGame::set_flags(unsigned int f)
{
  flags = f;

  if (flags & QUEEN_SPADE_f)
    ui->checkBox->setChecked(true);

  if (flags & PERFECT_100_f)
    ui->checkBox_2->setChecked(true);

  if (flags & OMNIBUS_f)
    ui->checkBox_3->setChecked(true);

  if (flags & NO_TRICK_BONUS_f)
    ui->checkBox_4->setChecked(true);

  if (flags & NEW_MOON_f)
    ui->checkBox_5->setChecked(true);

  if (flags & NO_DRAW_f)
    ui->checkBox_6->setChecked(true);
}

unsigned int CGame::get_flags()
{
  return flags;
}

void CGame::on_buttonBox_accepted()
{
  flags = 0;
  if (ui->checkBox->isChecked())
    flags = QUEEN_SPADE_f;

  if (ui->checkBox_2->isChecked())
    flags |= PERFECT_100_f;

  if (ui->checkBox_3->isChecked())
    flags |= OMNIBUS_f;

  if (ui->checkBox_4->isChecked())
    flags |= NO_TRICK_BONUS_f;

  if (ui->checkBox_5->isChecked())
    flags |= NEW_MOON_f;

  if (ui->checkBox_6->isChecked())
    flags |= NO_DRAW_f;
}

#endif // ONLINE_PLAY
