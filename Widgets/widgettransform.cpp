#include "widgettransform.h"
#include "ui_widgettransform.h"
#include <QTreeWidget>
#include <QKeyEvent>

#include "scene.h"
#include "visualobject.h"
#include "gslpch.h"
#include "renderwindow.h"

WidgetTransform::WidgetTransform(QWidget *parent) :
    QWidget(parent), ui(new Ui::WidgetTransform)
{
    ui->setupUi(this);
    setStepRates();
    setFocusPolicy(Qt::NoFocus);
}

WidgetTransform::~WidgetTransform()
{
    delete ui;
}

void WidgetTransform::readPosition()
{
    gsl::vec3 position = mCurrentScene->mEntities[indexInSceneArray].mTransform.position;
    ui->doubleSpinBoxXPosition->setValue(position.x);
    ui->doubleSpinBoxYPosition->setValue(position.y);
    ui->doubleSpinBoxZPosition->setValue(position.z);
}

void WidgetTransform::readRotation()
{
    gsl::vec3 rotation = mCurrentScene->mEntities[indexInSceneArray].mTransform.rotation;
    ui->doubleSpinBoxXRotation->setValue(rotation.x);
    ui->doubleSpinBoxYRotation->setValue(rotation.y);
    ui->doubleSpinBoxZRotation->setValue(rotation.z);
}

void WidgetTransform::readScale()
{
    gsl::vec3 scale = mCurrentScene->mEntities[indexInSceneArray].mTransform.scale;
    ui->doubleSpinBoxXScale->setValue(scale.x);
    ui->doubleSpinBoxYScale->setValue(scale.y);
    ui->doubleSpinBoxZScale->setValue(scale.z);
}

void WidgetTransform::setPosition()
{
    gsl::vec3 position;
    position.x = ui->doubleSpinBoxXPosition->value();
    position.y = ui->doubleSpinBoxYPosition->value();
    position.z = ui->doubleSpinBoxZPosition->value();

    mCurrentScene->mEntities[indexInSceneArray].mTransform.position = position;
}

void WidgetTransform::setStepRates()
{
    ui->doubleSpinBoxXPosition->setSingleStep(mPositionStep);
    ui->doubleSpinBoxYPosition->setSingleStep(mPositionStep);
    ui->doubleSpinBoxZPosition->setSingleStep(mPositionStep);
    ui->doubleSpinBoxXScale->setSingleStep(mScaleStep);
    ui->doubleSpinBoxYScale->setSingleStep(mScaleStep);
    ui->doubleSpinBoxZScale->setSingleStep(mScaleStep);
    ui->doubleSpinBoxXRotation->setSingleStep(mRotationStep);
    ui->doubleSpinBoxYRotation->setSingleStep(mRotationStep);
    ui->doubleSpinBoxZRotation->setSingleStep(mRotationStep);
}

void WidgetTransform::on_doubleSpinBoxXPosition_valueChanged(double arg1)
{
    mCurrentScene->mEntities[indexInSceneArray].mTransform.position.x = arg1;
}

void WidgetTransform::on_doubleSpinBoxYPosition_valueChanged(double arg1)
{
    mCurrentScene->mEntities[indexInSceneArray].mTransform.position.y = arg1;
}

void WidgetTransform::on_doubleSpinBoxZPosition_valueChanged(double arg1)
{
    mCurrentScene->mEntities[indexInSceneArray].mTransform.position.z = arg1;
}

void WidgetTransform::on_doubleSpinBoxXScale_valueChanged(double arg1)
{
    mCurrentScene->mEntities[indexInSceneArray].mTransform.scale.x = arg1;
}

void WidgetTransform::on_doubleSpinBoxYScale_valueChanged(double arg1)
{
    mCurrentScene->mEntities[indexInSceneArray].mTransform.scale.y = arg1;
}

void WidgetTransform::on_doubleSpinBoxZScale_valueChanged(double arg1)
{
    mCurrentScene->mEntities[indexInSceneArray].mTransform.scale.z = arg1;
}

void WidgetTransform::on_doubleSpinBoxXRotation_valueChanged(double arg1)
{
    if(arg1 <= -360.0 || arg1 >= 360.0)
    {
        arg1 = 0.0;
        ui->doubleSpinBoxXRotation->setValue(arg1);
    }
    mCurrentScene->mEntities[indexInSceneArray].mTransform.rotation.x = arg1;
}

void WidgetTransform::on_doubleSpinBoxYRotation_valueChanged(double arg1)
{
    if(arg1 <= -360.0 || arg1 >= 360.0)
    {
        arg1 = 0.0;
        ui->doubleSpinBoxYRotation->setValue(arg1);
    }
    mCurrentScene->mEntities[indexInSceneArray].mTransform.rotation.y = arg1;
}

void WidgetTransform::on_doubleSpinBoxZRotation_valueChanged(double arg1)
{
    if(arg1 <= -360.0 || arg1 >= 360.0)
    {
        arg1 = 0.0;
        ui->doubleSpinBoxZRotation->setValue(arg1);
    }
    mCurrentScene->mEntities[indexInSceneArray].mTransform.rotation.z = arg1;
}

void WidgetTransform::keyPressEvent(QKeyEvent *event)
{
    mCurrentScene->mRenderWindow->keyPressEvent(event);
}

void WidgetTransform::keyReleaseEvent(QKeyEvent *event)
{
    mCurrentScene->mRenderWindow->keyReleaseEvent(event);
}
