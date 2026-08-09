#include "main_window.hpp"

#include "domain_errors.hpp"

#include <QAbstractItemView>
#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QVBoxLayout>

#include <iomanip>
#include <sstream>
#include <type_traits>
#include <variant>

namespace {

QString money_text(double value) {
    std::ostringstream output;
    output << "R$ " << std::fixed << std::setprecision(2) << value;
    return QString::fromStdString(output.str());
}

QString product_type_text(const product_record& record) {
    return std::visit([](const auto& concrete_record) {
        using record_type = std::decay_t<decltype(concrete_record)>;
        if constexpr (std::is_same_v<record_type, food_record>) {
            return QString{"food"};
        } else if constexpr (std::is_same_v<record_type, beverage_record>) {
            return QString{"beverage"};
        } else {
            return QString{"combo"};
        }
    }, record);
}

QString product_name_text(const product_record& record) {
    return std::visit([](const auto& concrete_record) {
        return QString::fromStdString(concrete_record.name);
    }, record);
}

QString product_price_text(const product_record& record) {
    return std::visit([](const auto& concrete_record) {
        if constexpr (requires { concrete_record.price; }) {
            return money_text(concrete_record.price);
        } else {
            return QString{"-"};
        }
    }, record);
}

QString product_detail_text(const product_record& record) {
    return std::visit([](const auto& concrete_record) {
        using record_type = std::decay_t<decltype(concrete_record)>;
        if constexpr (std::is_same_v<record_type, food_record>) {
            return QString("%1 kcal | %2")
                .arg(concrete_record.calories)
                .arg(QString::fromStdString(concrete_record.category));
        } else if constexpr (std::is_same_v<record_type, beverage_record>) {
            return QString("%1 ml | %2")
                .arg(concrete_record.volume_ml)
                .arg(concrete_record.has_gas ? "com gas" : "sem gas");
        } else {
            return QString("%1% desconto | %2 itens")
                .arg(concrete_record.discount_percent)
                .arg(concrete_record.item_names.size());
        }
    }, record);
}

} // namespace

main_window::main_window(lanchonete_app& core, QWidget* parent)
    : QMainWindow(parent),
      core_(core),
      products_table_(nullptr),
      name_input_(nullptr),
      price_input_(nullptr),
      calories_input_(nullptr),
      category_input_(nullptr),
      total_label_(nullptr),
      add_button_(nullptr),
      remove_button_(nullptr),
      save_button_(nullptr),
      load_button_(nullptr),
      calculate_button_(nullptr) {
    build_ui();
    connect_signals();
    refresh_products();
}

void main_window::build_ui() {
    auto* central_widget = new QWidget(this);
    auto* main_layout = new QVBoxLayout(central_widget);

    products_table_ = new QTableWidget(0, 4, central_widget);
    products_table_->setHorizontalHeaderLabels({"Tipo", "Nome", "Preco", "Detalhes"});
    products_table_->horizontalHeader()->setStretchLastSection(true);
    products_table_->setSelectionBehavior(QAbstractItemView::SelectRows);
    products_table_->setSelectionMode(QAbstractItemView::SingleSelection);
    products_table_->setEditTriggers(QAbstractItemView::NoEditTriggers);
    main_layout->addWidget(products_table_);

    auto* form_group = new QGroupBox("Adicionar alimento", central_widget);
    auto* form_layout = new QFormLayout(form_group);

    name_input_ = new QLineEdit(form_group);
    price_input_ = new QDoubleSpinBox(form_group);
    calories_input_ = new QDoubleSpinBox(form_group);
    category_input_ = new QLineEdit(form_group);

    price_input_->setRange(0.0, 10000.0);
    price_input_->setDecimals(2);
    price_input_->setPrefix("R$ ");
    calories_input_->setRange(0.0, 100000.0);
    calories_input_->setDecimals(1);
    calories_input_->setSuffix(" kcal");

    form_layout->addRow("Nome", name_input_);
    form_layout->addRow("Preco", price_input_);
    form_layout->addRow("Calorias", calories_input_);
    form_layout->addRow("Categoria", category_input_);
    main_layout->addWidget(form_group);

    auto* buttons_layout = new QGridLayout();
    add_button_ = new QPushButton("Adicionar", central_widget);
    remove_button_ = new QPushButton("Remover selecionado", central_widget);
    save_button_ = new QPushButton("Salvar", central_widget);
    load_button_ = new QPushButton("Carregar", central_widget);
    calculate_button_ = new QPushButton("Calcular total", central_widget);

    buttons_layout->addWidget(add_button_, 0, 0);
    buttons_layout->addWidget(remove_button_, 0, 1);
    buttons_layout->addWidget(save_button_, 1, 0);
    buttons_layout->addWidget(load_button_, 1, 1);
    buttons_layout->addWidget(calculate_button_, 2, 0, 1, 2);
    main_layout->addLayout(buttons_layout);

    total_label_ = new QLabel("Total: R$ 0.00", central_widget);
    main_layout->addWidget(total_label_);

    setCentralWidget(central_widget);
    setWindowTitle("Lanchonete POO - TP3 Qt");
    resize(720, 520);

}

void main_window::connect_signals() {
    connect(add_button_, &QPushButton::clicked, this, [this] {
        add_food();
    });
    connect(remove_button_, &QPushButton::clicked, this, [this] {
        remove_selected();
    });
    connect(save_button_, &QPushButton::clicked, this, [this] {
        save_state();
    });
    connect(load_button_, &QPushButton::clicked, this, [this] {
        load_state();
    });
    connect(calculate_button_, &QPushButton::clicked, this, [this] {
        calculate_total();
    });
}

void main_window::refresh_products() {
    const auto& products = core_.current_state().products;
    products_table_->setRowCount(static_cast<int>(products.size()));

    for (int row = 0; row < static_cast<int>(products.size()); ++row) {
        const auto& record = products.at(static_cast<std::size_t>(row));
        products_table_->setItem(row, 0, new QTableWidgetItem(product_type_text(record)));
        products_table_->setItem(row, 1, new QTableWidgetItem(product_name_text(record)));
        products_table_->setItem(row, 2, new QTableWidgetItem(product_price_text(record)));
        products_table_->setItem(row, 3, new QTableWidgetItem(product_detail_text(record)));
    }

    products_table_->resizeColumnsToContents();
}

void main_window::show_error(const QString& title, const QString& message) {
    QMessageBox::warning(this, title, message);
}

void main_window::add_food() {
    try {
        core_.add_food_record(name_input_->text().toStdString(),
                              price_input_->value(),
                              calories_input_->value(),
                              category_input_->text().toStdString());
        name_input_->clear();
        price_input_->setValue(0.0);
        calories_input_->setValue(0.0);
        category_input_->clear();
        refresh_products();
        calculate_total();
    } catch (const domain_error& error) {
        show_error("Erro ao adicionar", QString::fromStdString(error.what()));
    }
}

void main_window::remove_selected() {
    const int row = products_table_->currentRow();
    if (row < 0) {
        show_error("Remover produto", "Selecione um produto antes de remover.");
        return;
    }

    try {
        core_.remove_product_at(static_cast<std::size_t>(row));
        refresh_products();
        calculate_total();
    } catch (const domain_error& error) {
        show_error("Erro ao remover", QString::fromStdString(error.what()));
    }
}

void main_window::save_state() {
    try {
        core_.save();
        QMessageBox::information(this, "Salvar", "Estado salvo em estado_tp3.json.");
    } catch (const domain_error& error) {
        show_error("Erro ao salvar", QString::fromStdString(error.what()));
    }
}

void main_window::load_state() {
    try {
        core_.load();
        refresh_products();
        calculate_total();
    } catch (const domain_error& error) {
        show_error("Erro ao carregar", QString::fromStdString(error.what()));
    }
}

void main_window::calculate_total() {
    total_label_->setText("Total: " + money_text(core_.total_product_price()));
}
