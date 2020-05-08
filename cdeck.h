#ifndef CDECK_H
#define CDECK_H

#include <QImage>
#include "chearts.h"

const int STANDARD_DECK = 0;
const int ENGLISH_DECK  = 1;
const int RUSSIAN_DECK  = 2;

class CDeck
{
public:
    CDeck(int deck);
    ~CDeck();

private:
    QImage *img_cards[DECK_SIZE];
    QImage *img_empty_card;
    QImage *img_your_turn;
    QImage *img_back_card;
    QImage *img_sit_here;

    int current_deck;

    void delete_current_deck();
    void set_standard_deck();
    void set_english_deck();
    void set_russian_deck();

public:
    QImage *get_img_card(int card);
    void reverse_card_rgb(int card);
    void set_deck(int deck);
};

#endif // CDECK_H
