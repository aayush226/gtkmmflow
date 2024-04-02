#pragma once
#include <gtkmm.h>
#include "Node.h"
#include<memory>
#include "Edge.h"
#include<iostream>
#include "Socket.h"
#include <unordered_set>
#include "HistoryManager.h"

class NodeView : public Gtk::Fixed {
public:
    NodeView();    
    
    void addNode(const std::string& title, double x, double y, int width, int height,bool spin, bool color, bool switch1_, bool check, Node::NodeType type);    
    void delete_node(std::shared_ptr<Node> node);
    
    void value_changed(std::string value, std::shared_ptr<Socket> socket);
    
    void on_filter_toggled();
    void on_add_node_button_clicked();
    void delete_selected_edge();
    void reveal_node_info();
    void on_undo_button_clicked();
    void on_redo_button_clicked();

    void dfs_propagate(std::shared_ptr<Socket> startSocket, const std::string& value, std::shared_ptr<Edge> edge);
    
    bool isWithinRubberband(double x, double y, double minX, double minY, double maxX, double maxY);
    bool intersectsRubberband(double rectX, double rectY, double rectWidth, double rectHeight, double minX, double minY, double maxX, double maxY);
    
    bool checkCycleDetection(std::shared_ptr<Node> base,std::shared_ptr<Node> target);

    bool dfs_check_cycle(const std::shared_ptr<Node>& currentNode, std::unordered_set<std::shared_ptr<Node>>& visitedNodes, const std::shared_ptr<Node>& base); 
    
    bool on_scroll_zoom(double dx, double dy);
    
    void togglePanning();
    
    void addEdge(const std::shared_ptr<Edge>& edge);
    void removeEdge(const std::shared_ptr<Edge>& edge);
    
protected:
    void snapshot_vfunc(const Glib::RefPtr<Gtk::Snapshot>& snapshot) override;
    
    void on_button_press_event(int, double x, double y) ;
    void on_button_release_event(int, double x, double y) ;
    
    void on_motion_notify_event(double x, double y) ;
    
    bool on_key_press_event(guint keyval, guint keycode, Gdk::ModifierType state);
    void on_key_release_event(guint keyval, guint keycode, Gdk::ModifierType state);

private:    
    
    std::vector<std::shared_ptr<Node>> nodes;
    std::vector<std::shared_ptr<Edge>> edges;
    std::shared_ptr<Edge> tempEdge;
    std::vector<std::shared_ptr<Socket>> sockets;

    Gtk::Button node_details_button;
    Gtk::Button add_node_button;
    Gtk::Button delete_edge_button;
    Gtk::Button pan_button;
    Gtk::CheckButton toggle_filter;
    Gtk::Button settings_button_;
    Gtk::Button new_filter_button;
    Gtk::Button filter_setting_button_;
    Gtk::Button undo_button_;
    Gtk::Button redo_button_;
    
    std::unordered_map<std::shared_ptr<Socket>, std::vector<std::shared_ptr<Socket>>> m;
    std::unordered_set<std::shared_ptr<Socket>> visited; 

    double zoom_factor = 1.0; //havent implemented yet. 

    bool rubberband_active = false;
    double rubberband_start_x = 0, rubberband_start_y = 0;
    double rubberband_end_x = 0, rubberband_end_y = 0;

    bool dragging_selected_items = false;    
    double initial_click_x = 0, initial_click_y = 0;

    bool panning_mode = false;    
    double start_pan_x = 0.0;
    double start_pan_y = 0.0;
    bool panning_motion = false;

    Gtk::Label alertLabel;
    Gtk::Label undoLabel;
    Gtk::Label redoLabel;
    HistoryManager historyManager;
};

