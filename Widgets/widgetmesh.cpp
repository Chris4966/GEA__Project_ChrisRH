#include "widgetmesh.h"
#include "ui_widgetmesh.h"

#include "scene.h"
#include "visualobject.h"

WidgetMesh::WidgetMesh(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetMesh)
{
    ui->setupUi(this);
}

WidgetMesh::~WidgetMesh()
{
    delete ui;
}

void WidgetMesh::readMeshData()
{
    ui->comboBox->addItem(mCurrentScene->mEntities.at(indexInSceneArray).mName.c_str());
    ui->VAOlabel->setText(QString::number(mCurrentScene->mEntities[indexInSceneArray].mMesh.mVAO));
    int temp = mCurrentScene->mEntities[indexInSceneArray].mMesh.mVertexCount;
    ui->VerticesLabel->setText(QString::number(temp));
    ui->IndicesLabel->setText(QString::number(mCurrentScene->mEntities[indexInSceneArray].mMesh.mIndexCount));
}
