#include "Edge.h"
#include "Socket.h"
/*Edge::Edge(double startX, double startY, double tempX, double tempY)
    :startX(startX),startY(startY),tempEndX(tempX), tempEndY(tempY), isTemporary(true) {}*/

/*Edge::Edge(double startX, double startY, double tempX, double tempY, bool perm)
    :startX(startX),startY(startY),tempEndX(tempX), tempEndY(tempY), isTemporary(perm) {
        permEndX = tempEndX;
        permEndY = tempEndY;
}*/
Edge::Edge(std::shared_ptr<Socket> startSocket, std::shared_ptr<Node> startNode, double tempX, double tempY) 
    : startSocket(startSocket), startNode(startNode), tempEndX(tempEndX), tempEndY(tempEndY), isTemporary(true) {}

//Edge::Edge(std::shared_ptr<Socket> endSocket) 
   //: endSocket(endSocket), isTemporary(false) {}
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

    //std::cout << "Edge: " << startX << " " << startY << " " << endX << " " << endY << std::endl;
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
        cr->set_line_width(4.0); // Make the line thicker
        cr->set_source_rgba(1.0, 0.0, 0.0, 1.0); // Change color to red for highlighted edge
    } else {
        cr->set_line_width(2.0); // Default line thickness
        cr->set_source_rgba(1,1,1, 1.0); // Default color
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
//void temp_start(double x, double y);

double Edge::get_x(){
    return startX;
}
double Edge::get_y(){
    return startY;
}
//void Edge::updatePerm()

bool Edge::is_point_near(double x, double y){
    double startX = startSocket->get_x() + startNode->get_allocation().get_x() + 17;
    double startY = startSocket->get_y() + startNode->get_allocation().get_y() + 17;
    double endX, endY;
    endX = endSocket->get_x() + endNode->get_allocation().get_x() + 17;
    endY = endSocket->get_y() + endNode->get_allocation().get_y() + 17;
    /*if (!isTemporary) {
        
    } else {
        // For a temporary edge, use the temporary end points
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

    const double NEAR_THRESHOLD = 5.0; // Adjust based on how "near" you want it to be
    return distance < NEAR_THRESHOLD;
}


void Edge::setSelected(bool selected) {
    isSelected = selected;
}
bool Edge::getSelected() const {
    return isSelected;
}