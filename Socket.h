#pragma once

#include <gtkmm.h>
#include <iostream>
class Node;
class Socket : public Gtk::Widget{
public:
    enum class Type { INPUT, OUTPUT };
    enum class Data { SPIN, COLOR, SWITCH, CHECK };
      
    Socket(int col, int row, Type type, Data data); 

    void set_x(double x);
    void set_y(double y);
    double get_x();
    double get_y();

    void snapshot_vfunc(const Glib::RefPtr<Gtk::Snapshot>& snapshot);
    
    bool is_inside(double x1,double y1);
    std::string get_data();
    
    int get_row();
    int get_col();
    Type get_type();

protected:
    
private:    
    int row, col;
    std::string id_;
    
    Type type;
    Data data;
    
    double x_, y_;
    static constexpr double radius_ = 5.0;     
};
