#include "nodeeditor.h"

NodeEditor::NodeEditor(QWidget *parent) : QGraphicsView(parent)
{
    mScene = new GraphScene(this);
    setScene(mScene);
}

