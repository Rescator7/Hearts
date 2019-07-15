#ifndef CIMG_DECKCARDS_H
#define CIMG_DECKCARDS_H

#include <QImage>

const int STANDARD_DECK = 0;
const int ENGLISH_DECK  = 1;
const int RUSSIAN_DECK  = 2;

class CImg_deckcards
{
public:
    CImg_deckcards(int deck);
    ~CImg_deckcards();

private:
    QImage *img_cards[52];
    QImage *img_pass[4];
    QImage *img_empty_card;
    QImage *img_your_turn;

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

#endif // CIMG_DECKCARDS_H
