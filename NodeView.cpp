#include "NodeView.h"
#include "Socket.h"

NodeView::NodeView() {
    
    set_name("nodeview"); //add name for the css styling
    
    addNode("Composite",50,50,75,350,true,true,true,true,Node::NodeType::A);
    addNode("Blur",300,50,75,350,true,false,true,false,Node::NodeType::B);
    addNode("Filter",600,250,75,150,false,true,true,true,Node::NodeType::C);
    addNode("Opacity",370,400,75,350,true,false,true,false,Node::NodeType::D);
    
    //Event controllers

    auto motion_controller = Gtk::EventControllerMotion::create();    
    motion_controller->signal_motion().connect(sigc::mem_fun(*this, &NodeView::on_motion_notify_event), false);
    add_controller(motion_controller);

    auto click_controller = Gtk::GestureClick::create();    
    click_controller->signal_pressed().connect(sigc::mem_fun(*this, &NodeView::on_button_press_event), false);
    click_controller->signal_released().connect(sigc::mem_fun(*this, &NodeView::on_button_release_event), false);
    add_controller(click_controller);
    
    auto key_controller = Gtk::EventControllerKey::create();
    key_controller->signal_key_pressed().connect(sigc::mem_fun(*this, &NodeView::on_key_press_event), false);
    key_controller->signal_key_released().connect(sigc::mem_fun(*this, &NodeView::on_key_release_event), false);
    add_controller(key_controller);

    /*auto focus_controller = Gtk::EventControllerFocus::create();
    //key_controller->signal_key_pressed().connect(sigc::mem_fun(*this, &NodeView::on_key_press_event), false);
    add_controller(focus_controller);*/

    /*auto scroll_controller = Gtk::EventControllerScroll::create();
    scroll_controller->set_flags(Gtk::EventControllerScroll::Flags::VERTICAL);
    scroll_controller->signal_scroll().connect(sigc::mem_fun(*this, &NodeView::on_scroll_zoom), false);
    add_controller(scroll_controller);*/

    
    //Action Buttons
    add_node_button.set_label("Add Node");
    add_node_button.signal_clicked().connect(sigc::mem_fun(*this, &NodeView::on_add_node_button_clicked));
    put(add_node_button,0,0);

    delete_edge_button.set_label("Delete Edge");
    delete_edge_button.signal_clicked().connect([this] () { delete_selected_edge(); });
    put(delete_edge_button,110,0);
    
    pan_button.set_label("Panning");
    pan_button.signal_clicked().connect([this] () { togglePanning(); });
    put(pan_button,235,0);

    toggle_filter.set_label("Filter Toggle");
    toggle_filter.signal_toggled().connect(sigc::mem_fun(*this, &NodeView::on_filter_toggled));
    put(toggle_filter,350,0);

    undo_button_.set_label("Undo");
    undo_button_.signal_clicked().connect(sigc::mem_fun(*this, &NodeView::on_undo_button_clicked));
    put(undo_button_,465,0);

    redo_button_.set_label("Redo");
    redo_button_.signal_clicked().connect(sigc::mem_fun(*this, &NodeView::on_redo_button_clicked));
    put(redo_button_,580,0);

    alertLabel.set_text("Cycle Detected! Operation not allowed.");
    alertLabel.set_visible(false); // Start with the label hidden
    put(alertLabel, 580, 50); // Adjust position according to your layout
    
    undoLabel.set_text("Undoing last action!");
    undoLabel.set_visible(false); // Start with the label hidden
    put(undoLabel, 580, 50); // Adjust position according to your layout
    
    redoLabel.set_text("Redoing last action!");
    redoLabel.set_visible(false); // Start with the label hidden
    put(redoLabel, 580, 50); // Adjust position according to your layout
    
    //Custom CSS for the NodeView
    const char* css_style = R"(
        #nodeview { /* Assuming 'nodeview' is a custom name you give to your NodeView widget */
            background-color: #f0f4f7; /* Light grey background */
        }       
        .nodeview-inactive {
            background-color: #000000; /* Gray background color */
        }        
    )";
    
    auto css_provider = Gtk::CssProvider::create();
    auto style_context = get_style_context();
    
    css_provider->load_from_data(css_style);
    style_context->add_provider(css_provider, GTK_STYLE_PROVIDER_PRIORITY_USER);    

    //custom css for the edge detection alert To:do change this to GTk::MessageWindow
    auto cssProvider2 = Gtk::CssProvider::create();
    cssProvider2->load_from_data(R"(
        .alert-label {
            background-color: #f44336; /* Red */
            color: white;
            border-radius: 4px;
            padding: 6px;
            margin: 10px;
            font-weight: bold;
        }
    )");

    auto styleContext2 = alertLabel.get_style_context();
    styleContext2->add_provider(cssProvider2, GTK_STYLE_PROVIDER_PRIORITY_USER);
    styleContext2->add_class("alert-label");

    auto cssProvider3 = Gtk::CssProvider::create();
    cssProvider3->load_from_data(R"(
        .undo-label {
            background-color: red; /* Red */
            color: white;
            border-radius: 4px;
            padding: 6px;
            margin: 10px;
            font-weight: bold;
        }
    )");

    auto styleContext3 = undoLabel.get_style_context();
    styleContext3->add_provider(cssProvider3, GTK_STYLE_PROVIDER_PRIORITY_USER);
    styleContext3->add_class("undo-label");

    auto cssProvider4 = Gtk::CssProvider::create();
    cssProvider4->load_from_data(R"(
        .redo-label {
            background-color: green; /* Red */
            color: white;
            border-radius: 4px;
            padding: 6px;
            margin: 10px;
            font-weight: bold;
        }
    )");

    auto styleContext4 = redoLabel.get_style_context();
    styleContext4->add_provider(cssProvider4, GTK_STYLE_PROVIDER_PRIORITY_USER);
    styleContext4->add_class("redo-label");
}

void NodeView::snapshot_vfunc(const Glib::RefPtr<Gtk::Snapshot>& snapshot) {
    
    Gtk::Fixed::snapshot_vfunc(snapshot);
    auto allocation = get_allocation();
    
    // Using the allocation width and height for drawing bounds
    Gdk::Rectangle bounds(0, 0, allocation.get_width(), allocation.get_height());
    
    for (const auto& edge : edges) {
        edge->snapshot_vfunc(snapshot, bounds);
    }

    // Drawing temporary edge if it exists
    if (tempEdge) {
        tempEdge->snapshot_vfunc(snapshot,bounds);
    }

    if (rubberband_active) {
        auto allocation = get_allocation();
        Gdk::Rectangle bounds(0, 0, allocation.get_width(), allocation.get_height());
        auto cr = snapshot->append_cairo(bounds);
        
        double x = std::min(rubberband_start_x, rubberband_end_x);
        double y = std::min(rubberband_start_y, rubberband_end_y);
        double width = std::abs(rubberband_end_x - rubberband_start_x);
        double height = std::abs(rubberband_end_y - rubberband_start_y);

        cr->set_source_rgba(0.741, 0.204, 0.820, 0.25); // Semi-transparent purple. 
        cr->rectangle(x, y, width, height);
        cr->fill();
    }
}

void NodeView::addNode(const std::string& title, double x, double y, int width, int height, bool spin, bool color, bool switch1_, bool check, Node::NodeType type) {
    auto node = std::make_shared<Node>(title, x, y, width, height,spin,color,switch1_,check,type);
    node->signal_delete_node.connect(sigc::mem_fun(*this, &NodeView::delete_node));
    
    //Grabbing signal from a node. Currently the logic is not defined. Simple passing of a string is implemented to simulate signals.
    node->signal_value_changed.connect(sigc::mem_fun(*this, &NodeView::value_changed));
    /*node->signal_position_changed.connect([this](std::shared_ptr<Node> n, double x, double y) {
        move(*n,x,y);
    });*/
    nodes.push_back(node); // Adding the node pointer to the container    
    put(*node, x, y); // Using the put method from Gtk::Fixed to place the node
}

void NodeView::on_add_node_button_clicked() {
    
    // To-Do: Add Logic to find the optimum place to place the node 
    // auto [x,y] = getOptimumPosition(); 
    double x = 10;
    double y = 10;
    addNode("Node x", x, y, 75, 350, true, true, false, false,Node::NodeType::A);
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

bool NodeView::on_key_press_event(guint keyval, guint keycode, Gdk::ModifierType state) {
    if (keyval == GDK_KEY_a) {
        panning_mode = true;
        // Additional actions for enabling panning
        // To-Do: Change cursor for panning action.
        return true; // Event has been handled
    }
    return false;
    // if (keyval == GDK_KEY_Delete) .... do something
}

void NodeView::on_key_release_event(guint keyval, guint keycode, Gdk::ModifierType state) {
    if (keyval == GDK_KEY_a) {
        panning_mode = false;
        // Reset the cursor if changed
        return;
    }
    return;
}

void NodeView::delete_selected_edge() {
    
    rubberband_active = false;
    
    for (auto it = edges.begin(); it != edges.end(); ) {
        if ((*it)->getSelected()) {
            // Get the start and end sockets of the edge
            auto start_socket = (*it)->get_start_socket();
            auto end_socket = (*it)->get_end_socket();
            
            // Remove the mapping from the start socket to the end socket
            m.erase(start_socket);
            
            // If map is bidirectional, also remove the reverse mapping
            if(end_socket) {
                m.erase(end_socket);
            }            
            // Erase the edge and advance the iterator
            it = edges.erase(it);
        } else {
            ++it; // Only increment the iterator if didn't erase
        }
    }

    // redraw the NodeView to reflect changes
    queue_draw();
}

// For scroll-based zooming:
/*bool NodeView::on_scroll_zoom(double dx, double dy) {
    if (dy > 0) zoom_factor *= 1.1; // Zoom in
    else if (dy < 0) zoom_factor *= 0.9; // Zoom out
    queue_draw();
    return true;
}*/

void NodeView::on_button_press_event(int, double x, double y) {
    grab_focus();
    
    if (panning_mode) {        
        panning_motion = true;
        start_pan_x = x;
        start_pan_y = y;
        // Alternate Panning strategy : Moving the nodes instead of the scrollwindow
        /*for(auto& node:nodes){
            node->start_dragging(node->get_x(),node->get_y());
        }*/
        return;
    }
    
    for(auto& node: nodes){        
        auto socket = node->is_inside_socket(x,y);
        //Optional Behaviour to only allow edge creation from output to input.
        //if(socket && socket->get_type()!= Socket::Type::INPUT){
        if(socket){
            tempEdge = std::make_shared<Edge>(socket,node,x, y);
            return;
        }
    }

    for (auto& edge : edges) {
        if (edge->is_point_near(x, y)) {            
            edge->setSelected(!edge->getSelected());
            queue_draw();            
            return; // Stop checking further if found an edge
        }
    }

    for (auto& node : nodes) {
        if (node->is_inside(x, y)) { 
            //node->activate();           
            //node->start_dragging(x, y);
            if(node->isActivated()){
                initial_click_x = x;
                initial_click_y = y;
                dragging_selected_items = true;
                for(auto& node1:nodes){
                    if(node1->isActivated()){
                        node1->start_dragging(node1->get_x(),node1->get_y());
                    }
                }
                //std::cout << "Attmepting to drag a selected group item after rubberband" << std::endl;
            }   
            else{
                //node->set_sensitive(false);
                node->start_dragging(x, y);
            }  
            return;
        }
    }    
        
    for(auto& node:nodes){
        if(node->isActivated()){
            node->deactivate();
        }
    }
    /*for (auto& edge : edges) {
        if (edge->getSelected()) {
            edge->setSelected(false);
        }
    }*/
    rubberband_active = true;
    rubberband_start_x = x;
    rubberband_start_y = y;
    rubberband_end_x = x;
    rubberband_end_y = y;
}

void NodeView::on_button_release_event(int, double x, double y) {    
    if (panning_mode) {
        panning_motion = false;
        // std::cout << "Scroll up" << std::endl;
        /*for (auto& node : nodes) {
            node->stop_dragging();
            //node->deactivate();
        }*/
        return;
    }
    if (tempEdge) {
        for (auto& node : nodes) {            
            auto endsocket = node->is_inside_socket(x, y);
            if(endsocket == tempEdge->get_start_socket()) break;
            if (endsocket) {
                auto startsocket = tempEdge->get_start_socket();
                //Check if adding edge makes a cycle
                if (checkCycleDetection(tempEdge->get_start_node(), node)) {
                    //g_warning("Cycle");
                    //std::cout << "Adding this edge would create a cycle!" << std::endl;    
                    alertLabel.set_visible(true);
                    Glib::signal_timeout().connect([this]() -> bool {
                        alertLabel.set_visible(false);
                        return false; // Return false to stop the timeout and prevent it from repeating
                    }, 3000);            
                    break;
                } else {
                    // Safe to add the edge.
                    auto newEdge = std::make_shared<Edge>(startsocket, tempEdge->get_start_node(), endsocket, node);
                    
                    auto action = std::make_unique<CreateEdgeAction>(*this, newEdge);
                    historyManager.executeAction(std::move(action));
                    
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
    
    else if(dragging_selected_items){
        dragging_selected_items = false;
        //queue_draw(); // Ensure one last redraw to finalize position changes
        for (auto& node : nodes) {
            node->stop_dragging();
            //node->deactivate();
        }
        
    }
    else if(rubberband_active){
        //rubberband_active = false;
        // Perform selection of nodes within rubberband area
        // ...
        //queue_draw(); // Redraw to remove the rubberband
        rubberband_active = false;

        double minX = std::min(rubberband_start_x, rubberband_end_x);
        double minY = std::min(rubberband_start_y, rubberband_end_y);
        double maxX = std::max(rubberband_start_x, rubberband_end_x);
        double maxY = std::max(rubberband_start_y, rubberband_end_y);

        // Select nodes within rubberband area
        for (auto& node : nodes) {
            auto nodeAllocation = node->get_allocation();
            if (intersectsRubberband(node->get_x(), node->get_y(), nodeAllocation.get_width(), nodeAllocation.get_height(), minX, minY, maxX, maxY)) {
                node->activate(); // Assuming activate() marks the node as selected
            }
        }

        // Select edges that have either start or end within rubberband area
        for (auto& edge : edges) {
            auto startSocket = edge->get_start_socket();
            auto endSocket = edge->get_end_socket();
            auto startNode = edge->get_start_node();
            auto endNode = edge->get_end_node();
            
            double startX = startSocket->get_x() + startNode->get_allocation().get_x() + 17, startY = startSocket->get_y() + startNode->get_allocation().get_y() + 17;
            double endX = endSocket->get_x() + endNode->get_allocation().get_x() + 17, endY = endSocket->get_y() + endNode->get_allocation().get_y() + 17;

            if (isWithinRubberband(startX, startY, minX, minY, maxX, maxY) || isWithinRubberband(endX, endY, minX, minY, maxX, maxY)) {
                edge->setSelected(true); // Assuming setSelected marks the edge as selected
            }
        }

        queue_draw();

    }
    else{
        for (auto& node : nodes) {
            if(node->is_dragging()){

                auto action = std::make_unique<MoveNodeAction>(*this, node, node->get_initial_x(), node->get_initial_y(), node->get_x(), node->get_y());
                historyManager.executeAction(std::move(action));

                node->stop_dragging();

                break;
            }
            //node->set_sensitive(true);
            //node->deactivate();
        }
    }
    queue_draw();
}

void NodeView::on_motion_notify_event(double x, double y) {
    static double last_x = x, last_y = y;

    if (panning_mode && panning_motion) {
        
        //For alternate Panning behaviour. To-Fix: Behaviour of the first node on panning.
        /*double dx = x - start_pan_x;
        double dy = y - start_pan_y;
        
        start_pan_x = x;
        start_pan_y = y;
        // Apply the delta to all selected nodes
        for (auto& node : nodes) {
            node->drag_to(node->get_x() + dx, node->get_y() + dy);
            move(*node, node->get_x(), node->get_y()); // Move the widget in the Gtk::Fixed container
        }*/
        
        double dx = x - start_pan_x;
        double dy = y - start_pan_y;

        // Update starting position
        start_pan_x = x;
        start_pan_y = y;

        // Get the parent scrolled window and its adjustments
        auto* parent = get_parent();
        auto* viewport = dynamic_cast<Gtk::Viewport*>(get_parent());
        if (viewport) {
            auto* scrolled_window = dynamic_cast<Gtk::ScrolledWindow*>(viewport->get_parent());
            if (scrolled_window) {
                auto hadjustment = scrolled_window->get_hadjustment();
                auto vadjustment = scrolled_window->get_vadjustment();
                
                hadjustment->set_value(hadjustment->get_value() + dx);
                vadjustment->set_value(vadjustment->get_value() + dy);
            }
        }
    }

    if (tempEdge) {
        tempEdge->updateTemporary(x, y);
        queue_draw();        
    }
    else if(dragging_selected_items) {
        // Calculate movement delta
        double dx = x - initial_click_x;
        double dy = y - initial_click_y;

        // Apply the delta to all selected nodes
        for (auto& node : nodes) {
            if (node->is_dragging()) {
                node->drag_to(node->get_x() + dx, node->get_y() + dy);
                move(*node, node->get_x(), node->get_y()); // Move the widget in the Gtk::Fixed container
            }
        }

        initial_click_x = x;
        initial_click_y = y;
        queue_draw();
    }
    else if(rubberband_active){        
        rubberband_end_x = x;
        rubberband_end_y = y;
        queue_draw(); // Queue a redraw to update the rubberband visually    
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
    auto itEdge = edges.begin();
    while (itEdge != edges.end()) {
        auto edge = *itEdge;
        if (edge->get_start_node() == node ||
            (edge->get_end_socket() && edge->get_end_node() == node)) {            
            m.erase((*itEdge)->get_start_socket());
            m.erase((*itEdge)->get_end_socket());               
            itEdge = edges.erase(itEdge);
        } else {
            ++itEdge;
        }
    }

    auto itNode = std::find(nodes.begin(), nodes.end(), node);
    if (itNode != nodes.end()) {
        // Remove the node widget from the Gtk::Fixed container
        remove(**itNode);
        nodes.erase(itNode);
    }    
    // redraw NodeView to reflect changes
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
        auto nextNode = edge->get_end_node(); 
        if (nextNode == base) {
            // Reached back to the start node, indicating a cycle
            return true;
        }
        if (dfs_check_cycle(nextNode, visitedNodes, base)) {
            return true; // Cycle found in recursion
        }
    }
    //optional    
    visitedNodes.erase(currentNode);

    return false;
}

bool NodeView::checkCycleDetection(std::shared_ptr<Node> base,std::shared_ptr<Node> target){
    
    std::unordered_set<std::shared_ptr<Node>> visitedNodes;    
    return dfs_check_cycle(target, visitedNodes, base);
}

bool NodeView::isWithinRubberband(double x, double y, double minX, double minY, double maxX, double maxY) {
    return x >= minX && x <= maxX && y >= minY && y <= maxY;
}

// Helper function to check if a rectangle intersects with the rubberband. To-do : Improve Logic for edge detection in rubberband
bool NodeView::intersectsRubberband(double rectX, double rectY, double rectWidth, double rectHeight, double minX, double minY, double maxX, double maxY) {
    // Check if any of the rectangle's corners are within the rubberband
    return isWithinRubberband(rectX, rectY, minX, minY, maxX, maxY) ||
           isWithinRubberband(rectX + rectWidth, rectY, minX, minY, maxX, maxY) ||
           isWithinRubberband(rectX, rectY + rectHeight, minX, minY, maxX, maxY) ||
           isWithinRubberband(rectX + rectWidth, rectY + rectHeight, minX, minY, maxX, maxY);
}

void NodeView::togglePanning(){
    rubberband_active = false;
    panning_mode = !panning_mode;
}

void NodeView::on_filter_toggled() {
    rubberband_active = false;
    //To-DO: logic to disable the effect of the filters here.
    /*if (toggle_filter.get_active()) {
        std::cout << "Adding inactive class" << std::endl;
        get_style_context()->add_class("nodeview-inactive");
    } else {
        std::cout << "Removing inactive class" << std::endl;
        get_style_context()->remove_class("nodeview-inactive");
    }*/    
}

void NodeView::on_undo_button_clicked(){
    rubberband_active = false;
    if (historyManager.canUndo()) {
        undoLabel.set_visible(true);
        Glib::signal_timeout().connect([this]() -> bool {
            undoLabel.set_visible(false);
            return false;
        }, 500);
        historyManager.undo();
        queue_draw(); 
    }
}
void NodeView::on_redo_button_clicked(){
    rubberband_active = false;
    if (historyManager.canRedo()) {
        redoLabel.set_visible(true);
        Glib::signal_timeout().connect([this]() -> bool {
            redoLabel.set_visible(false);
            return false;
        }, 500);
        historyManager.redo();
        queue_draw();
    }
}

void NodeView::addEdge(const std::shared_ptr<Edge>& edge) {
    edges.push_back(edge);    
    m[edge->get_start_socket()].push_back(edge->get_end_socket());
                    //m[endsocket].push_back(startsocket);
                    //signal changes to the end socket.
                    //dfs_propagate(startsocket,tempEdge->get_start_node()->get_value(startsocket),newEdge);
    value_changed(edge->get_start_node()->get_value(edge->get_start_socket()), edge->get_start_socket());
}

void NodeView::removeEdge(const std::shared_ptr<Edge>& edge) {
    if (m.count(edge->get_start_socket()) > 0) {        
        auto& sockets = m[edge->get_start_socket()];
        auto it = std::remove(sockets.begin(), sockets.end(), edge->get_end_socket());
        sockets.erase(it, sockets.end());
    }
    edges.erase(std::remove(edges.begin(), edges.end(), edge), edges.end()); 
}