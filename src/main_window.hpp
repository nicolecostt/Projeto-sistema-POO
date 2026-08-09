#ifndef MAIN_WINDOW_HPP
#define MAIN_WINDOW_HPP

#include "lanchonete_app.hpp"

#include <QMainWindow>

class QLabel;
class QLineEdit;
class QPushButton;
class QDoubleSpinBox;
class QTableWidget;

class main_window : public QMainWindow {
private:
    lanchonete_app& core_;

    QTableWidget* products_table_;
    QLineEdit* name_input_;
    QDoubleSpinBox* price_input_;
    QDoubleSpinBox* calories_input_;
    QLineEdit* category_input_;
    QLabel* total_label_;
    QPushButton* add_button_;
    QPushButton* remove_button_;
    QPushButton* save_button_;
    QPushButton* load_button_;
    QPushButton* calculate_button_;

    void build_ui();
    void connect_signals();
    void refresh_products();
    void show_error(const QString& title, const QString& message);

    void add_food();
    void remove_selected();
    void save_state();
    void load_state();
    void calculate_total();

public:
    explicit main_window(lanchonete_app& core, QWidget* parent = nullptr);
};

#endif
