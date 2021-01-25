#ifdef DEBUG

#include "ccardsplayed.h"

CCardsPlayed::CCardsPlayed(CDeck *d)
{
  setWindowTitle(tr("Cards played"));

  setAttribute( Qt::WA_QuitOnClose, false );

  deck = d;

  box_clubs = new QHBoxLayout;
  box_spade = new QHBoxLayout;
  box_diamond = new QHBoxLayout;
  box_hearts = new QHBoxLayout;

  for (int i=0; i<DECK_SIZE; i++) {
    labels[i] = new QLabel(this);
    labels[i]->resize(55, 100);
    labels[i]->setPixmap(QPixmap::fromImage(deck->get_img_card(i)->scaledToHeight(100)));
    if (i <= 12)
      box_clubs->addWidget(labels[i]);
    else
       if (i <= 25)
         box_spade->addWidget(labels[i]);
       else
          if (i <= 38)
            box_diamond->addWidget(labels[i]);
          else
            box_hearts->addWidget(labels[i]);
  }

  QGroupBox *grp1 = new QGroupBox;
  grp1->setLayout(box_clubs);

  QGroupBox *grp2 = new QGroupBox;
  grp2->setLayout(box_spade);

  QGroupBox *grp3 = new QGroupBox;
  grp3->setLayout(box_diamond);

  QGroupBox *grp4 = new QGroupBox;
  grp4->setLayout(box_hearts);

  layout = new QVBoxLayout();
  layout->addWidget(grp1);
  layout->addWidget(grp2);
  layout->addWidget(grp3);
  layout->addWidget(grp4);

  setLayout(layout);

  QPalette pal = palette();

  pal.setColor(QPalette::Window, QColor(63, 159, 82));
  pal.setColor(QPalette::WindowText, Qt::black);
  setPalette(pal);

  resize(800, 600);
}

CCardsPlayed::~CCardsPlayed()
{
  for (int i=0; i<DECK_SIZE; i++)
     delete labels[i];

  delete box_clubs;
  delete box_spade;
  delete box_diamond;
  delete box_hearts;
  delete layout;
}

void CCardsPlayed::play(int card)
{
  if ((card < 0) || (card >= DECK_SIZE)) return;

  labels[card]->setDisabled(true);
}

void CCardsPlayed::reset()
{
  for (int i=0; i<DECK_SIZE; i++)
    labels[i]->setDisabled(false);
}

void CCardsPlayed::set_deck()
{
  for (int i=0; i<DECK_SIZE; i++)
    labels[i]->setPixmap(QPixmap::fromImage(deck->get_img_card(i)->scaledToHeight(100)));
}

void CCardsPlayed::Translate()
{
  setWindowTitle(tr("Cards played"));
}

#endif // DEBUG
