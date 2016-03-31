#include "graphslot.h"
#include <QtWidgets>
#include "graphscene.h"
#include "graphlink.h"
#include "graphnode.h"

QVector<GraphSlot*> GraphSlot::mSlots;

GraphSlot::GraphSlot(GraphScene* scene, GraphNode* node, const QString& fieldName, bool isOut, bool isMultiple)
    :mScene(scene)
    ,mNode(node)
    ,mFieldName(fieldName)
    ,mId(mSlots.size())
    ,mIsOut(isOut)
    ,mIsMultiple(isMultiple)
{   
    setRect(0, 0, 10, isMultiple ? 20 : 10);

    setAcceptDrops(true);
    mSlots.push_back(this);
}

GraphSlot::~GraphSlot()
{
}

void GraphSlot::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
    if (event->mimeData() && event->mimeData()->hasFormat("application/x-graphslot"))
    {
        QByteArray b = event->mimeData()->data("application/x-graphslot");
        QDataStream dataStream(&b, QIODevice::ReadOnly);

        int id;
        int index;
        dataStream >> id;
        dataStream >> index;
        if (id != mId && id < mSlots.size())
        {
            GraphSlot* slot = mSlots[id];
            if (canLinkTo(slot))
            {
                event->setAccepted(true);
                return;
            }
        }
    }

    event->setAccepted(false);
}

void GraphSlot::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    if (event->mimeData() && event->mimeData()->hasFormat("application/x-graphslot"))
    {
        QByteArray b = event->mimeData()->data("application/x-graphslot");
        QDataStream dataStream(&b, QIODevice::ReadOnly);

        int id;
        int index;
        dataStream >> id;
        dataStream >> index;
        if (id != mId && id < mSlots.size())
        {
            GraphSlot* slot = mSlots[id];
            if (canLinkTo(slot))
            {
                if (mIsOut)
                {
                    mScene->AddLink(this, slot, index);
                }
                else
                {
                    if (mIsMultiple)
                    {
                        index = (int)((mLinks.size() + 1) * event->pos().y() / rect().height());
                        mScene->AddLink(slot, this, index);
                    }
                    else
                    {
                        mScene->AddLink(slot, this, 0);
                    }
                }
            }
        }
    }
}

void GraphSlot::mousePressEvent(QGraphicsSceneMouseEvent* /*event*/)
{
}

void GraphSlot::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (QLineF(event->screenPos(), event->buttonDownScreenPos(Qt::LeftButton))
        .length() < QApplication::startDragDistance()) {
        return;
    }

    QDrag *drag = new QDrag(event->widget());
    QMimeData *mime = new QMimeData;

    QByteArray itemData;
    QDataStream dataStream(&itemData, QIODevice::WriteOnly);
    dataStream << mId;
    if (!mIsOut && mIsMultiple)
    {
        int index = (int)((mLinks.size() + 1) * event->pos().y() / rect().height());
        dataStream << index;
    }
    else
    {
        dataStream << 0;
    }

    mime->setData("application/x-graphslot", itemData);
    drag->setMimeData(mime);

    drag->exec();
}

void GraphSlot::mouseReleaseEvent(QGraphicsSceneMouseEvent* /*event*/)
{
}

void GraphSlot::updateLinks()
{
    for (int i = 0; i < mLinks.size(); ++i)
    {
        mLinks[i]->updateShape();
    }
}

void GraphSlot::addLink(GraphLink* l)
{
    if (mLinks.indexOf(l) != -1)
    {
        return;
    }
    mLinks.push_back(l);
    updateLinks();
}

void GraphSlot::addLink(GraphLink* l, int index)
{
    if (mLinks.indexOf(l) != -1)
    {
        return;
    }

    if (index < 0)
    {
        index = 0;
    }
    if (index > mLinks.size())
    {
        index = mLinks.size();
    }

    mLinks.insert(index, l);
    updateLinks();
}

void GraphSlot::removeLink(GraphLink* l)
{
    int index = mLinks.indexOf(l);
    if (index != -1)
    {
        mLinks.removeAt(index);
        updateLinks();
    }
}

bool GraphSlot::canLinkTo(GraphSlot* slot)
{
    if (slot->mNode != mNode && slot->mIsOut != mIsOut)
    {
        GraphNode* nIn = NULL;
        GraphNode* nOut = NULL;

        if (mIsOut)
        {
            nOut = mNode;
            nIn = slot->mNode;
        }
        else
        {
            nIn = mNode;
            nOut = slot->mNode;
        }

        return !nOut->hasParent(nIn);
    }
    return false;
}

QPointF GraphSlot::linkAnchorPoint(GraphLink* link) const
{
    QPointF endPos = mapToScene(rect().center());
    int linkCount = this->linkCount();
    if (isMultipleInput() && linkCount > 1)
    {
        int index = mLinks.indexOf(link);
        if (index != -1)
        {
            endPos.setY(endPos.y() + rect().height() * (((qreal)index + 1) / (linkCount + 1) - 0.5));
        }
    }
    return endPos;
}
