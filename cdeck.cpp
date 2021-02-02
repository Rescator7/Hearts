#include "cdeck.h"
#include <assert.h>
#include <QGroupBox>

CDeck::CDeck(int deck)
{
  current_deck = deck;

  for (int i=0; i<DECK_SIZE; i++)
     img_cards[i] = nullptr;

  img_empty_card = new QImage(":/SVG-cards/Default/empty.png", "PNG");
  img_your_turn = new QImage(":/SVG-cards/Default/card-base2.png", "PNG");

  switch (deck) {
     case ENGLISH_DECK:          set_english_deck(); break;
     case RUSSIAN_DECK:          set_russian_deck(); break;
     case NICU_WHITE_DECK:       set_nicu_white_deck(); break;
     case TIGULLIO_MODERN_DECK : set_tigullio_modern_deck(); break;

     default: set_standard_deck();
  }

#ifdef DEBUG
  cards_played = new CCardsPlayed(this);
}

void CDeck::set_card_played(int card)
{
  cards_played->play(card);
}

void CDeck::reset_cards_played()
{
  cards_played->reset();
}

void CDeck::show_cards_played()
{
  cards_played->show();
}

void CDeck::hide_cards_played()
{
  cards_played->hide();
}

void CDeck::Translate()
{
  cards_played->Translate();
}
#else
}
#endif // DEBUG

void CDeck::set_standard_deck()
{
  img_cards[0] = new QImage(":/SVG-cards/Default/club_2.png", "PNG");
  img_cards[1] = new QImage(":/SVG-cards/Default/club_3.png", "PNG");
  img_cards[2] = new QImage(":/SVG-cards/Default/club_4.png", "PNG");
  img_cards[3] = new QImage(":/SVG-cards/Default/club_5.png", "PNG");
  img_cards[4] = new QImage(":/SVG-cards/Default/club_6.png", "PNG");
  img_cards[5] = new QImage(":/SVG-cards/Default/club_7.png", "PNG");
  img_cards[6] = new QImage(":/SVG-cards/Default/club_8.png", "PNG");
  img_cards[7] = new QImage(":/SVG-cards/Default/club_9.png", "PNG");
  img_cards[8] = new QImage(":/SVG-cards/Default/club_10.png", "PNG");
  img_cards[9] = new QImage(":/SVG-cards/Default/club_jack.png", "PNG");
  img_cards[10] = new QImage(":/SVG-cards/Default/club_queen.png", "PNG");
  img_cards[11] = new QImage(":/SVG-cards/Default/club_king.png", "PNG");
  img_cards[12] = new QImage(":/SVG-cards/Default/club_1.png", "PNG");

  img_cards[13] = new QImage(":/SVG-cards/Default/spade_2.png", "PNG");
  img_cards[14] = new QImage(":/SVG-cards/Default/spade_3.png", "PNG");
  img_cards[15] = new QImage(":/SVG-cards/Default/spade_4.png", "PNG");
  img_cards[16] = new QImage(":/SVG-cards/Default/spade_5.png", "PNG");
  img_cards[17] = new QImage(":/SVG-cards/Default/spade_6.png", "PNG");
  img_cards[18] = new QImage(":/SVG-cards/Default/spade_7.png", "PNG");
  img_cards[19] = new QImage(":/SVG-cards/Default/spade_8.png", "PNG");
  img_cards[20] = new QImage(":/SVG-cards/Default/spade_9.png", "PNG");
  img_cards[21] = new QImage(":/SVG-cards/Default/spade_10.png", "PNG");
  img_cards[22] = new QImage(":/SVG-cards/Default/spade_jack.png", "PNG");
  img_cards[23] = new QImage(":/SVG-cards/Default/spade_queen.png", "PNG");
  img_cards[24] = new QImage(":/SVG-cards/Default/spade_king.png", "PNG");
  img_cards[25] = new QImage(":/SVG-cards/Default/spade_1.png", "PNG");

  img_cards[26] = new QImage(":/SVG-cards/Default/diamond_2.png", "PNG");
  img_cards[27] = new QImage(":/SVG-cards/Default/diamond_3.png", "PNG");
  img_cards[28] = new QImage(":/SVG-cards/Default/diamond_4.png", "PNG");
  img_cards[29] = new QImage(":/SVG-cards/Default/diamond_5.png", "PNG");
  img_cards[30] = new QImage(":/SVG-cards/Default/diamond_6.png", "PNG");
  img_cards[31] = new QImage(":/SVG-cards/Default/diamond_7.png", "PNG");
  img_cards[32] = new QImage(":/SVG-cards/Default/diamond_8.png", "PNG");
  img_cards[33] = new QImage(":/SVG-cards/Default/diamond_9.png", "PNG");
  img_cards[34] = new QImage(":/SVG-cards/Default/diamond_10.png", "PNG");
  img_cards[35] = new QImage(":/SVG-cards/Default/diamond_jack.png", "PNG");
  img_cards[36] = new QImage(":/SVG-cards/Default/diamond_queen.png", "PNG");
  img_cards[37] = new QImage(":/SVG-cards/Default/diamond_king.png", "PNG");
  img_cards[38] = new QImage(":/SVG-cards/Default/diamond_1.png", "PNG");

  img_cards[39] = new QImage(":/SVG-cards/Default/heart_2.png", "PNG");
  img_cards[40] = new QImage(":/SVG-cards/Default/heart_3.png", "PNG");
  img_cards[41] = new QImage(":/SVG-cards/Default/heart_4.png", "PNG");
  img_cards[42] = new QImage(":/SVG-cards/Default/heart_5.png", "PNG");
  img_cards[43] = new QImage(":/SVG-cards/Default/heart_6.png", "PNG");
  img_cards[44] = new QImage(":/SVG-cards/Default/heart_7.png", "PNG");
  img_cards[45] = new QImage(":/SVG-cards/Default/heart_8.png", "PNG");
  img_cards[46] = new QImage(":/SVG-cards/Default/heart_9.png", "PNG");
  img_cards[47] = new QImage(":/SVG-cards/Default/heart_10.png", "PNG");
  img_cards[48] = new QImage(":/SVG-cards/Default/heart_jack.png", "PNG");
  img_cards[49] = new QImage(":/SVG-cards/Default/heart_queen.png", "PNG");
  img_cards[50] = new QImage(":/SVG-cards/Default/heart_king.png", "PNG");
  img_cards[51] = new QImage(":/SVG-cards/Default/heart_1.png", "PNG");

  img_back_card = new QImage(":/SVG-cards/Default/back.png", "PNG");
}

void CDeck::set_english_deck()
{
  img_cards[0] = new QImage(":/SVG-cards/English/club_2.svg", "SVG");
  img_cards[1] = new QImage(":/SVG-cards/English/club_3.svg", "SVG");
  img_cards[2] = new QImage(":/SVG-cards/English/club_4.svg", "SVG");
  img_cards[3] = new QImage(":/SVG-cards/English/club_5.svg", "SVG");
  img_cards[4] = new QImage(":/SVG-cards/English/club_6.svg", "SVG");
  img_cards[5] = new QImage(":/SVG-cards/English/club_7.svg", "SVG");
  img_cards[6] = new QImage(":/SVG-cards/English/club_8.svg", "SVG");
  img_cards[7] = new QImage(":/SVG-cards/English/club_9.svg", "SVG");
  img_cards[8] = new QImage(":/SVG-cards/English/club_10.svg", "SVG");
  img_cards[9] = new QImage(":/SVG-cards/English/club_jack.svg", "SVG");
  img_cards[10] = new QImage(":/SVG-cards/English/club_queen.svg", "SVG");
  img_cards[11] = new QImage(":/SVG-cards/English/club_king.svg", "SVG");
  img_cards[12] = new QImage(":/SVG-cards/English/club_1.svg", "SVG");

  img_cards[13] = new QImage(":/SVG-cards/English/spade_2.svg", "SVG");
  img_cards[14] = new QImage(":/SVG-cards/English/spade_3.svg", "SVG");
  img_cards[15] = new QImage(":/SVG-cards/English/spade_4.svg", "SVG");
  img_cards[16] = new QImage(":/SVG-cards/English/spade_5.svg", "SVG");
  img_cards[17] = new QImage(":/SVG-cards/English/spade_6.svg", "SVG");
  img_cards[18] = new QImage(":/SVG-cards/English/spade_7.svg", "SVG");
  img_cards[19] = new QImage(":/SVG-cards/English/spade_8.svg", "SVG");
  img_cards[20] = new QImage(":/SVG-cards/English/spade_9.svg", "SVG");
  img_cards[21] = new QImage(":/SVG-cards/English/spade_10.svg", "SVG");
  img_cards[22] = new QImage(":/SVG-cards/English/spade_jack.svg", "SVG");
  img_cards[23] = new QImage(":/SVG-cards/English/spade_queen.svg", "SVG");
  img_cards[24] = new QImage(":/SVG-cards/English/spade_king.svg", "SVG");
  img_cards[25] = new QImage(":/SVG-cards/English/spade_1.svg", "SVG");

  img_cards[26] = new QImage(":/SVG-cards/English/diamond_2.svg", "SVG");
  img_cards[27] = new QImage(":/SVG-cards/English/diamond_3.svg", "SVG");
  img_cards[28] = new QImage(":/SVG-cards/English/diamond_4.svg", "SVG");
  img_cards[29] = new QImage(":/SVG-cards/English/diamond_5.svg", "SVG");
  img_cards[30] = new QImage(":/SVG-cards/English/diamond_6.svg", "SVG");
  img_cards[31] = new QImage(":/SVG-cards/English/diamond_7.svg", "SVG");
  img_cards[32] = new QImage(":/SVG-cards/English/diamond_8.svg", "SVG");
  img_cards[33] = new QImage(":/SVG-cards/English/diamond_9.svg", "SVG");
  img_cards[34] = new QImage(":/SVG-cards/English/diamond_10.svg", "SVG");
  img_cards[35] = new QImage(":/SVG-cards/English/diamond_jack.svg", "SVG");
  img_cards[36] = new QImage(":/SVG-cards/English/diamond_queen.svg", "SVG");
  img_cards[37] = new QImage(":/SVG-cards/English/diamond_king.svg", "SVG");
  img_cards[38] = new QImage(":/SVG-cards/English/diamond_1.svg", "SVG");

  img_cards[39] = new QImage(":/SVG-cards/English/heart_2.svg", "SVG");
  img_cards[40] = new QImage(":/SVG-cards/English/heart_3.svg", "SVG");
  img_cards[41] = new QImage(":/SVG-cards/English/heart_4.svg", "SVG");
  img_cards[42] = new QImage(":/SVG-cards/English/heart_5.svg", "SVG");
  img_cards[43] = new QImage(":/SVG-cards/English/heart_6.svg", "SVG");
  img_cards[44] = new QImage(":/SVG-cards/English/heart_7.svg", "SVG");
  img_cards[45] = new QImage(":/SVG-cards/English/heart_8.svg", "SVG");
  img_cards[46] = new QImage(":/SVG-cards/English/heart_9.svg", "SVG");
  img_cards[47] = new QImage(":/SVG-cards/English/heart_10.svg", "SVG");
  img_cards[48] = new QImage(":/SVG-cards/English/heart_jack.svg", "SVG");
  img_cards[49] = new QImage(":/SVG-cards/English/heart_queen.svg", "SVG");
  img_cards[50] = new QImage(":/SVG-cards/English/heart_king.svg", "SVG");
  img_cards[51] = new QImage(":/SVG-cards/English/heart_1.svg", "SVG");

  img_back_card = new QImage(":/SVG-cards/Default/back.png", "PNG");
}

void CDeck::set_nicu_white_deck()
{
  img_cards[0] = new QImage(":/SVG-cards/white/white_c_2.svg", "SVG");
  img_cards[1] = new QImage(":/SVG-cards/white/white_c_3.svg", "SVG");
  img_cards[2] = new QImage(":/SVG-cards/white/white_c_4.svg", "SVG");
  img_cards[3] = new QImage(":/SVG-cards/white/white_c_5.svg", "SVG");
  img_cards[4] = new QImage(":/SVG-cards/white/white_c_6.svg", "SVG");
  img_cards[5] = new QImage(":/SVG-cards/white/white_c_7.svg", "SVG");
  img_cards[6] = new QImage(":/SVG-cards/white/white_c_8.svg", "SVG");
  img_cards[7] = new QImage(":/SVG-cards/white/white_c_9.svg", "SVG");
  img_cards[8] = new QImage(":/SVG-cards/white/white_c_10.svg", "SVG");
  img_cards[9] = new QImage(":/SVG-cards/white/white_c_j.svg", "SVG");
  img_cards[10] = new QImage(":/SVG-cards/white/white_c_q.svg", "SVG");
  img_cards[11] = new QImage(":/SVG-cards/white/white_c_k.svg", "SVG");
  img_cards[12] = new QImage(":/SVG-cards/white/white_c_a.svg", "SVG");

  img_cards[13] = new QImage(":/SVG-cards/white/white_s_2.svg", "SVG");
  img_cards[14] = new QImage(":/SVG-cards/white/white_s_3.svg", "SVG");
  img_cards[15] = new QImage(":/SVG-cards/white/white_s_4.svg", "SVG");
  img_cards[16] = new QImage(":/SVG-cards/white/white_s_5.svg", "SVG");
  img_cards[17] = new QImage(":/SVG-cards/white/white_s_6.svg", "SVG");
  img_cards[18] = new QImage(":/SVG-cards/white/white_s_7.svg", "SVG");
  img_cards[19] = new QImage(":/SVG-cards/white/white_s_8.svg", "SVG");
  img_cards[20] = new QImage(":/SVG-cards/white/white_s_9.svg", "SVG");
  img_cards[21] = new QImage(":/SVG-cards/white/white_s_10.svg", "SVG");
  img_cards[22] = new QImage(":/SVG-cards/white/white_s_j.svg", "SVG");
  img_cards[23] = new QImage(":/SVG-cards/white/white_s_q.svg", "SVG");
  img_cards[24] = new QImage(":/SVG-cards/white/white_s_k.svg", "SVG");
  img_cards[25] = new QImage(":/SVG-cards/white/white_s_a.svg", "SVG");

  img_cards[26] = new QImage(":/SVG-cards/white/white_d_2.svg", "SVG");
  img_cards[27] = new QImage(":/SVG-cards/white/white_d_3.svg", "SVG");
  img_cards[28] = new QImage(":/SVG-cards/white/white_d_4.svg", "SVG");
  img_cards[29] = new QImage(":/SVG-cards/white/white_d_5.svg", "SVG");
  img_cards[30] = new QImage(":/SVG-cards/white/white_d_6.svg", "SVG");
  img_cards[31] = new QImage(":/SVG-cards/white/white_d_7.svg", "SVG");
  img_cards[32] = new QImage(":/SVG-cards/white/white_d_8.svg", "SVG");
  img_cards[33] = new QImage(":/SVG-cards/white/white_d_9.svg", "SVG");
  img_cards[34] = new QImage(":/SVG-cards/white/white_d_10.svg", "SVG");
  img_cards[35] = new QImage(":/SVG-cards/white/white_d_j.svg", "SVG");
  img_cards[36] = new QImage(":/SVG-cards/white/white_d_q.svg", "SVG");
  img_cards[37] = new QImage(":/SVG-cards/white/white_d_k.svg", "SVG");
  img_cards[38] = new QImage(":/SVG-cards/white/white_d_a.svg", "SVG");

  img_cards[39] = new QImage(":/SVG-cards/white/white_h_2.svg", "SVG");
  img_cards[40] = new QImage(":/SVG-cards/white/white_h_3.svg", "SVG");
  img_cards[41] = new QImage(":/SVG-cards/white/white_h_4.svg", "SVG");
  img_cards[42] = new QImage(":/SVG-cards/white/white_h_5.svg", "SVG");
  img_cards[43] = new QImage(":/SVG-cards/white/white_h_6.svg", "SVG");
  img_cards[44] = new QImage(":/SVG-cards/white/white_h_7.svg", "SVG");
  img_cards[45] = new QImage(":/SVG-cards/white/white_h_8.svg", "SVG");
  img_cards[46] = new QImage(":/SVG-cards/white/white_h_9.svg", "SVG");
  img_cards[47] = new QImage(":/SVG-cards/white/white_h_10.svg", "SVG");
  img_cards[48] = new QImage(":/SVG-cards/white/white_h_j.svg", "SVG");
  img_cards[49] = new QImage(":/SVG-cards/white/white_h_q.svg", "SVG");
  img_cards[50] = new QImage(":/SVG-cards/white/white_h_k.svg", "SVG");
  img_cards[51] = new QImage(":/SVG-cards/white/white_h_a.svg", "SVG");

  img_back_card = new QImage(":/SVG-cards/white/back07.png", "PNG");
}

void CDeck::set_russian_deck()
{
  img_cards[0] = new QImage(":/SVG-cards/Russian/club_2.png", "PNG");
  img_cards[1] = new QImage(":/SVG-cards/Russian/club_3.png", "PNG");
  img_cards[2] = new QImage(":/SVG-cards/Russian/club_4.png", "PNG");
  img_cards[3] = new QImage(":/SVG-cards/Russian/club_5.png", "PNG");
  img_cards[4] = new QImage(":/SVG-cards/Russian/club_6.png", "PNG");
  img_cards[5] = new QImage(":/SVG-cards/Russian/club_7.png", "PNG");
  img_cards[6] = new QImage(":/SVG-cards/Russian/club_8.png", "PNG");
  img_cards[7] = new QImage(":/SVG-cards/Russian/club_9.png", "PNG");
  img_cards[8] = new QImage(":/SVG-cards/Russian/club_10.png", "PNG");
  img_cards[9] = new QImage(":/SVG-cards/Russian/club_jack.png", "PNG");
  img_cards[10] = new QImage(":/SVG-cards/Russian/club_queen.png", "PNG");
  img_cards[11] = new QImage(":/SVG-cards/Russian/club_king.png", "PNG");
  img_cards[12] = new QImage(":/SVG-cards/Russian/club_1.png", "PNG");

  img_cards[13] = new QImage(":/SVG-cards/Russian/spade_2.png", "PNG");
  img_cards[14] = new QImage(":/SVG-cards/Russian/spade_3.png", "PNG");
  img_cards[15] = new QImage(":/SVG-cards/Russian/spade_4.png", "PNG");
  img_cards[16] = new QImage(":/SVG-cards/Russian/spade_5.png", "PNG");
  img_cards[17] = new QImage(":/SVG-cards/Russian/spade_6.png", "PNG");
  img_cards[18] = new QImage(":/SVG-cards/Russian/spade_7.png", "PNG");
  img_cards[19] = new QImage(":/SVG-cards/Russian/spade_8.png", "PNG");
  img_cards[20] = new QImage(":/SVG-cards/Russian/spade_9.png", "PNG");
  img_cards[21] = new QImage(":/SVG-cards/Russian/spade_10.png", "PNG");
  img_cards[22] = new QImage(":/SVG-cards/Russian/spade_jack.png", "PNG");
  img_cards[23] = new QImage(":/SVG-cards/Russian/spade_queen.png", "PNG");
  img_cards[24] = new QImage(":/SVG-cards/Russian/spade_king.png", "PNG");
  img_cards[25] = new QImage(":/SVG-cards/Russian/spade_1.png", "PNG");

  img_cards[26] = new QImage(":/SVG-cards/Russian/diamond_2.png", "PNG");
  img_cards[27] = new QImage(":/SVG-cards/Russian/diamond_3.png", "PNG");
  img_cards[28] = new QImage(":/SVG-cards/Russian/diamond_4.png", "PNG");
  img_cards[29] = new QImage(":/SVG-cards/Russian/diamond_5.png", "PNG");
  img_cards[30] = new QImage(":/SVG-cards/Russian/diamond_6.png", "PNG");
  img_cards[31] = new QImage(":/SVG-cards/Russian/diamond_7.png", "PNG");
  img_cards[32] = new QImage(":/SVG-cards/Russian/diamond_8.png", "PNG");
  img_cards[33] = new QImage(":/SVG-cards/Russian/diamond_9.png", "PNG");
  img_cards[34] = new QImage(":/SVG-cards/Russian/diamond_10.png", "PNG");
  img_cards[35] = new QImage(":/SVG-cards/Russian/diamond_jack.png", "PNG");
  img_cards[36] = new QImage(":/SVG-cards/Russian/diamond_queen.png", "PNG");
  img_cards[37] = new QImage(":/SVG-cards/Russian/diamond_king.png", "PNG");
  img_cards[38] = new QImage(":/SVG-cards/Russian/diamond_1.png", "PNG");

  img_cards[39] = new QImage(":/SVG-cards/Russian/heart_2.png", "PNG");
  img_cards[40] = new QImage(":/SVG-cards/Russian/heart_3.png", "PNG");
  img_cards[41] = new QImage(":/SVG-cards/Russian/heart_4.png", "PNG");
  img_cards[42] = new QImage(":/SVG-cards/Russian/heart_5.png", "PNG");
  img_cards[43] = new QImage(":/SVG-cards/Russian/heart_6.png", "PNG");
  img_cards[44] = new QImage(":/SVG-cards/Russian/heart_7.png", "PNG");
  img_cards[45] = new QImage(":/SVG-cards/Russian/heart_8.png", "PNG");
  img_cards[46] = new QImage(":/SVG-cards/Russian/heart_9.png", "PNG");
  img_cards[47] = new QImage(":/SVG-cards/Russian/heart_10.png", "PNG");
  img_cards[48] = new QImage(":/SVG-cards/Russian/heart_jack.png", "PNG");
  img_cards[49] = new QImage(":/SVG-cards/Russian/heart_queen.png", "PNG");
  img_cards[50] = new QImage(":/SVG-cards/Russian/heart_king.png", "PNG");
  img_cards[51] = new QImage(":/SVG-cards/Russian/heart_1.png", "PNG");

  img_back_card = new QImage(":/SVG-cards/Default/back.png", "PNG");
}

void CDeck::set_tigullio_modern_deck()
{
  img_cards[0] = new QImage(":/SVG-cards/tigullio-modern/club_2.png", "PNG");
  img_cards[1] = new QImage(":/SVG-cards/tigullio-modern/club_3.png", "PNG");
  img_cards[2] = new QImage(":/SVG-cards/tigullio-modern/club_4.png", "PNG");
  img_cards[3] = new QImage(":/SVG-cards/tigullio-modern/club_5.png", "PNG");
  img_cards[4] = new QImage(":/SVG-cards/tigullio-modern/club_6.png", "PNG");
  img_cards[5] = new QImage(":/SVG-cards/tigullio-modern/club_7.png", "PNG");
  img_cards[6] = new QImage(":/SVG-cards/tigullio-modern/club_8.png", "PNG");
  img_cards[7] = new QImage(":/SVG-cards/tigullio-modern/club_9.png", "PNG");
  img_cards[8] = new QImage(":/SVG-cards/tigullio-modern/club_10.png", "PNG");
  img_cards[9] = new QImage(":/SVG-cards/tigullio-modern/club_jack.png", "PNG");
  img_cards[10] = new QImage(":/SVG-cards/tigullio-modern/club_queen.png", "PNG");
  img_cards[11] = new QImage(":/SVG-cards/tigullio-modern/club_king.png", "PNG");
  img_cards[12] = new QImage(":/SVG-cards/tigullio-modern/club_1.png", "PNG");

  img_cards[13] = new QImage(":/SVG-cards/tigullio-modern/spade_2.png", "PNG");
  img_cards[14] = new QImage(":/SVG-cards/tigullio-modern/spade_3.png", "PNG");
  img_cards[15] = new QImage(":/SVG-cards/tigullio-modern/spade_4.png", "PNG");
  img_cards[16] = new QImage(":/SVG-cards/tigullio-modern/spade_5.png", "PNG");
  img_cards[17] = new QImage(":/SVG-cards/tigullio-modern/spade_6.png", "PNG");
  img_cards[18] = new QImage(":/SVG-cards/tigullio-modern/spade_7.png", "PNG");
  img_cards[19] = new QImage(":/SVG-cards/tigullio-modern/spade_8.png", "PNG");
  img_cards[20] = new QImage(":/SVG-cards/tigullio-modern/spade_9.png", "PNG");
  img_cards[21] = new QImage(":/SVG-cards/tigullio-modern/spade_10.png", "PNG");
  img_cards[22] = new QImage(":/SVG-cards/tigullio-modern/spade_jack.png", "PNG");
  img_cards[23] = new QImage(":/SVG-cards/tigullio-modern/spade_queen.png", "PNG");
  img_cards[24] = new QImage(":/SVG-cards/tigullio-modern/spade_king.png", "PNG");
  img_cards[25] = new QImage(":/SVG-cards/tigullio-modern/spade_1.png", "PNG");

  img_cards[26] = new QImage(":/SVG-cards/tigullio-modern/diamond_2.png", "PNG");
  img_cards[27] = new QImage(":/SVG-cards/tigullio-modern/diamond_3.png", "PNG");
  img_cards[28] = new QImage(":/SVG-cards/tigullio-modern/diamond_4.png", "PNG");
  img_cards[29] = new QImage(":/SVG-cards/tigullio-modern/diamond_5.png", "PNG");
  img_cards[30] = new QImage(":/SVG-cards/tigullio-modern/diamond_6.png", "PNG");
  img_cards[31] = new QImage(":/SVG-cards/tigullio-modern/diamond_7.png", "PNG");
  img_cards[32] = new QImage(":/SVG-cards/tigullio-modern/diamond_8.png", "PNG");
  img_cards[33] = new QImage(":/SVG-cards/tigullio-modern/diamond_9.png", "PNG");
  img_cards[34] = new QImage(":/SVG-cards/tigullio-modern/diamond_10.png", "PNG");
  img_cards[35] = new QImage(":/SVG-cards/tigullio-modern/diamond_jack.png", "PNG");
  img_cards[36] = new QImage(":/SVG-cards/tigullio-modern/diamond_queen.png", "PNG");
  img_cards[37] = new QImage(":/SVG-cards/tigullio-modern/diamond_king.png", "PNG");
  img_cards[38] = new QImage(":/SVG-cards/tigullio-modern/diamond_1.png", "PNG");

  img_cards[39] = new QImage(":/SVG-cards/tigullio-modern/heart_2.png", "PNG");
  img_cards[40] = new QImage(":/SVG-cards/tigullio-modern/heart_3.png", "PNG");
  img_cards[41] = new QImage(":/SVG-cards/tigullio-modern/heart_4.png", "PNG");
  img_cards[42] = new QImage(":/SVG-cards/tigullio-modern/heart_5.png", "PNG");
  img_cards[43] = new QImage(":/SVG-cards/tigullio-modern/heart_6.png", "PNG");
  img_cards[44] = new QImage(":/SVG-cards/tigullio-modern/heart_7.png", "PNG");
  img_cards[45] = new QImage(":/SVG-cards/tigullio-modern/heart_8.png", "PNG");
  img_cards[46] = new QImage(":/SVG-cards/tigullio-modern/heart_9.png", "PNG");
  img_cards[47] = new QImage(":/SVG-cards/tigullio-modern/heart_10.png", "PNG");
  img_cards[48] = new QImage(":/SVG-cards/tigullio-modern/heart_jack.png", "PNG");
  img_cards[49] = new QImage(":/SVG-cards/tigullio-modern/heart_queen.png", "PNG");
  img_cards[50] = new QImage(":/SVG-cards/tigullio-modern/heart_king.png", "PNG");
  img_cards[51] = new QImage(":/SVG-cards/tigullio-modern/heart_1.png", "PNG");

  img_back_card = new QImage(":/SVG-cards/tigullio-modern/back.png", "PNG");
}
CDeck::~CDeck()
{
  delete img_empty_card;
  delete img_your_turn;

#ifdef DEBUG
  delete cards_played;
#endif // DEBUG

  delete_current_deck();
}

QImage *CDeck::get_img_card(int card) {
  switch (card) {
     case empty:     return img_empty_card;
     case back_card: return img_back_card;
     case your_turn: return img_your_turn;

     default:        assert((card >= 0) && (card < DECK_SIZE));
                     return img_cards[card];
  }
}

void CDeck::reverse_card_rgb(int card) {
  assert((card >= 0) && (card < DECK_SIZE));
  img_cards[card]->invertPixels();
}

void CDeck::delete_current_deck()
{
  for (int i=0; i<DECK_SIZE; i++)
    delete(img_cards[i]);

  delete img_back_card;
}

void CDeck::set_deck(int deck)
{
  if (deck == current_deck)
    return;

  delete_current_deck();

  switch (deck) {
     case ENGLISH_DECK:         set_english_deck(); break;
     case RUSSIAN_DECK:         set_russian_deck(); break;
     case NICU_WHITE_DECK:      set_nicu_white_deck(); break;
     case TIGULLIO_MODERN_DECK: set_tigullio_modern_deck(); break;

     default: set_standard_deck();
  }

  current_deck = deck;

#ifdef DEBUG
  cards_played->set_deck();
#endif
}
