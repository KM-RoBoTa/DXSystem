#ifndef MULTIREADWIDGET_H
#define MULTIREADWIDGET_H

#include <QWidget>

namespace Ui {
class MultiReadWidget;
}

class MultiReadWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MultiReadWidget(QWidget *parent = 0);
    ~MultiReadWidget();

private:
    Ui::MultiReadWidget *ui;
};

#endif // MULTIREADWIDGET_H
