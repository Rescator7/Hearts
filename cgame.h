#ifdef ONLINE_PLAY

#ifndef CGAME_H
#define CGAME_H

#include <QDialog>

namespace Ui {
class CGame;
}

class CGame : public QDialog
{
    Q_OBJECT

public:
    explicit CGame(QWidget *parent = nullptr);
    ~CGame();

private:
    Ui::CGame *ui;
    unsigned int flags;

public:
    void set_flags(unsigned int f);
    unsigned int get_flags();
private slots:
    void on_buttonBox_accepted();
};

#endif // CGAME_H

#endif // ONLINE_PLAY
