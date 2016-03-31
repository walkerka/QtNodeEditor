#ifndef GRAPHSCENE_H
#define GRAPHSCENE_H
#include "graphtypes.h"
#include <QGraphicsScene>

class GraphNode;
class GraphSlot;
class GraphLink;

class GraphScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit GraphScene(QObject *parent = 0);

    void Clear();
    int AddNode(const QString& name);
    void SetNodePosition(int id, const QPointF& position);
    void AddNodeField(int id, bool isOutput, const QString& name, bool isMultipleInput, GraphNodeFieldType type);
    void RemoveNode(int id);
    int AddLink(int fromNode, const QString& fromName, int toNode, const QString& toName, int index);
    void RemoveLink(int fromNode, const QString& fromName, int toNode, const QString& toName, int index);

private:
    friend class GraphNode;
    friend class GraphSlot;
    friend class GraphLink;
    GraphNode* CreateNode(const QString& name);
    void RemoveNode(GraphNode* node);
    GraphLink* AddLink(GraphSlot* fromSlot, GraphSlot* toSlot, int index, bool enableSignal = true);
    GraphLink* AddLink(GraphNode* fromNode, const QString& fromName, GraphNode* toNode, const QString& toName, int index);
    void RemoveLink(GraphLink* link);
    void RemoveLinksOfNode(GraphNode* node);
    GraphNode* GetNode(int id);
    void OnNodePositionChanged(GraphNode* node);
    void OnNodeSelectionChanged(GraphNode* node);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);

signals:
    void linkAdded(int id, int fromNode, QString fromName, int toNode, QString toName, int index);
    void linkRemoved(int id, int fromNode, QString fromName, int toNode, QString toName, int index);
    void nodeAdded(int id);
    void nodeRemoved(int id);
    void nodeMoved(int id, QPointF position);
    void nodeSelected(int id);

public slots:

private:
    int mNextNodeId;
    int mNextLinkId;
    std::vector<GraphNode*> mNodes;
    std::vector<GraphLink*> mLinks;
};

#endif // GRAPHSCENE_H
