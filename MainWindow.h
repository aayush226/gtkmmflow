#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "NodeView.h"
#include <gtkmm.h>

class MainWindow : public Gtk::ApplicationWindow {
public:
    MainWindow();

private:
    std::unique_ptr<NodeView> node_view;    
};

#endif // MAINWINDOW_H
