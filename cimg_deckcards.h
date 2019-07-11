#ifndef CIMG_DECKCARDS_H
#define CIMG_DECKCARDS_H

#include <QImage>

const int DEFAULT_DECK = 0;

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

    void set_default_deck();

public:
    QImage *get_img_card(int card);
    void reverse_card_rgb(int card);
};

#endif // CIMG_DECKCARDS_H
