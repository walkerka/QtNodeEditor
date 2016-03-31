#ifndef GRAPHTYPES_H
#define GRAPHTYPES_H

#include <QGraphicsItem>

enum GraphType
{
    GraphTypeNode = QGraphicsItem::UserType + 1,
    GraphTypeLink
};

enum GraphNodeFieldType
{
    GraphNodeFieldTypeNode,
    GraphNodeFieldTypeFloat,
    GraphNodeFieldTypeInt,
    GraphNodeFieldTypeBool,
};

#endif // GRAPHTYPES_H

