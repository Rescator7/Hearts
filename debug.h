#ifndef DEBUG_H
#define DEBUG_H

#include "define.h"

#ifdef DEBUG
#include <QWidget>
#include <QLabel>
#include <QScrollBar>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QImage>

const int MAX_HISTORY_SIZE = 1000;

class CDebug : public QWidget
{
    Q_OBJECT

private: // variables;
    QVBoxLayout *layout;
    QHBoxLayout *box1;
    QHBoxLayout *box2;
    QScrollBar *bar;

    QLabel *labels[14];
    QImage *img_stack[MAX_HISTORY_SIZE];
    QImage *img_empty;

    const char *plr_names[MAX_HISTORY_SIZE];

    int ptr_screen;
    int history_size;

private: // functions
    void show_history(int slide);

public:
    CDebug(QImage *_img_empty);
    ~CDebug();

    void save_card(const char *name, QImage *img);
    void reverse_order();
    void reset();

public slots:
    void handle_bar(int value);
};
#endif // DEBUG

#endif // DEBUG_H
