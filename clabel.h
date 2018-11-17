#ifndef CLABEL_H
#define CLABEL_H

#include <QLabel>
#include <QWidget>
#include <Qt>

class CLabel : public QLabel {
    Q_OBJECT

public:
    explicit CLabel(QWidget* parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());
    ~CLabel();

signals:
    void clicked();

protected:
    void mousePressEvent(QMouseEvent* event);

};

#endif // CLABEL_H
