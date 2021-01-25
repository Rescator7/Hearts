#include <QFile>
#include <QDir>
#include <QTextStream>

#include "cstatistics.h"
#include "ui_cstatistics.h"

CStatistics::CStatistics(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CStatistics)
{
  ui->setupUi(this);

  setWindowTitle(tr("Game Statistics"));

  setAttribute( Qt::WA_QuitOnClose, false );

  init_vars();

  QFile file(QDir::homePath() + STATS_FILENAME);

  file_corrupted = false;
  if (!file.exists())
    save_stats_file();                         // create the file by saving the default values.
  else {
    if (load_stats_file() == FCORRUPTED)
      file_corrupted = true;
  }

  ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

  create_window();

  file.close();
}

CStatistics::~CStatistics()
{
  delete ui;
}

void CStatistics::init_vars()
{
  game_started = 1;
  game_finished = 0;
  hands_played = 0;
  undo = 0;

  for (int i=0; i<MAX_PLR_NAMES; i++) {
    first_place[i] = 0;
    second_place[i] = 0;
    third_place[i] = 0;
    fourth_place[i] = 0;

    total_score[i] = 0;
    best_score[i] = -1;
    worst_score[i] = -1;

    shoot_moon[i] = 0;
    queen_spade_taken[i] = 0;
    omnibus_bonus_taken[i] = 0;
    no_tricks_bonus_taken[i] = 0;
    perfect_100_bonus_taken[i] = 0;
  }
}

int CStatistics::save_stats_file()
{
  QFile file(QDir::homePath() + STATS_FILENAME);

  if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
    return ERROPENWO;
  }

  QTextStream out(&file);

  out << game_started << " " << game_finished << " " << hands_played << " " << undo << EOL;

  for (int i=0; i<MAX_PLR_NAMES; i++) {
     out << first_place[i]           << " " << second_place[i]        << " " <<
            third_place[i]           << " " << fourth_place[i]        << " " <<
            total_score[i]           << " " << best_score[i]          << " " <<
            worst_score[i]           << " " << shoot_moon[i]          << " " <<
            queen_spade_taken[i]     << " " << omnibus_bonus_taken[i] << " " <<
            no_tricks_bonus_taken[i] << " " << perfect_100_bonus_taken[i] << EOL;
  }

  file.close();

  return FNOERR;
}

int CStatistics::load_stats_file()
{
  QFile file(QDir::homePath() + STATS_FILENAME);

  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    return ERROPENRO;
  }

  int value;
  QString line = file.readLine();

  value = line.section(' ', 0, 0).toInt();
  if (value < 1) {
    file.close();
    return FCORRUPTED;
  }

  game_started = value;

  value = line.section(' ', 1, 1).toInt();
  if (value < 0) {
    file.close();
    return FCORRUPTED;
  }

  game_finished = value;

  value = line.section(' ', 2, 2).toInt();
  if (value < 0) {
    file.close();
    return FCORRUPTED;
  }

  hands_played = value;

  value = line.section(' ', 3, 3).toInt();
  if (value < 0) {
    file.close();
    return FCORRUPTED;
  }

  undo = value;

  int cpt = 0;

  while (!file.atEnd()) {
    line = file.readLine();

    for (int i=0; i<12; i++) {
       value = line.section(' ', i, i).toInt();

       if ((value < 0) && (i != 5) && (i != 6)) {
         file.close();
         return FCORRUPTED;
       }

       switch (i) {
          case 0 : first_place[cpt] = value; break;
          case 1 : second_place[cpt] = value; break;
          case 2 : third_place[cpt] = value; break;
          case 3 : fourth_place[cpt] = value; break;
          case 4 : total_score[cpt] = value; break;
          case 5 : best_score[cpt] = value; break;
          case 6 : worst_score[cpt] = value; break;
          case 7 : shoot_moon[cpt] = value; break;
          case 8 : queen_spade_taken[cpt] = value; break;
          case 9 : omnibus_bonus_taken[cpt] = value; break;
          case 10: no_tricks_bonus_taken[cpt] = value; break;
          case 11: perfect_100_bonus_taken[cpt] = value; break;
       }
    }
    cpt++;
  }

 if (cpt != MAX_PLR_NAMES) {
   file.close();
   return FCORRUPTED;
 }

 file.close();

 return FNOERR;
}

void CStatistics::update_window(int plr, int stats)
{
 double total = first_place[plr] + second_place[plr] + third_place[plr] + fourth_place[plr];

 if (!static_cast<bool>(total))
   total = 1;                // avoid division by 0

 double started = game_started;

 if (!static_cast<bool>(started))
   started = 1;

 int value;

 switch (stats) {
   case STATS_SCORES :      if (best_score[plr] != -1) {
                               item_best_score[plr]->setData(Qt::EditRole, best_score[plr]);
                               item_avg_score[plr]->setData(Qt::EditRole, total_score[plr] / total);
                             } else {
                                 item_best_score[plr]->setData(Qt::EditRole, "");
                                 item_avg_score[plr]->setData(Qt::EditRole, "");
                               }

                             if (worst_score[plr] != -1)
                               item_worst_score[plr]->setData(Qt::EditRole, worst_score[plr]);
                             else
                               item_worst_score[plr]->setData(Qt::EditRole, "");
                             break;
   case STATS_GAME_STARTED :
   case STATS_UNDO :
   case STATS_GAME_FINISHED :
   case STATS_HANDS_PLAYED :  ui->label_games_started->setText(QString::number(game_started));
                              ui->label_games_finished->setText(QString::number(game_finished) + " (" + QString::number(double(game_finished * 100 / started), 'f', 1) + QString("%)") );
                              ui->label_hands_played->setText(QString::number(hands_played));
                              ui->label_undos->setText(QString::number(undo));
                              break;
   case STATS_FIRST_PLACE :
   case STATS_SECOND_PLACE :
   case STATS_THIRD_PLACE :
   case STATS_FOURTH_PLACE :
                             value = first_place[plr];
                             item_first_place[plr]->setText(QString::number(value) + " (" +
                                                            QString::number(double(value * 100 / total), 'f', 1) + "%)");
                             value = second_place[plr];
                             item_second_place[plr]->setText(QString::number(value) + " (" +
                                                            QString::number(double(value * 100 / total), 'f', 1) + "%)");
                             value = third_place[plr];
                             item_third_place[plr]->setText(QString::number(value) + " (" +
                                                            QString::number(double(value * 100 / total), 'f', 1) + "%)");
                             value = fourth_place[plr];
                             item_fourth_place[plr]->setText(QString::number(value) + " (" +
                                                             QString::number(double(value * 100 / total), 'f', 1) + "%)");
                             break;
   case STATS_SHOOT_MOON :   item_shoot_moon[plr]->setData(Qt::EditRole, shoot_moon[plr]);
                             break;
   case STATS_QUEEN_SPADE :  item_queen_spade[plr]->setData(Qt::EditRole, queen_spade_taken[plr]);
                             break;
   case STATS_OMNIBUS :      item_omnibus[plr]->setData(Qt::EditRole, omnibus_bonus_taken[plr]);
                             break;
   case STATS_NO_TRICKS :    item_no_tricks[plr]->setData(Qt::EditRole, no_tricks_bonus_taken[plr]);
                             break;
   case STATS_PERFECT_100 :  item_perfect_100[plr]->setData(Qt::EditRole, perfect_100_bonus_taken[plr]);
                             break;
  }
}

void CStatistics::increase_stats(int plr, int stats)
{
  switch (stats) {
     case STATS_GAME_STARTED:  game_started++; break;
     case STATS_GAME_FINISHED: game_finished++; break;
     case STATS_UNDO:          undo++; break;
     case STATS_HANDS_PLAYED:  hands_played++; break;
     case STATS_FIRST_PLACE:   first_place[plr]++; break;
     case STATS_SECOND_PLACE:  second_place[plr]++; break;
     case STATS_THIRD_PLACE:   third_place[plr]++; break;
     case STATS_FOURTH_PLACE:  fourth_place[plr]++; break;
     case STATS_SHOOT_MOON:    shoot_moon[plr]++; break;
     case STATS_QUEEN_SPADE:   queen_spade_taken[plr]++; break;
     case STATS_OMNIBUS:       omnibus_bonus_taken[plr]++; break;
     case STATS_NO_TRICKS:     no_tricks_bonus_taken[plr]++; break;
     case STATS_PERFECT_100:   perfect_100_bonus_taken[plr]++; break;
  }

  update_window(plr, stats);
}

void CStatistics::set_score(int plr, int score)
{
  total_score[plr] += score;

  if ((best_score[plr] == -1) || (score < best_score[plr]))
    best_score[plr] = score;

  if ((worst_score[plr] == -1) || (score > worst_score[plr]))
    worst_score[plr] = score;

  update_window(plr, STATS_SCORES);
}

void CStatistics::reset()
{
 init_vars();

 update_window(0, STATS_GAME_STARTED);

 for (int i=0; i<MAX_PLR_NAMES; i++) {
   update_window(i, STATS_SCORES);
   update_window(i, STATS_FIRST_PLACE);
   update_window(i, STATS_SECOND_PLACE);
   update_window(i, STATS_THIRD_PLACE);
   update_window(i, STATS_FOURTH_PLACE);
   update_window(i, STATS_SHOOT_MOON);
   update_window(i, STATS_QUEEN_SPADE);
   update_window(i, STATS_OMNIBUS);
   update_window(i, STATS_NO_TRICKS);
   update_window(i, STATS_PERFECT_100);
 }

 save_stats_file();
}

void CStatistics::create_window()
{
  QColor color1(78, 199, 102);
  QColor color2(94, 239, 123);

  for (int i=0; i<MAX_PLR_NAMES; i++) {
     item_names[i] = new QTableWidgetItem(QString(names[i]));
     item_names[i]->setFlags(item_names[i]->flags() & ~Qt::ItemIsEditable);
     item_names[i]->setBackground(color1);
     ui->tableWidget->setItem(i, 0, item_names[i]);

     double total = first_place[i] + second_place[i] + third_place[i] + fourth_place[i];

     if (!static_cast<bool>(total))
       total = 1;

     for (int i2=0; i2<4; i2++) {
       int value;

       switch (i2) {
         case 0 : value = first_place[i];
                  item_first_place[i] = new QTableWidgetItem(QString::number(value) + " (" +
                                                             QString::number(double(value * 100 / total), 'f', 1) + "%)");
                  item_first_place[i]->setFlags(item_first_place[i]->flags() & ~Qt::ItemIsEditable);
                  item_first_place[i]->setBackground(color2);
                  ui->tableWidget->setItem(i, 1, item_first_place[i]);
                  break;
         case 1 : value = second_place[i];
                  item_second_place[i] = new QTableWidgetItem(QString::number(value) + " (" +
                                                              QString::number((value * 100 / total), 'f', 1) + "%)");
                  item_second_place[i]->setFlags(item_second_place[i]->flags() & ~Qt::ItemIsEditable);
                  item_second_place[i]->setBackground(color1);
                  ui->tableWidget->setItem(i, 2, item_second_place[i]);
                  break;
         case 2 : value = third_place[i];
                  item_third_place[i] = new QTableWidgetItem(QString::number(value) + " (" +
                                                             QString::number(double(value * 100 / total), 'f', 1) + "%)");
                  item_third_place[i]->setFlags(item_third_place[i]->flags() & ~Qt::ItemIsEditable);
                  item_third_place[i]->setBackground(color2);
                  ui->tableWidget->setItem(i, 3, item_third_place[i]);
                  break;
         case 3 : value = fourth_place[i];
                  item_fourth_place[i] = new QTableWidgetItem(QString::number(value) + " (" +
                                                              QString::number(double(value * 100 / total), 'f', 1) + "%)");
                  item_fourth_place[i]->setFlags(item_fourth_place[i]->flags() & ~Qt::ItemIsEditable);
                  item_fourth_place[i]->setBackground(color1);
                  ui->tableWidget->setItem(i, 4, item_fourth_place[i]);
                  break;
       }
     }

     item_best_score[i] = new QTableWidgetItem;
     item_avg_score[i] = new QTableWidgetItem;

     if (best_score[i] != -1) {
       item_best_score[i]->setData(Qt::EditRole, best_score[i]);
       item_avg_score[i]->setData(Qt::EditRole, total_score[i] / total);
     }

     item_worst_score[i] = new QTableWidgetItem;
     if (worst_score[i] != -1) {
       item_worst_score[i]->setData(Qt::EditRole, worst_score[i]);
     }

     item_shoot_moon[i] = new QTableWidgetItem;
     item_shoot_moon[i]->setData(Qt::EditRole, shoot_moon[i]);

     item_queen_spade[i] = new QTableWidgetItem;
     item_queen_spade[i]->setData(Qt::EditRole, queen_spade_taken[i]);

     item_omnibus[i] = new QTableWidgetItem;
     item_omnibus[i]->setData(Qt::EditRole, omnibus_bonus_taken[i]);

     item_no_tricks[i] = new QTableWidgetItem;
     item_no_tricks[i]->setData(Qt::EditRole, no_tricks_bonus_taken[i]);

     item_perfect_100[i] = new QTableWidgetItem;
     item_perfect_100[i]->setData(Qt::EditRole, perfect_100_bonus_taken[i]);

     item_avg_score[i]->setFlags(item_avg_score[i]->flags() & ~Qt::ItemIsEditable);
     item_best_score[i]->setFlags(item_best_score[i]->flags() & ~Qt::ItemIsEditable);
     item_worst_score[i]->setFlags(item_worst_score[i]->flags() & ~Qt::ItemIsEditable);
     item_shoot_moon[i]->setFlags(item_shoot_moon[i]->flags() & ~Qt::ItemIsEditable);
     item_queen_spade[i]->setFlags(item_queen_spade[i]->flags() & ~Qt::ItemIsEditable);
     item_omnibus[i]->setFlags(item_omnibus[i]->flags() & ~Qt::ItemIsEditable);
     item_no_tricks[i]->setFlags(item_no_tricks[i]->flags() & ~Qt::ItemIsEditable);
     item_perfect_100[i]->setFlags(item_perfect_100[i]->flags() & ~Qt::ItemIsEditable);

     item_avg_score[i]->setBackground(color2);
     item_best_score[i]->setBackground(color1);
     item_worst_score[i]->setBackground(color2);
     item_shoot_moon[i]->setBackground(color1);
     item_queen_spade[i]->setBackground(color2);
     item_omnibus[i]->setBackground(color1);
     item_no_tricks[i]->setBackground(color2);
     item_perfect_100[i]->setBackground(color1);

     ui->tableWidget->setItem(i, 5, item_avg_score[i]);
     ui->tableWidget->setItem(i, 6, item_best_score[i]);
     ui->tableWidget->setItem(i, 7, item_worst_score[i]);
     ui->tableWidget->setItem(i, 8, item_shoot_moon[i]);
     ui->tableWidget->setItem(i, 9, item_queen_spade[i]);
     ui->tableWidget->setItem(i, 10, item_omnibus[i]);
     ui->tableWidget->setItem(i, 11, item_no_tricks[i]);
     ui->tableWidget->setItem(i, 12, item_perfect_100[i]);
  }

  ui->tableWidget->setSortingEnabled(true);

  update_window(0, STATS_GAME_STARTED);
}

bool CStatistics::is_file_corrupted()
{
  return file_corrupted;
}

void CStatistics::Translate()
{
  ui->retranslateUi(this);
}

