#ifndef GRAPHNODEFIELD_H
#define GRAPHNODEFIELD_H

#include "graphtypes.h"
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>

class GraphScene;
class GraphNode;
class GraphSlot;

class GraphNodeField : public QGraphicsRectItem
{
public:
    GraphNodeField(GraphScene* scene, GraphNode* node, const QString& name, bool isMultipleInput, bool isOutput, GraphNodeFieldType type);
    void updateLinks();
    GraphSlot* slot() { return mSlot; }
    const QString& name() const { return mName; }

private:
    QGraphicsTextItem* mNameItem;
    GraphSlot* mSlot;
    QString mName;
};

#endif // GRAPHNODEFIELD_H
