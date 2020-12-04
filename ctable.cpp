#ifdef ONLINE_PLAY

#include "ctable.h"
#include "define.h"
#include "ui_ctable.h"

CTable::CTable(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CTable)
{
  ui->setupUi(this);
  ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

  setWindowTitle(tr("Tables list"));

  setAttribute( Qt::WA_QuitOnClose, false );
}

CTable::~CTable()
{
  delete ui;
}

void CTable::AddRow(QString id, QString flags)
{
  int rowc = ui->tableWidget->rowCount();
  ui->tableWidget->insertRow(rowc);

  QTableWidgetItem *item_id = new QTableWidgetItem(id);
  ui->tableWidget->setItem(rowc, 0, item_id);

  QString s;
  int i = flags.toInt();

  if (i & QUEEN_SPADE_f) s += "Q ";
  if (i & PERFECT_100_f) s += "P ";
  if (i & OMNIBUS_f) s += "O ";
  if (i & NO_TRICK_BONUS_f) s += "T ";
  if (i & NEW_MOON_f) s += "M ";
  if (!(i & NO_DRAW_f)) s += "D";

  QTableWidgetItem *item_flags = new QTableWidgetItem(s);
  ui->tableWidget->setItem(rowc, 1, item_flags);

  for (int i=0; i<4; i++) {
    QTableWidgetItem *empty_item = new QTableWidgetItem("");
    ui->tableWidget->setItem(rowc, i+2, empty_item);
  }
}

void CTable::Empty()
{
  ui->tableWidget->setRowCount(0);
}

void CTable::RemoveRow(QString id)
{
  int rowc = ui->tableWidget->rowCount();

  for (int i=0; i<rowc; i++) {
    if (ui->tableWidget->item(i, 0)->text() == id) {
      ui->tableWidget->removeRow(i); // by removing a row, the index changes, don't forget to break.
      break;
    }
  }
}

void CTable::SetPlayer(QString id, QString name, QString chair)
{
 int rowc = ui->tableWidget->rowCount();
 int c = 0;

 if (chair == "n") c = 2;
 else
   if (chair == "s") c = 3;
   else
     if (chair == "w") c = 4;
     else
       if (chair == "e") c = 5;

 if (!c) return;

 for (int i=0; i<rowc; i++) {
   if (ui->tableWidget->item(i, 0)->text() == id) {
     ui->tableWidget->item(i, c)->setText(name);
   }
 }
}

void CTable::on_tableWidget_cellDoubleClicked(int row, int column)
{
  int id = ui->tableWidget->item(row, 0)->text().toInt();
  char chair = ' ';

  switch (column) {
     case 2: chair = 'n'; break;
     case 3: chair = 's'; break;
     case 4: chair = 'w'; break;
     case 5: chair = 'e'; break;
  }

  emit clicked(id, chair);
}

void CTable::Translate()
{
  ui->retranslateUi(this);
}

#endif // ONLINE_PLAY
