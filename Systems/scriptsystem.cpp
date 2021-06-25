#include "scriptsystem.h"
#include "ecs/components.h"
#include "resourcemanager.h"
#include "scene.h"
#include <QFile>
#include <QDebug>


ScriptSystem::ScriptSystem()
{

}

void ScriptSystem::readData(std::string scriptName)
{
    std::string fileName = gsl::ScriptFilePath + scriptName;
    QFile scriptFile(fileName.c_str());

    if (!scriptFile.open(QIODevice::ReadOnly))
        qDebug() << "Error - NO FILE: " << fileName.c_str();

    QTextStream stream(&scriptFile);
    QString contents = stream.readAll();
    scriptFile.close();

    mScriptEngine.evaluate(contents, fileName.c_str());

    qDebug() << "Player Stats";
    QJSValue tempVar = mScriptEngine.evaluate("Name");
    qDebug() << "Name: " << tempVar.toString();
    tempVar = mScriptEngine.evaluate("Level");
    qDebug() << "Level: " << tempVar.toInt();
    tempVar = mScriptEngine.evaluate("Health");
    qDebug() << "Health: " << tempVar.toNumber() << "%";
    tempVar = mScriptEngine.evaluate("Strength");
    qDebug() << "Strength: " << tempVar.toNumber();
    tempVar = mScriptEngine.evaluate("Speed");
    qDebug() << "Speed: " << tempVar.toNumber();
}

void ScriptSystem::setData(MAX_ENTITIES_TYPE entityID, std::string scriptName)
{
    std::string fileName = gsl::ScriptFilePath + scriptName;
    QFile scriptFile(fileName.c_str());

    if (!scriptFile.open(QIODevice::ReadOnly))
        qDebug() << "Error - NO FILE: " << fileName.c_str();

    QTextStream stream(&scriptFile);
    QString contents = stream.readAll();
    scriptFile.close();

    mScriptEngine.evaluate(contents, fileName.c_str());

    QJSValue tempVar = mScriptEngine.evaluate("Name");
    ResourceManager::getInstance().mCurrentScene->mEntities[entityID].mScript.pName = tempVar.toString();
    tempVar = mScriptEngine.evaluate("Level");
    ResourceManager::getInstance().mCurrentScene->mEntities[entityID].mScript.pLevel = tempVar.toInt();
    tempVar = mScriptEngine.evaluate("Health");
    ResourceManager::getInstance().mCurrentScene->mEntities[entityID].mScript.pHP = tempVar.toNumber();
    tempVar = mScriptEngine.evaluate("Strength");
    ResourceManager::getInstance().mCurrentScene->mEntities[entityID].mScript.pStr = tempVar.toNumber();
    tempVar = mScriptEngine.evaluate("Speed");
    ResourceManager::getInstance().mCurrentScene->mEntities[entityID].mScript.pSpeed = tempVar.toNumber();
}
