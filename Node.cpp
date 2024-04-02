#include "Node.h"
#include "Socket.h"
#include "Edge.h"

Node::Node(const std::string& title, double x, double y, int width, int height, bool spin, bool color, bool switch1_, bool check, NodeType type)
    : title_(title), x_(x), y_(y), width(width), height(height), dragging_(false),
       spin(spin), color(color), switch1_(switch1_), check(check), activated(false), type(type)  {
    
    set_name("node");
    int i = 0;
    set_column_spacing(10);
    set_row_spacing(10);
    setUpBackgroundColor();   
    //set_column_homogeneous(true);
    //set_tooltip_text("This is a node tooltip: " + title_);
    label_ = Gtk::make_managed<Gtk::Label>();
    label_->set_text(title);    
    label_->set_hexpand(true); 
    label_->set_halign(Gtk::Align::FILL);
    
    delete_button_ = Gtk::make_managed<Gtk::Button>();
    delete_button_->set_image_from_icon_name("edit-delete");    
    delete_button_->set_has_frame(false);
    delete_button_->set_halign(Gtk::Align::CENTER);
    delete_button_->set_valign(Gtk::Align::CENTER);
    delete_button_->set_name("my-delete-button");
    tooltip_button_ = Gtk::make_managed<Gtk::Button>();
    tooltip_button_->set_has_frame(false);
    tooltip_button_->set_halign(Gtk::Align::CENTER);
    tooltip_button_->set_valign(Gtk::Align::CENTER);
    tooltip_button_->set_image_from_icon_name("help-about");
    tooltip_button_->set_tooltip_text("This is a node tooltip: " + title_);
    tooltip_button_->set_name("my-tooltip-button");
    //tooltip_button_->set_sensitive(false);
    /*duplicate_button_ = Gtk::make_managed<Gtk::Button>();
    duplicate_button_->set_has_frame(false);
    duplicate_button_->set_halign(Gtk::Align::CENTER);
    duplicate_button_->set_valign(Gtk::Align::CENTER);
    duplicate_button_->set_image_from_icon_name("edit-copy");*/
    //attach(*label_,1,i,1,1);
    //attach(*delete_button_, 2, i, 1, 1); 
    //attach(*tooltip_button_,0,i,1,1);
    //Gtk::Box layout_;
    layout_ = Gtk::make_managed<Gtk::Box>();
    layout_->set_orientation(Gtk::Orientation::HORIZONTAL);
    
    layout_->append(*tooltip_button_);
    //layout_->append(*duplicate_button_);
    layout_->append(*label_);
    layout_->append(*delete_button_);    
    layout_->set_spacing(5);
    layout_->set_expand(true);
    //layout_->set_name("box11");
    //layout_->add_css_class("box1");
    attach(*layout_, 0, i, 3, 1);
    i++;
    
    if(spin){
        auto socketl = std::make_shared<Socket>(0,i,Socket::Type::INPUT, Socket::Data::SPIN);
        sockets.push_back(socketl);
        attach(*socketl, 0, i, 1, 1);
        socketl->set_halign(Gtk::Align::CENTER); // Align to the left edge


        spin_button_ = Gtk::make_managed<Gtk::SpinButton>();
        spin_button_->set_hexpand(true); // Allow it to expand
        //spin_button_.set_halign(Gtk::Align::FILL); // Fill the space
        spin_button_->set_range(0.0, 100.0); // Range of the spin button
        spin_button_->set_increments(1.0, 10.0); // Single and page increments
        spin_button_->set_value(50.0); // Default value
        //spin_button_.set_size_request(width,-1);
        //layout_.append(spin_button_);
        attach(*spin_button_,1,i,1,1);

        auto socketr = std::make_shared<Socket>(2,i,Socket::Type::OUTPUT, Socket::Data::SPIN);
        sockets.push_back(socketr);
        attach(*socketr, 2, i, 1, 1);
        socketr->set_halign(Gtk::Align::CENTER); // Align to the right edge

        socketl->set_valign(Gtk::Align::CENTER);
        spin_button_->set_valign(Gtk::Align::CENTER);
        socketr->set_valign(Gtk::Align::CENTER);
        
        i++;
        m4[spin_button_] = socketr;
        sibling[socketr] = socketl;        
    }
   
    if(color){
        
        auto socketl = std::make_shared<Socket>(0,i,Socket::Type::INPUT, Socket::Data::COLOR);
        sockets.push_back(socketl);
        attach(*socketl, 0, i, 1, 1);
        socketl->set_halign(Gtk::Align::CENTER); // Align to the left edge

        color_button_ = Gtk::make_managed<Gtk::ColorButton>();
        color_button_->set_hexpand(true); // Allow it to expand
        //color_button_.set_halign(Gtk::Align::FILL); // Fill the space
        attach(*color_button_, 1, i, 1, 1);

        // Right socket - Align to end
        auto socketr = std::make_shared<Socket>(2,i,Socket::Type::OUTPUT, Socket::Data::COLOR);
        sockets.push_back(socketr);
        attach(*socketr, 2, i, 1, 1);
        socketr->set_halign(Gtk::Align::CENTER); // Align to the right edge

        socketl->set_valign(Gtk::Align::CENTER);
        color_button_->set_valign(Gtk::Align::CENTER);
        socketr->set_valign(Gtk::Align::CENTER);
        i++;
        m1[color_button_] = socketr;
        sibling[socketr] = socketl;
        //map[socketl] = color_button_;
        //map[socketr] = color_button_;
    }
    
    
    if(switch1_){
        auto socketl = std::make_shared<Socket>(0,i,Socket::Type::INPUT, Socket::Data::SWITCH);
        sockets.push_back(socketl);
        attach(*socketl, 0, i, 1, 1);
        socketl->set_halign(Gtk::Align::CENTER); // Align to the left edge

        switch_ = Gtk::make_managed<Gtk::Switch>();
        switch_->set_hexpand(true); // Allow it to expand
        //switch_.set_halign(Gtk::Align::FILL); // Fill the space
        switch_->set_state(false); // Initial state
        switch_->set_size_request(width,-1);
        //layout_.append(switch_);
        attach(*switch_, 1, i, 1, 1);


        auto socketr = std::make_shared<Socket>(2,i,Socket::Type::OUTPUT, Socket::Data::SWITCH);
        sockets.push_back(socketr);
        attach(*socketr, 2, i, 1, 1);
        socketr->set_halign(Gtk::Align::CENTER); // Align to the right edge

       
        socketl->set_valign(Gtk::Align::CENTER);
        switch_->set_valign(Gtk::Align::CENTER);
        socketr->set_valign(Gtk::Align::CENTER);
        i++;
        m3[switch_] = socketr;
        sibling[socketr] = socketl;
    }
    
    if(check){
        auto socketl = std::make_shared<Socket>(0,i,Socket::Type::INPUT, Socket::Data::CHECK);
        sockets.push_back(socketl);
        attach(*socketl, 0, i, 1, 1);
        socketl->set_halign(Gtk::Align::CENTER); // Align to the left edge

        check_button_ = Gtk::make_managed<Gtk::CheckButton>();
        check_button_->set_hexpand(true); // Allow it to expand
        //check_button_.set_halign(Gtk::Align::FILL); // Fill the space
        check_button_->set_label("tick");
        //check_button_.set_size_request(width,-1);
        attach(*check_button_,1,i,1,1);
        
        auto socketr = std::make_shared<Socket>(0,i,Socket::Type::OUTPUT, Socket::Data::CHECK);
        sockets.push_back(socketr);
        attach(*socketr, 2, i, 1, 1);
        socketr->set_halign(Gtk::Align::CENTER); // Align to the right edge
        
        socketl->set_valign(Gtk::Align::CENTER);
        check_button_->set_valign(Gtk::Align::CENTER);
        socketr->set_valign(Gtk::Align::CENTER);
        i++;
        m2[check_button_] = socketr;
        sibling[socketr] = socketl;
    }



    if(spin){
        spin_button_->signal_value_changed().connect(sigc::mem_fun(*this, &Node::on_spin_button_changed));
        //spin_button_->signal_focus_out_event().connect(sigc::mem_fun(*this, &Node::on_spin_button_focus_out));
        spin_button_->set_can_focus(false);

    }
    if(color) color_button_->signal_color_set().connect(sigc::mem_fun(*this, &Node::on_color_set));
    if(check) check_button_->signal_toggled().connect(sigc::mem_fun(*this, &Node::on_check_button_toggled));
    //switch_.signal_state_set().connect(sigc::mem_fun(*this, &Node::on_switch_state_changed));
    if(switch1_) switch_->property_state().signal_changed().connect(sigc::mem_fun(*this, &Node::on_switch_state_flags_changed));
    //color_button2.signal_color_set().connect(sigc::mem_fun(*this, &Node::on_color_set));

    delete_button_->signal_clicked().connect(sigc::mem_fun(*this, &Node::on_delete_button_clicked));
    tooltip_button_->signal_clicked().connect([this]() {
        stop_dragging();  // Show the popover when the button is clicked
    });

    auto css_provider = Gtk::CssProvider::create();
    auto style_context = get_style_context();

    const char* css_style = R"(
             
        grid {            
            border: 2px solid #000000;
            padding: 15px;
            border-radius: 10px;
            background-color: #ffffff; 
            color: #000000;
            font-size: 16px;
        }          
        .active-node {
            border-color: #2c88d9; /* Example: Change border color to indicate active state */
            /*border-width: 3px;*/ /* Optionally, make the border thicker */
        }             
        
    )";
    
    css_provider->load_from_data(css_style);
    style_context->add_provider(css_provider, GTK_STYLE_PROVIDER_PRIORITY_USER);
    //queue_draw();  

    auto css_provider2 = Gtk::CssProvider::create();
    css_provider2->load_from_data(R"(
        box {
            background-color: )" + background_color + R"(; /* Variable color */
            color: white;
            border: 1px solid #000000;
            border-radius: 5px;
            
        }
    )");
    
    auto style_context2 = layout_->get_style_context();   
    style_context2->add_provider(css_provider2, GTK_STYLE_PROVIDER_PRIORITY_USER); 
}

void Node::on_delete_button_clicked() {    
    stop_dragging();
    signal_delete_node.emit(shared_from_this());
    
}
std::string Node::getLabel(){
    return title_;
}
bool Node::is_inside(double x, double y) const {
    auto allocation = get_allocation();
    return x >= x_ && x <= x_ + allocation.get_width() && y >= y_ && y <= y_ + allocation.get_height();
}

void Node::start_dragging(double x, double y) {
    initial_x_ = x_;
    initial_y_ = y_;
    dragging_ = true;
    drag_offset_x_ = x - x_;
    drag_offset_y_ = y - y_;
}

void Node::drag_to(double x, double y) {
    if (dragging_) {
        x_ = x - drag_offset_x_;
        y_ = y - drag_offset_y_;        
    }
}

void Node::stop_dragging() {
    dragging_ = false;
}

bool Node::is_dragging() const {
    return dragging_;
}

double Node::get_x() const{
    return x_;
}
double Node::get_y() const{
    return y_;
}

void Node::enableDragging(){
    dragging_ = true;
}
void Node::disableDragging(){
    dragging_ = false;
}
int Node::get_height(){
    return height;
}
int Node::get_width(){
    return width;
}

double Node::get_initial_x(){
    return initial_x_;
}

double Node::get_initial_y(){
    return initial_y_;
}

void Node::on_color_set() {
    
    stop_dragging();
    auto socket = m1[color_button_];
    std::string colorValue = color_button_->get_rgba().to_string();
    signal_value_changed(colorValue, socket);
}

void Node::on_check_button_toggled() {
   
    
    stop_dragging();
    auto socket = m2[check_button_];
    //std::string colorValue = color_button_->get_rgba().to_string();
    signal_value_changed(std::to_string(check_button_->get_active()), socket);
    //signal_value_changed.emit("check");
}

void Node::on_switch_state_flags_changed() {
    
    stop_dragging();    
    //signal_value_changed.emit("switch");
    auto socket = m3[switch_];
    //std::string colorValue = color_button_->get_rgba().to_string();
    signal_value_changed(std::to_string(switch_->get_state()), socket);
    //return true;
}


void Node::on_spin_button_changed(){
    
    stop_dragging();
    auto socket = m4[spin_button_];
    //std::string colorValue = color_button_->get_rgba().to_string();
    signal_value_changed(std::to_string(spin_button_->get_value()), socket);
    //signal_value_changed.emit("spin");
}
/*void Node::on_spin_button_focus_out_event(){
    
    //auto* parent = get_parent();
        
    auto* parent = dynamic_cast<Gtk::Fixed*>(get_parent());
    parent->grab_focus();
    //signal_value_changed.emit("spin");
}*/
Gtk::Allocation Node::return_allocation(){
    return this->get_allocation();
}

//std::pair<bool,std::pair<double,double>> Node::is_inside_socket(double x1, double y1){
std::shared_ptr<Socket> Node::is_inside_socket(double x1, double y1){
    //std::cout << "clicking at x: "<<x1<<" ,y: "<<y1 <<std::endl;
    
    auto a = get_allocation();
    for(auto socket: sockets){
        auto s = socket->get_allocation();
        //std::cout<<"socket: "<<s.get_x() + a.get_x()<< " " << s.get_y()+a.get_y() << std::endl;
        double x = s.get_x() + a.get_x();
        double y = s.get_y() + a.get_y();
        if(x1>=x-5+17 && x1<=x+5+17 &&y1>=y-5+17&&y1<=y+5+17){
            //std::cout << "Inside" << std::endl;
            //return {true, {x+17,y+17}};
            return socket;
        }
    }    
    //return {false, {0,0}};
    return nullptr;
}


void Node::activate() {
    auto style_context = get_style_context();
    style_context->add_class("active-node");
    activated = true;
}

void Node::deactivate() {
    auto style_context = get_style_context();
    style_context->remove_class("active-node");
    activated = false;
}
bool Node::isActivated(){
    return activated;
}
void Node::get_children_data(){
    std::cout << title_ << std::endl;
    /*if(spin){
        std::cout << spin_button_->get_value() << std::endl;
    }
    if(color){
        std::cout << color_button_->get_rgba().to_string() << std::endl;
    }
    if(switch1_){
        std::cout << switch_->get_state() << std::endl;
    }
    if(check){
        std::cout << check_button_->get_active() << std::endl;
    }*/
    /*for(auto s:incoming_data){
        std::cout << s << ", ";
    }*/
    std::cout << incoming_data << std::endl;
    //std::cout << std::endl;
}


std::string Node::get_value(std::shared_ptr<Socket> socket){
    int r = socket->get_row();
    int c = socket->get_col();
    
    Socket::Type type = socket->get_type();
    int adjcol = (type == Socket::Type::INPUT) ? c + 1 : c - 1;
    //std::cout << "Socket: " << r <<", " << c << ", ";
    //std::cout << "Element: " << r <<", " << adjcol;
    auto e = get_child_at(adjcol,r);
    /*if (!e) {
        std::cout << "Widget not found at position (" << adjcol << ", " << r << ")\n";
    } else {
        std::cout << "Widget found. Checking type...\n";
    }*/

    if(spin && e == spin_button_){
        
        return std::to_string(spin_button_->get_value());
    }
    if(color && e == color_button_){
        return color_button_->get_rgba().to_string();
    }
    if(switch1_ && e == switch_){
        return std::to_string(switch_->get_state());
    }
    if(check && e == check_button_){
        return std::to_string(check_button_->get_active());
    }

    return "";
}
void Node::push_state(std::string incoming){
    //incoming_data.clear();
    incoming_data = incoming;

}

std::shared_ptr<Socket> Node::get_socket_sibling(std::shared_ptr<Socket> socket){
    
    return sibling[socket];
}


void Node::socket_value_changed(std::shared_ptr<Socket> socket, std::string value){
    int r = socket->get_row();
    int c = socket->get_col();
    
    Socket::Type type = socket->get_type();
    int adjcol = (type == Socket::Type::INPUT) ? c + 1 : c - 1;
    //std::cout << "Socket: " << r <<", " << c << ", ";
    //std::cout << "Element: " << r <<", " << adjcol;
    auto e = get_child_at(adjcol,r);
    
    if(spin && e == spin_button_){
        if(value.length()!=0){
            spin_button_->set_value(spin_button_->get_value()+1);
            //on_spin_button_changed();
        }
        
    }
    if(color && e == color_button_){
        if(value.length()!=0){
            Gdk::RGBA color;
            double x = 0.001;
            color.set_rgba(1.0, 0.647 + x, 0.0, 1.0); // RGBA for orange

            color_button_->set_rgba(color);
            //color_button_->set_rgba()
            //on_color_set();
        }
    }
    if(switch1_ && e == switch_){
        if(value.length()!=0){
            //switch_->set_state(!switch_->get_active());
            bool current_state = switch_->property_active().get_value();

            if(current_state) switch_->property_active().set_value(false);
            else switch_->property_active().set_value(true);
            //on_switch_state_flags_changed();
        }
    }
    if(check && e == check_button_){
        if(value.length()!=0){
            /*bool current_state = check_button_->property_active().get_value();

            if(current_state) check_button_->property_active().set_value(false);
            else check_button_->property_active().set_value(true);*/
            //check_button_->queue_draw();
            check_button_->set_active(!(check_button_->get_active()));
            //on_check_button_toggled();
        }
    }

    
}

void Node::setUpBackgroundColor(){
    if(type == Node::NodeType::A){
        background_color = "#d3455b";
    }
    else if(type == Node::NodeType::B){
        background_color = "#f7c325";
    }
    else if(type == Node::NodeType::C){
        background_color = "#1aae9f";
    }
    else{
        background_color = "#e8833a";
    }
}

void Node::set_new_position(double x, double y){
    x_ = x;
    y_ = y;
    //signal_position_changed.emit(shared_from_this(),x_,y_);
}