#include "chearts.h"
#include "variants.h"
#include <assert.h>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include "define.h"

CHearts::CHearts()
{
  // load_save_game() doesn't call init_vars()
  undo_data = {};
  moon_wait = false;
}

CHearts::~CHearts()
{
}

void CHearts::new_game(bool booting)
{
  init_vars();

  reset_score();
  reset_hand_score();

  reset_cards_on_table();
  reset_cards_left_in_suit();
  reset_cards_played();
  reset_cards_passed();
  reset_plr_cards_in_suit();
  reset_plr_has_card();

  random_deck();

  user_id = PLAYER_SOUTH;

  emit sig_clear_table();

  if (!booting)
    emit sig_new_game();

  emit sig_refresh_deck(true);
  emit sig_pass_to(passed_to);
}

void CHearts::init_vars()
{
  card_left = DECK_SIZE;
  fresh_game = true;
  game_over = false;
  shoot_moon = false;
  moon_wait = false;
  heart_broken = false;
  jack_diamond_played = false;
  moon_add_to_scores = true;
  passed_to = pLEFT;
  mode_playing = false;
  plr_jack_diamond = NOT_FOUND;
  plr_best_hand = NOT_FOUND;
  hand_score = 0;
  best_hand = 0;
  hand_turn = FIRST_CARD;
  current_suit = CLUB;
}

int CHearts::save_game(int plr1, int plr2, int plr3, int plr4)
{
  QFile file(QDir::homePath() + SAVEDGAME_FILENAME);

  if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
     return ERROPENWO;
  }

  QTextStream out(&file);

  out << plr1 << " " << plr2 << " "
      << plr3 << " " << plr4 << EOL;                             // plr names idx (0 = whoami)

  out << turn          << " " << hand_turn << " "                // turn, hand turn, plr_best_hand,
      << plr_best_hand << " " << plr_jack_diamond << " "         // plr_jack_diamond, passed_to, current_suit
      << passed_to     << " " << current_suit << " " << EOL;

  out << heart_broken  << " " << hand_score   << " "             // heart_broken, hand_score, best_hand
      << best_hand     << " " << mode_playing << EOL;           // mode_playing

  out << plr_hand_score[PLAYER_SOUTH] << " "
      << plr_hand_score[PLAYER_WEST]  << " "                     // players hand_score
      << plr_hand_score[PLAYER_NORTH] << " "
      << plr_hand_score[PLAYER_EAST]  << EOL;

  out << plr_score[PLAYER_SOUTH] << " "
      << plr_score[PLAYER_WEST]  << " "                          // players score
      << plr_score[PLAYER_NORTH] << " "
      << plr_score[PLAYER_EAST]  << EOL;

  out << hand_cards[FIRST_CARD]  << " "
      << hand_cards[SECOND_CARD] << " "                          // hand_cards
      << hand_cards[THIRD_CARD]  << " "
      << hand_cards[FOURTH_CARD] << EOL;

  for (int i=0; i<4; i++) {                                      // player's cards
    for (int i2=0; i2<13; i2++) {
       out << plr_cards[i][i2] << " ";
     }
     out << EOL;
  }

  file.close();
  return NOERROR;
}

int CHearts::load_saved_game()
{
  QFile file(QDir::homePath() + SAVEDGAME_FILENAME);

  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    return ERROPENRO;
  }

  int cpt = 0;
  bool found_whoami = false;
  bool card_found[DECK_SIZE];
  for (int i=0; i<DECK_SIZE; i++)
    card_found[i] = false;

  while (!file.atEnd()) {
    int value;

    QString line = file.readLine();
    cpt++;
    switch (cpt) {
       case 1 :   // extract the players names index in the name lists. also, find (whoami == 0)
                 for (int i=0; i<4; i++) {
                    value = line.section(' ', i, i).toInt();
                    if ((value < 0) || (value > MAX_PLR_NAMES - 1))
                      return FCORRUPTED;

                    plr_name_id[i] = value;
                    if (!value) {
                      if (found_whoami)
                        return FCORRUPTED;
                      user_id = i;
                      if (user_id != PLAYER_SOUTH)
                        return FCORRUPTED; // version prior 1.6 saved game with user id != PLAYER_SOUTH is unsupported
                      found_whoami = true;
                    }
                 }
                 break;

       case 2 :  // extract turn, hand_turn, passed_to
                 for (int i=0; i<6; i++) {
                   value = line.section(' ', i, i).toInt();
                   if ((value < -1) || (value > 4))
                     return FCORRUPTED;

                   if ((value > 3) && (i != 5))                // current_suit can have value = 4 = FREESUIT
                     return FCORRUPTED;

                   if ((value == -1) && (i != 2) && (i != 3))  // plr_best_hand, plr_jack_diamond = -1 = NOTFOUND
                      return FCORRUPTED;

                   switch (i) {
                      case 0 : turn = value; break;
                      case 1 : hand_turn = value; break;
                      case 2 : plr_best_hand = value; break;
                      case 3 : plr_jack_diamond = value; break;
                      case 4 : passed_to = value; break;
                      case 5 : current_suit = value; break;
                   }
                 }
                 break;

       case 3 :  // extract heart_broken, hand_score, best_hand, mode_playing
                 for (int i=0; i<4; i++) {
                   value = line.section(' ', i, i).toInt();
                   switch (i) {
                      case 0 : if ((value < 0) || (value > 1))
                                 return FCORRUPTED;
                               heart_broken = value;
                               break;
                      case 1 : if ((value < -10) || (value > 26))
                                 return FCORRUPTED;
                               hand_score = value;
                               break;
                      case 2 : if ((value < two_clubs) || (value > DECK_SIZE - 1))
                                 return FCORRUPTED;
                               best_hand = value;
                               break;
                      case 3 : if ((value < 0) || (value > 1))
                                 return FCORRUPTED;
                               mode_playing = value;
                               break;
                   }
                 }
                 break;
       case 4 :  // extract players hand_score
                 for (int i=0; i<4; i++) {
                   value = line.section(' ', i, i).toInt();
                   if ((value < -10) || (value > 26))
                     return FCORRUPTED;
                   plr_hand_score[i] = value;
                 }
                 break;
       case 5 :  // extract players score
                 for (int i=0; i<4; i++) {
                   value = line.section(' ', i, i).toInt();
                   if ((value < 0) || (value > game_over_score - 1))
                     return FCORRUPTED;      // FIX ME: it's possible to have a score above 100 pts,
                                             // in no draw enabled situation...
                   plr_score[i] = value;
                 }
                 break;
       case 6 : // extract hand_cards
                for (int i=0; i<4; i++) {
                  value = line.section(' ', i, i).toInt();
                  if ((value < two_clubs) || (value >= DECK_SIZE)) {
                    if (value != empty)
                      return FCORRUPTED;
                  }
                  hand_cards[i] = value;
                }
                break;
       case 7 :
       case 8 :
       case 9 :
       case 10: // read players cards
                for (int i=0; i<13; i++) {
                  value = line.section(' ', i, i).toInt();

                  // found the same card twice !
                  if ((value != empty) && card_found[value]) {
                    return FCORRUPTED;
                  }

                  if (((value < two_clubs) || (value > DECK_SIZE - 1)) && (value != empty))
                    return FCORRUPTED;     

                  if (value != empty)
                    card_found[value] = true;

                  plr_cards[cpt - 7][i] = value;
                }
                break;

       default : break; // return FCORRUPTED ?? or break ??
    }
  }

  if (cpt != 10)
    return FCORRUPTED;

  // make sure there is no card played twice on the table.
  if (((hand_cards[FIRST_CARD] == hand_cards[SECOND_CARD]) ||
       (hand_cards[FIRST_CARD] == hand_cards[THIRD_CARD])) && (hand_cards[FIRST_CARD] != empty))
    return FCORRUPTED;

  if ((hand_cards[SECOND_CARD] == hand_cards[THIRD_CARD]) && (hand_cards[SECOND_CARD] != empty))
    return FCORRUPTED;

  file.remove();

  shoot_moon = false;
  game_over = false;

  // check if the jack of diamond is played in the current hand
  if ((hand_cards[FIRST_CARD] == jack_diamond) || (hand_cards[SECOND_CARD] == jack_diamond) ||
      (hand_cards[THIRD_CARD] == jack_diamond) || (hand_cards[FOURTH_CARD] == jack_diamond))
    jack_diamond_played = true;
  else
    jack_diamond_played = false;

  moon_add_to_scores = true;
  reset_cards_played();
  reset_cards_passed();
  reset_plr_cards_in_suit();
  reset_plr_has_card();

  // Those inits are different from a new_game() -> reset_function().
  for (int i=0; i<4; i++) {
    cpt_plr_cards[i] = 0;
    cards_left_in_suit[i] = 0;
  }

  card_left = 0;
  // analyse the cards for counters, etc... and plr_has_card[]
  for (int i=0; i<4; i++) {
    for (int i2=0; i2<13; i2++) {
      int card = plr_cards[i][i2];
      int card_suit = card / 13;

      if (card != empty) {
        card_left++;
        cpt_plr_cards[i]++;
        plr_has_card[i][card] = true;
        plr_cards_in_suit[i][card_suit]++;
        cards_left_in_suit[card_suit]++;
      }
    }
  }

  // make sure there is enough card left
  if (!mode_playing && (card_left < DECK_SIZE))
    return FCORRUPTED;

  if (mode_playing && !card_left)
    return FCORRUPTED;

  // make sure that we found who user_id is
  if (!found_whoami)
    return FCORRUPTED;

 // check that the number of cards are valid (they should have the same number of card, not more than 1 card diff)
  int cpt_card = cpt_plr_cards[user_id];
  int next = user_id;
  int diff;
  bool found = false;
  for (int i=0; i<3; i++) {
    if (++next > 3)
      next = 0;

    diff = cpt_card - cpt_plr_cards[next];

    if (found && (diff != 1))
      return FCORRUPTED;

    if ((diff < 0) || (diff > 1))
      return FCORRUPTED;

    if (diff == 1)
      found = true;
  }

 // analyse the cards to find those that has been played.
 for (int i=0; i<DECK_SIZE; i++)
   if (!plr_has_card[PLAYER_SOUTH][i] && !plr_has_card[PLAYER_WEST][i] &&
       !plr_has_card[PLAYER_NORTH][i] && !plr_has_card[PLAYER_EAST][i]) {
     cards_played[i] = true;
  }

  // check that the hand cards has been found as played.
  for (int i=0; i<3; i++)
    if ((hand_cards[i] != empty) && !cards_played[hand_cards[i]]) {
      return FCORRUPTED;
    }

  // emit the new score/hand score of this game.
  for (int i=0; i<4; i++) {
    emit sig_hand_score(i, plr_hand_score[i]);
    emit sig_score(i, plr_score[i]);
  }

  emit sig_clear_table();
  emit sig_refresh_deck(false);
  emit sig_pass_to(passed_to);

  if (mode_playing)
    emit sig_your_turn(user_id);

  fresh_game = false;

  return NOERROR;
}

void CHearts::reset_cards_on_table()
{
  hand_cards[FIRST_CARD]  = empty;
  hand_cards[SECOND_CARD] = empty;
  hand_cards[THIRD_CARD]  = empty;
  hand_cards[FOURTH_CARD] = empty;
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

    emit sig_hand_score(i, plr_hand_score[i]);
  }
}

void CHearts::reset_score()
{
  for (int i=0; i<4; i++) {
    plr_score[i] = 0;

    emit sig_score(i, plr_score[i]);
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
  cpt_plr_cards[PLAYER_SOUTH] = 13;
  cpt_plr_cards[PLAYER_WEST] = 13;
  cpt_plr_cards[PLAYER_NORTH] = 13;
  cpt_plr_cards[PLAYER_EAST] = 13;

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

  int save_pos[4][3] = {};

  for (int i=0; i<3; i++) {
    while (!cards_selected[PLAYER_SOUTH][++a]) assert(a < 12);
    while (!cards_selected[PLAYER_WEST][++b]) assert(b < 12);
    while (!cards_selected[PLAYER_NORTH][++c]) assert(c < 12);
    while (!cards_selected[PLAYER_EAST][++d]) assert(d < 12);
    save_pos[PLAYER_SOUTH][i] = a;
    save_pos[PLAYER_WEST][i] = b;
    save_pos[PLAYER_NORTH][i] = c;
    save_pos[PLAYER_EAST][i] = d;
  }

  emit sig_pass_cards(save_pos[PLAYER_WEST][0], save_pos[PLAYER_WEST][1], save_pos[PLAYER_WEST][2],
                      save_pos[PLAYER_NORTH][0], save_pos[PLAYER_NORTH][1], save_pos[PLAYER_NORTH][2],
                      save_pos[PLAYER_EAST][0], save_pos[PLAYER_EAST][1], save_pos[PLAYER_EAST][2]);

  for (int i=0; i<3; i++) {
    a = save_pos[PLAYER_SOUTH][i];
    b = save_pos[PLAYER_WEST][i];
    c = save_pos[PLAYER_NORTH][i];
    d = save_pos[PLAYER_EAST][i];

    w = plr_cards[PLAYER_SOUTH][a];
    x = plr_cards[PLAYER_WEST][b];
    y = plr_cards[PLAYER_NORTH][c];
    z = plr_cards[PLAYER_EAST][d];

    plr_cards_in_suit[PLAYER_SOUTH][w / 13]--;
    plr_cards_in_suit[PLAYER_WEST][x / 13]--;
    plr_cards_in_suit[PLAYER_NORTH][y / 13]--;
    plr_cards_in_suit[PLAYER_EAST][z / 13]--;

    plr_has_card[PLAYER_SOUTH][w] = false;
    plr_has_card[PLAYER_WEST][x] = false;
    plr_has_card[PLAYER_NORTH][y] = false;
    plr_has_card[PLAYER_EAST][z] = false;

    switch (passed_to) {
       case pLEFT:    plr_cards[PLAYER_SOUTH][a] = z;
                      plr_cards[PLAYER_WEST][b] = w;
                      plr_cards[PLAYER_NORTH][c] = x;
                      plr_cards[PLAYER_EAST][d] = y;

                      plr_cards_in_suit[PLAYER_SOUTH][z / 13]++;
                      plr_cards_in_suit[PLAYER_WEST][w / 13]++;
                      plr_cards_in_suit[PLAYER_NORTH][x / 13]++;
                      plr_cards_in_suit[PLAYER_EAST][y / 13]++;

                      plr_has_card[PLAYER_SOUTH][z] = true;
                      plr_has_card[PLAYER_WEST][w] = true;
                      plr_has_card[PLAYER_NORTH][x] = true;
                      plr_has_card[PLAYER_EAST][y] = true;
                      break;

       case pRIGHT:   plr_cards[PLAYER_SOUTH][a] = x;
                      plr_cards[PLAYER_WEST][b] = y;
                      plr_cards[PLAYER_NORTH][c] = z;
                      plr_cards[PLAYER_EAST][d] = w;

                      plr_cards_in_suit[PLAYER_SOUTH][x / 13]++;
                      plr_cards_in_suit[PLAYER_WEST][y / 13]++;
                      plr_cards_in_suit[PLAYER_NORTH][z / 13]++;
                      plr_cards_in_suit[PLAYER_EAST][w / 13]++;

                      plr_has_card[PLAYER_SOUTH][x] = true;
                      plr_has_card[PLAYER_WEST][y] = true;
                      plr_has_card[PLAYER_NORTH][z] = true;
                      plr_has_card[PLAYER_EAST][w] = true;
                      break;

       case pACCROSS: plr_cards[PLAYER_SOUTH][a] = y;
                      plr_cards[PLAYER_WEST][b] = z;
                      plr_cards[PLAYER_NORTH][c] = w;
                      plr_cards[PLAYER_EAST][d] = x;

                      plr_cards_in_suit[PLAYER_SOUTH][y / 13]++;
                      plr_cards_in_suit[PLAYER_WEST][z / 13]++;
                      plr_cards_in_suit[PLAYER_NORTH][w / 13]++;
                      plr_cards_in_suit[PLAYER_EAST][x / 13]++;

                      plr_has_card[PLAYER_SOUTH][y] = true;
                      plr_has_card[PLAYER_WEST][z] = true;
                      plr_has_card[PLAYER_NORTH][w] = true;
                      plr_has_card[PLAYER_EAST][x] = true;
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

int CHearts::eval_card_strength(int plr, int card)
{
 int suit = card / 13;

 if (!plr_cards_in_suit[plr][suit])
   return 0;

 int cpt = 0;

 for (int i=first_card_suit[suit]; i<=last_card_suit[suit]; i++) {
    if (!cards_played[i] && !plr_has_card[plr][i]) {
      if (card > i)
        cpt++;
    }
  }

 assert(cards_left_in_suit[suit]);
 int value = (cpt / cards_left_in_suit[suit]) * 100;

 return value;
}

void CHearts::AI_pass_spades(int cpu, int &cpt)
{
  int spade_count = plr_cards_in_suit[cpu][SPADE];

  if (!spade_count)
    return;

  int pos_queen_spade = get_card_position(cpu, queen_spade);

  bool qs_passed = false;

  // if AI_flags_safe_keep_qs is not set, or it's not safe to keep -> pass the queen of spade.
  if (!(AI_cpu_flags[cpu] & AI_flags_safe_keep_qs) || (spade_count < 5)) {
    if (pos_queen_spade != NOT_FOUND) {
      AI_pass_save_card(pos_queen_spade, cpt);
      qs_passed = true;
     }
  }

  // if we kept the queen of spade then don't pass either ace or king of spade, otherwise try to pass them.
  if ((pos_queen_spade == NOT_FOUND) || qs_passed) {
    int pos_ace_spade = get_card_position(cpu, ace_spade);

    if (pos_ace_spade != NOT_FOUND)
      AI_pass_save_card(pos_ace_spade, cpt);

    int pos_king_spade = get_card_position(cpu, king_spade);

    if (pos_king_spade != NOT_FOUND)
      AI_pass_save_card(pos_king_spade, cpt);
  }
}

void CHearts::AI_pass_clubs(int cpu, int &cpt)
{
 if (!plr_cards_in_suit[cpu][CLUB])
   return;

 // if AI_flags_elim_suit is set, try to eliminate our clubs.
 if (AI_cpu_flags[cpu] & AI_flags_pass_elim_suit) {
   AI_pass_remove_suit(cpu, CLUB, cpt);
   if (cpt == 3)
     return;
 }

 // if AI_flags_try_moon is set, pass our lowest cards, otherwise pass the highest.
 if ((AI_cpu_flags[cpu] & AI_flags_try_moon) && is_prepass_to_moon())
   AI_pass_save_card(get_lowest_suit_pos(cpu, CLUB), cpt);
 else
   AI_pass_save_card(get_highest_suit_pos(cpu, CLUB), cpt);
}

void CHearts::AI_pass_hearts(int cpu, int &cpt)
{
 int heart_count = plr_cards_in_suit[cpu][HEART];

 if (!heart_count)
   return;

 // if AI_flags_pass_hearts_zero is set, let's roll the dice so 70% of the time we don't pass.
 int chance = rand() % 100;
 if ((AI_cpu_flags[cpu] & AI_flags_pass_hearts_zero) && (chance < 70))
   return;

 // adjust_low is use to pass a low heart, but not our lowest, it design on how many hearts we own.
 const int adjust_low[14]  = {0, 0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6};
 int adjust_high = heart_count > 1 ? 1 : 0;

 int low = NOT_FOUND;
 int high = NOT_FOUND;

 // if AI_flags_pass_hearts_low is set, this cpu will try to pass a low heart, but
 // not the ace of hearts.
 if ((AI_cpu_flags[cpu] & AI_flags_pass_hearts_low) || (chance >= 70))
   low = get_lowest_suit_pos(cpu, HEART) + adjust_low[heart_count];

 // if AI_flags_pass_hearts_high, this cpu will try to pass a high heart, but
 // not the ace of hearts.
 if (AI_cpu_flags[cpu] & AI_flags_pass_hearts_high)
   high = get_highest_suit_pos(cpu, HEART) - adjust_high;

 if ((low != NOT_FOUND) && (plr_cards[cpu][low] != ace_heart)) {
   AI_pass_save_card(low, cpt);
   if (cpt == 3)
     return;
  }

 if ((high != NOT_FOUND) && (high != low) && (plr_cards[cpu][high] != ace_heart))
   AI_pass_save_card(high, cpt);
}

void CHearts::AI_pass_diamonds(int cpu, int &cpt)
{
 if (!plr_cards_in_suit[cpu][DIAMOND])
   return;

 // check for omnibus friendly pass. only 1 friendly pass.
 if (omnibus && AI_pass_friendly()) {
   if (plr_has_card[turn][jack_diamond])
     AI_pass_save_card(get_card_position(turn, jack_diamond), cpt);
   else
   if (plr_has_card[turn][ace_diamond])
     AI_pass_save_card(get_card_position(turn, ace_diamond), cpt);
   else
   if (plr_has_card[turn][king_diamond])
     AI_pass_save_card(get_card_position(turn, king_diamond), cpt);

   if (cpt == 3)
     return;
 }

 // if AI_flags_elim_suit is set, try to eliminate our diamond.
 if (AI_cpu_flags[cpu] & AI_flags_pass_elim_suit) {
   AI_pass_remove_suit(cpu, DIAMOND, cpt);
   if (cpt == 3)
     return;
 }

 // if not omnibus pass highest diamond, but not in try to moon
 if (!omnibus && (!(AI_cpu_flags[cpu] & AI_flags_try_moon) || !is_prepass_to_moon()))
   AI_pass_save_card(get_highest_suit_pos(cpu, DIAMOND), cpt);
}

void CHearts::AI_pass_random(int cpu, int &cpt)
{
 int spade_count = plr_cards_in_suit[cpu][SPADE];

 int card_id;
 int tries = 0;

 while (cpt != 3) {
   card_id = rand() % 13;

   int card = plr_cards[cpu][card_id];

   if (++tries < 50) { // avoid infinite loop. 50 tries is enough.
     if (omnibus && ((card == ace_diamond) || (card == king_diamond) ||
                     (card == queen_diamond) || (card == jack_diamond)))
       continue;

     if ((card / 13 == SPADE) && (spade_count < 5) && (card < queen_spade))
       continue;

     if ((AI_cpu_flags[cpu] & AI_flags_try_moon) && ((card % 13 + 2) > 9))
       continue;
   }

   tries = 0;

   AI_pass_save_card(card_id, cpt);
 }
}

void CHearts::AI_pass_to_moon(int cpu, int &cpt)
{
  int heart_count = plr_cards_in_suit[turn][HEART];

  // if we're having heart, but not the ace of heart, we needs to eliminate heart absolutely.
  if (heart_count && ((heart_count < 9) || !plr_has_card[turn][ace_heart])) {
    int low = get_lowest_suit_pos(cpu, HEART);

    int high_cards = 0;
    if (plr_has_card[turn][king_heart]) high_cards++;
    if (plr_has_card[turn][queen_heart]) high_cards++;
    if (plr_has_card[turn][jack_heart]) high_cards++;

    do {
      int card = plr_cards[turn][low];
      if ((card < jack_heart - high_cards) || !plr_has_card[turn][ace_heart])
        AI_pass_save_card(low, cpt);
      low++;
    } while ((cpt != 3) && (low < 13));
    if (cpt == 3)
     return;
  }

  AI_pass_remove_suit(cpu, CLUB, cpt);
  AI_pass_remove_suit(cpu, SPADE, cpt);
  AI_pass_remove_suit(cpu, DIAMOND, cpt);
}

void CHearts::AI_pass_remove_suit(int cpu, int suit, int &cpt)
{
 int suit_count = plr_cards_in_suit[cpu][suit];

 if (!suit_count || (cpt == 3))
   return;

 if ((3-cpt) >= suit_count) {
   int low = get_lowest_suit_pos(cpu, suit);
   do {
       AI_pass_save_card(low, cpt);
       low++;
   } while ((cpt != 3) && (low < 13));
 }
}

void CHearts::AI_pass_save_card(int card_id, int &cpt)
{
  if (card_id == NOT_FOUND)                    // safety check
    return;

  if (plr_cards[turn][card_id] == empty)
    return;

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

void CHearts::AI_pass_cpus_cards()
{
 for (int cpu=0; cpu<4; cpu++)
   if (cpu != user_id) {
     int cpt = 0;

     if ((AI_cpu_flags[turn] & AI_flags_try_moon) && is_prepass_to_moon()) {
       AI_pass_to_moon(cpu, cpt);

       if (cpt == 3) {
         set_cpu_passing_cards(cpu);
         continue; // next cpu
       }
     }

     AI_pass_spades(cpu, cpt);

     if (cpt == 3) {
       set_cpu_passing_cards(cpu);
       continue;
     }

     AI_pass_hearts(cpu, cpt);

     if (cpt == 3) {
       set_cpu_passing_cards(cpu);
       continue;
     }

     AI_pass_clubs(cpu, cpt);

     if (cpt == 3) {
       set_cpu_passing_cards(cpu);
       continue;
     }

     AI_pass_diamonds(cpu, cpt);

     if (cpt == 3) {
       set_cpu_passing_cards(cpu);
       continue;
     }

     AI_pass_random(cpu, cpt);

     set_cpu_passing_cards(cpu);
  }
}

bool CHearts::AI_pass_friendly()
{
  if (passed_to == pNOPASS)
    return false;

  if (!(AI_cpu_flags[turn] & AI_flags_friendly))
    return false;

  if (plr_score[turn] == get_lowest_score())
    return false;

  int score_pass_to = plr_score[get_plr_pass_to(turn)];

  if ((score_pass_to == get_highest_score()) && (score_pass_to - plr_score[turn]) > 20)
    return true;

  return false;
}

int CHearts::AI_eval_lead_freesuit(int card)
{
  int card_suit = card / 13;

  // try to catch someone with the queen of spade.
  if (card == queen_spade) {
    int diff_spade = cards_left_in_suit[SPADE] - plr_cards_in_suit[turn][SPADE];

    if ((AI_CPU_flags[turn] & AI_flags_count_spade) && !is_moon_an_option()) {
      if ((diff_spade == 1) && !plr_has_card[turn][ace_spade] && !cards_played[ace_spade])
        return 100;

      if ((diff_spade == 1) && !plr_has_card[turn][king_spade] && !cards_played[king_spade])
        return 101;

      if ((diff_spade == 2) && !plr_has_card[turn][ace_spade] && !plr_has_card[turn][king_spade] &&
                               !cards_played[ace_spade] && !cards_played[king_spade])
        return 102;
    }

    // don't lead the queen of spade
    if (!is_moon_an_option())
      return -100;
  }

  // don't lead in a suit where there is no cards left beside ours.
  if (!is_moon_an_option()) {
    if (cards_left_in_suit[card_suit] == plr_cards_in_suit[turn][card_suit])
      return -80;
  }

  // avoid leading the ace/king of spade, if the queen hasn't been played and we don't own it
  // otherwise, let's try to elimitate some spade.
  if (((card == ace_spade) || (card == king_spade)) && !cards_played[queen_spade]) {
    if (plr_has_card[turn][queen_spade] && ((plr_cards_in_suit[turn][SPADE] > 4) || is_moon_an_option()))
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

  // the stronger that card is, the worst it is to play.
  if (is_moon_an_option())
    return eval_card_strength(turn, card);
  else
    return -eval_card_strength(turn, card);
}

int CHearts::AI_eval_lead_spade(int card)
{
 // the lead is spade and the ace/king is in the trick? play the queen otherwise don't.
 if (card == queen_spade) {
   if ((is_card_on_table(ace_spade) || is_card_on_table(king_spade)) && !is_moon_an_option())
     return 104;
   else
     return -104;
 }

 // avoid giving the jack of diamond
  if (omnibus && (card == jack_diamond))
    return -62;

  // last to talk, the queen is not in the trick.. throw your ace/king.
   if (!is_moon_an_option()) {
     if ((hand_turn == FOURTH_CARD) && !is_card_on_table(queen_spade) && ((card == ace_spade) || (card == king_spade)))
       return 40;
   }

 // if the ace is in the trick throw away our king
  if ((card == king_spade) && is_card_on_table(ace_spade) && !is_moon_an_option())
    return 75;
  else
    return -25;
}

int CHearts::AI_eval_lead_diamond(int card)
{
  if (omnibus) {
  // If the jack of diamond is on the table, try to grab it with the ace/king/queen. (if it's safe).
    if (((card == ace_diamond) || (card == king_diamond) || (card == queen_diamond)) && is_card_on_table(jack_diamond)) {
      if (is_card_on_table(queen_spade))
        return -65;
      else
      if (cards_played[queen_spade] || plr_has_card[turn][queen_spade] ||
         (hand_turn == FOURTH_CARD) || is_moon_an_option())
        return 25 + (card % 13);
     }

  // will we win this hand?
     if (card == jack_diamond) {
       if (((cards_played[queen_spade] && !is_card_on_table(queen_spade)) || plr_has_card[turn][queen_spade]) &&
           ((cards_played[ace_diamond] && !is_card_on_table(ace_diamond)) || plr_has_card[turn][ace_diamond]) &&
           ((cards_played[king_diamond] && !is_card_on_table(king_diamond)) || plr_has_card[turn][king_diamond]) &&
           ((cards_played[queen_diamond] && !is_card_on_table(queen_diamond)) || plr_has_card[turn][queen_diamond]))
         return  62;

  // You are the last one to play in the trick, try to use your Jack of diamond:
  // if it the strongest card of the trick.
       if ((hand_turn == FOURTH_CARD) && !is_card_on_table(queen_spade) &&
                                         !is_card_on_table(ace_diamond) &&
                                         !is_card_on_table(king_diamond) &&
                                         !is_card_on_table(queen_diamond))
         return 30;

       // don't play the jack of diamond
       return -65;
     }
  }

  return 0;
}

int CHearts::AI_eval_lead_hearts(int card)
{
  int highest = get_highest_card_table();

  if (is_moon_an_option()) {
    if ((card == queen_spade) || (omnibus && (card == jack_diamond)))
      return -50;
    else {
       if (card > highest)
         return -card + 30;
       else
         return -30;
    }

  }
  else {
     if (card < highest)
       return card - get_highest_card_table() + 30;
     else
       return -30;
  }
}

int CHearts::AI_get_cpu_move()
{
 int eval[13] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

 for (int i=0; i<13; i++) {
    int card = plr_cards[turn][i];
    // invalide move, and empty card are avoided by having the worst eval.
    if ((card == empty) || (check_invalid_move(turn, card) != NOERROR))
      eval[i] = -32768;
    else {

      // only valid move from here
      switch (current_suit) {
         case FREESUIT: eval[i] = AI_eval_lead_freesuit(card);
                        break;

         case SPADE:    eval[i] = AI_eval_lead_spade(card);
                        break;

         case DIAMOND:  eval[i] = AI_eval_lead_diamond(card);
                        break;

         case HEART:    eval[i] = AI_eval_lead_hearts(card);
                        break;
      }

      if ((current_suit != FREESUIT) && (current_suit != SPADE)) {
        // give away the queen of spade
        if (!is_moon_an_option()) {
          if (card == queen_spade)
            eval[i] = 105;
          else
          // the queen of spade has not been played yet, let's throw our ace/king spade away.
          if (((card == ace_spade) || (card == king_spade)) && !cards_played[queen_spade])
            eval[i] = 61;
        }
      }

    // if we didn't hit a specific evaluation in previous section, let's try those evaluations.
      if (eval[i] == 0) {
        // don't give away the jack of diamond.
        if ((current_suit != DIAMOND) && omnibus && (card == jack_diamond))
         eval[i] = -63;
        else
        // we are the last one of the trick: play our bigger cards, but not in hearts, and not if queen spade is in
         if ((hand_turn == FOURTH_CARD) && (current_suit != HEART) && !is_card_on_table(queen_spade))
           eval[i] = card % 13 + 2;
        else
        if (!is_moon_an_option() && (current_suit != FREESUIT)) {
           // throw away our big hearts cards
           if ((current_suit != HEART) && (card / 13 == HEART))
             eval[i] = (card % 13) + 30;
           else
           // not in the suit, throw the bigest cards
           if (current_suit != card / 13)
             eval[i] = (card % 13) + 15;
           else {
           // in the suit, throw our bigest cards under the best cards on the table
              int highest = get_highest_card_table();

              if (card < highest)
                eval[i] = -(card - highest);
           }
        }
        else
        // play the lowest card.
          eval[i] = -(card % 13 + 2);
      }
    }
 }

 // search for the best move.
 int best_move = -32768, best_card_id = 0;

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
          int the_score = plr_score[i];

          if (new_moon && (the_score >= 26)) {
            if (i == user_id) {
              moon_wait = true;
              emit sig_shoot_moon(i);

              return;                      // if new_moon is enabled, the score is above 26, and
                                           // it's not a cpu who moon... we return, because
                                           // we need to select [add] or [subtract]...
                                           // emit_signal(SIG_SHOOT_MOON) will trigger the choice
                                           // and, we will return by set_moon_add_score() that will skip
                                           // this part.
            } else
                moon_add_to_scores = get_cpu_moon_add(i);
          }
          emit sig_shoot_moon(i);
          break;                    // we found someone who moon. no need to look further.
      }
    }
  }

  for (int i=0; i<4; i++) {
     if (shoot_moon) {
        if ((plr_hand_score[i] == 0) && (!new_moon || moon_add_to_scores))
          plr_hand_score[i] = 26;
        else {
          if (plr_hand_score[i] == 26) {
            if (new_moon && !moon_add_to_scores)
              plr_hand_score[i] = -26;
            else
              plr_hand_score[i] = 0;
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
       game_over = true;

     emit sig_score(i, plr_score[i]);
  }

  moon_add_to_scores = true;         // reset to default: true. this is important.

  if (is_it_draw() && no_draw)
    game_over = false;               // we must reset game_over to false, if it's a draw and no draw
                                     // otherwise, on the next hand a game could end with a score of:
                                     // 95 70 24 26 because the looser got his score below 100 by a bonus.
                                     // the game shouldn't be over anymore !

  // This signal must be done after the no_draw check to fix a draw glitch,
  // not having the shuffling card sound due to "game over = true", before the test.
  emit sig_end_hand(plr_hand_score[PLAYER_SOUTH], plr_hand_score[PLAYER_WEST],
                    plr_hand_score[PLAYER_NORTH], plr_hand_score[PLAYER_EAST]);

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
    reset_cards_left_in_suit();
    reset_plr_cards_in_suit();
    reset_plr_has_card();
    random_deck();

    mode_playing = false;
    emit sig_refresh_deck(true);

    heart_broken = false;
    if (passed_to == pNOPASS)
      play_2clubs();
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
  int card_id = AI_get_cpu_move();

  assert((card_id >= 0) && (card_id < 13));

  int card = plr_cards[turn][card_id];

  assert((card >= 0) && (card < DECK_SIZE));

  emit sig_play_card(card, turn);

  if (current_suit == FREESUIT)
    current_suit = card / 13;

  plr_cards[turn][card_id] = empty;
  process_card(card);
  sort_plr_cards();

  emit sig_refresh_deck(false);

  advance_turn();
}

void CHearts::advance_turn()
{
 bool tram = false;

 if (++hand_turn > FOURTH_CARD) {
   if (is_card_on_table(queen_spade))
     emit sig_got_queen_spade(plr_best_hand);

   reset_cards_on_table();

   turn = plr_best_hand;

   if (omnibus && (jack_diamond_played)) {
     jack_diamond_played = false;
     plr_jack_diamond = plr_best_hand;
   }

   plr_hand_score[turn] += hand_score;

   emit sig_hand_score(turn, plr_hand_score[turn]);

   hand_turn = FIRST_CARD;
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
   undo_data.undo_available = false;

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

  save_undo();

  if (current_suit == FREESUIT)
    current_suit = card / 13;

  plr_cards[user_id][idx] = empty;
  process_card(card);
  sort_plr_cards();

  emit sig_play_card(card, user_id);
  emit sig_refresh_deck(false);

  advance_turn();

  return retc;
}

void CHearts::play_2clubs()
{
 mode_playing = true;
 fresh_game = false;
 for (int i=0; i<4; i++) {
   if (plr_cards[i][0] == two_clubs) {
     turn = i;
     hand_turn = FIRST_CARD;
     current_suit = CLUB;
     if (i == user_id) {
       if (auto_start)
         play_card(two_clubs);
       else
         emit sig_your_turn(i);
     }
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

int CHearts::undo()
{
  if (!undo_data.undo_available || moon_wait)
    return 0;

  int num_cards = 0;

  undo_data.undo_available = false;

  // This part is unnecessary for now, but i'll keep it.
  if (passed_to != undo_data.passed_to) {
    passed_to = undo_data.passed_to;
    emit sig_pass_to(passed_to);
  }

  turn = PLAYER_SOUTH;
  game_over = false;
  mode_playing = true;

  heart_broken = undo_data.heart_broken;
  hand_score = undo_data.hand_score;
  best_hand = undo_data.best_hand;
  jack_diamond_played = undo_data.jack_diamond_played;
  moon_add_to_scores = undo_data.moon_add_to_scores;
  shoot_moon = undo_data.shoot_moon;
  hand_turn = undo_data.hand_turn;
  current_suit = undo_data.current_suit;
  card_left = undo_data.card_left;
  plr_best_hand = undo_data.plr_best_hand;
  plr_jack_diamond = undo_data.plr_jack_diamond;

  for (int i=0; i<13; i++) {
    plr_cards[PLAYER_SOUTH][i] = undo_data.plr_cards[PLAYER_SOUTH][i];
    plr_cards[PLAYER_WEST][i] = undo_data.plr_cards[PLAYER_WEST][i];
    plr_cards[PLAYER_NORTH][i] = undo_data.plr_cards[PLAYER_NORTH][i];
    plr_cards[PLAYER_EAST][i] = undo_data.plr_cards[PLAYER_EAST][i];
  }

  if (plr_score[PLAYER_SOUTH] != undo_data.plr_score[PLAYER_SOUTH]) {
    plr_score[PLAYER_SOUTH] = undo_data.plr_score[PLAYER_SOUTH];
    emit sig_score(PLAYER_SOUTH, plr_score[PLAYER_SOUTH]);
  }

  if (plr_score[PLAYER_WEST] != undo_data.plr_score[PLAYER_WEST]) {
    plr_score[PLAYER_WEST] = undo_data.plr_score[PLAYER_WEST];
    emit sig_score(PLAYER_WEST, plr_score[PLAYER_WEST]);
  }

  if (plr_score[PLAYER_NORTH] != undo_data.plr_score[PLAYER_NORTH]) {
    plr_score[PLAYER_NORTH] = undo_data.plr_score[PLAYER_NORTH];
    emit sig_score(PLAYER_NORTH, plr_score[PLAYER_NORTH]);
  }

  if (plr_score[PLAYER_EAST] != undo_data.plr_score[PLAYER_EAST]) {
    plr_score[PLAYER_EAST] = undo_data.plr_score[PLAYER_EAST];
    emit sig_score(PLAYER_EAST, plr_score[PLAYER_EAST]);
  }

  if (plr_hand_score[PLAYER_SOUTH] != undo_data.plr_hand_score[PLAYER_SOUTH]) {
    plr_hand_score[PLAYER_SOUTH] = undo_data.plr_hand_score[PLAYER_SOUTH];
    emit sig_hand_score(PLAYER_SOUTH, plr_hand_score[PLAYER_SOUTH]);
  }

  if (plr_hand_score[PLAYER_WEST] != undo_data.plr_hand_score[PLAYER_WEST]) {
    plr_hand_score[PLAYER_WEST] = undo_data.plr_hand_score[PLAYER_WEST];
    emit sig_hand_score(PLAYER_WEST, plr_hand_score[PLAYER_WEST]);
  }

  if (plr_hand_score[PLAYER_NORTH] != undo_data.plr_hand_score[PLAYER_NORTH]) {
    plr_hand_score[PLAYER_NORTH] = undo_data.plr_hand_score[PLAYER_NORTH];
    emit sig_hand_score(PLAYER_NORTH, plr_hand_score[PLAYER_NORTH]);
  }

  if (plr_hand_score[PLAYER_EAST] != undo_data.plr_hand_score[PLAYER_EAST]) {
    plr_hand_score[PLAYER_EAST] = undo_data.plr_hand_score[PLAYER_EAST];
    emit sig_hand_score(PLAYER_EAST, plr_hand_score[PLAYER_EAST]);
  }

  cpt_plr_cards[PLAYER_SOUTH] = undo_data.cpt_plr_cards[PLAYER_SOUTH];
  cpt_plr_cards[PLAYER_WEST] = undo_data.cpt_plr_cards[PLAYER_WEST];
  cpt_plr_cards[PLAYER_NORTH] = undo_data.cpt_plr_cards[PLAYER_NORTH];
  cpt_plr_cards[PLAYER_EAST] = undo_data.cpt_plr_cards[PLAYER_EAST];

  cards_left_in_suit[CLUB] = undo_data.cards_left_in_suit[CLUB];
  cards_left_in_suit[SPADE] = undo_data.cards_left_in_suit[SPADE];
  cards_left_in_suit[DIAMOND] = undo_data.cards_left_in_suit[DIAMOND];
  cards_left_in_suit[HEART] = undo_data.cards_left_in_suit[HEART];

  for (int i=0; i<DECK_SIZE; i++) {
    plr_has_card[PLAYER_SOUTH][i] = undo_data.plr_has_card[PLAYER_SOUTH][i];
    plr_has_card[PLAYER_WEST][i] = undo_data.plr_has_card[PLAYER_WEST][i];
    plr_has_card[PLAYER_NORTH][i] = undo_data.plr_has_card[PLAYER_NORTH][i];
    plr_has_card[PLAYER_EAST][i] = undo_data.plr_has_card[PLAYER_EAST][i];

    if (cards_played[i] != undo_data.cards_played[i]) {
       num_cards++;
       cards_played[i] = undo_data.cards_played[i];
    }
  }

  plr_cards_in_suit[PLAYER_SOUTH][CLUB] = undo_data.plr_cards_in_suit[PLAYER_SOUTH][CLUB];
  plr_cards_in_suit[PLAYER_SOUTH][SPADE] = undo_data.plr_cards_in_suit[PLAYER_SOUTH][SPADE];
  plr_cards_in_suit[PLAYER_SOUTH][DIAMOND] = undo_data.plr_cards_in_suit[PLAYER_SOUTH][DIAMOND];
  plr_cards_in_suit[PLAYER_SOUTH][HEART] = undo_data.plr_cards_in_suit[PLAYER_SOUTH][HEART];

  plr_cards_in_suit[PLAYER_WEST][CLUB] = undo_data.plr_cards_in_suit[PLAYER_WEST][CLUB];
  plr_cards_in_suit[PLAYER_WEST][SPADE] = undo_data.plr_cards_in_suit[PLAYER_WEST][SPADE];
  plr_cards_in_suit[PLAYER_WEST][DIAMOND] = undo_data.plr_cards_in_suit[PLAYER_WEST][DIAMOND];
  plr_cards_in_suit[PLAYER_WEST][HEART] = undo_data.plr_cards_in_suit[PLAYER_WEST][HEART];

  plr_cards_in_suit[PLAYER_NORTH][CLUB] = undo_data.plr_cards_in_suit[PLAYER_NORTH][CLUB];
  plr_cards_in_suit[PLAYER_NORTH][SPADE] = undo_data.plr_cards_in_suit[PLAYER_NORTH][SPADE];
  plr_cards_in_suit[PLAYER_NORTH][DIAMOND] = undo_data.plr_cards_in_suit[PLAYER_NORTH][DIAMOND];
  plr_cards_in_suit[PLAYER_NORTH][HEART] = undo_data.plr_cards_in_suit[PLAYER_NORTH][HEART];

  plr_cards_in_suit[PLAYER_EAST][CLUB] = undo_data.plr_cards_in_suit[PLAYER_EAST][CLUB];
  plr_cards_in_suit[PLAYER_EAST][SPADE] = undo_data.plr_cards_in_suit[PLAYER_EAST][SPADE];
  plr_cards_in_suit[PLAYER_EAST][DIAMOND] = undo_data.plr_cards_in_suit[PLAYER_EAST][DIAMOND];
  plr_cards_in_suit[PLAYER_EAST][HEART] = undo_data.plr_cards_in_suit[PLAYER_EAST][HEART];

  hand_cards[FIRST_CARD] = undo_data.hand_cards[FIRST_CARD];
  hand_cards[SECOND_CARD] = undo_data.hand_cards[SECOND_CARD];
  hand_cards[THIRD_CARD] = undo_data.hand_cards[THIRD_CARD];
  hand_cards[FOURTH_CARD] = undo_data.hand_cards[FOURTH_CARD];

  emit sig_refresh_deck(false);
  emit sig_clear_table();

  int cpt = 0;

  if (hand_cards[FIRST_CARD] != empty) cpt++;
  if (hand_cards[SECOND_CARD] != empty) cpt++;
  if (hand_cards[THIRD_CARD] != empty) cpt++;

  if (cpt == 3) {
    emit sig_play_card(hand_cards[FIRST_CARD], PLAYER_WEST);
    emit sig_play_card(hand_cards[SECOND_CARD], PLAYER_NORTH);
    emit sig_play_card(hand_cards[THIRD_CARD], PLAYER_EAST);
  } else
      if (cpt == 2) {
        emit sig_play_card(hand_cards[FIRST_CARD], PLAYER_NORTH);
        emit sig_play_card(hand_cards[SECOND_CARD], PLAYER_EAST);
      } else
          if (cpt == 1) {
            emit sig_play_card(hand_cards[FIRST_CARD], PLAYER_EAST);
          }

  emit sig_your_turn(PLAYER_SOUTH);

  return num_cards;
}

void CHearts::save_undo()
{
  undo_data.undo_available = true;

  undo_data.passed_to = passed_to;
  undo_data.heart_broken = heart_broken;
  undo_data.hand_score = hand_score;
  undo_data.best_hand = best_hand;
  undo_data.jack_diamond_played = jack_diamond_played;
  undo_data.moon_add_to_scores = moon_add_to_scores;
  undo_data.shoot_moon = shoot_moon;
  undo_data.hand_turn = hand_turn;
  undo_data.current_suit = current_suit;
  undo_data.card_left = card_left;
  undo_data.plr_best_hand = plr_best_hand;
  undo_data.plr_jack_diamond = plr_jack_diamond;

  for (int i=0; i<13; i++) {
    undo_data.plr_cards[PLAYER_SOUTH][i] = plr_cards[PLAYER_SOUTH][i];
    undo_data.plr_cards[PLAYER_WEST][i] = plr_cards[PLAYER_WEST][i];
    undo_data.plr_cards[PLAYER_NORTH][i] = plr_cards[PLAYER_NORTH][i];
    undo_data.plr_cards[PLAYER_EAST][i] = plr_cards[PLAYER_EAST][i];
  }

  undo_data.plr_score[PLAYER_SOUTH] = plr_score[PLAYER_SOUTH];
  undo_data.plr_score[PLAYER_WEST] = plr_score[PLAYER_WEST];
  undo_data.plr_score[PLAYER_NORTH] = plr_score[PLAYER_NORTH];
  undo_data.plr_score[PLAYER_EAST] = plr_score[PLAYER_EAST];

  undo_data.plr_hand_score[PLAYER_SOUTH] = plr_hand_score[PLAYER_SOUTH];
  undo_data.plr_hand_score[PLAYER_WEST] = plr_hand_score[PLAYER_WEST];
  undo_data.plr_hand_score[PLAYER_NORTH] = plr_hand_score[PLAYER_NORTH];
  undo_data.plr_hand_score[PLAYER_EAST] = plr_hand_score[PLAYER_EAST];

  undo_data.cpt_plr_cards[PLAYER_SOUTH] = cpt_plr_cards[PLAYER_SOUTH];
  undo_data.cpt_plr_cards[PLAYER_WEST] = cpt_plr_cards[PLAYER_WEST];
  undo_data.cpt_plr_cards[PLAYER_NORTH] = cpt_plr_cards[PLAYER_NORTH];
  undo_data.cpt_plr_cards[PLAYER_EAST] = cpt_plr_cards[PLAYER_EAST];

  undo_data.cards_left_in_suit[CLUB] = cards_left_in_suit[CLUB];
  undo_data.cards_left_in_suit[SPADE] = cards_left_in_suit[SPADE];
  undo_data.cards_left_in_suit[DIAMOND] = cards_left_in_suit[DIAMOND];
  undo_data.cards_left_in_suit[HEART] = cards_left_in_suit[HEART];

  for (int i=0; i<DECK_SIZE; i++) {
    undo_data.plr_has_card[PLAYER_SOUTH][i] = plr_has_card[PLAYER_SOUTH][i];
    undo_data.plr_has_card[PLAYER_WEST][i] = plr_has_card[PLAYER_WEST][i];
    undo_data.plr_has_card[PLAYER_NORTH][i] = plr_has_card[PLAYER_NORTH][i];
    undo_data.plr_has_card[PLAYER_EAST][i] = plr_has_card[PLAYER_EAST][i];

    undo_data.cards_played[i] = cards_played[i];
  }

  undo_data.plr_cards_in_suit[PLAYER_SOUTH][CLUB] = plr_cards_in_suit[PLAYER_SOUTH][CLUB];
  undo_data.plr_cards_in_suit[PLAYER_SOUTH][SPADE] = plr_cards_in_suit[PLAYER_SOUTH][SPADE];
  undo_data.plr_cards_in_suit[PLAYER_SOUTH][DIAMOND] = plr_cards_in_suit[PLAYER_SOUTH][DIAMOND];
  undo_data.plr_cards_in_suit[PLAYER_SOUTH][HEART] = plr_cards_in_suit[PLAYER_SOUTH][HEART];

  undo_data.plr_cards_in_suit[PLAYER_WEST][CLUB] = plr_cards_in_suit[PLAYER_WEST][CLUB];
  undo_data.plr_cards_in_suit[PLAYER_WEST][SPADE] = plr_cards_in_suit[PLAYER_WEST][SPADE];
  undo_data.plr_cards_in_suit[PLAYER_WEST][DIAMOND] = plr_cards_in_suit[PLAYER_WEST][DIAMOND];
  undo_data.plr_cards_in_suit[PLAYER_WEST][HEART] = plr_cards_in_suit[PLAYER_WEST][HEART];

  undo_data.plr_cards_in_suit[PLAYER_NORTH][CLUB] = plr_cards_in_suit[PLAYER_NORTH][CLUB];
  undo_data.plr_cards_in_suit[PLAYER_NORTH][SPADE] = plr_cards_in_suit[PLAYER_NORTH][SPADE];
  undo_data.plr_cards_in_suit[PLAYER_NORTH][DIAMOND] = plr_cards_in_suit[PLAYER_NORTH][DIAMOND];
  undo_data.plr_cards_in_suit[PLAYER_NORTH][HEART] = plr_cards_in_suit[PLAYER_NORTH][HEART];

  undo_data.plr_cards_in_suit[PLAYER_EAST][CLUB] = plr_cards_in_suit[PLAYER_EAST][CLUB];
  undo_data.plr_cards_in_suit[PLAYER_EAST][SPADE] = plr_cards_in_suit[PLAYER_EAST][SPADE];
  undo_data.plr_cards_in_suit[PLAYER_EAST][DIAMOND] = plr_cards_in_suit[PLAYER_EAST][DIAMOND];
  undo_data.plr_cards_in_suit[PLAYER_EAST][HEART] = plr_cards_in_suit[PLAYER_EAST][HEART];

  undo_data.hand_cards[FIRST_CARD] = hand_cards[FIRST_CARD];
  undo_data.hand_cards[SECOND_CARD] = hand_cards[SECOND_CARD];
  undo_data.hand_cards[THIRD_CARD] = hand_cards[THIRD_CARD];
  undo_data.hand_cards[FOURTH_CARD] = hand_cards[FOURTH_CARD];
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

bool CHearts::is_moon_an_option()
{
  if (!(AI_cpu_flags[turn] & AI_flags_try_moon))
    return false;

  if ((turn != PLAYER_SOUTH) && (plr_hand_score[PLAYER_SOUTH] || (omnibus && (plr_jack_diamond == PLAYER_SOUTH)))) return false;
  if ((turn != PLAYER_WEST) && (plr_hand_score[PLAYER_WEST] || (omnibus && (plr_jack_diamond == PLAYER_WEST)))) return false;
  if ((turn != PLAYER_NORTH) && (plr_hand_score[PLAYER_NORTH] || (omnibus && (plr_jack_diamond == PLAYER_NORTH)))) return false;
  if ((turn != PLAYER_EAST) && (plr_hand_score[PLAYER_EAST] || (omnibus && (plr_jack_diamond == PLAYER_EAST)))) return false;

  if (plr_cards_in_suit[turn][HEART] && !plr_has_card[turn][ace_heart] && !cards_played[ace_heart])
    return false;

  return true;
}

bool CHearts::is_prepass_to_moon()
{
  const int aces[4] = {ace_clubs, ace_spade, ace_diamond, ace_heart};

  int bonus = 0;
  int discard = 0;

  for (int i=0; i<4; i++) {
     int cpt_suit = plr_cards_in_suit[turn][i];

     if (cpt_suit) {
       if (cpt_suit > 8)
         bonus = 2;
       else
       if (cpt_suit > 6)
         bonus = 1;

       if (!plr_has_card[turn][aces[i]] && ((i == HEART) || ((cpt_suit >= 3) && (cpt_suit <= 6))))
         discard += cpt_suit;
     }

     if (discard > 3)
       return false;
  }

  int card_value;
  int low1 = 65535;
  int low2 = 65535;
  int low3 = 65535;

  double total = 0;
  for (int i=0; i<13; i++) {
    card_value = (plr_cards[turn][i] % 13) + 2;

    if (card_value < low1) {
      if (low1 < low2) {
        if (low2 < low3)
          low3 = low2;
        low2 = low1;
      }
      low1 = card_value;
    }
    else
    if (card_value < low2) {
      if (low2 < low3)
        low3 = low2;
      low2 = card_value;
    }
    else
    if (card_value < low3)
      low3 = card_value;

    total += card_value;
  }

  total = (total - low1 - low2 - low3) / 10 + bonus;

  if (total < 10)                                     // although, some hands with a total of 7 are promising and
    return false;                                     // some with total of 11 aren't, it's usually better to
                                                      // have total above 10.

  return true;
}

bool CHearts::is_it_draw()
{
  if (!game_over)                                     // if a game isn't over, well it's not a draw
    return false;                                     // even if the 2 lowests scores are the same.

  int lowest = plr_score[0];

  int cpt = 0;
  for (int i=1; i<4; i++) {
     if (plr_score[i] < lowest) {
       lowest = plr_score[i];
       cpt = 0;
     }
     else
       if (plr_score[i] == lowest)
         cpt++;
  }

  return cpt;
}

bool CHearts::is_card_selectable(int card)
{
  if (!mode_playing)
    return true;

  if (turn != user_id)
    return false;

  return (check_invalid_move(user_id, card) == NOERROR);
}

bool CHearts::can_break_heart(int plr)
{
  if (is_only_heart_left(plr))
    return true;

  if (current_suit == FREESUIT)
    return false;

  if (card_left > DECK_SIZE - 4)
    return false;

  return !plr_cards_in_suit[plr][current_suit];
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

     if (card == empty) return highest;
     if (card_suit > suit) return highest;
     if (card_suit == suit)
       highest = i;
  }
  return highest;
}

int CHearts::get_highest_suit(int plr, int suit)
{
  int highest = NOT_FOUND;

  for (int i=0; i<13; i++) {
    int card = plr_cards[plr][i];
    int card_suit = card /13;

    if (card == empty) return highest;
    if (card_suit > suit) return highest;
    if (card_suit == suit)
      highest = card;
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
  }
  return false;
}

bool CHearts::is_card_on_table(int card)
{
// This function is always called after ++hand_turn, so
// instead of doing e.g: if (hand_turn == FOURTH_CARD + 1)
// I'll let the numeral check 4, 3, 2
  if ((hand_turn == 4) && (hand_cards[FOURTH_CARD] == card)) return true;
  if ((hand_turn >= 3) && (hand_cards[THIRD_CARD] == card)) return true;
  if ((hand_turn >= 2) && (hand_cards[SECOND_CARD] == card)) return true;
// if (hand_turn >= 1) && (hand_cards[FIRST_CARD] == card)) return true;
     if (hand_cards[FIRST_CARD] == card) return true;

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

  if ((card_left == DECK_SIZE) && (card != two_clubs))
    return ERR2CLUBS;

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

int CHearts::get_plr_name_id(int plr)
{
 assert((plr_name_id[plr] >= -1) && (plr_name_id[plr] < MAX_PLR_NAMES));

 return plr_name_id[plr];
}

int CHearts::get_plr_hand_card(int plr)
{
 return hand_cards[plr];
}

int CHearts::get_highest_card_table()
{
  int highest = hand_cards[FIRST_CARD];

  if (hand_turn == FOURTH_CARD) {
    if ((hand_cards[THIRD_CARD] / 13 == current_suit) && (hand_cards[THIRD_CARD] > highest))
      highest = hand_cards[THIRD_CARD];
  }

  if (hand_turn >= THIRD_CARD) {
    if ((hand_cards[SECOND_CARD] / 13 == current_suit) && (hand_cards[SECOND_CARD] > highest))
      highest = hand_cards[SECOND_CARD];
  }

  // if (hand_turn == SECOND_CARD) highest = hand_cards[FIRST_CARD];
  // if (hand_turn == FOURTH_CARD) doesn't apply, we already move to next round.
  return highest;
}

int CHearts::get_lowest_score()
{
  int lowest_score = plr_score[0];

  for (int i=1; i<4; i++)
    if (plr_score[i] < lowest_score)
      lowest_score = plr_score[i];

  return lowest_score;
}

int CHearts::get_highest_score()
{
  int highest_score = plr_score[0];

  for (int i=1; i<4; i++)
    if (plr_score[i] > highest_score)
      highest_score = plr_score[i];

  return highest_score;
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

bool CHearts::is_card_played(int card)
{
  return cards_played[card];
}

int CHearts::get_current_suit()
{
  return current_suit;
}

int CHearts::get_my_score()
{
  return plr_score[user_id];
}

int CHearts::get_turn()
{
  return turn;
}

int CHearts::get_card(int plr, int idx)
{
  return plr_cards[plr][idx];
}

int CHearts::get_card(int idx)
{
  return plr_cards[user_id][idx];
}

int CHearts::get_plr_num_cards(int plr)
{
  return cpt_plr_cards[plr];
}

int CHearts::is_fresh_game()
{
  return fresh_game;
}

int CHearts::get_plr_pass_to(int plr_from)
{
  const int plr_pass_to[4][4] = {{PLAYER_WEST, PLAYER_NORTH, PLAYER_EAST, PLAYER_SOUTH},   // pass to the left.
                                 {PLAYER_EAST, PLAYER_SOUTH, PLAYER_WEST, PLAYER_NORTH},   // pass to the right.
                                 {PLAYER_NORTH, PLAYER_EAST, PLAYER_SOUTH, PLAYER_WEST},   // pass accross.
                                 {PLAYER_SOUTH, PLAYER_WEST, PLAYER_NORTH, PLAYER_EAST}};  // no pass.

  return plr_pass_to[passed_to][plr_from];
}

void CHearts::AI_set_cpu_flags(int cpu, int flags)
{
  AI_cpu_flags[cpu] = flags;
}

void CHearts::set_moon_add_to_score(bool enable)
{
  moon_add_to_scores = enable;
  moon_wait = false;
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

void CHearts::set_auto_start(bool enable)
{
  auto_start = enable;
  if (auto_start && is_starting())
    play_card(two_clubs);
}

bool CHearts::is_starting()
{
  if (mode_playing && (card_left == DECK_SIZE))
    return true;
  else
    return false;
}

bool CHearts::is_undo_available()
{
  return undo_data.undo_available;
}

bool CHearts::is_moon_wait()
{
  return moon_wait;
}

bool CHearts::get_cpu_moon_add(int plr)
{
  if (plr_score[PLAYER_SOUTH] + 26 < plr_score[plr]) return false;
  if ((plr != PLAYER_WEST) && (plr_score[PLAYER_WEST] + 26 < plr_score[plr])) return false;
  if ((plr != PLAYER_NORTH) && (plr_score[PLAYER_NORTH] + 26 < plr_score[plr])) return false;
  if ((plr != PLAYER_EAST) && (plr_score[PLAYER_EAST] + 26 < plr_score[plr])) return false;

  return true;
}
