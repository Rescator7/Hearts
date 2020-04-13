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

private:
    Ui::Connect *ui;

public slots:
    void diagButtonClicked(QAbstractButton *button);
};

#endif // CONNECT_H
