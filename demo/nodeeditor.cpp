#include "nodeeditor.h"
#include "graphnode.h"

NodeEditor::NodeEditor(QWidget *parent) : QGraphicsView(parent)
{
    // Users use GraphScene class to manipulate graph nodes and links.
    // It supports drag & drop style link operation.
    // Users can also use "delete" key to remove node or link.
    mScene = new GraphScene(this);
    setScene(mScene);

    // node ids returns by scene
    int node1;
    int node2;
    int node3;

    // add a node
    {
        int nodeId = mScene->AddNode("Node1");
        // One node may contains multiple fields.
        // A field has a input or output slot to connect to other nodes.
        // There're two type of slots: input & output.
        // The output slot can be only linked to input slots.
        mScene->AddNodeField(nodeId, true, "output1", false, GraphNodeFieldTypeNode);
        mScene->AddNodeField(nodeId, false, "input1", true, GraphNodeFieldTypeNode);
        // Set the node position
        mScene->SetNodePosition(nodeId, QPointF(10, 10));
        node1 = nodeId;
    }

    // add a node
    {
        int nodeId = mScene->AddNode("Node2");
        mScene->AddNodeField(nodeId, true, "output1", false, GraphNodeFieldTypeNode);
        mScene->AddNodeField(nodeId, false, "input1", true, GraphNodeFieldTypeNode);
        mScene->AddNodeField(nodeId, false, "input2", false, GraphNodeFieldTypeNode);
        mScene->AddNodeField(nodeId, false, "input3", false, GraphNodeFieldTypeNode);
        mScene->SetNodePosition(nodeId, QPointF(200, 10));
        node2 = nodeId;
    }

    // add a node
    {
        int nodeId = mScene->AddNode("Node3");
        mScene->AddNodeField(nodeId, true, "output1", false, GraphNodeFieldTypeNode);
        mScene->AddNodeField(nodeId, false, "input1", true, GraphNodeFieldTypeNode);
        mScene->SetNodePosition(nodeId, QPointF(10, 200));
        node3 = nodeId;
    }

    // add a link
    {
        mScene->AddLink(node1, "output1", node2, "input2", 0);
    }

    // add multiple links to one slot
    {
        // An input slot can accept multiple input nodes.
        // User can indicate the order of input nodes.
        // This is useful when the output depends on the order of inputs.

        // For example, in composition graph a composite node can blend multiple input images.
        // The composite image depends on the draw order of input images.

        // Some implementation of DAG use separate slots to support this function.
        // The cost is user must sort the slots by output order.
        // Others use separate nodes to simulate this function, which is not intuitive.
        mScene->AddLink(node1, "output1", node2, "input1", 0);
        mScene->AddLink(node3, "output1", node2, "input1", 1);
    }
}

