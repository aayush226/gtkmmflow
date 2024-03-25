#include "NodeView.h"
#include "Socket.h"

NodeView::NodeView() {
    
    set_name("nodeview");
    
    addNode("Composite",50,50,75,350,true,true,false,false);
    addNode("Blur",300,50,75,350,true,false,true,false);
    addNode("Filter",600,250,75,150,true,false,false,true);
    addNode("Opacity",370,400,75,350,true,false,false,false);
    
    auto motion_controller = Gtk::EventControllerMotion::create();    
    motion_controller->signal_motion().connect(sigc::mem_fun(*this, &NodeView::on_motion_notify_event), false);
    add_controller(motion_controller);

    auto click_controller = Gtk::GestureClick::create();    
    click_controller->signal_pressed().connect(sigc::mem_fun(*this, &NodeView::on_button_press_event), false);
    click_controller->signal_released().connect(sigc::mem_fun(*this, &NodeView::on_button_release_event), false);
    add_controller(click_controller);
    
    /*auto key_controller = Gtk::EventControllerKey::create();
    key_controller->signal_key_pressed().connect(sigc::mem_fun(*this, &NodeView::on_key_press_event), false);
    add_controller(key_controller);*/

    /*auto focus_controller = Gtk::EventControllerFocus::create();
    //key_controller->signal_key_pressed().connect(sigc::mem_fun(*this, &NodeView::on_key_press_event), false);
    add_controller(focus_controller);*/

    add_node_button.set_label("Add Node");
    add_node_button.signal_clicked().connect(sigc::mem_fun(*this, &NodeView::on_add_node_button_clicked));
    put(add_node_button,0,0);

    delete_edge_button.set_label("Delete Edge");
    delete_edge_button.signal_clicked().connect([this] () { delete_selected_edge(); });
    put(delete_edge_button,120,0);

    /*node_details_button.set_label("Reveal Info");
    node_details_button.signal_clicked().connect([this] () { reveal_node_info(); });
    put(node_details_button,200,0);*/

    const char* css_style = R"(
    #nodeview { /* Assuming 'nodeview' is a custom name you give to your NodeView widget */
            background-color: #1a1c1d; /* Light grey background */
        }
    )";
    
    auto css_provider = Gtk::CssProvider::create();
    auto style_context = get_style_context();
    
    css_provider->load_from_data(css_style);
    style_context->add_provider(css_provider, GTK_STYLE_PROVIDER_PRIORITY_USER);    
}

void NodeView::snapshot_vfunc(const Glib::RefPtr<Gtk::Snapshot>& snapshot) {
    
    Gtk::Fixed::snapshot_vfunc(snapshot);
    auto allocation = get_allocation();
    
    // Use the allocation width and height for drawing bounds
    Gdk::Rectangle bounds(0, 0, allocation.get_width(), allocation.get_height());
    
    for (const auto& edge : edges) {
        edge->snapshot_vfunc(snapshot, bounds);
    }

    // Draw temporary edge if it exists
    if (tempEdge) {
        tempEdge->snapshot_vfunc(snapshot,bounds);
    }
}

void NodeView::addNode(const std::string& title, double x, double y, int width, int height, bool spin, bool color, bool switch1_, bool check) {
    auto node = std::make_shared<Node>(title, x, y, width, height,spin,color,switch1_,check); // Create a new node dynamically    
    node->signal_delete_node.connect(sigc::mem_fun(*this, &NodeView::delete_node));
    node->signal_value_changed.connect(sigc::mem_fun(*this, &NodeView::value_changed));
    nodes.push_back(node); // Add the node pointer to the container    
    put(*node, x, y); // Use the put method from Gtk::Fixed to place the node
}

void NodeView::on_add_node_button_clicked() {
    // Calculate a position for the new node to avoid overlaps
    //double x, y;
    //calculateNewNodePosition(x, y, 75, 350); // Assuming fixed size for simplicity
    addNode("Node x", 10, 10, 75, 350, true, true, false, false);
}

void NodeView::value_changed(std::string value, std::shared_ptr<Socket> socket){
    
    // ----->(o) element (o)---->    
    for (auto& edge : edges) {
        if (edge->get_start_socket() == socket) {            
            auto endNode = edge->get_end_node();
            auto endSocket = edge->get_end_socket();
            if (endNode) {
                endNode->socket_value_changed(endSocket, value);
            }
        }
    }
}

void NodeView::dfs_propagate(std::shared_ptr<Socket> startSocket, const std::string& value, std::shared_ptr<Edge> edge) {
    /*if (visited.find(startSocket) != visited.end()) {
        // This socket has already been visited in this propagation cycle
        return;
    }
    visited.insert(startSocket); // Mark this socket as visited*/

    // Propagate the value change to this socket's node
    /*auto node = startSocket->get_node();
    if (node) {
        node->on_socket_value_changed(startSocket, value);
    }*/
    auto node = edge->get_end_node();
    if(node){
        node->push_state(value);
    }
    // Recursively visit all connected (forward) nodes
    for (auto& edge_ : edges) {
        if (edge_->get_start_socket() == edge->get_end_socket()) {
            auto endSocket = edge_->get_end_socket();
            auto endNode = edge_->get_end_node();
            if (endSocket && endNode) {
                dfs_propagate(endSocket, endNode->get_value(endSocket),edge_); // Recurse
            }
        }
    }
}

/*bool NodeView::on_key_press_event(guint keyval, guint keycode, Gdk::ModifierType state) {
    if (keyval == GDK_KEY_Delete) {
        // Handle deletion of selected edge
        std::cout<<"Delete pressed" << std::endl;
        delete_selected_edge();
        return true; // Indicate that the key press event has been handled
    }
    return false; // Indicate that the key press event has not been handled
}*/

void NodeView::delete_selected_edge() {
    /*auto it = std::remove_if(edges.begin(), edges.end(), [](const auto& edge) {
        return edge->getSelected(); // Assuming getSelected() returns whether the edge is selected
    });

    if (it != edges.end()) {
        // If any edges were selected, erase them from the collection
        //delete connection from map.
        edges.erase(it, edges.end());
        queue_draw(); // Redraw the NodeView to reflect the changes
    }*/
    for (auto it = edges.begin(); it != edges.end(); ) {
        if ((*it)->getSelected()) {
            // Get the start and end sockets of the edge
            auto start_socket = (*it)->get_start_socket();
            auto end_socket = (*it)->get_end_socket();
            //std::cout << "Map size: " << m.size() << std::endl;
            // Remove the mapping from the start socket to the end socket
            m.erase(start_socket);
            
            // If your map is bidirectional, also remove the reverse mapping
            if(end_socket) { // Make sure end_socket is not null before dereferencing
                m.erase(end_socket);
            }
            //std::cout << "Map size: " << m.size() << std::endl;
            // Erase the edge and advance the iterator
            it = edges.erase(it);
        } else {
            ++it; // Only increment the iterator if you didn't erase
        }
    }

    // After removing the edges, redraw the NodeView to reflect changes
    queue_draw();
}

void NodeView::on_button_press_event(int, double x, double y) {
    
    
    for(auto& node: nodes){
        //auto res = node->is_inside_socket(x,y);
        /*if(res.first==true){
            tempEdge = std::make_shared<Edge>(res.second.first,res.second.second,x, y);
            return;
        }*/
        auto socket = node->is_inside_socket(x,y);
        //if(socket && socket->get_type()!= Socket::Type::INPUT){
        if(socket){
            tempEdge = std::make_shared<Edge>(socket,node,x, y);
            return;
        }
    }

    for (auto& edge : edges) {
        if (edge->is_point_near(x, y)) {
            //selectedEdge = edge; // Assume selectedEdge is a std::shared_ptr<Edge> member of NodeView
            edge->setSelected(!edge->getSelected());
            //grab_focus();
            queue_draw();
            //std::cout<<"Edge Clicked" <<std::endl;
            return; // Stop checking further if we've found an edge
        }
    }


    for (auto& node : nodes) {
        if (node->is_inside(x, y)) { 
            node->activate();           
            node->start_dragging(x, y);            
            return;
        }
    }    
}

void NodeView::on_button_release_event(int, double x, double y) {    
    //std::cout << "Button released at " << x <<" "<<y<<std::endl;
    if (tempEdge) {
        //std::shared_ptr<Socket> endSocket = nullptr;
        for (auto& node : nodes) {            
            auto endsocket = node->is_inside_socket(x, y);
            if(endsocket == tempEdge->get_start_socket()) break;
            if (endsocket) {
                //cycle detection
                
                auto startsocket = tempEdge->get_start_socket();
                //auto startSocketSibling = tempEdge->get_start_node()->get_socket_sibling(startsocket);
                
                /*if(!checkCycleDetection(startsocket,tempEdge->get_start_node()->get_socket_sibling(startsocket))){
                    auto newEdge = std::make_shared<Edge>(startsocket, tempEdge->get_start_node(), endsocket, node);
                    edges.push_back(newEdge);
                    m[startsocket].push_back(endsocket);
                    //m[endsocket].push_back(startsocket);
                    //signal changes to the end socket.
                    //dfs_propagate(startsocket,tempEdge->get_start_node()->get_value(startsocket),newEdge);
                    break;
                } */

                // node1 ----> node2
                if (checkCycleDetection(tempEdge->get_start_node(), node)) {
                    std::cout << "Adding this edge would create a cycle!" << std::endl;
                    //m[startsocket].pop_back();
                    break;
                } else {
                    // Safe to add the edge.
                    auto newEdge = std::make_shared<Edge>(startsocket, tempEdge->get_start_node(), endsocket, node);
                    edges.push_back(newEdge);
                    m[startsocket].push_back(endsocket);
                    //m[endsocket].push_back(startsocket);
                    //signal changes to the end socket.
                    //dfs_propagate(startsocket,tempEdge->get_start_node()->get_value(startsocket),newEdge);
                    value_changed(tempEdge->get_start_node()->get_value(startsocket), startsocket);
                    break;
                }
            }
        }        
        tempEdge.reset();
        queue_draw();
    }
    else{
        for (auto& node : nodes) {
            node->stop_dragging();
            node->deactivate();
        }
    }
    
}

void NodeView::on_motion_notify_event(double x, double y) {
    
    if (tempEdge) {
        tempEdge->updateTemporary(x, y);
        queue_draw();        
    }
    else{
        for (auto& node : nodes) {
            if (node->is_dragging()) {              
                node->drag_to(x, y);                
                move(*node, node->get_x(), node->get_y());  
                break;
            }
        }
    }
}

void NodeView::delete_node(std::shared_ptr<Node> node) {
    // Remove the edges connected to the node
    //std::cout << "Delete button clicked!!" << std::endl;

    auto itEdge = edges.begin();
    while (itEdge != edges.end()) {
        auto edge = *itEdge;
        if (edge->get_start_node() == node ||
            (edge->get_end_socket() && edge->get_end_node() == node)) {
            std::cout << "Map size: " << m.size() << std::endl;
            m.erase((*itEdge)->get_start_socket());
            m.erase((*itEdge)->get_end_socket());   
            std::cout << "Map size: " << m.size() << std::endl;
            itEdge = edges.erase(itEdge);  // Remove edge and advance iterator
        } else {
            ++itEdge;
        }
    }


    auto itNode = std::find(nodes.begin(), nodes.end(), node);
    if (itNode != nodes.end()) {
        // Also, remove the node widget from the Gtk::Fixed container
        remove(**itNode);
        nodes.erase(itNode);
    }

    // Trigger a redraw of the NodeView to reflect changes
    queue_draw();
}

void NodeView::reveal_node_info(){
    std::cout << "=======nodes=========" << std::endl;
    for (auto& node : nodes) {
        node->get_children_data();
    }
    std::cout << "===================" << std::endl;
    /*std::cout << "=======edges========" << std::endl;
    for(auto &edge : edges) {
        //std::cout << edge->get_start_node()->getLabel() << ", "<< edge->get_start_socket()->get_data() <<"---" << edge->get_end_node()->getLabel() << ", "<< edge->get_start_socket()->get_data()<< std::endl;
        std::cout << edge->get_start_node()->get_value(edge->get_start_socket()) <<"---"<< edge->get_end_node()->get_value(edge->get_end_socket())<< std::endl;
    
    }
    std::cout << "===================" << std::endl;*/
}

bool NodeView::dfs_check_cycle(const std::shared_ptr<Node>& currentNode, std::unordered_set<std::shared_ptr<Node>>& visitedNodes, const std::shared_ptr<Node>& base) {
    if (visitedNodes.find(currentNode) != visitedNodes.end()) {
        // Found a cycle since the current node is already visited
        return true;
    }
    visitedNodes.insert(currentNode); // Mark the current node as visited

    // Iterate over all outgoing edges of the current node
    for (const auto& edge : edges) {
        //if (edge == comingEdge) continue; // Skip the edge we're coming from
        if(edge->get_start_node()!=currentNode) continue;
        auto nextNode = edge->get_end_node(); // Assuming this gets the node at the end of the edge
        if (nextNode == base) {
            // Reached back to the start node, indicating a cycle
            return true;
        }
        if (dfs_check_cycle(nextNode, visitedNodes, base)) {
            return true; // Cycle found in recursion
        }
    }

    // Optional: Remove the current node from visited if backtracking is required in another path
    visitedNodes.erase(currentNode);

    return false; // No cycle found along this path
}

bool NodeView::checkCycleDetection(std::shared_ptr<Node> base,std::shared_ptr<Node> target){
    //std::unordered_set<std::shared_ptr<Edge>> visited; // Create an empty set for visited sockets.
    //return dfs_check_cycle(endSocket, visited,, startSocketSibling);
    std::unordered_set<std::shared_ptr<Node>> visitedNodes;
    //auto startNode = newEdge->get_start_node();
    // Optionally, determine if there's a special case with startNode's sibling
    
    // Start DFS from the node at the end of the new edge to see if we can reach back to startNode
    return dfs_check_cycle(target, visitedNodes, base);
}
