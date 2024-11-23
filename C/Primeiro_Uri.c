/*Com base na tabela abaixo, escreva um programa que leia o código de um item e
a quantidade deste item. A seguir, calcule e mostre o valor da conta a pagar.

CODIGO  ESPECIFICAÇÃO    PREÇO
1       Cachorro-Quente  R$4.00
2       X-Salada         R$4.50
3       X-Bacon          R$5.00
4       Torrada Simples  R$2.00
5       Refrigerante     R$1.50

Entrada
O arquivo de entrada contém dois valores inteiros correspondentes ao código
e à quantidade de um item conforme tabela acima.

Saída
O arquivo de saída deve conter a mensagem "Total: R$ "
seguido pelo valor a ser pago, com 2 casas após o ponto decimal.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Lanche {
  int codigo;
  char especificacao[25];
  double preco;
  void (*funcao)(struct Lanche *);
};

void Total(struct Lanche *this) {
  int quantidade;
  double total;
  printf("Digite a quantidade que deseja: \n");
  scanf("%d", &quantidade);
  total = quantidade * this->preco;
  printf("Total: R$ %.2lf\n", total);
}

struct Lanche *criarLanche(int codigo, char especificacao[], double preco) {
  struct Lanche *this = (struct Lanche *)malloc(sizeof(struct Lanche));
  this->codigo = codigo;
  strcpy(this->especificacao, especificacao);
  this->preco = preco;

  this->funcao = &Total;
  return this;
}

int main() {
  int selecionado;
  struct Lanche *lanche1 =
      (struct Lanche *)criarLanche(1, "Cachorro-Quente", 4.00);
  struct Lanche *lanche2 = (struct Lanche *)criarLanche(2, "X-Salada", 4.50);
  struct Lanche *lanche3 = (struct Lanche *)criarLanche(3, "X-Bacon", 5.00);
  struct Lanche *lanche4 =
      (struct Lanche *)criarLanche(4, "Torrada Simples", 2.00);
  struct Lanche *lanche5 =
      (struct Lanche *)criarLanche(5, "Refrigerante", 1.50);

  printf("Digite o código do produto: \n");
  scanf("%d", &selecionado);

  switch (selecionado) {
  case 1:
    printf("O produto selecionado foi:\n%s\nR$: %.2lf\n",
           lanche1->especificacao, lanche1->preco);

    lanche1->funcao(lanche1);
    free(lanche1);
    break;
  case 2:
    printf("O produto selecionado foi:\n%s\nR$: %.2lf\n",
           lanche2->especificacao, lanche2->preco);

    lanche2->funcao(lanche2);
    free(lanche2);
    break;
  case 3:
    printf("O produto selecionado foi:\n%s\nR$: %.2lf\n",
           lanche3->especificacao, lanche3->preco);

    lanche3->funcao(lanche3);
    free(lanche3);
    break;
  case 4:
    printf("O produto selecionado foi:\n%s\nR$: %.2lf\n",
           lanche4->especificacao, lanche4->preco);

    lanche4->funcao(lanche4);
    free(lanche4);
    break;
  case 5:
    printf("O produto selecionado foi:\n%s\nR$: %.2lf\n",
           lanche5->especificacao, lanche5->preco);

    lanche5->funcao(lanche5);
    free(lanche5);
    break;
  default:
    printf("Por favor digite uma opção válida.");
    break;
  }

  return 0;
}
