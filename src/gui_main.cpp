#include "lanchonete_app.hpp"
#include "main_window.hpp"
#include "repository.hpp"

#include <QApplication>

int main(int argc, char** argv) {
    QApplication qt_app{argc, argv};

    json_repository repository{"estado_tp3.json"};
    lanchonete_app core{repository};

    main_window window{core};
    window.show();

    return qt_app.exec();
}
