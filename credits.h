#ifndef CREDITS_H
#define CREDITS_H

#include <QDialog>

namespace Ui {
class Credits;
}

class Credits : public QDialog
{
    Q_OBJECT

public:
    explicit Credits(QWidget *parent = 0);
    ~Credits();

private:
    Ui::Credits *ui;
};

#endif // CREDITS_H
