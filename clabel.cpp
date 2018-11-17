#include "clabel.h"

CLabel::CLabel(QWidget* parent, Qt::WindowFlags f)
    : QLabel(parent) {

}

CLabel::~CLabel() {}

void CLabel::mousePressEvent(QMouseEvent* event) {
    emit clicked();
}

