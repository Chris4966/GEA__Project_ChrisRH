#include "widgetmaterial.h"
#include "ui_widgetmaterial.h"

#include <QColorDialog>

#include "scene.h"
#include "renderwindow.h"
#include "visualobject.h"
#include "texture.h"
#include "gslpch.h"
#include "resourcemanager.h"
#include "ecs/components.h"

WidgetMaterial::WidgetMaterial(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetMaterial)
{
    ui->setupUi(this);
}

WidgetMaterial::~WidgetMaterial()
{
    delete ui;
}

void WidgetMaterial::readMaterialData()
{
    unsigned short tempID = mCurrentScene->mEntities[indexInSceneArray].mMaterialIDIndex;
    mCurrentMaterial = ResourceManager::getInstance().mMaterial[tempID];
    qDebug() << "Shader" << mCurrentMaterial->mShader;
    fillShaders();
    ui->shaderComboBox->setCurrentIndex(mCurrentMaterial->mShader);
    fillMaterials();
    ui->materialComboBox->setCurrentIndex(mCurrentMaterial->mMaterialID);
    fillTextures();
    ui->textureComboBox->setCurrentIndex(mCurrentMaterial->mTextureId);

    QPalette pal;
    QColor tempColor(mCurrentMaterial->mColor.x*255, mCurrentMaterial->mColor.y*255, mCurrentMaterial->mColor.z*255);
    pal.setColor(QPalette::Window, tempColor);
    ui->colorLabel->setPalette(pal);

    ui->specularStrength->setValue(mCurrentMaterial->mSpecularStrenght);
    ui->specularExponent->setValue(mCurrentMaterial->mSpecularExponent);
}

void WidgetMaterial::fillShaders()
{
    for (int var = 0; var < gsl::Shaders::ENDSHADER; ++var) {
        ui->shaderComboBox->addItem(ResourceManager::getInstance().mShaderProgram[var]->mName.c_str());
    }
}

void WidgetMaterial::fillMaterials()
{
    for (int var = 0; var < gsl::NumberOfMaterials; ++var) {
        if (ResourceManager::getInstance().mMaterial[var])
            ui->materialComboBox->addItem(ResourceManager::getInstance().mMaterial[var]->mMatName.c_str());
    }
}

void WidgetMaterial::fillTextures()
{
    for (int var = 0; var < gsl::NumberOfTextures; ++var) {
        if(ResourceManager::getInstance().mTexture[var])
            ui->textureComboBox->addItem(ResourceManager::getInstance().mTexture[var]->mTextureFilename.c_str());
    }
}

void WidgetMaterial::on_specularStrength_valueChanged(double arg1)
{
    mCurrentMaterial->mSpecularStrenght = arg1;
}

void WidgetMaterial::on_specularExponent_valueChanged(int arg1)
{
    mCurrentMaterial->mSpecularExponent = arg1;
}

void WidgetMaterial::on_colorButton_clicked()
{
    QColor currentColor(mCurrentMaterial->mColor.x*255, mCurrentMaterial->mColor.y*255, mCurrentMaterial->mColor.z*255);
    QColor newColor = QColorDialog::getColor(currentColor, this, "Select New Color");

    if (newColor.isValid()) {
        QPalette pal;
        pal.setColor(QPalette::Window, newColor);
        ui->colorLabel->setPalette(pal);
        mCurrentMaterial->mColor.x = newColor.red()/255.f;  //maybe use QColor.redF instead?
        mCurrentMaterial->mColor.y = newColor.green()/255.f;
        mCurrentMaterial->mColor.z = newColor.blue()/255.f;
    }
}

void WidgetMaterial::on_textureComboBox_currentIndexChanged(int index)
{
    qDebug() << "Texture changed to:" << index + 1;
    mCurrentMaterial->mTextureId = index + 1;
}

void WidgetMaterial::on_materialComboBox_currentIndexChanged(int index)
{
    unsigned short tempID = mCurrentScene->mEntities[indexInSceneArray].mEntityID;
    qDebug() << "Material changed to:" << ResourceManager::getInstance().mMaterial[index]->mMatName.c_str();
    ResourceManager::getInstance().setMaterial(tempID, ResourceManager::getInstance().mMaterial[index]->mMatName);

}
