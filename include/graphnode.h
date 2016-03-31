#ifndef GRAPHLAYERITEM_H
#define GRAPHLAYERITEM_H

#include <QGraphicsProxyWidget>
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <vector>
#include "graphtypes.h"

class GraphSlot;
class GraphScene;
class GraphNodeField;

enum GraphHitTestResult
{
    GraphHitTestResultNone,
    GraphHitTestResultInput
};

class GraphNode : public QGraphicsRectItem
{
public:
    GraphNode(GraphScene* scene, const QString& name, int id);
    virtual ~GraphNode();

    int type() const { return (int)GraphTypeNode; }
    bool hasParent(GraphNode* node);
    void addField(bool isOutput, const QString& name, bool isMultipleInput, GraphNodeFieldType type);
    QVector<GraphNodeField*>& fields() { return mFields; }
    int id() const { return mId; }
    GraphNodeField* field(const QString& name) const;

protected:
    virtual QVariant itemChange( GraphicsItemChange change, const QVariant &value );

private:
    GraphScene* mScene;
    QVector<GraphNodeField*> mFields;
    int mId;
};


#endif // LAYERGRAPHITEM_H
