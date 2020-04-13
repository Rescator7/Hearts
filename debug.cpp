#include "debug.h"

#ifdef DEBUG

#include "assert.h"

#include <QString>
#include <QPalette>
#include <QColor>
#include <QGroupBox>

const char PLR_NOBODY[7] = "Nobody";

CDebug::CDebug(QImage *_img_empty)
{
 setWindowTitle(tr("Cards history"));

 setAttribute( Qt::WA_QuitOnClose, false );

 img_empty = _img_empty;

 box1 = new QHBoxLayout;

 for (int i=0; i<7; i++) {
   labels[i] = new QLabel(this);
   labels[i]->resize(80, 100);
   box1->addWidget(labels[i]);
 }

 box2 = new QHBoxLayout;

 for (int i=7; i<14; i++) {
   labels[i] = new QLabel(this);
   box2->addWidget(labels[i]);
 }

 QGroupBox *grp1 = new QGroupBox;
 grp1->setLayout(box1);

 QGroupBox *grp2 = new QGroupBox;
 grp2->setLayout(box2);

 bar = new QScrollBar(Qt::Horizontal, this);
 bar->setMaximum(0);

 connect(bar, SIGNAL(valueChanged(int)), this, SLOT(handle_bar(int)));

 layout = new QVBoxLayout();
 layout->addWidget(grp1);
 layout->addWidget(grp2);
 layout->addWidget(bar);

 setLayout(layout);

 QPalette pal = palette();
 pal.setColor(QPalette::Background, QColor(63, 159, 82));
 pal.setColor(QPalette::WindowText, Qt::black);
 setPalette(pal);

 reset();

 resize(600, 200);
}

CDebug::~CDebug()
{
 for (int i=0; i<14; i++)
   delete labels[i];

 delete bar;
 delete box1;
 delete box2;
 delete layout;
}

void CDebug::show_history(int slide)
{
 for (int i=0; i<7; i++) {
   labels[i]->setPixmap(QPixmap::fromImage(img_stack[i + ptr_screen - slide]->scaledToHeight(100)));
   labels[i+7]->setText(plr_names[i + ptr_screen - slide]);
 }
}

void CDebug::save_card(const char *name, QImage *img)
{
  assert(img);

  if (history_size >= MAX_HISTORY_SIZE)
    reset();

  img_stack[history_size] = img;
  plr_names[history_size] = name ? name : reinterpret_cast<const char *>(&PLR_NOBODY);
  history_size++;

  if (history_size >= 7) {
    bar->setMaximum(history_size - 7);
    bar->setValue(history_size - 7);

    ptr_screen = history_size - 7;
  } else
      ptr_screen = 0;

  show_history(0);
}

void CDebug::reset()
{
 history_size = 0;
 ptr_screen = 0;

 bar->setMaximum(0);
 bar->setValue(0);

 // show 7 empty cards
 for (int i=0; i<7; i++) {
   labels[i]->setPixmap(QPixmap::fromImage(img_empty->scaledToHeight(100)));
   labels[i+7]->setText("Nobody");
 }

 // show 7 Nobody as names
 for (int i=0; i<MAX_HISTORY_SIZE; i++) {
   img_stack[i] = img_empty;
   plr_names[i] = reinterpret_cast<const char *>(&PLR_NOBODY);
 }
}

// This function is used only for saved games, reversing the order of the hands cards
// played.
void CDebug::reverse_order()
{
 int ptr;

 if (history_size == 2)
   ptr = 1;
 else
 if (history_size == 3)
   ptr = 2;
 else
   return;

 QImage *first_img = img_stack[0];
 const char *first_name = plr_names[0];

 labels[0]->setPixmap(QPixmap::fromImage(img_stack[ptr]->scaledToHeight(100)));
 labels[ptr]->setPixmap(QPixmap::fromImage(img_stack[0]->scaledToHeight(100)));

 img_stack[0] = img_stack[ptr];
 img_stack[ptr] = first_img;

// labels[7]->setText(names[plr_stack[ptr]]);
 labels[7]->setText(plr_names[ptr]);
 labels[7+ptr]->setText(first_name);

 plr_names[0] = plr_names[ptr];
 plr_names[ptr] = first_name;
}

void CDebug::handle_bar(int value)
{
 show_history(bar->maximum() - value);
}

#endif
