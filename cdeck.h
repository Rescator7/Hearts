#ifndef CDECK_H
#define CDECK_H

#include "define.h"
#include <QImage>
#include "chearts.h"

#ifdef DEBUG

#include "ccardsplayed.h"

class CCardsPlayed;
#endif // DEBUG

const int STANDARD_DECK        = 0;
const int ENGLISH_DECK         = 1;
const int RUSSIAN_DECK         = 2;
const int NICU_WHITE_DECK      = 3;
const int TIGULLIO_MODERN_DECK = 4;

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

#ifdef DEBUG
    CCardsPlayed *cards_played;
#endif // DEBUG

    int current_deck;

    void delete_current_deck();
    void set_standard_deck();
    void set_english_deck();
    void set_russian_deck();
    void set_nicu_white_deck();
    void set_tigullio_modern_deck();

public:
    QImage *get_img_card(int card);
    void reverse_card_rgb(int card);
    void set_deck(int deck);

#ifdef DEBUG
    void set_card_played(int card);
    void reset_cards_played();
    void show_cards_played();
    void hide_cards_played();
    void Translate();
#endif // DEBUG
};

#endif // CDECK_H
