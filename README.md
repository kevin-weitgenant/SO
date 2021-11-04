# SO


A Fim de conhecer mais  sobre Thread foi definido um trabalho na disciplina de Sistemas Operacionais que consiste utilizar Programação Concorrente para simular o atendimento de um gerente de banco, considerando as seguintes características:

-O local deve apenas permitir a permanência de 3 clientes por vez para obter o atendimento do gerente.

-O Gerente não deve consumir recursos enquanto não há clientes aguardando no local.

-O Gerente deve voltar a consumir recursos quando um novo cliente chegar no local.

-O tempo de atendimento para cada cliente deve ser algo entre 5 a 12 segundos.

-Se a entrada de clientes for maior do que a ocupação do local, o mesmo deve se retirar do estabelecimento.


















**Foram utilizados 2 threads, 2 mutex e uma variável condicional para a resolução do problema.**



Exemplo de execução:

![image](https://user-images.githubusercontent.com/20791940/140244950-ba8fdc00-1211-4330-9e90-1456884ceee7.png)
