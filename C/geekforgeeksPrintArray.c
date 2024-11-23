/*Given a linked list. Print all the elements of the linked list.

Example 1:

Input:
N=2
LinkedList={1 , 2}
Output:
1 2
Explanation:
The linked list contains two
elements 1 and 2.The elements
are printed in a single line.
Example 2:

Input:
N = 3
Linked List = { 49, 10, 30}
Output:
49 10 30
Explanation:
The linked list contains 3
elements 49, 10 and 30. The
elements are printed in a single
line.

Expected Time Complexity : O(N)
Expected Auxiliary Space : O(1)
*/

#include <stdio.h>
#include <stdlib.h>

struct Node {
  int data;
  struct Node *next;
};

struct Node *criarNode(int data, struct Node *next) {
  struct Node *node = (struct Node *)malloc(sizeof(struct Node));
  node->data = data;
  node->next = NULL;

  return node;
};

void display(struct Node *head) {
  int arrSize = 3;
  int LinkedList[3] = {49, 10, 30};
  int i;
  while (head) {
    head->data = LinkedList[i];
    i++;
    printf("%d ", head->data);
    if (i < arrSize) {
      head->next = criarNode(0, NULL);
      head = head->next;
    } else {
      break;
    }
  }
};

int main() {
  struct Node *head = criarNode(0, NULL);
  display(head);

  return 0;
};
