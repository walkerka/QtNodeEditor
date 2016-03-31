#ifndef GRAPHLINK_H
#define GRAPHLINK_H
#include <QGraphicsPathItem>
#include "graphtypes.h"

class GraphScene;
class GraphSlot;

class GraphLink : public QGraphicsPathItem
{
public:
    GraphLink(GraphScene* scene, GraphSlot* beginSlot, GraphSlot* endSlot, int id);
    ~GraphLink();
    int type() const { return (int)GraphTypeLink; }
    void updateShape();

    GraphSlot* beginSlot() { return mBeginSlot; }
    GraphSlot* endSlot() { return mEndSlot; }
    int id() const { return mId; }

public:
    QPainterPath shape() const;

private:
    GraphScene* mScene;
    GraphSlot* mBeginSlot;
    GraphSlot* mEndSlot;
    int mId;
};

#endif // GRAPHLINK_H
