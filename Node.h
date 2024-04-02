#pragma once

#include <gtkmm.h>
#include "Socket.h"
#include <sigc++/sigc++.h>
#include<memory>
//#include<unordered_map>

//#include "Edge.h"
//class Socket;
class Edge;
class Node : public Gtk::Grid, public std::enable_shared_from_this<Node> {
public:
    enum class NodeType { A, B, C, D }; //depending on type of filter

    Node(const std::string& title, double x, double y, int width, int height, bool spin, bool color, bool switch_, bool check, NodeType type);
    //void snapshot_vfunc(const Glib::RefPtr<Gtk::Snapshot>& snapshot) override;
    
    bool is_inside(double x, double y) const;
    
    void start_dragging(double x, double y);
    void drag_to(double x, double y);
    void stop_dragging();
    bool is_dragging() const;
    
    size_t getSocketCount() const;
    std::pair<double, double> getSocketPosition(int index) const;
    //void updateSockets();
    double get_x() const;
    double get_y() const;
    void enableDragging();
    void disableDragging();
    int get_width();
    int get_height();
    void on_color_set();
    void on_check_button_toggled();
    void on_spin_button_changed();
    //void on_spin_button_focus_out_event();
    void on_switch_state_flags_changed();
    Gtk::Allocation return_allocation();
    
    std::shared_ptr<Socket> is_inside_socket(double x, double y);
    void activate();
    void deactivate();
    bool isActivated();
    sigc::signal<void(std::shared_ptr<Node>)> signal_delete_node;
    sigc::signal<void(std::string, std::shared_ptr<Socket>)> signal_value_changed;
    //sigc::signal<void(std::shared_ptr<Node>, double x_, double y_)> signal_position_changed;
    void on_delete_button_clicked();
    std::string getLabel();
    void get_children_data();
    std::string get_value(std::shared_ptr<Socket> socket);
    std::unordered_map<std::shared_ptr<Socket>, Gtk::ColorButton> map;
    std::unordered_map<Gtk::ColorButton*, std::shared_ptr<Socket>> m1;
    std::unordered_map<Gtk::CheckButton*, std::shared_ptr<Socket>> m2;
    std::unordered_map<Gtk::Switch*, std::shared_ptr<Socket>> m3;
    std::unordered_map<Gtk::SpinButton*, std::shared_ptr<Socket>> m4;
    void socket_value_changed(std::shared_ptr<Socket> startSocket, std::string value);
    void get_connected_data();
    void push_state(std::string incoming);
    std::string incoming_data;
    void setUpBackgroundColor();
    std::shared_ptr<Socket> get_socket_sibling(std::shared_ptr<Socket> socket);

    double get_initial_x();
    double get_initial_y();
    void set_new_position(double x, double y);
protected:
    
private:
    NodeType type;
    std::string title_;
    double x_, y_;
    int width, height;
    bool dragging_;
    double drag_offset_x_, drag_offset_y_;
    double initial_x_, initial_y_;
    std::vector<std::shared_ptr<Socket>> sockets;
   
    Gtk::SpinButton* spin_button_;   
    Gtk::ColorButton* color_button_; 
    Gtk::Switch* switch_;
    Gtk::CheckButton* check_button_; 
    
    bool spin, color, switch1_, check;
    
    std::vector<std::shared_ptr<Edge>> edges;
    std::shared_ptr<Edge> tempEdge;
    Gtk::Button* delete_button_;
    Gtk::Button* tooltip_button_;
    Gtk::Button* duplicate_button_;
    Gtk::Label* label_;
    Gtk::SpinButton* spin_button1;
    Gtk::SpinButton* spin_button2;
    Gtk::SpinButton* spin_button3;
    Gtk::Box* layout_;
    std::unordered_map<std::shared_ptr<Socket>,std::shared_ptr<Socket>> sibling;
    bool activated;
    std::string background_color;
};
