#include "Application.h"
#include <locale>

int main() {
    setlocale(LC_CTYPE, "Russian");
    Application app(1480, 920, "Lab 2 - Variant 1");
    return app.run();
}