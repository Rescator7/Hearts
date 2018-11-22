#ifndef DEFINE
#define DEFINE

const char version[80] = "Hearts 1.0b";

const int FNOERR                         = 0;
const int ERROPENRO                      = 1;
const int ERROPENWO                      = 2;
const int FCORRUPTED                     = 3;

const int MAX_PLR_NAMES                  = 43;

const char names[MAX_PLR_NAMES][10] = {"Aina", "Airi", "Alex", "Charles", "Christian", "Christine", "Cindy",
                                       "David", "Danny", "Denis", "Elena", "Erica", "Gabriel", "Grace",
                                       "Karine", "Karl", "Jason", "John", "Jennifer", "Linda", "Mai",
                                       "Maimi", "Patricia", "Paul", "Marc", "Mary", "Masaki", "Michael",
                                       "Michelle", "Myriam", "Nadia", "Reina", "Rick", "Riho", "Robert",
                                       "Sam", "Sandy", "Sayuki", "Sayumi", "Sara", "Sophie", "Sonia",
                                       "Tomoko"};
#define USE_LIBALLEGRO5

#endif // DEFINE

