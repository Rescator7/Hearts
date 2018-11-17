#include "chearts.h"
#include "variants.h"
#include "assert.h"
#include <QMessageBox>

CHearts::CHearts()
{
}

CHearts::~CHearts()
{
}

void CHearts::new_game()
{
  init_vars();
  reset_cards_on_table();
  reset_score();
  reset_hand_score();
  reset_cards_left_in_suit();
  reset_cards_played();
  reset_cards_passed();
  reset_plr_cards_in_suit();
  reset_plr_has_card();

  random_deck();
  user_id = turn;

  emit sig_clear_table();
  emit sig_refresh_deck(user_id);
  emit sig_pass_to(passed_to);
}

void CHearts::init_vars()
{
  card_left = DECK_SIZE;
  game_over = false;
  shoot_moon = false;
  heart_broken = false;
  jack_diamond_played = false;
  moon_add_to_scores = true;
  passed_to = pLEFT;
  mode_playing = false;
  plr_jack_diamond = NOT_FOUND;
  plr_best_hand = NOT_FOUND;
  hand_score = 0;
  best_hand = 0;
  hand_turn = 0;
  current_suit = CLUB;
}

void CHearts::reset_cards_on_table()
{
  hand_cards[0] = empty;
  hand_cards[1] = empty;
  hand_cards[2] = empty;
  hand_cards[3] = empty;
}

void CHearts::reset_cards_passed()
{
  for (int i=0; i<4; i++) {
    for (int i2=0; i2<13; i2++)
      cards_selected[i][i2] = false;
    cards_selected_count[i] = 0;
  }
}

void CHearts::reset_hand_score()
{
  for (int i=0; i<4; i++) {
    plr_hand_score[i] = 0;

    emit sig_hand_score(plr_hand_score[i], i);
  }
}

void CHearts::reset_score()
{
  for (int i=0; i<4; i++) {
    plr_score[i] = 0;

    emit sig_score(plr_score[i], i);
  }
}

void CHearts::reset_cards_played()
{
  for (int i=two_clubs; i<DECK_SIZE; i++)
    cards_played[i] = false;
}

void CHearts::reset_plr_cards_in_suit()
{
 for (int i=0; i<4; i++)
   for (int i2=0; i2<4; i2++)
     plr_cards_in_suit[i][i2] = 0;
}

void CHearts::reset_cards_left_in_suit()
{
  cards_left_in_suit[SPADE] = 13;
  cards_left_in_suit[CLUB] = 13;
  cards_left_in_suit[DIAMOND] = 13;
  cards_left_in_suit[HEART] = 13;
}

void CHearts::reset_plr_has_card()
{
  for (int i=0; i<4; i++)
    for (int i2=0; i2<DECK_SIZE; i2++)
       plr_has_card[i][i2] = false;
}

void CHearts::random_deck()
{
  cpt_plr_cards[0] = 13;
  cpt_plr_cards[1] = 13;
  cpt_plr_cards[2] = 13;
  cpt_plr_cards[3] = 13;

  bool card_free[DECK_SIZE];

  for (int i=0; i < DECK_SIZE; i++)
    card_free[i] = true;

  int plr = 0, card;
  for (int i=0; i < DECK_SIZE; i++) {
    card = rand() % DECK_SIZE;
    while (!card_free[card])
       card = rand() % DECK_SIZE;
    if (card == two_clubs)
      turn = plr;
    card_free[card] = false;
    plr_cards[plr][i / 4] = card;
    plr_has_card[plr][card] = true;
    plr_cards_in_suit[plr][card / 13]++;
    if (plr++ == 3)
      plr = 0;
  }

  sort_plr_cards();
}

void CHearts::pass_cards()
{
  int a = -1, b = -1, c = -1, d = -1,
      w, x, y, z;

  for (int i=0; i<3; i++) {
    while (!cards_selected[0][++a]) assert(a < 12);
    while (!cards_selected[1][++b]) assert(b < 12);
    while (!cards_selected[2][++c]) assert(c < 12);
    while (!cards_selected[3][++d]) assert(d < 12);

    w = plr_cards[0][a];
    x = plr_cards[1][b];
    y = plr_cards[2][c];
    z = plr_cards[3][d];

    plr_cards_in_suit[0][w / 13]--;
    plr_cards_in_suit[1][x / 13]--;
    plr_cards_in_suit[2][y / 13]--;
    plr_cards_in_suit[3][z / 13]--;

    plr_has_card[0][w] = false;
    plr_has_card[1][x] = false;
    plr_has_card[2][y] = false;
    plr_has_card[3][z] = false;

    switch (passed_to) {
       case pLEFT:    plr_cards[0][a] = z;
                      plr_cards[1][b] = w;
                      plr_cards[2][c] = x;
                      plr_cards[3][d] = y;

                      plr_cards_in_suit[0][z / 13]++;
                      plr_cards_in_suit[1][w / 13]++;
                      plr_cards_in_suit[2][x / 13]++;
                      plr_cards_in_suit[3][y / 13]++;

                      plr_has_card[0][z] = true;
                      plr_has_card[1][w] = true;
                      plr_has_card[2][x] = true;
                      plr_has_card[3][y] = true;
                      break;

       case pRIGHT:   plr_cards[0][a] = x;
                      plr_cards[1][b] = y;
                      plr_cards[2][c] = z;
                      plr_cards[3][d] = w;

                      plr_cards_in_suit[0][x / 13]++;
                      plr_cards_in_suit[1][y / 13]++;
                      plr_cards_in_suit[2][z / 13]++;
                      plr_cards_in_suit[3][w / 13]++;

                      plr_has_card[0][x] = true;
                      plr_has_card[1][y] = true;
                      plr_has_card[2][z] = true;
                      plr_has_card[3][w] = true;
                      break;

       case pACCROSS: plr_cards[0][a] = y;
                      plr_cards[1][b] = z;
                      plr_cards[2][c] = w;
                      plr_cards[3][d] = x;

                      plr_cards_in_suit[0][y / 13]++;
                      plr_cards_in_suit[1][z / 13]++;
                      plr_cards_in_suit[2][w / 13]++;
                      plr_cards_in_suit[3][x / 13]++;

                      plr_has_card[0][y] = true;
                      plr_has_card[1][z] = true;
                      plr_has_card[2][w] = true;
                      plr_has_card[3][x] = true;
                      break;
    }
  }
}

void CHearts::sort_plr_cards() {
  int sorted_cards[4][13];

  int small, idx;
  for (int i=0; i<4; i++) {
    for (int i3=0; i3<13; i3++) {
      idx = 0;
      small = empty;
      for (int i2=0; i2<13; i2++)
        if (plr_cards[i][i2] < small) {
          small = plr_cards[i][i2];
          idx = i2;
        }
     plr_cards[i][idx] = empty;
     sorted_cards[i][i3] = small;
    }
  }

  for (int i=0; i<4; i++)
    for (int i2=0; i2<13; i2++)
       plr_cards[i][i2] = sorted_cards[i][i2];
}

void CHearts::set_cpu_passing_cards(int plr)
{
  assert ((cpu_cards[0] >=0) && (cpu_cards[0] < 13));
  assert ((cpu_cards[1] >=0) && (cpu_cards[1] < 13));
  assert ((cpu_cards[2] >=0) && (cpu_cards[2] < 13));

  assert (cpu_cards[0] != cpu_cards[1]);
  assert (cpu_cards[0] != cpu_cards[2]);
  assert (cpu_cards[1] != cpu_cards[2]);

  cards_selected[plr][cpu_cards[0]] = true;
  cards_selected[plr][cpu_cards[1]] = true;
  cards_selected[plr][cpu_cards[2]] = true;
  cards_selected_count[plr] = 3;
}

void CHearts::select_cpus_cards()
{
 for (int i=0; i<4; i++)
   if (i != user_id) {
     int cpt = 0;

     int spade_count = plr_cards_in_suit[i][SPADE];
     int pos_card = get_card_position(i, ace_spade);

     if (pos_card != NOT_FOUND)
       cpu_cards[cpt++] = pos_card;

     pos_card = get_card_position(i, king_spade);
     if (pos_card != NOT_FOUND)
       cpu_cards[cpt++] = pos_card;

     pos_card = get_card_position(i, queen_spade);
     if (pos_card != NOT_FOUND)
       cpu_cards[cpt++] = pos_card;

     if (cpt == 3) {
       set_cpu_passing_cards(i);
       continue; // next cpu
     }

     int heart_count = plr_cards_in_suit[i][HEART];

     if ((heart_count > 1) || ((heart_count == 1) && !plr_has_card[i][ace_heart])) {
       cpu_cards[cpt++] = get_lowest_suit_pos(i, HEART);
     }

     if (cpt == 3) {
       set_cpu_passing_cards(i);
       continue; // next cpu
     }

     // if not omnibus pass highest diamond (if available)
     int d_count = plr_cards_in_suit[i][DIAMOND];

     if (!omnibus && d_count)
       cpu_cards[cpt++] = get_highest_suit_pos(i, DIAMOND);

       if (cpt == 3) {
         set_cpu_passing_cards(i);
         continue; // next cpu
       }

     // pass highest club if available
     int c_count = plr_cards_in_suit[i][CLUB];
     if (c_count) {
       cpu_cards[cpt++] = get_highest_suit_pos(i, CLUB);
     }

     if (cpt == 3) {
       set_cpu_passing_cards(i);
       continue; // next cpu
     }

     // if we did not choose 3 cards yet, fill with random cards to pass.
     int card_id;
     while (cpt != 3) {
        card_id = rand() % 13;

        int card = plr_cards[i][card_id];

        if (omnibus && (card / 13 == DIAMOND) && ((d_count < 3) || (card == jack_diamond)))
          continue;

        if ((card / 13 == SPADE) && (spade_count < 5))
          continue;

        switch (cpt) {
           case 0 : cpu_cards[cpt++] = card_id;
                    break;
           case 1 : if (card_id != cpu_cards[0])
                      cpu_cards[cpt++] = card_id;
                    break;
           case 2 : if ((card_id != cpu_cards[0]) && (card_id != cpu_cards[1]))
                      cpu_cards[cpt++] = card_id;
        }
     }
     set_cpu_passing_cards(i);
  }
}

int CHearts::freesuit_lead_eval(int card)
{
  int card_suit = card / 13;

  // try to catch someone with the queen of spade.
  if (card == queen_spade) {
    int diff_spade = cards_left_in_suit[SPADE] - plr_cards_in_suit[turn][SPADE];

    if ((diff_spade == 1) && !plr_has_card[turn][ace_spade] && !cards_played[ace_spade])
      return 100;

    if ((diff_spade == 1) && !plr_has_card[turn][king_spade] && !cards_played[king_spade])
      return 101;

    if ((diff_spade == 2) && !plr_has_card[turn][ace_spade] && !plr_has_card[turn][king_spade] &&
                             !cards_played[ace_spade] && !cards_played[king_spade])
      return 102;

    // don't lead the queen of spade
    return -100;
  }

  // don't lead in a suit where there is no cards left beside ours.
  if (cards_left_in_suit[card_suit] == plr_cards_in_suit[turn][card_suit])
    return -80;

  // avoid leading the ace/king of spade, if the queen hasn't been played and we don't own it
  // otherwise, let's try to elimitate some spade.

  if (((card == ace_spade) || (card == king_spade)) && !cards_played[queen_spade]) {
    if (plr_has_card[turn][queen_spade])
     return 70;
    else
     return -90;
  }

  // avoid leading in spade if the queen of spade is not played yet and the ace/king/queen spade is in your hand,
  // and you got less than 5 spades... otherwise lead in spade.
  if (card_suit == SPADE) {
    if (!cards_played[queen_spade]) {
      if ((plr_has_card[turn][queen_spade] || plr_has_card[turn][ace_spade] || plr_has_card[turn][king_spade]) &&
          (plr_cards_in_suit[turn][SPADE] < 4))
        return -70;
      else
        return 60;
    }
  }

  // play the jack of diamond (omnibus) only if your sure to wins the trick
  // but, not the queen of spade.
  if (omnibus && (card == jack_diamond)) {
    if ((cards_played[ace_diamond] || plr_has_card[turn][ace_diamond]) &&
        (cards_played[king_diamond] || plr_has_card[turn][king_diamond]) &&
        (cards_played[queen_diamond] || plr_has_card[turn][queen_diamond])
     && (cards_played[queen_spade] || plr_has_card[turn][queen_spade]))
      return 64;
    else
      return -64;
  }

  // try to catch the jack of diamond with a/k/q if the queen of spade has been played.
  if (omnibus && (cards_played[queen_spade] || plr_has_card[turn][queen_spade]) &&
                 !cards_played[jack_diamond] && !plr_has_card[turn][jack_diamond] &&
                ((card == ace_diamond) || (card == king_diamond) || (card == queen_diamond)))
    return 63;

  // if nothing has been found => return 0
  return 0;
}

int CHearts::spade_lead_eval(int card)
{
 // the lead is spade and the ace/king is in the trick? play the queen otherwise don't.
 if (card == queen_spade) {
   if (is_card_on_table(ace_spade) || is_card_on_table(king_spade))
     return 104;
   else
     return -104;
 }

 // if the ace is in the trick throw away our king
  if ((card == king_spade) && is_card_on_table(ace_spade))
    return 75;

 // avoid giving the jack of diamond
  if (omnibus && (card == jack_diamond))
    return -62;

 // QMessageBox::warning(0, "warning", QString("turn: ") + QString::number(turn) + QString(" handturn: ") + QString::number(hand_turn));

 // last to talk, the queen is not in the trick.. throw your ace/king.
  if ((hand_turn == 3) && !is_card_on_table(queen_spade) && ((card == ace_spade) || (card == king_spade)))
    return 40;

  return 0;
}

int CHearts::diamond_lead_eval(int card)
{
  if (omnibus) {
  // Try to catch the jack of diamond with the ace/king/queen if the queen of spade isn't in the trick.
  // note: Maybe after !is_card_on_table(queen_spade) add (cards_played[queen_spade] || has_card(turn, queen_spade))
     if (((card == ace_diamond) || (card == king_diamond) || (card == queen_diamond)) &&
          is_card_on_table(jack_diamond) && !is_card_on_table(queen_spade))
        return 20 + (card % 13);

  // will we win this hand?
     if (card == jack_diamond) {
       if (((cards_played[queen_spade] && !is_card_on_table(queen_spade)) || plr_has_card[turn][queen_spade]) &&
           ((cards_played[ace_diamond] && !is_card_on_table(ace_diamond)) || plr_has_card[turn][ace_diamond]) &&
           ((cards_played[king_diamond] && !is_card_on_table(king_diamond)) || plr_has_card[turn][king_diamond]) &&
           ((cards_played[queen_diamond] && !is_card_on_table(queen_diamond)) || plr_has_card[turn][queen_diamond]))
         return  62;

  // You are the last one to play in the trick, try to use your Jack of diamond:
  // if it the strongest card of the trick.
       if ((hand_turn == 3) && !is_card_on_table(queen_spade) && !is_card_on_table(ace_diamond) &&
                               !is_card_on_table(king_diamond) && !is_card_on_table(queen_diamond))
         return 30;

       // don't play the jack of diamond
       return -65;
     }
  }

  return 0;
}

int CHearts::get_cpu_move()
{
 int eval[13] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

 for (int i=0; i<13; i++) {
    int card = plr_cards[turn][i];
    // invalide move, and empty card are avoided by having the worst eval.
    if ((card == empty) || (check_invalid_move(turn, card) != NOERROR))
      eval[i] = -32768;
    else {
      // only valid move from here
      switch (current_suit) {
         case FREESUIT: eval[i] = freesuit_lead_eval(card);
                        break;

         case SPADE:    eval[i] = spade_lead_eval(card);

                        break;

         case DIAMOND:  eval[i] = diamond_lead_eval(card);
      }

    // if we didn't hit a specific evaluation in previous section, let's try those evaluation.
    if (eval[i] == 0) {

      // give away the queen of spade
      if (card == queen_spade)
         eval[i] = 105;
      else

      // the queen of spade has not been played yet, let's throw our ace/king spade away.
      if ((current_suit != SPADE) && ((card == ace_spade) || (card == king_spade)) && !cards_played[queen_spade])
        eval[i] = 76;
      else
      // don't give away the jack of diamond.
      if ((current_suit != DIAMOND) && omnibus && (card == jack_diamond))
       eval[i] = -63;
      else
      // we are the last one of the trick: play our bigger cards, but not in hearts, and not if queen spade is in
       if ((hand_turn == 3) && (current_suit != HEART) && !is_card_on_table(queen_spade))
         eval[i] = card % 13;
      else
      // not last, play the lowest card.
      if (hand_turn != 3)
        eval[i] = -(card % 13);
    }
   }
 }

 // search for the best move... eventually, it could go into the first loop...
 int best_move = -32768, best_card_id;

 for (int i=0; i<13; i++)
   if (eval[i] > best_move) {
     best_move = eval[i];
     best_card_id = i;
    }

  return best_card_id;
}

void CHearts::process_next_pass(bool skip_moon_check)
{
  if (!skip_moon_check) {
    shoot_moon = false;

    // checking if a player shoot the moon.
    for (int i=0; i<4; i++) {
      if (!omnibus || (plr_jack_diamond == i))
        if (plr_hand_score[i] == 26) {
          shoot_moon = true;
          emit sig_shoot_moon(i);

          if (new_moon && ((i == user_id) && plr_score[user_id] >= 26))
            return;    // if new_moon is enabled, the score is above 26, and
                       // it's not a cpu who moon... we return, because
                       // we need to select [add] or [subtract]...
                       // emit_signal(SIG_SHOOT_MOON) will trigger the choice
                       // and, we will return by set_moon_add_score() that will skip
                       // this part.
          else
            break;     // we found someone who moon. no need to look further.
        }
    }
  }

  for (int i=0; i<4; i++) {
     if (shoot_moon) {
        if ((plr_hand_score[i] == 0) && (!new_moon || moon_add_to_scores))
          plr_hand_score[i] = 26;
        else {
          if (plr_hand_score[i] == 26) {
            if (i != user_id)           // computer player don't subsctract.. only add.
              plr_hand_score[i] = 0;
            else
              plr_hand_score[i] = new_moon && !moon_add_to_scores ? -26 : 0;
          }
        }
     } else {
         // bonus that doesn't apply if someone shoot the moon
         if (no_trick_bonus && !plr_hand_score[i]) {
           if (plr_score[i] >= no_trick_value)
             plr_hand_score[i] = -no_trick_value;
           else
             plr_hand_score[i] = -plr_score[i];

           if (plr_hand_score[i] != 0)
             emit sig_bonus(i, NO_TRICK_BONUS, plr_hand_score[i]);
         }

         if (omnibus && (plr_jack_diamond == i)) {
           int value;

           if (plr_score[i] + plr_hand_score[i] > omnibus_value) {
             value = -omnibus_value;
             plr_hand_score[i] -= omnibus_value;
           }
           else {
             value =  -plr_score[i] - plr_hand_score[i];
             plr_hand_score[i] = -plr_score[i];
           }
           if (value != 0)
             emit sig_bonus(i, OMNIBUS_BONUS, value);
         }
       }

     plr_score[i] += plr_hand_score[i];

     if (perfect_100 && (plr_score[i] == 100)) {
       plr_score[i] = perfect_100_value;
       emit sig_perfect_100(i);
     }

     if (plr_score[i] >= game_over_score)
       if (!is_it_draw() || !no_draw)
         game_over = true;

     emit sig_score(plr_score[i], i);
  }

  moon_add_to_scores = true;         // reset to default: true. this is important.
  emit sig_end_hand(plr_hand_score[0], plr_hand_score[1], plr_hand_score[2], plr_hand_score[3]);

  if (!game_over) {
    card_left = DECK_SIZE;
    plr_jack_diamond = NOT_FOUND;

    emit sig_clear_table();
    if (++passed_to > pNOPASS)
      passed_to = pLEFT;

    emit sig_pass_to(passed_to);

    reset_cards_on_table();
    reset_cards_played();
    reset_cards_passed();
    reset_hand_score();
    reset_plr_cards_in_suit();
    reset_plr_has_card();
    random_deck();

    emit sig_refresh_deck(user_id);

    heart_broken = false;
    if (passed_to == pNOPASS) {
      mode_playing = true;
      play_2clubs();
    } else {
       mode_playing = false;
      }
  } else
      emit sig_game_over(plr_score[0], plr_score[1], plr_score[2], plr_score[3]);
}

void CHearts::process_card(int card)
{
 int card_suit = card / 13;

 cards_played[card] = true;
 hand_cards[hand_turn] = card;
 cards_left_in_suit[card_suit]--;
 plr_cards_in_suit[turn][card_suit]--;
 cpt_plr_cards[turn]--;
 plr_has_card[turn][card] = false;

 if (omnibus && (card == jack_diamond))
   jack_diamond_played = true;

 if (card == queen_spade) {
   hand_score += 13;

   if (queen_spade_break_heart) {
     emit sig_breaking_heart();
     heart_broken = true;
   }
 } else
     if (card_suit == HEART) {
       hand_score++;

       if (!heart_broken) {
         emit sig_breaking_heart();
         heart_broken = true;
       }
     }

 if ((card > best_hand) && (card_suit == current_suit)) {
   best_hand = card;
   plr_best_hand = turn;
 }
}

void CHearts::play_hand()
{
  int card_id = get_cpu_move();

  assert((card_id >= 0) && (card_id < 13));

  int card = plr_cards[turn][card_id];

  assert((card >= 0) && (card < DECK_SIZE));

  if (current_suit == FREESUIT)
    current_suit = card / 13;

  plr_cards[turn][card_id] = empty;
  process_card(card);
  sort_plr_cards();

  emit sig_play_card(card, turn);
  advance_turn();
}

void CHearts::advance_turn()
{
 bool tram = false;

 if (++hand_turn > 3) {
   reset_cards_on_table();

   turn = plr_best_hand;

   if (omnibus && (jack_diamond_played)) {
     jack_diamond_played = false;
     plr_jack_diamond = plr_best_hand;
   }

   plr_hand_score[turn] += hand_score;

   emit sig_hand_score(plr_hand_score[turn], turn);

   hand_turn = 0;
   plr_best_hand = NOT_FOUND;
   best_hand = 0;
   hand_score = 0;

   emit sig_clear_table();
   current_suit = FREESUIT;

   if (card_left  > 5 && is_tram(turn)) {
     tram = true;
     int score = 26 - plr_hand_score[0] - plr_hand_score[1] -
                      plr_hand_score[2] - plr_hand_score[3];
     plr_hand_score[turn] += score;

     if (plr_jack_diamond == NOT_FOUND)
       plr_jack_diamond = turn;            // if the bonus apply, it will be counted in process_next_pass

     emit sig_tram(turn);
   }
 } else {
     if (++turn > 3) {
       turn = 0;
     }
   }

 if (!--card_left || tram) {
   process_next_pass(false);
   return;
 }

 if (turn != user_id)
   play_hand();
 else
    emit sig_your_turn(user_id);
}

int CHearts::play_card(int idx)
{
  int card = plr_cards[user_id][idx], retc;

  retc = check_invalid_move(user_id, card);

  if (retc != NOERROR)
    return retc;

  if (current_suit == FREESUIT)
    current_suit = card / 13;

  plr_cards[user_id][idx] = empty;
  process_card(card);
  sort_plr_cards();

  emit sig_refresh_deck(user_id);
  emit sig_play_card(card, user_id);

  advance_turn();

  return retc;
}

void CHearts::play_2clubs()
{
 mode_playing = true;
 for (int i=0; i<4; i++) {
   if (plr_cards[i][0] == two_clubs) {
     turn = i;
     hand_turn = 0;
     current_suit = CLUB;
     if (i == user_id)
       play_card(two_clubs);
     else {
        plr_cards[i][0] = empty;
        process_card(two_clubs);
        sort_plr_cards();

        emit sig_play_card(two_clubs, i);

        advance_turn();
     }
   break;
   }
 }
}

bool CHearts::is_tram(int plr) {

  if (!tram_enabled)
    return false;

  // check for no penality cards left, and jack of diamond played in omnibus.
  if (cards_played[queen_spade] && !cards_left_in_suit[HEART]) {
    if (!omnibus || cards_played[jack_diamond])
       return true;
  }

  // now check for usual tram
  int best_card[4];

  for (int i=0; i<4; i++)
    best_card[i] = NOT_FOUND;

  for (int i=0; i<4; i++) {
    if (i != plr)
      for (int i2=0; i2<13; i2++) {
         int card = plr_cards[i][i2];

         if (card == empty) break;

         int suit = card / 13;

         if (card > best_card[suit])
           best_card[suit] = card;
      }
  }

  if (plr_cards_in_suit[plr][CLUB] && (best_card[CLUB] > get_lowest_suit(plr, CLUB))) return false;
  if (plr_cards_in_suit[plr][SPADE] && (best_card[SPADE] > get_lowest_suit(plr, SPADE))) return false;
  if (plr_cards_in_suit[plr][DIAMOND] && (best_card[DIAMOND] > get_lowest_suit(plr, DIAMOND))) return false;
  if (plr_cards_in_suit[plr][HEART] && (best_card[HEART] > get_lowest_suit(plr, HEART))) return false;

  return true;
}

bool CHearts::is_moon_still_possible()
{
  int cpt = 0;

  if (plr_hand_score[0] || (omnibus && (plr_jack_diamond == 0))) cpt++;
  if (plr_hand_score[1] || (omnibus && (plr_jack_diamond == 1))) cpt++;
  if (plr_hand_score[2] || (omnibus && (plr_jack_diamond == 2))) cpt++;
  if (plr_hand_score[3] || (omnibus && (plr_jack_diamond == 3))) cpt++;

  return cpt < 2;
}

bool CHearts::is_it_draw()
{
  int lowest = plr_score[0];

  for (int i=1; i<4; i++) {
     if (plr_score[i] < lowest)
       lowest = plr_score[i];
     else
       if (plr_score[i] == lowest)
         return true;
  }

  return false;
}

bool CHearts::can_break_heart(int plr)
{
  if (card_left > DECK_SIZE - 3) return false;

  if (current_suit != FREESUIT)
    return !plr_cards_in_suit[plr][current_suit];

  return is_only_heart_left(plr);
}

bool CHearts::is_only_heart_left(int plr)
{
  return (!plr_cards_in_suit[plr][SPADE] &&
          !plr_cards_in_suit[plr][CLUB] &&
          !plr_cards_in_suit[plr][DIAMOND]);
}

bool CHearts::can_play_qs_first_hand(int plr)
{
  for (int i=0; i<13; i++) {
    if (plr_cards[plr][i] == empty) return true;
    if ((plr_cards[plr][i] / 13 != HEART) && (plr_cards[plr][i] != queen_spade)) return false;
  }
  return true;
}

int CHearts::get_lowest_suit(int plr, int suit)
{
  for (int i=0; i<13; i++) {
     int card = plr_cards[plr][i];

     if (card == empty) return NOT_FOUND;
     if (card / 13 == suit) return card;
   }
  return NOT_FOUND;
}

int CHearts::get_lowest_suit_pos(int plr, int suit)
{
  for (int i=0; i<13; i++) {
     int card = plr_cards[plr][i];

     if (card == empty) return NOT_FOUND;
     if (card / 13 == suit) return i;
  }
  return NOT_FOUND;
}

int CHearts::get_highest_suit_pos(int plr, int suit)
{
  int highest = NOT_FOUND;

  for (int i=0; i<13; i++) {
     int card = plr_cards[plr][i];
     int card_suit = card / 13;

     if (card == empty) return NOT_FOUND;
     if (card_suit > suit) return highest;
     if (card_suit == suit)
       highest = i;
  }
  return highest;
}

bool CHearts::unselect_card(int idx)
{
  if (cards_selected_count[user_id]) {
    cards_selected_count[user_id]--;
    cards_selected[user_id][idx] = false;
    return true;
  }
  return false;
}

bool CHearts::select_card(int idx)
{
  if (cards_selected_count[user_id] < 3) {
    cards_selected_count[user_id]++;
    cards_selected[user_id][idx] = true;
    return true;
  }\
  return false;
}

bool CHearts::is_card_on_table(int card)
{
  switch (hand_turn) {
      case 3 : if (hand_cards[2] == card) return true;
      case 2 : if (hand_cards[1] == card) return true;
      case 1 : if (hand_cards[0] == card) return true;
  }

  return false;
}

int CHearts::count_plr_cards(int plr)
{
  return cpt_plr_cards[plr];
}

int CHearts::count_my_cards()
{
  return cpt_plr_cards[user_id];
}

int CHearts::get_card_position(int plr, int card)
{
  for (int i=0; i<13; i++) {
    if (plr_cards[plr][i] == empty) return NOT_FOUND;
    if (plr_cards[plr][i] == card) return i;
  }
  return NOT_FOUND;
}

int CHearts::check_invalid_move(int plr, int card)
{
  int card_suit = card / 13;

  if ((card_suit == HEART) && !heart_broken && !can_break_heart(plr)) {
    return ERRHEART;
  }

  if ((card == queen_spade) && (card_left > DECK_SIZE - 4) && !can_play_qs_first_hand(plr)) {
     return ERRQUEEN;
  }

  if ((current_suit != FREESUIT) && (card_suit != current_suit) && plr_cards_in_suit[plr][current_suit]) {
    return ERRSUIT;
  }

  return NOERROR;
}

bool CHearts::is_ready_to_pass()
{
 return cards_selected_count[user_id] == 3;
}

bool CHearts::is_game_over()
{
  return game_over;
}

bool CHearts::is_no_pass()
{
  return passed_to == pNOPASS;
}

bool CHearts::is_mode_playing()
{
 return mode_playing;
}

bool CHearts::is_it_my_turn()
{
 return turn == user_id;
}

bool CHearts::is_card_selected(int plr, int idx)
{
 return cards_selected[plr][idx];
}

bool CHearts::is_card_selected(int idx)
{
 return cards_selected[user_id][idx];
}

int CHearts::get_current_suit()
{
  return current_suit;
}

int CHearts::get_my_score()
{
  return plr_score[user_id];
}

int CHearts::whoami()
{
 return user_id;
}

int CHearts::get_card(int plr, int idx)
{
  return plr_cards[plr][idx];
}

int CHearts::get_card(int idx)
{
  return plr_cards[user_id][idx];
}

void CHearts::set_moon_add_to_score(bool enable)
{
  moon_add_to_scores = enable;
  process_next_pass(true);
}

void CHearts::set_perfect_100(bool enable)
{
  perfect_100 = enable;
}

void CHearts::set_omnibus(bool enable)
{
  omnibus = enable;
}

void CHearts::set_no_trick_bonus(bool enable)
{
  no_trick_bonus = enable;
}

void CHearts::set_queen_spade_break_heart(bool enable)
{
  queen_spade_break_heart = enable;
}

void CHearts::set_tram_enabled(bool enable)
{
  tram_enabled = enable;
}

void CHearts::set_new_moon(bool enable)
{
  new_moon = enable;
}

void CHearts::set_no_draw(bool enable)
{
  no_draw = enable;
}

bool CHearts::is_first_card()
{
  return card_left == DECK_SIZE;
}
