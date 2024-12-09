//source: https://www.youtube.com/watch?v=oTEiQx88B2U

/*Assignment:
  Read a string from the user.

  If the string is CAT, say "Meow".
  If the string us DOG, say "Woof".

  IF AT ANY POINT YOUR CODE IS UNSAFE WE WILL
  POWER DOWN YOUR COMPUTER

*/

#include <stdio.h>
#include <string.h>

int main(int argc, const char **argv){
  
  if(argc > 2)
  {
    printf("Escreva apenas um animal por vez.\n");
    return 1;
  }
  else if(argc < 0 || argc == 0 || argv[1] == NULL)
  {
    printf("Por favor, insira o nome do animal desejado.\n");
    return 1;   
  }
  
  const char *entrada = argv[1];
  if(strlen(entrada) > 3){
    printf("Entrada inválida.\n");
    return 1;
  }

  char animal[strlen(entrada)];
  strncpy(animal, entrada, 3);

  if(strcmp(animal, "CAT") == 0){
    printf("Meow\n");
  }else if (strcmp(animal, "DOG") == 0) {
    printf("Woof\n");
  }else{
    printf("Somente suportamos cães e gatos no momento.\n");
  }

  return 0;
}
