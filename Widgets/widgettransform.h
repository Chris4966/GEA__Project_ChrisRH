#ifndef WIDGETTRANSFORM_H
#define WIDGETTRANSFORM_H

#include <QWidget>

#include "gslpch.h"

namespace Ui {
    class WidgetTransform;
}

class WidgetTransform : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetTransform(class QWidget *parent = nullptr);
    ~WidgetTransform();

    void readPosition();
    void readRotation();
    void readScale();
    void setPosition();

    void setStepRates();
    int indexInSceneArray{-1};
    class Scene* mCurrentScene{nullptr};

    float mPositionStep{1.f};
    float mRotationStep{1.f};
    float mScaleStep{0.1f};

private slots:
    void on_doubleSpinBoxXPosition_valueChanged(double arg1);

    void on_doubleSpinBoxYPosition_valueChanged(double arg1);

    void on_doubleSpinBoxZPosition_valueChanged(double arg1);

    void on_doubleSpinBoxXScale_valueChanged(double arg1);

    void on_doubleSpinBoxYScale_valueChanged(double arg1);

    void on_doubleSpinBoxZScale_valueChanged(double arg1);

    void on_doubleSpinBoxXRotation_valueChanged(double arg1);

    void on_doubleSpinBoxYRotation_valueChanged(double arg1);

    void on_doubleSpinBoxZRotation_valueChanged(double arg1);

private:
    Ui::WidgetTransform *ui;

protected:
//    void focusInEvent(class QFocusEvent *event) override;

//    void focusOutEvent(QFocusEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
};

#endif // WIDGETTRANSFORM_H
