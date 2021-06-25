#include "widgetsound.h"
#include "ui_widgetsound.h"

#include "scene.h"

WidgetSound::WidgetSound(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetSound)
{
    ui->setupUi(this);
}

WidgetSound::~WidgetSound()
{
    delete ui;
}

void WidgetSound::readSoundData()
{
    ui->comboBox->addItem(mCurrentScene->mEntities[indexInSceneArray].mSound.mName.c_str());
}
