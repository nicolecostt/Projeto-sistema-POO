#include "customer.hpp"
#include "order.hpp"
#include "product.hpp"
#include "food.hpp"
#include "beverage.hpp"
#include "combo.hpp"
#include "promotable.hpp"

#include <iostream>
#include <memory>
#include <vector>

void aplicar_promocao(const promocionavel& item, double desconto) {
    if (item.elegivel_para_promocao()) {
        double preco_promo = item.calcular_preco_promocional(desconto);
        std::cout << "Preco promocional: R$ " << preco_promo << "\n";
    }
}

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

    std::cout << "\nSaindo do escopo (destruicao automatica do vector)...\n";
    std::cout << "\n=== Fim do programa ===\n";

    return 0;
}
