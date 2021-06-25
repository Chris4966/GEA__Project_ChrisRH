#ifndef SCRIPTSYSTEM_H
#define SCRIPTSYSTEM_H

#include "gslpch.h"
#include <string>
#include <QJSEngine>

/**
    @brief System Class for using Javascript files.
 */

class ScriptSystem
{
public:
    ScriptSystem();

    /** Reads Data from script file.
       @param scriptName - file name
     */
    void readData(std::string scriptName);
    /** Inserts script data to entity.
       @param entityID
       @param scriptName
     */
    void setData(MAX_ENTITIES_TYPE entityID, std::string scriptName);

    QJSEngine mScriptEngine;
};

#endif // SCRIPTSYSTEM_H
