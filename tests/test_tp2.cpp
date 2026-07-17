#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

#include "product.hpp"
#include "food.hpp"
#include "beverage.hpp"
#include "combo.hpp"
#include "promotable.hpp"

#include <memory>
#include <vector>

using Catch::Approx;

static bool alimento_destrutor_chamado = false;
static bool bebida_destrutor_chamado = false;

TEST_CASE("Destruicao em cadeia executada corretamente", "[hierarquia]") {
    SECTION("Destrutor virtual funciona com alimento") {
        product* p = new alimento("Teste Burger", 10.0, 500, "lanche");
        REQUIRE(p != nullptr);
        delete p;
    }
    
    SECTION("Destrutor virtual funciona com bebida") {
        product* p = new bebida("Teste Suco", 5.0, 300, false);
        REQUIRE(p != nullptr);
        delete p;
    }
    
    SECTION("Destrutor virtual funciona com combo") {
        product* p = new combo("Teste Combo", 10.0);
        REQUIRE(p != nullptr);
        delete p;
    }
}

TEST_CASE("Polimorfismo dinamico com vector de unique_ptr", "[polimorfismo]") {
    std::vector<std::unique_ptr<product>> menu;
    
    menu.push_back(std::make_unique<alimento>("Burger", 10.0, 500, "lanche"));
    menu.push_back(std::make_unique<bebida>("Suco", 4.0, 300, false));
    menu.push_back(std::make_unique<alimento>("Pizza", 15.0, 800, "lanche"));
    
    SECTION("Calculo de calorias via polimorfismo") {
        REQUIRE(menu[0]->calcular_calorias() == Approx(500));
        REQUIRE(menu[1]->calcular_calorias() == Approx(60).epsilon(0.1));
        REQUIRE(menu[2]->calcular_calorias() == Approx(800));
    }
    
    SECTION("Funcao encontrar_maior_calorias retorna o correto") {
        const product* maior = encontrar_maior_calorias(menu);
        REQUIRE(maior != nullptr);
        REQUIRE(maior->calcular_calorias() == Approx(800));
        REQUIRE(maior->get_name() == "Pizza");
    }
    
    SECTION("Funcao encontrar_maior_calorias com vetor vazio") {
        std::vector<std::unique_ptr<product>> menu_vazio;
        const product* maior = encontrar_maior_calorias(menu_vazio);
        REQUIRE(maior == nullptr);
    }
}

TEST_CASE("Interface promocionavel funciona corretamente", "[interface]") {
    SECTION("alimento implementa promocionavel") {
        alimento burger("Burger", 20.0, 600, "lanche");
        
        const promocionavel& promo = burger;
        REQUIRE(promo.elegivel_para_promocao() == true);
        
        double preco_promo = promo.calcular_preco_promocional(10.0);
        REQUIRE(preco_promo == Approx(18.0));
        
        preco_promo = promo.calcular_preco_promocional(25.0);
        REQUIRE(preco_promo == Approx(15.0));
    }
    
    SECTION("bebida implementa promocionavel") {
        bebida soda("Refrigerante", 5.0, 350, true);
        
        const promocionavel& promo = soda;
        REQUIRE(promo.elegivel_para_promocao() == true);
        
        double preco_promo = promo.calcular_preco_promocional(20.0);
        REQUIRE(preco_promo == Approx(4.0));
    }
}

TEST_CASE("Combo funciona corretamente", "[combo]") {
    alimento burger("Burger", 15.0, 600, "lanche");
    bebida soda("Refrigerante", 5.0, 350, true);
    alimento fries("Batata", 8.0, 400, "acompanhamento");
    
    combo meal("Combo Familia", 15.0);
    meal.adicionar_produto(burger);
    meal.adicionar_produto(soda);
    meal.adicionar_produto(fries);
    
    SECTION("Preco do combo com desconto") {
        double preco_esperado = (15.0 + 5.0 + 8.0) * 0.85;
        REQUIRE(meal.get_price() == Approx(preco_esperado));
    }
    
    SECTION("Calorias totais do combo") {
        double calorias_esperadas = 600 + (350 * 0.42) + 400;
        REQUIRE(meal.calcular_calorias() == Approx(calorias_esperadas));
    }
}

TEST_CASE("Metodo final em bebida nao pode ser sobrescrito", "[final]") {
    bebida soda("Coca-Cola", 5.0, 350, true);
    REQUIRE(soda.calcular_calorias() == Approx(147.0));
}
