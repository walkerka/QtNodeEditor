#include "graphnodefield.h"
#include <QtWidgets>
#include "graphslot.h"
#include "graphscene.h"

GraphNodeField::GraphNodeField(GraphScene* scene, GraphNode* node, const QString& name, bool isMultipleInput, bool isOutput, GraphNodeFieldType type)
    :mName(name)
{
    setRect(0, 0, 100, 30);
    mNameItem = scene->addText(name);
    mNameItem->setParentItem(this);
    mNameItem->setPos(5, (rect().height() - mNameItem->boundingRect().height()) * 0.5);

    if (isOutput)
    {
        mSlot = new GraphSlot(scene, node, name, true, false);
        mSlot->setParentItem(this);
        mSlot->setPos(rect().width(), (rect().height() - mSlot->rect().height()) * 0.5);
    }
    else
    {
        mSlot = new GraphSlot(scene, node, name, false, isMultipleInput);
        mSlot->setParentItem(this);
        mSlot->setPos(-mSlot->rect().width(), (rect().height() - mSlot->rect().height()) * 0.5);

        QWidget* editorWidget = NULL;
        switch (type)
        {
        case GraphNodeFieldTypeInt:
            editorWidget = new QSpinBox();
            break;
        case GraphNodeFieldTypeFloat:
            editorWidget = new QDoubleSpinBox();
            break;
        case GraphNodeFieldTypeBool:
            editorWidget = new QCheckBox();
            break;
        case GraphNodeFieldTypeNode:
        default:
            break;
        }

        if (editorWidget)
        {
            QGraphicsProxyWidget* editor = new QGraphicsProxyWidget(this);
            editor->setWidget(editorWidget);
            editor->setPos(mNameItem->boundingRect().right() + 5, (rect().height() - editor->rect().height()) * 0.5);
        }
    }
}

void GraphNodeField::updateLinks()
{
    mSlot->updateLinks();
}
