#include "MainWindow.h"
MainWindow::MainWindow() {
    set_title("Node Editor");
    set_default_size(1080,720);

    /*node_view = std::make_unique<NodeView>();
    set_child(*node_view);*/
    Gtk::ScrolledWindow scrolled_window;
    scrolled_window.set_policy(Gtk::PolicyType::AUTOMATIC, Gtk::PolicyType::AUTOMATIC);

    // Your NodeView or content widget
    node_view = std::make_unique<NodeView>();

    // Add your NodeView to the ScrolledWindow
    scrolled_window.set_child(*node_view);

    // Add the ScrolledWindow to the main window
    set_child(scrolled_window);
}
