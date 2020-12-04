#ifdef ONLINE_PLAY

#include "online.h"
#include "ui_online.h"

online::online(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::online)
{
    ui->setupUi(this);
}

online::~online()
{
    delete ui;
}

#endif // ONLINE_PLAY
