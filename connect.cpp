#include "connect.h"
#include "ui_connect.h"

Connect::Connect(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Connect)
{
  ui->setupUi(this);
  ui->buttonBox->addButton(QString("Register"), QDialogButtonBox::ActionRole);
  ui->buttonBox->addButton(QString("Connect"), QDialogButtonBox::YesRole);

  connect(ui->buttonBox, SIGNAL(clicked(QAbstractButton*)), SLOT(diagButtonClicked(QAbstractButton*)));
}

Connect::~Connect()
{
  delete ui;
}

QString Connect::getHost()
{
  return ui->lineEdit->text();
}

QString Connect::getPort()
{
  return ui->lineEdit_2->text();
}

QString Connect::getHandle()
{
  return ui->lineEdit_3->text();
}

QString Connect::getPassword()
{
  return ui->lineEdit_4->text();
}

void Connect::diagButtonClicked(QAbstractButton* button)
{
    // QDialog return only: Accepted or Rejected. This hack allow me to return a 3rd value (3=register). This must done after close(), as close would set
    // Result as Rejected.
    if (button->text() == "Register") {
      close();
      setResult(3);
    }
}
