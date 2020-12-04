#ifdef DEBUG

#include "debug.h"

#include "assert.h"
#include "chearts.h"

#include <QString>
#include <QPalette>
#include <QColor>
#include <QGroupBox>

const char PLR_NOBODY[7] = "Nobody";

CDebug::CDebug(CDeck *d)
{
  setWindowTitle(tr("Cards history"));

  setAttribute( Qt::WA_QuitOnClose, false );

  deck = d;

  box1 = new QHBoxLayout;

  for (int i=0; i<7; i++) {
    labels[i] = new QLabel(this);
    labels[i]->resize(80, 100);
    box1->addWidget(labels[i]);
  }

  box2 = new QHBoxLayout;

  for (int i=7; i<14; i++) {
    labels[i] = new QLabel(this);
    box2->addWidget(labels[i]);
  }

  QGroupBox *grp1 = new QGroupBox;
  grp1->setLayout(box1);

  QGroupBox *grp2 = new QGroupBox;
  grp2->setLayout(box2);

  bar = new QScrollBar(Qt::Horizontal, this);
  bar->setMaximum(0);

  connect(bar, SIGNAL(valueChanged(int)), this, SLOT(handle_bar(int)));

  layout = new QVBoxLayout();
  layout->addWidget(grp1);
  layout->addWidget(grp2);
  layout->addWidget(bar);

  setLayout(layout);

  QPalette pal = palette();
  pal.setColor(QPalette::Background, QColor(63, 159, 82));
  pal.setColor(QPalette::WindowText, Qt::black);
  setPalette(pal);

  reset();

  resize(600, 200);
}

CDebug::~CDebug()
{
  for (int i=0; i<14; i++)
    delete labels[i];

  delete bar;
  delete box1;
  delete box2;
  delete layout;
}

void CDebug::show_history(int slide)
{
  for (int i=0; i<7; i++) {
    labels[i]->setPixmap(QPixmap::fromImage(deck->get_img_card(cards[i + ptr_screen - slide])->scaledToHeight(100)));
    labels[i+7]->setText(plr_names[i + ptr_screen - slide]);

    if (winners[i + ptr_screen - slide])
      labels[i+7]->setStyleSheet("QLabel { color : white; }");
    else
      labels[i+7]->setStyleSheet("QLabel { color : black; }");
  }
}

void CDebug::save_card(int card, const char *name)
{
  if (history_size >= MAX_HISTORY_SIZE)
    reset();

  cards[history_size] = card;

  if (name)
    plr_names[history_size] = name;
  else
    plr_names[history_size] = reinterpret_cast<const char *>(&PLR_NOBODY);

  if (card == empty)
    cards_saved = 0;
  else
    cards_saved++;

  if (cards_saved == 1) {
    suit = card / 13;
    best_pos = 3;
    best = card;
  } else {
      if ((card / 13 == suit) && (card > best)) {
        best = card;
        best_pos = 4 - cards_saved;
      }
      if (cards_saved == 4) {
        cards_saved = 0;
        winners[history_size - best_pos] = true;
      }
  }

  history_size++;

  if (history_size >= 7) {
    bar->setMaximum(history_size - 7);
    bar->setValue(history_size - 7);

    ptr_screen = history_size - 7;
  } else
      ptr_screen = 0;

  show_history(0);
}

void CDebug::reset()
{
  history_size = 0;
  cards_saved = 0;
  ptr_screen = 0;
  suit = 0;
  best = 0;
  best_pos = 0;

  bar->setMaximum(0);
  bar->setValue(0);

  // show 7 empty cards
  for (int i=0; i<7; i++) {
    labels[i]->setPixmap(QPixmap::fromImage(deck->get_img_card(empty)->scaledToHeight(100)));
    labels[i+7]->setText("Nobody");
    labels[i+7]->setStyleSheet("QLabel { color : black; }");
  }

  // show 7 Nobody as names
  for (int i=0; i<MAX_HISTORY_SIZE; i++) {
    winners[i] = false;
    cards[i] = empty;
    plr_names[i] = reinterpret_cast<const char *>(&PLR_NOBODY);
  }
}

void CDebug::handle_bar(int value)
{
  show_history(bar->maximum() - value);
}

void CDebug::Translate()
{
  setWindowTitle(tr("Cards history"));
}

void CDebug::refresh()
{
  show_history(bar->maximum() - bar->value());
}

#endif // DEBUG
