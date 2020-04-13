#ifndef RULES_H
#define RULES_H

#include <QDialog>

namespace Ui {
class Rules;
}

class Rules : public QDialog
{
    Q_OBJECT

public:
    explicit Rules(QWidget *parent = nullptr);
    ~Rules();

private:
    Ui::Rules *ui;
};

#endif // RULES_H
