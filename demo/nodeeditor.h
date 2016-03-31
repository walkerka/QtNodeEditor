#ifndef NODEEDITOR_H
#define NODEEDITOR_H

#include <QWidget>
#include <QGraphicsView>
#include "graphscene.h"

class NodeEditor : public QGraphicsView
{
    Q_OBJECT
public:
    explicit NodeEditor(QWidget *parent = 0);

signals:

public slots:

private:
    GraphScene* mScene;
};

#endif // NODEEDITOR_H
