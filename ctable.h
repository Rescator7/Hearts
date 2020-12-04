#ifdef ONLINE_PLAY

#ifndef CTABLE_H
#define CTABLE_H

#include <QWidget>

namespace Ui {
class CTable;
}

class CTable : public QWidget
{
    Q_OBJECT

public:
    explicit CTable(QWidget *parent = nullptr);
    ~CTable();

private:
    Ui::CTable *ui;

public:
    void AddRow(QString id, QString flags);
    void Empty();
    void RemoveRow(QString id);
    void SetPlayer(QString id, QString name, QString chair);
    void Translate();

signals:
    void clicked(int id, char chair);

private slots:
    void on_tableWidget_cellDoubleClicked(int row, int column);
};

#endif // CTABLE_H

#endif // ONLINE_PLAY
