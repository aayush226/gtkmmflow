#include "Socket.h"
#include "Node.h"


/*Socket::Socket(std::shared_ptr<Node> parent, const std::string& id, Position position)
    : parent_node_(parent), id_(id), position_(position) {
    calculate_position();
    
}*/

//Socket::Socket(Node* parent) : parent_node_(parent) {
Socket::Socket(int col, int row, Type type, Data data) : col(col), row(row), type(type), data(data){
    /*auto a = get_allocation();
    std::cout<< a.get_x() << " " << a.get_y() << std::endl;*/
}


void Socket::snapshot_vfunc(const Glib::RefPtr<Gtk::Snapshot>& snapshot){

    /*Gdk::Rectangle bounds(0 - radius_, 0 - radius_, radius_ * 2, radius_ * 2);
    auto cr = snapshot->append_cairo(bounds);  
    
    cr->arc(x_, y_, radius_, 0, 2 * M_PI);
    cr->set_source_rgb(0.0, 0.0, 0.0); // Use a visible color for testing    
    
    cr->fill(); 
    
    cr->stroke();    */
    double outer_radius_ = 7.0;
    double inner_radius_ = 2.0;
    Gdk::Rectangle bounds_outer(0 - outer_radius_, 0 - outer_radius_, outer_radius_ * 2, outer_radius_ * 2);
    auto cr_outer = snapshot->append_cairo(bounds_outer);
    
    // Draw the outer circle (stroke)
    cr_outer->arc(x_, y_, outer_radius_, 0, 2 * M_PI);
    if(type == Socket::Type::INPUT) cr_outer->set_source_rgb(0,0,1); // Use a visible color for testing    
    else cr_outer->set_source_rgb(0,1,0);
    cr_outer->fill(); 
    
    cr_outer->stroke();

    // Determine the bounds of the inner circle
    Gdk::Rectangle bounds_inner(0 - inner_radius_, 0 - inner_radius_, inner_radius_ * 2, inner_radius_ * 2);
    auto cr_inner = snapshot->append_cairo(bounds_inner);

    // Draw the inner circle (fill)
    cr_inner->arc(x_, y_, inner_radius_, 0, 2 * M_PI);
    cr_inner->set_source_rgb(1,1,1); // Use a visible color for testing    
    
    cr_inner->fill(); 
    
    cr_inner->stroke();
}
/*void Socket::calculate_position() {
    

    double node_x = parent_node_->get_x();
    double node_y = parent_node_->get_y();
    int node_width = parent_node_->get_width();
    int node_height = parent_node_->get_height();
    
    int border_thickness = 2; // Border thickness from your CSS
    int padding = 15; // Padding from your CSS

    int adjustmentForRightSocket = border_thickness + padding;
    
    x_ = (position_ == Position::LEFT)?node_x-5:node_x+ node_width + 90;
    y_ = (position_ == Position::LEFT)?node_y + node_height / 2.0 - radius_ : node_y + node_height / 2.0 - radius_;
}*/
//146,193
void Socket::set_x(double x1){
    x_ = x1;
    
}
void Socket::set_y(double y1){
    y_ = y1;
    
}


bool Socket::is_inside(double x1,double y1){
    auto a = get_allocation();
    //std::cout << a.get_x() <<" "<<a.get_y()<<" "<< std::endl;
    return x1 >= a.get_x() - 5 && x1 <= a.get_y() + 5 && y1 >= a.get_y() - 5 && y1 <= a.get_y() + 5;
}

/*Node* Socket::get_socket_parent(){
    return parent_node_;
}*/

double Socket::get_x(){
    double x_ = get_allocation().get_x();
    //std::cout<<"socket: x: "<<s.get_x() + parent_node_->get_allocation().get_x()<< " y: " << s.get_y()+parent_node_->get_allocation().get_y() << std::endl;
        
    //double x_ = s.get_x() + parent_node_->get_allocation().get_x();
        //double y = s.get_y() + a.get_y();
    return x_;
}
double Socket::get_y(){
    double y_ = get_allocation().get_y();
        //std::cout<<"socket: "<<s.get_x() + a.get_x()<< " " << s.get_y()+a.get_y() << std::endl;
    //double y_ = s.get_y() + parent_node_->get_allocation().get_y();
        //double y = s.get_y() + a.get_y();
        
    return y_;
}

std::string Socket::get_data(){

    /*int adjcol = (type == Socket::Type::INPUT) ? col + 1 : col - 1;

        // Use Gtk::Grid's child_at method to get the widget at the specified position
    
    auto grid_parent = dynamic_cast<Gtk::Grid*>(get_parent());
    auto element = grid_parent->get_child_at(adjcol, row);
    if(data == Socket::Data::CHECK){
        return dynamic_cast<Gtk::CheckButton*>(element)->get_active();
    }
    else if(data == Socket::Data::SPIN){
        return dynamic_cast<Gtk::SpinButton*>(element)->get_value();
    }
    else if(data == Socket::Data::SWITCH){
        return dynamic_cast<Gtk::Switch*>(element)->get_state() == 1 ? "True" : "False";
    }
    else{
        return dynamic_cast<Gtk::ColorButton*>(element)->get_rgba().to_string();
    }*/
    int adjcol = (type == Socket::Type::INPUT) ? col + 1 : col - 1;

    auto* grid_parent = dynamic_cast<Gtk::Grid*>(get_parent());
    if (!grid_parent) {
        std::cerr << "Failed to get grid parent\n";
        return "";
    }

    auto* element = grid_parent->get_child_at(adjcol, row);
    if (!element) {
        std::cerr << "Failed to get element at position\n";
        return "";
    }
    
    if (data == Socket::Data::CHECK) {
        auto* check_button = dynamic_cast<Gtk::CheckButton*>(element);
        if (!check_button) {
            std::cerr << "Element is not a Gtk::CheckButton\n";
            return "";
        }
        if (check_button) {
            return std::to_string(check_button->get_active());
        }
    } else if (data == Socket::Data::SPIN) {
        auto* spin_button = dynamic_cast<Gtk::SpinButton*>(element);
        if (spin_button) {
            return std::to_string(spin_button->get_value());
        }
    } else if (data == Socket::Data::SWITCH) {
        auto* switch_widget = dynamic_cast<Gtk::Switch*>(element);
        if (switch_widget) {
            return std::to_string(switch_widget->get_state());
        }
    } else if (data == Socket::Data::COLOR) {
        auto* color_button = dynamic_cast<Gtk::ColorButton*>(element);
        if (color_button) {
            //Gdk::RGBA rgba = 
            // Manually format the RGBA values into a string
            //char rgba_str[256];
            //snprintf(rgba_str, sizeof(rgba_str), "%.2f,%.2f,%.2f,%.2f", rgba.get_red(), rgba.get_green(), rgba.get_blue(), rgba.get_alpha());
            return color_button->get_rgba().to_string();
        }
    }
    return "jingu";
}

int Socket::get_row(){
    return row;
}
int Socket::get_col(){
    return col;
}
Socket::Type Socket::get_type(){
    return type;
}