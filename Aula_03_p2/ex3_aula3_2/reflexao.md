### Explicação e Reflexão:

1. **Alocação Dinâmica Manual:**
   - A alocação manual envolve o uso de ponteiros e a alocação de memória para cada linha individualmente.
   - A liberação da memória também deve ser feita manualmente para evitar vazamentos de memória.
   - Esse método é mais propenso a erros, como esquecer de liberar memória ou acessar memória fora dos limites.

2. **Uso de `std::vector`:**
   - `std::vector` automatiza a alocação e liberação de memória, reduzindo o risco de erros e vazamentos de memória.
   - A interface é mais simples e segura, com menos chances de acessar memória fora dos limites.
   - O desempenho do `std::vector` é frequentemente comparável ou melhor em cenários práticos devido às otimizações internas que o STL (Standard Template Library) oferece.

### Perguntas para Reflexão:

1. **Qual abordagem foi mais rápida?**
   - O desempenho pode variar dependendo do compilador e do sistema, mas geralmente o uso de `std::vector` é comparável ao manual e pode até ser mais rápido devido às otimizações internas.

2. **Quais vantagens você percebe ao usar `std::vector`?**
   - Segurança de memória (não há necessidade de gerenciar manualmente a alocação e liberação).
   - Melhor legibilidade e manutenção do código.
   - Proteção contra erros comuns, como vazamentos de memória e acessos fora dos limites.

3. **Como a alocação automática e a liberação de memória pelo `std::vector` afetam a segurança e robustez do código?**
   - `std::vector` torna o código mais seguro, evitando vazamentos de memória e acessos inválidos, além de facilitar a manutenção e a leitura do código, o que é especialmente importante em projetos grandes e de longa duração.

### Sobre as velocidades:

- A velocidade de execução da alocação manual foi ligeiramente mais rápida que a da alocaçãi oir vetor. Isso pode ser por conta da segurança inicial que o `std::vector` oferece, que pode ser mais lenta, mas é mais segura.