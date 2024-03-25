#include <gtkmm.h>
#include "Node.h"
#include<memory>
#include "Edge.h"
#include<iostream>
#include "Socket.h"
#include <unordered_set>
class NodeView : public Gtk::Fixed {
public:
    NodeView();    
    void addNode(const std::string& title, double x, double y, int width, int height,bool spin, bool color, bool switch1_, bool check);    
    void delete_node(std::shared_ptr<Node> node);
    void value_changed(std::string value, std::shared_ptr<Socket> socket);
    void on_add_node_button_clicked();
    void delete_selected_edge();
    void reveal_node_info();
    void dfs_propagate(std::shared_ptr<Socket> startSocket, const std::string& value, std::shared_ptr<Edge> edge);

    bool checkCycleDetection(std::shared_ptr<Node> base,std::shared_ptr<Node> target);

    bool dfs_check_cycle(const std::shared_ptr<Node>& currentNode,
    std::unordered_set<std::shared_ptr<Node>>& visitedNodes,
    const std::shared_ptr<Node>& base); 
protected:
    void snapshot_vfunc(const Glib::RefPtr<Gtk::Snapshot>& snapshot) override;
    void on_button_press_event(int, double x, double y) ;
    void on_button_release_event(int, double x, double y) ;
    void on_motion_notify_event(double x, double y) ;
    bool on_key_press_event(guint keyval, guint keycode, Gdk::ModifierType state);
private:    
    std::vector<std::shared_ptr<Node>> nodes;
    std::vector<std::shared_ptr<Edge>> edges;
    std::shared_ptr<Edge> tempEdge;
    std::vector<std::shared_ptr<Socket>> sockets;
    Gtk::Button add_node_button;
    Gtk::Button delete_edge_button;
    std::unordered_map<std::shared_ptr<Socket>, std::vector<std::shared_ptr<Socket>>> m;
    Gtk::Button node_details_button;
    std::unordered_set<std::shared_ptr<Socket>> visited; 
};

