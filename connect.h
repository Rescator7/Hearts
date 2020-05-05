#ifndef CONNECT_H
#define CONNECT_H

#include <QDialog>
#include <QAbstractButton>

namespace Ui {
class Connect;
}

class Connect : public QDialog
{
    Q_OBJECT

public:
    explicit Connect(QWidget *parent = nullptr);
    ~Connect();

    QString getHost();
    QString getPort();
    QString getHandle();
    QString getPassword();

    void config(QString &username, QString &password, bool warning);
    bool WarningAccepted();
    bool WarningDisabled();

private:
    Ui::Connect *ui;
    bool warning_accepted;
    bool warning_disabled;

private slots:
    void on_checkBox_clicked();

public slots:
    void diagButtonClicked(QAbstractButton *button);
};

#endif // CONNECT_H