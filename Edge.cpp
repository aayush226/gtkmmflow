#include "Edge.h"
#include "Socket.h"

Edge::Edge(std::shared_ptr<Socket> startSocket, std::shared_ptr<Node> startNode, double tempX, double tempY) 
    : startSocket(startSocket), startNode(startNode), tempEndX(tempEndX), tempEndY(tempEndY), isTemporary(true) {}

Edge::Edge(std::shared_ptr<Socket> startSocket, std::shared_ptr<Node> startNode, std::shared_ptr<Socket> endSocket, std::shared_ptr<Node> endNode)
    : startSocket(startSocket), startNode(startNode), endSocket(endSocket), endNode(endNode), isTemporary(false) {}

void Edge::snapshot_vfunc(const Glib::RefPtr<Gtk::Snapshot>& snapshot, const Gdk::Rectangle& bounds) {
    
    auto cr = snapshot->append_cairo(bounds);
    double startX = startSocket->get_x() + startNode->get_allocation().get_x() + 17, startY = startSocket->get_y() + startNode->get_allocation().get_y() + 17;

    
    double endX, endY;

    if (isTemporary) {
        endX = tempEndX;
        endY = tempEndY;
    } else {
        
        endX = endSocket->get_x() + endNode->get_allocation().get_x() + 17;
        endY = endSocket->get_y() + endNode->get_allocation().get_y() + 17;
        
    }
    //Bezier Curve and Straight Line. To-do: Add logic to detect click near a Bezier Curve.
    /*double dx = endX - startX;
    double dy = endY - startY;

    // Determine the direction of the curve
    bool isLeftToRight = dx >= 0;

    // Adjust the magnitude of the control point offsets based on the maximum of the absolute differences
    double offset = std::max(std::abs(dx), std::abs(dy)) * 0.5;

    // Initialize control points
    double ctrlX1, ctrlX2;
    double ctrlY1 = startY, ctrlY2 = endY;

    if (isLeftToRight) {
        ctrlX1 = startX + offset;
        ctrlX2 = endX - offset;
    } else {
        // For right to left, adjust control points to push them further out
        ctrlX1 = startX - offset;
        ctrlX2 = endX + offset;

        // Optionally adjust Y control points to alter the curve's shape, "lifting" or "dropping" it
        ctrlY1 += dy * 0.2; // Adjust the start control point vertically
        ctrlY2 -= dy * 0.2; // Adjust the end control point vertically
    }
    //Gdk::RGBA color;
    //color.set("CBC3E3");
    //cr->set_source_rgba(color.get_red(), color.get_green(), color.get_blue(), color.get_alpha());
*/
    if (isSelected) {
        cr->set_line_width(2.5); // Make the line thicker
        cr->set_source_rgba(0.173, 0.533, 0.851, 1.0); // Change color to red for highlighted edge
    } else {
        cr->set_line_width(2.5); // Default line thickness
        cr->set_source_rgba(0,0,0, 1.0); // Default color
    }
    //cr->set_source_rgba(0.576, 0.439, 0.859, 1.0);
    cr->move_to(startX, startY);
    //cr->curve_to(ctrlX1, ctrlY1, ctrlX2, ctrlY2, endX, endY); // Draw Bezier curve
    
    cr->line_to(endX, endY); // Draw a line to the ending point
    
    cr->stroke();
}

void Edge::updateTemporary(double x, double y) {
    if (isTemporary) {
        tempEndX = x;
        tempEndY = y;
    }
}

std::shared_ptr<Socket> Edge::get_start_socket(){
    return startSocket;
}

std::shared_ptr<Socket> Edge::get_end_socket(){
    return endSocket;
}

std::shared_ptr<Node> Edge::get_start_node(){
    return startNode;
}

std::shared_ptr<Node> Edge::get_end_node(){
    return endNode;
}

double Edge::get_x(){
    return startX;
}
double Edge::get_y(){
    return startY;
}

bool Edge::is_point_near(double x, double y){
    double startX = startSocket->get_x() + startNode->get_allocation().get_x() + 17;
    double startY = startSocket->get_y() + startNode->get_allocation().get_y() + 17;
    double endX, endY;
    endX = endSocket->get_x() + endNode->get_allocation().get_x() + 17;
    endY = endSocket->get_y() + endNode->get_allocation().get_y() + 17;
    /*if (!isTemporary) {
        
    } else {
        // For a temporary edge
        endX = tempEndX;
        endY = tempEndY;
    }*/

    double A = x - startX;
    double B = y - startY;
    double C = endX - startX;
    double D = endY - startY;

    double dot = A * C + B * D;
    double len_sq = C * C + D * D;
    double param = -1;
    if (len_sq != 0) { // in case of 0 length line
        param = dot / len_sq;
    }

    double xx, yy;

    if (param < 0) {
        xx = startX;
        yy = startY;
    }
    else if (param > 1) {
        xx = endX;
        yy = endY;
    }
    else {
        xx = startX + param * C;
        yy = startY + param * D;
    }

    double dx = x - xx;
    double dy = y - yy;
    double distance = sqrt(dx * dx + dy * dy);

    const double NEAR_THRESHOLD = 5.0; // how near the click must be
    return distance < NEAR_THRESHOLD;
}


void Edge::setSelected(bool selected) {
    isSelected = selected;
}
bool Edge::getSelected() const {
    return isSelected;
}


/*
Colors for the edges: 
#d3455b, #675af5, #f7c325, #1aae9f, #e8833a, 
#d3455b -> (0.827, 0.271, 0.357, 1.0)
#675af5 -> (0.404, 0.353, 0.961, 1.0)
#f7c325 -> (0.969, 0.765, 0.145, 1.0)
#1aae9f -> (0.102, 0.682, 0.623, 1.0)
#e8833a -> (0.910, 0.514, 0.227, 1.0)
*/