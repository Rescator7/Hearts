#include "clabel.h"

CLabel::CLabel(QWidget* parent, Qt::WindowFlags f)
    : QLabel(parent) {
    if (f) {} // do nothing, and remove unused parameter warning
}

CLabel::~CLabel() {}

void CLabel::mousePressEvent(QMouseEvent* event) {
    emit clicked();
    if (event) {} // do nothing, and remove unused parameter warning
}

