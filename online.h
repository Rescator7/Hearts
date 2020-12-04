#ifdef ONLINE_PLAY

#ifndef ONLINE_H
#define ONLINE_H

#include <QDialog>

namespace Ui {
class online;
}

class online : public QDialog
{
    Q_OBJECT

public:
    explicit online(QWidget *parent = 0);
    ~online();

private:
    Ui::online *ui;
};

#endif // ONLINE_H

#endif // ONLINE_PLAY
