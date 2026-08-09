#include "customer.hpp"
#include "order.hpp"
#include "product.hpp"
#include "food.hpp"
#include "beverage.hpp"
#include "combo.hpp"
#include "domain_errors.hpp"
#include "promotable.hpp"
#include "lanchonete_app.hpp"
#include "menu_catalog.hpp"
#include "product_concepts.hpp"
#include "registry.hpp"
#include "serialization.hpp"

#include <iostream>
#include <memory>
#include <string>
#include <variant>
#include <vector>

void aplicar_promocao(const promocionavel& item, double desconto) {
    if (item.elegivel_para_promocao()) {
        double preco_promo = item.calcular_preco_promocional(desconto);
        std::cout << "Preco promocional: R$ " << preco_promo << "\n";
    }
}

struct product_record_printer {
    void operator()(const food_record& record) const {
        std::cout << "Registro food: " << record.name
                  << " (" << record.calories << " kcal)\n";
    }

    void operator()(const beverage_record& record) const {
        std::cout << "Registro beverage: " << record.name
                  << " (" << record.volume_ml << "ml)\n";
    }

    void operator()(const combo_record& record) const {
        std::cout << "Registro combo: " << record.name
                  << " com " << record.item_names.size() << " itens\n";
    }
};

int main() {
    std::cout << "=== Sistema de pedidos da lanchonete ===\n\n";

    std::cout << "=== QUESTAO 1: Demonstracao de Destrutor Virtual ===\n";
    product* p = new alimento("X-Burger", 15.0, 600, "lanche");
    std::cout << "Deletando produto via ponteiro base...\n";
    delete p;
    std::cout << "\n";

    std::cout << "=== QUESTAO 2: Polimorfismo Dinamico ===\n";
    std::vector<std::unique_ptr<product>> menu;

    menu.push_back(std::make_unique<alimento>("X-Tudo", 18.0, 850, "lanche"));
    menu.push_back(std::make_unique<bebida>("Coca-Cola", 5.0, 350, true));
    menu.push_back(std::make_unique<alimento>("Batata Frita", 8.0, 400, "acompanhamento"));
    menu.push_back(std::make_unique<bebida>("Suco Natural", 7.0, 300, false));

    std::cout << "\nIteracao polimorfica pelo menu:\n";
    for (const auto& item : menu) {
        item->exibir_info();
    }

    std::cout << "\nFuncao livre encontrar_maior_calorias():\n";
    const product* maior = encontrar_maior_calorias(menu);
    if (maior) {
        std::cout << "Produto com mais calorias: " << maior->get_name() 
                  << " (" << maior->calcular_calorias() << " kcal)\n";
    }
    std::cout << "\n";

    std::cout << "=== QUESTAO 3: Interface Pura e Heranca Multipla ===\n";
    alimento x_burguer("X-Burguer", 20.0, 700, "lanche");
    std::cout << "\nUsando interface promocionavel:\n";
    aplicar_promocao(x_burguer, 10.0);
    std::cout << "\n";

    std::cout << "=== Demonstracao de Combo (classe final) ===\n";
    alimento burger_combo("Hamburguer", 15.0, 600, "lanche");
    bebida soda_combo("Refrigerante", 5.0, 350, true);
    alimento fries_combo("Batata Frita", 8.0, 400, "acompanhamento");

    combo meal_combo("Combo Familia", 15.0);
    meal_combo.adicionar_produto(burger_combo);
    meal_combo.adicionar_produto(soda_combo);
    meal_combo.adicionar_produto(fries_combo);
    
    meal_combo.exibir_info();
    std::cout << "\n";

    std::cout << "=== Sistema Original: Pedidos ===\n";
    customer customer_one("Maria Silva", "12345678901");

    std::cout << "\nVerificando CPF do cliente...\n";
    if (customer_one.is_valid_cpf()) {
        std::cout << "CPF valido\n";
    } else {
        std::cout << "CPF invalido\n";
    }

    std::cout << "\n=== TP3 - QUESTAO 1: Programacao Generica ===\n";
    registry<std::string> tags;
    tags.add("lanche");
    tags.add("bebida");

    registry<int> priorities;
    priorities.add(1);
    priorities.add(2);

    std::cout << "Registry<string>: " << tags.size()
              << " tags; primeira tag = " << tags.at(0) << "\n";
    std::cout << "Registry<int>: " << priorities.size()
              << " prioridades; segunda prioridade = " << priorities.at(1) << "\n";

    std::cout << "Instancias vivas via CRTP - alimento: " << alimento::alive()
              << ", bebida: " << bebida::alive()
              << ", combo: " << combo::alive() << "\n";

    std::vector<alimento> foods_for_sum;
    foods_for_sum.emplace_back("Mini Burger", 12.0, 350.0, "lanche");
    foods_for_sum.emplace_back("Cookie", 6.0, 220.0, "sobremesa");
    std::cout << "Soma generica de precos: R$ "
              << calculate_price_sum(foods_for_sum) << "\n";

    menu_catalog catalog;
    catalog.add_product(x_burguer, x_burguer.get_category());
    catalog.add_product(soda_combo, "bebida");
    catalog.add_product(fries_combo, fries_combo.get_category());

    std::cout << "Pipeline ranges (filter + transform) para itens >= R$ 8:\n";
    for (const auto& name : catalog.expensive_product_names(8.0)) {
        std::cout << "- " << name << "\n";
    }

    std::cout << "\n=== TP3 - QUESTAO 2: Tratamento de Erros ===\n";
    try {
        order invalid_order(404, customer_one);
        invalid_order.add_item(x_burguer, 0);
    } catch (const domain_error& error) {
        std::cout << "Erro capturado pela base: " << error.what() << "\n";
    }

    const auto found = catalog.find_by_name("X-Burguer");
    const auto missing = catalog.find_by_name("Produto inexistente");
    std::cout << "Busca optional existente: "
              << (found ? found->get().get_name() : "nao encontrado") << "\n";
    std::cout << "Busca optional inexistente: "
              << (missing ? missing->get().get_name() : "nao encontrado") << "\n";

    product_record variant_record =
        combo_record{"Combo Demonstracao", 10.0, {"X-Burguer", "Refrigerante"}};
    std::visit(product_record_printer{}, variant_record);

    std::cout << "\n=== TP3 - QUESTAO 3: STL e Concorrencia ===\n";
    std::cout << "Categorias unicas no catalogo: " << catalog.categories().size() << "\n";
    std::cout << "Produtos acima de R$ 7: " << catalog.count_products_above(7.0) << "\n";
    std::cout << "Preco total dos itens catalogados: R$ "
              << catalog.total_price() << "\n";

    std::cout << "Produtos ordenados por preco:\n";
    for (const auto* item : catalog.products_sorted_by_price()) {
        std::cout << "- " << item->get_name() << ": R$ " << item->get_price() << "\n";
    }

    std::vector<std::unique_ptr<product>> parallel_products;
    parallel_products.push_back(
        std::make_unique<alimento>("Wrap", 14.0, 520.0, "lanche"));
    parallel_products.push_back(
        std::make_unique<bebida>("Agua com gas", 4.0, 500, true));
    parallel_products.push_back(
        std::make_unique<alimento>("Salada", 16.0, 250.0, "refeicao"));

    const double serial_calories = calculate_total_calories_serial(parallel_products);
    const double parallel_calories = calculate_total_calories_parallel(parallel_products);
    std::cout << "Calorias serial: " << serial_calories << "\n";
    std::cout << "Calorias paralelas: " << parallel_calories << "\n";

    std::cout << "\n=== TP3 - QUESTAO 4: Serializacao e SOLID/DIP ===\n";
    app_state state;
    state.products.push_back(
        food_record{x_burguer.get_name(), x_burguer.get_price(),
                    x_burguer.get_calories(), x_burguer.get_category()});
    state.products.push_back(
        beverage_record{soda_combo.get_name(), soda_combo.get_price(),
                        soda_combo.get_volume_ml(), soda_combo.has_gas()});
    state.products.push_back(
        combo_record{meal_combo.get_name(), meal_combo.get_discount_percent(),
                     {"Hamburguer", "Refrigerante", "Batata Frita"}});

    memory_repository memory_repo;
    lanchonete_app memory_app{memory_repo};
    memory_app.replace_state(state);
    memory_app.save();
    const app_state memory_loaded = memory_app.load();
    std::cout << "Memory repository carregou "
              << memory_loaded.products.size() << " produtos\n";

    try {
        json_repository file_repo{"estado_tp3.json"};
        lanchonete_app file_app{file_repo};
        file_app.replace_state(state);
        file_app.save();
        const app_state file_loaded = file_app.load();
        std::cout << "JSON repository carregou "
                  << file_loaded.products.size() << " produtos\n";
    } catch (const domain_error& error) {
        std::cout << "Falha de repositorio: " << error.what() << "\n";
    }

    std::cout << "\nSaindo do escopo (destruicao automatica do vector)...\n";
    std::cout << "\n=== Fim do programa ===\n";

    return 0;
}
