#pragma once

#include <gtkmm.h>
#include <iostream>
class Node;
class Socket : public Gtk::Widget{
public:
    enum class Type { INPUT, OUTPUT };
    enum class Data { SPIN, COLOR, SWITCH, CHECK };
    //Socket(std::shared_ptr<Node> parent, const std::string& id, Position position);  
    Socket(int col, int row, Type type, Data data);  
    void set_x(double x);
    void set_y(double y);
    double get_x();
    double get_y();
    void snapshot_vfunc(const Glib::RefPtr<Gtk::Snapshot>& snapshot);
    //void calculate_position();
    bool is_inside(double x1,double y1);
    std::string get_data();
    int get_row();
    int get_col();
    Type get_type();
    //Node* get_socket_parent();
protected:
    
private:
    //std::shared_ptr<Node> parent_node_;
    int row, col;
    std::string id_;
    Type type;
    Data data;
    double x_, y_;
    static constexpr double radius_ = 5.0;  
     
};
