#ifndef DEFINE
#define DEFINE

const char version[80] = "Hearts 1.1.1b";

const int FNOERR                         = 0;
const int ERROPENRO                      = 1;
const int ERROPENWO                      = 2;
const int FCORRUPTED                     = 3;

const int MAX_PLR_NAMES                  = 44;

const char names[MAX_PLR_NAMES][10] = {"You", "Aina", "Airi", "Alex", "Charles", "Christian", "Christine", "Cindy",
                                       "Danny", "David", "Denis", "Elena", "Erica", "Gabriel", "Grace", "Karine",
                                       "Karl", "Jason", "Jennifer", "John", "Linda", "Mai", "Maimi", "Marc", "Mary",
                                       "Masaki", "Michael", "Michelle", "Myriam", "Nadia", "Patricia", "Paul", "Reina",
                                       "Rick", "Riho", "Robert", "Sam", "Sandy", "Sayuki", "Sayumi", "Sara", "Sonia",
                                       "Sophie", "Tomoko"};

#define USE_LIBALLEGRO5

#endif // DEFINE

