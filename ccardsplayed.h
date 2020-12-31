#ifdef DEBUG

#ifndef CCARDSPLAYED_H
#define CCARDSPLAYED_H

#include "define.h"

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>

#include "cdeck.h"

class CDeck;

class CCardsPlayed : public QWidget
{
    Q_OBJECT

public:
    CCardsPlayed(CDeck *d);
    ~CCardsPlayed();

private: // variables;
    CDeck       *deck;
    QVBoxLayout *layout;
    QHBoxLayout *box_clubs;
    QHBoxLayout *box_spade;
    QHBoxLayout *box_diamond;
    QHBoxLayout *box_hearts;

    QLabel *labels[52];

public:
    void play(int card);
    void reset();
    void set_deck();
    void Translate();
signals:

public slots:
};
#endif // CCARDSPLAYED_H

#endif // DEBUG
