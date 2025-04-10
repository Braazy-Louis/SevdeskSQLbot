#include "Application.h"
#include <iostream>

int main() {
    try {
        Application app;
        
        if (!app.initialize()) {
            std::cerr << "Initialisierung der Anwendung fehlgeschlagen." << std::endl;		
            return 1;
        }
        
        return app.run();
    }
    catch (const std::exception& e) {
        std::cerr << "Fataler Fehler: " << e.what() << std::endl;		
        return 1;
    }
    catch (...) {
        std::cerr << "Unbekannter fataler Fehler aufgetreten." << std::endl;
        return 1;
    }
} 