#include "graphlink.h"
#include <QPainterPathStroker>
#include "graphslot.h"
#include <QGraphicsScene>

GraphLink::GraphLink(GraphScene* scene, GraphSlot* beginSlot, GraphSlot* endSlot, int id)
    :mScene(scene)
    ,mBeginSlot(beginSlot)
    ,mEndSlot(endSlot)
    ,mId(id)
{
    setFlag(QGraphicsItem::ItemIsSelectable, true);
}

GraphLink::~GraphLink()
{
}

void GraphLink::updateShape()
{
    const QPointF& beginPos = mBeginSlot->linkAnchorPoint(this);
    QPointF endPos = mEndSlot->linkAnchorPoint(this);

    QPainterPath path;
    path.moveTo(beginPos);
    qreal dx = endPos.x() - beginPos.x();
    if (dx < 0)
    {
        dx = -dx;
    }
    if (dx > 50)
    {
        dx = 50;
    }
    QPointF offset(dx, 0.0f);
    path.cubicTo(beginPos + offset, endPos - offset, endPos);
    setPath(path);
    update();
}

QPainterPath GraphLink::shape() const
{
    QPainterPathStroker s;
    s.setWidth(10);
    return s.createStroke(QGraphicsPathItem::shape());
}
