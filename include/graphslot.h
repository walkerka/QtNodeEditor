#ifndef GRAPHSLOT_H
#define GRAPHSLOT_H
#include <QGraphicsRectItem>

class GraphScene;
class GraphNode;
class GraphLink;

class GraphSlot : public QGraphicsRectItem
{
public:
    GraphSlot(GraphScene* scene, GraphNode* node, const QString& fieldName, bool isOut, bool isMultiple);
    ~GraphSlot();

    void updateLinks();
    void addLink(GraphLink* l);
    void addLink(GraphLink* l, int index);
    void removeLink(GraphLink* l);
    int linkCount() const { return (int)mLinks.size(); }
    bool isMultipleInput() const { return mIsMultiple; }
    QPointF linkAnchorPoint(GraphLink* link) const;
    GraphNode* node() { return mNode; }
    const QVector<GraphLink*>& links() { return mLinks; }
    QString fieldName() { return mFieldName; }
    bool isOutput() const { return mIsOut; }

protected:
    void dragEnterEvent(QGraphicsSceneDragDropEvent *event);
    void dropEvent(QGraphicsSceneDragDropEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

    bool canLinkTo(GraphSlot* slot);
private:
    GraphScene* mScene;
    GraphNode* mNode;
    QString mFieldName;
    int mId;
    bool mIsOut;
    bool mIsMultiple;
    QVector<GraphLink*> mLinks;
    static QVector<GraphSlot*> mSlots;
};

#endif // GRAPHSLOT_H
