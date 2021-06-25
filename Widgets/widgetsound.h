#ifndef WIDGETSOUND_H
#define WIDGETSOUND_H

#include <QWidget>

namespace Ui {
class WidgetSound;
}

class WidgetSound : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetSound(QWidget *parent = nullptr);
    ~WidgetSound();

    void readSoundData();

    int indexInSceneArray{-1};
    class Scene *mCurrentScene{nullptr};

private:
    Ui::WidgetSound *ui;
};

#endif // WIDGETSOUND_H
