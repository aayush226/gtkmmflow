#include "Socket.h"
#include "Node.h"


Socket::Socket(int col, int row, Type type, Data data) : col(col), row(row), type(type), data(data)
{
}


void Socket::snapshot_vfunc(const Glib::RefPtr<Gtk::Snapshot>& snapshot){

    double outer_radius_ = 7.0;
    double inner_radius_ = 2.0;
    Gdk::Rectangle bounds_outer(0 - outer_radius_, 0 - outer_radius_, outer_radius_ * 2, outer_radius_ * 2);
    auto cr_outer = snapshot->append_cairo(bounds_outer);
    
    // Draw the outer circle
    cr_outer->arc(x_, y_, outer_radius_, 0, 2 * M_PI);
    if(type == Socket::Type::INPUT) cr_outer->set_source_rgba(0,0,0,0.7);   
    else cr_outer->set_source_rgba(0,0,0,0.7);
    cr_outer->fill(); 
    
    cr_outer->stroke();
    
    /*Gdk::Rectangle bounds_inner(0 - inner_radius_, 0 - inner_radius_, inner_radius_ * 2, inner_radius_ * 2);
    auto cr_inner = snapshot->append_cairo(bounds_inner);

    // Draw the inner circle (fill)
    cr_inner->arc(x_, y_, inner_radius_, 0, 2 * M_PI);
    cr_inner->set_source_rgb(1,1,1); // Use a visible color for testing    
    
    cr_inner->fill(); 
    
    cr_inner->stroke()*/
}

void Socket::set_x(double x1){
    x_ = x1;
    
}
void Socket::set_y(double y1){
    y_ = y1;
    
}

bool Socket::is_inside(double x1,double y1){
    auto a = get_allocation();    
    return x1 >= a.get_x() - 5 && x1 <= a.get_y() + 5 && y1 >= a.get_y() - 5 && y1 <= a.get_y() + 5;
}

double Socket::get_x(){
    double x_ = get_allocation().get_x();    
    return x_;
}
double Socket::get_y(){
    double y_ = get_allocation().get_y();       
    //double y_ = s.get_y() + parent_node_->get_allocation().get_y();
    //double y = s.get_y() + a.get_y();        
    return y_;
}

std::string Socket::get_data(){

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
            return color_button->get_rgba().to_string();
        }
    }
    return "";
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