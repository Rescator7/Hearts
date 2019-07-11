#include "cimg_deckcards.h"
#include <assert.h>

CImg_deckcards::CImg_deckcards(int deck)
{
  set_default_deck();

  // We could had more deck (Here), eventually.
}

void CImg_deckcards::set_default_deck(){
  img_cards[0] = new QImage(":/SVG-cards/club_2.png", "PNG");
  img_cards[1] = new QImage(":/SVG-cards/club_3.png", "PNG");
  img_cards[2] = new QImage(":/SVG-cards/club_4.png", "PNG");
  img_cards[3] = new QImage(":/SVG-cards/club_5.png", "PNG");
  img_cards[4] = new QImage(":/SVG-cards/club_6.png", "PNG");
  img_cards[5] = new QImage(":/SVG-cards/club_7.png", "PNG");
  img_cards[6] = new QImage(":/SVG-cards/club_8.png", "PNG");
  img_cards[7] = new QImage(":/SVG-cards/club_9.png", "PNG");
  img_cards[8] = new QImage(":/SVG-cards/club_10.png", "PNG");
  img_cards[9] = new QImage(":/SVG-cards/club_jack.png", "PNG");
  img_cards[10] = new QImage(":/SVG-cards/club_queen.png", "PNG");
  img_cards[11] = new QImage(":/SVG-cards/club_king.png", "PNG");
  img_cards[12] = new QImage(":/SVG-cards/club_1.png", "PNG");

  img_cards[13] = new QImage(":/SVG-cards/spade_2.png", "PNG");
  img_cards[14] = new QImage(":/SVG-cards/spade_3.png", "PNG");
  img_cards[15] = new QImage(":/SVG-cards/spade_4.png", "PNG");
  img_cards[16] = new QImage(":/SVG-cards/spade_5.png", "PNG");
  img_cards[17] = new QImage(":/SVG-cards/spade_6.png", "PNG");
  img_cards[18] = new QImage(":/SVG-cards/spade_7.png", "PNG");
  img_cards[19] = new QImage(":/SVG-cards/spade_8.png", "PNG");
  img_cards[20] = new QImage(":/SVG-cards/spade_9.png", "PNG");
  img_cards[21] = new QImage(":/SVG-cards/spade_10.png", "PNG");
  img_cards[22] = new QImage(":/SVG-cards/spade_jack.png", "PNG");
  img_cards[23] = new QImage(":/SVG-cards/spade_queen.png", "PNG");
  img_cards[24] = new QImage(":/SVG-cards/spade_king.png", "PNG");
  img_cards[25] = new QImage(":/SVG-cards/spade_1.png", "PNG");

  img_cards[26] = new QImage(":/SVG-cards/diamond_2.png", "PNG");
  img_cards[27] = new QImage(":/SVG-cards/diamond_3.png", "PNG");
  img_cards[28] = new QImage(":/SVG-cards/diamond_4.png", "PNG");
  img_cards[29] = new QImage(":/SVG-cards/diamond_5.png", "PNG");
  img_cards[30] = new QImage(":/SVG-cards/diamond_6.png", "PNG");
  img_cards[31] = new QImage(":/SVG-cards/diamond_7.png", "PNG");
  img_cards[32] = new QImage(":/SVG-cards/diamond_8.png", "PNG");
  img_cards[33] = new QImage(":/SVG-cards/diamond_9.png", "PNG");
  img_cards[34] = new QImage(":/SVG-cards/diamond_10.png", "PNG");
  img_cards[35] = new QImage(":/SVG-cards/diamond_jack.png", "PNG");
  img_cards[36] = new QImage(":/SVG-cards/diamond_queen.png", "PNG");
  img_cards[37] = new QImage(":/SVG-cards/diamond_king.png", "PNG");
  img_cards[38] = new QImage(":/SVG-cards/diamond_1.png", "PNG");

  img_cards[39] = new QImage(":/SVG-cards/heart_2.png", "PNG");
  img_cards[40] = new QImage(":/SVG-cards/heart_3.png", "PNG");
  img_cards[41] = new QImage(":/SVG-cards/heart_4.png", "PNG");
  img_cards[42] = new QImage(":/SVG-cards/heart_5.png", "PNG");
  img_cards[43] = new QImage(":/SVG-cards/heart_6.png", "PNG");
  img_cards[44] = new QImage(":/SVG-cards/heart_7.png", "PNG");
  img_cards[45] = new QImage(":/SVG-cards/heart_8.png", "PNG");
  img_cards[46] = new QImage(":/SVG-cards/heart_9.png", "PNG");
  img_cards[47] = new QImage(":/SVG-cards/heart_10.png", "PNG");
  img_cards[48] = new QImage(":/SVG-cards/heart_jack.png", "PNG");
  img_cards[49] = new QImage(":/SVG-cards/heart_queen.png", "PNG");
  img_cards[50] = new QImage(":/SVG-cards/heart_king.png", "PNG");
  img_cards[51] = new QImage(":/SVG-cards/heart_1.png", "PNG");
}

CImg_deckcards::~CImg_deckcards()
{
 assert(img_cards[0]);

 for (int i=0; i<52; i++)
   delete (img_cards[i]);
}

QImage *CImg_deckcards::get_img_card(int card) {
  return img_cards[card];
}

void CImg_deckcards::reverse_card_rgb(int card) {
  img_cards[card]->invertPixels();
}
