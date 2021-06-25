QT          += core gui widgets qml

TEMPLATE    = app
CONFIG      += c++17

TARGET      = ChrisRHEngine

PRECOMPILED_HEADER = pch/gslpch.h

INCLUDEPATH += \
    ./pch \
    ./gsl \
    ./Shaders \
    ./Systems \
    ./Objects \
    ./Widgets \
    ./include

mac {
    LIBS += -framework OpenAL
}

win32 {
    INCLUDEPATH += $(OPENAL_HOME)\\include\\AL
    LIBS *= $(OPENAL_HOME)\\libs\\Win64\\libOpenAL32.dll.a
}

SOURCES += main.cpp \
    Systems/Sound/soundmanager.cpp \
    Systems/Sound/soundsrc.cpp \
    Systems/Sound/wavhandler.cpp \
    Shaders/hudshader.cpp \
    Systems/scriptsystem.cpp \
    camera.cpp \
    coreengine.cpp \
    Objects/editorgrid.cpp \
    Objects/gizmo.cpp \
    Objects/light.cpp \
    Objects/linebox.cpp \
    Objects/meshobject.cpp \
    Systems/rendersystem.cpp \
    gsl/vec4.cpp \
    renderwindow.cpp \
    mainwindow.cpp \
    resourcemanager.cpp \
    scene.cpp \
    Objects/skybox.cpp \
    texture.cpp \
    vertex.cpp \
    visualobject.cpp \
    Widgets/widgetmaterial.cpp \
    Widgets/widgetmesh.cpp \
    Widgets/widgetsound.cpp \
    Widgets/widgettransform.cpp \
    Objects/xyz.cpp \
    gsl/mat3x3.cpp \
    gsl/mat4x4.cpp \
    gsl/vec2.cpp \
    gsl/vec3.cpp \
    Shaders/colorshader.cpp \
    Shaders/mousepickershader.cpp \
    Shaders/phongshader.cpp \
    Shaders/shader.cpp \
    Shaders/singlecolorshader.cpp \
    Shaders/skyboxshader.cpp \
    Shaders/textureshader.cpp \
    Shaders/normalsshader.cpp

HEADERS += \
    Systems/scriptsystem.h \
    gsl/vec4.h \
    include/AL/al.h \
    include/AL/alc.h \
    include/AL/alext.h \
    include/AL/efx-creative.h \
    include/AL/efx-presets.h \
    include/AL/efx.h \
    Systems/Sound/soundmanager.h \
    Systems/Sound/soundsrc.h \
    Systems/Sound/wavhandler.h \
    Shaders/hudshader.h \
    camera.h \
    Systems/ecs/components.h \
    constants.h \
    coreengine.h \
    Objects/editorgrid.h \
    Systems/ecs/entity.h \
    Objects/gizmo.h \
    gltypes.h \
    input.h \
    Objects/light.h \
    Objects/linebox.h \
    Objects/meshobject.h \
    Systems/rendersystem.h \
    pch/gslpch.h \
    renderwindow.h \
    mainwindow.h \
    resourcemanager.h \
    scene.h \
    Objects/skybox.h \
    texture.h \
    vertex.h \
    visualobject.h \
    Widgets/widgetmaterial.h \
    Widgets/widgetmesh.h \
    Widgets/widgetsound.h \
    Widgets/widgettransform.h \
    Objects/xyz.h \
    gsl/mat3x3.h \
    gsl/mat4x4.h \
    gsl/math_constants.h \
    gsl/vec2.h \
    gsl/vec3.h \
    Shaders/colorshader.h \
    Shaders/mousepickershader.h \
    Shaders/phongshader.h \
    Shaders/singlecolorshader.h \
    Shaders/shader.h \
    Shaders/skyboxshader.h \
    Shaders/textureshader.h \
    Shaders/normalsshader.h

FORMS += \
    mainwindow.ui \
    Widgets/widgetmaterial.ui \
    Widgets/widgetmesh.ui \
    Widgets/widgetsound.ui \
    Widgets/widgettransform.ui

DISTFILES += \
    Scripts/EnemyData.js \
    Scripts/PlayerData.js \
    Shaders/colorshader.frag \
    Shaders/colorshader.vert \
    Shaders/hudshader.frag \
    Shaders/hudshader.vert \
    Shaders/mousepickershader.frag \
    Shaders/mousepickershader.vert \
    Shaders/phongshader.frag \
    Shaders/phongshader.vert \
    Shaders/shownormalsshader.frag \
    Shaders/shownormalsshader.geom \
    Shaders/shownormalsshader.vert \
    Shaders/singlecolorshader.frag \
    Shaders/singlecolorshader.vert \
    Shaders/skyboxshader.frag \
    Shaders/skyboxshader.vert \
    Shaders/textureshader.frag \
    Shaders/textureshader.vert
