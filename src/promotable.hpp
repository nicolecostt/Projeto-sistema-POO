#ifndef PROMOTABLE_HPP
#define PROMOTABLE_HPP

class promocionavel {
public:
    virtual double calcular_preco_promocional(double porcentagem_desconto) const = 0;
    virtual bool elegivel_para_promocao() const = 0;
    virtual ~promocionavel() = default;
};

#endif
