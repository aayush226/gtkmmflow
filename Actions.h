#pragma once
#include <memory>
#include "Node.h"
#include "Edge.h"

class NodeView;

class Action {
public:
    virtual ~Action() = default;
    virtual void doAction() = 0;
    virtual void undoAction() = 0;
};

class MoveNodeAction : public Action {
public:
    MoveNodeAction(NodeView& nodeView, std::shared_ptr<Node> node, double oldX, double oldY, double newX, double newY);

    void doAction() override;
    void undoAction() override;

private:
    NodeView& nodeView_;
    std::shared_ptr<Node> node_;
    double oldX_, oldY_, newX_, newY_;
};


/*class DeleteEdgesAction : public Action {
public:
    DeleteEdgesAction(NodeView& nodeView, std::vector<std::shared_ptr<Edge>>& edgesToDelete);

    void doAction() override;
    void undoAction() override;

private:
    NodeView& nodeView_;
    std::vector<std::shared_ptr<Edge>> edgesToDelete;
};
*/
class CreateEdgeAction : public Action {
public:
    CreateEdgeAction(NodeView& nodeView, std::shared_ptr<Edge> edge);

    void doAction() override;
    void undoAction() override;

private:
    NodeView& nodeView_;
    std::shared_ptr<Edge> edge_;
};