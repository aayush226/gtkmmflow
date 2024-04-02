#include "MainWindow.h"
MainWindow::MainWindow() {
    set_title("Node Editor");
    set_default_size(1080,720);

    scrolled_window.set_policy(Gtk::PolicyType::AUTOMATIC, Gtk::PolicyType::AUTOMATIC);

    //NodeView
    node_view = std::make_unique<NodeView>();

    // Add NodeView to the ScrolledWindow
    scrolled_window.set_child(*node_view);

    // Add the ScrolledWindow to the main window
    set_child(scrolled_window);
}
