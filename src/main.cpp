#include "customer.hpp"
#include "order.hpp"
#include "product.hpp"

#include <iostream>

int main() {
    std::cout << "=== Sistema de pedidos da lanchonete ===\n\n";

    // Produtos criados independentemente do pedido.
    product burger("Hamburguer", 18.50);
    product fries("Batata frita", 9.00);
    product soda("Refrigerante", 6.50);

    std::cout << "\n";

    // Cliente criado independentemente do pedido.
    customer customer_one("Maria Silva", "12345678901");

    std::cout << "\nVerificando CPF do cliente...\n";

    if (customer_one.is_valid_cpf()) {
        std::cout << "CPF valido\n";
    } else {
        std::cout << "CPF invalido\n";
    }

    std::cout << "\nTestando metodo de desconto do produto...\n";
    std::cout << "Preco original do hamburguer: R$ " << burger.get_price() << "\n";
    std::cout << "Preco com 10% de desconto: R$ " << burger.apply_discount(10) << "\n";

    std::cout << "\nEntrando no bloco do pedido...\n";

    {
        // Pedido agrega o cliente, mas nao e dono dele.
        order order_one(1, customer_one);

        // Pedido cria os itens internamente.
        // Isso representa composicao.
        order_one.add_item(burger, 2);
        order_one.add_item(fries, 1);
        order_one.add_item(soda, 3);

        order_one.print_summary();

        std::cout << "Saindo do bloco do pedido...\n";
    }

    std::cout << "\nPedido destruido, mas cliente e produtos continuam existindo.\n";
    std::cout << "Cliente ainda acessivel: " << customer_one.get_name() << "\n";
    std::cout << "Produto ainda acessivel: " << burger.get_name() << "\n";

    std::cout << "\n=== Fim do programa ===\n";

    return 0;
}
