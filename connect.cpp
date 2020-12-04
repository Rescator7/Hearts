#ifdef ONLINE_PLAY

#include "connect.h"
#include "ui_connect.h"
#include <QMessageBox>

Connect::Connect(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Connect)
{
  warning_accepted = false;
  warning_disabled = false;

  ui->setupUi(this);
  ui->buttonBox->addButton(QString("Register"), QDialogButtonBox::YesRole);
  ui->buttonBox->addButton(QString("Connect"), QDialogButtonBox::ActionRole);

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

void Connect::config(QString &username, QString &password, bool warning)
{
  ui->lineEdit_3->setText(username);
  ui->lineEdit_4->setText(password);

  if (!warning) {
    warning_disabled = true;
    ui->checkBox->hide();
  }
}

void Connect::diagButtonClicked(QAbstractButton* button)
{
  // QDialog return only: Accepted or Rejected. This hack allow me to return a 3rd value (3=register).
  // This must done after close(), as close would set Result as Rejected.

  if (!warning_disabled && ((button->text() == "Register") || (button->text() == "Connect"))) {
    QMessageBox msgBox(this);
    msgBox.setWindowTitle(tr("Warning!"));
    msgBox.setText(tr("The password will be sent unencrypted over the internet.\r\nIt is also saved unencrypted in ~/.hearts\r\n\r\n"
                      "DON'T reuse this password !\r\n\r\nDo you want to continue ?"));
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    if (msgBox.exec() == QMessageBox::Yes)
      warning_accepted = true;
  }

  if (button->text() == "Connect") {
    close();
    setResult(3);
  }
}

void Connect::on_checkBox_clicked()
{
  warning_disabled = true;
  ui->checkBox->hide();
}

bool Connect::WarningAccepted()
{
  return warning_disabled ? true : warning_accepted;
}

bool Connect::WarningDisabled()
{
  return warning_disabled;
}

#endif // ONLINE_PLAY
