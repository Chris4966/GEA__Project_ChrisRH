#ifndef WIDGETMESH_H
#define WIDGETMESH_H

#include <QWidget>

namespace Ui {
class WidgetMesh;
}

class WidgetMesh : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetMesh(QWidget *parent = nullptr);
    ~WidgetMesh();

    void readMeshData();

    int indexInSceneArray{-1};
    class Scene* mCurrentScene{nullptr};

private:
    Ui::WidgetMesh *ui;
};

#endif // WIDGETMESH_H
