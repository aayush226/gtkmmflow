#include "MainWindow.h"
#include <gtkmm.h>

int main(int argc, char* argv[]) {
    auto app = Gtk::Application::create("org.gtkmm.nodes");
    return app->make_window_and_run<MainWindow>(argc, argv);
}