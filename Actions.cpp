#include "Actions.h"
#include "Node.h"
#include "NodeView.h"
MoveNodeAction::MoveNodeAction(NodeView& nodeView, std::shared_ptr<Node> node, double oldX, double oldY, double newX, double newY)
    : nodeView_(nodeView), node_(std::move(node)), oldX_(oldX), oldY_(oldY), newX_(newX), newY_(newY) {}

void MoveNodeAction::doAction() {
    node_->set_new_position(newX_, newY_);
    this->nodeView_.move(*node_, newX_, newY_);
}

void MoveNodeAction::undoAction() {
    node_->set_new_position(oldX_, oldY_);
    this->nodeView_.move(*node_, oldX_, oldY_);
}


/*DeleteEdgesAction(NodeView& nodeView, std::vector<std::shared_ptr<Edge>>& edgesToDelete)
    : nodeView_(nodeView), edgesToDelete(std::move(edgesToDelete)) {}

void DeleteEdgesAction::doAction(){
    for (auto& edge : edgesToDelete) {
        
    }
}

void DeleteEdgesAction::undoAction(){
    
    for (auto& edge : edgesToDelete) {
        
    }
}*/

CreateEdgeAction::CreateEdgeAction(NodeView& nodeView, std::shared_ptr<Edge> edge)
    : nodeView_(nodeView), edge_(std::move(edge)) {}

void CreateEdgeAction::doAction() {    
    nodeView_.addEdge(edge_);
}

void CreateEdgeAction::undoAction() {    
    nodeView_.removeEdge(edge_);
}