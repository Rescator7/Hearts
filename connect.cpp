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
  UnRegistered = true;

  ui->setupUi(this);

  button_Register = new QPushButton(tr("Register"));
  ui->buttonBox->addButton(button_Register, QDialogButtonBox::AcceptRole);

  button_Connect = new QPushButton(tr("Connect 2"));
  button_Connect->setDefault(true);

  ui->buttonBox->addButton(button_Connect, QDialogButtonBox::AcceptRole);

  button_Cancel = new QPushButton(tr("Cancel"));
  ui->buttonBox->addButton(button_Cancel, QDialogButtonBox::RejectRole);

  connect(ui->buttonBox, SIGNAL(clicked(QAbstractButton*)), SLOT(diagButtonClicked(QAbstractButton*)));
}

Connect::~Connect()
{
  delete ui;
  delete button_Register;
  delete button_Connect;
  delete button_Cancel;
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
  if (!warning_disabled && ((button == button_Register) || (button == button_Connect))) {
    QMessageBox msgBox(this);
    msgBox.setWindowTitle(tr("Warning!"));
    msgBox.setText(tr("The password will be sent unencrypted over the internet.\r\nIt is also saved unencrypted in ~/.hearts\r\n\r\n"
                      "DON'T reuse this password !\r\n\r\nDo you want to continue ?"));
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    if (msgBox.exec() == QMessageBox::Yes)
      warning_accepted = true;
  }

  if (button == button_Connect)
    UnRegistered = false;
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

bool Connect::isUnRegistered()
{
  return UnRegistered;
}

#endif // ONLINE_PLAY
