#ifndef EDGE_H
#define EDGE_H

#include "Node.h"
#include <gtkmm.h>
#include <memory>
class Socket;
class Edge {
public:
    //Edge(double startX, double startY, double tempX, double tempY);
    //Edge(double startX, double startY, double tempX, double tempY, bool perm);
    Edge(std::shared_ptr<Socket> startSocket, std::shared_ptr<Node> startNode, double tempX, double tempY);
    //Edge(std::shared_ptr<Socket> endSocket);
    Edge(std::shared_ptr<Socket> startSocket, std::shared_ptr<Node> startNode, std::shared_ptr<Socket> endSocket, std::shared_ptr<Node> endNode);
    
    void snapshot_vfunc(const Glib::RefPtr<Gtk::Snapshot>& snapshot, const Gdk::Rectangle& bounds);
    void updateTemporary(double x, double y);
    std::shared_ptr<Socket> get_start_socket();
    std::shared_ptr<Socket> get_end_socket();
    //void temp_start(double x, double y);
    std::shared_ptr<Node> get_start_node();
    std::shared_ptr<Node> get_end_node();
    double get_y();
    double get_x();
    bool is_point_near(double x, double y);

    void setSelected(bool selected);
    bool getSelected() const;

private:

    std::shared_ptr<Socket> startSocket;
    std::shared_ptr<Socket> endSocket;

    std::shared_ptr<Node> startNode;
    int startSocketIndex;
    std::shared_ptr<Node> endNode;
    int endSocketIndex;
    bool isTemporary;
    double tempEndX, tempEndY;
    double startX, startY;
    double permEndX,permEndY;

    bool isSelected = false;
};

#endif // EDGE_H
