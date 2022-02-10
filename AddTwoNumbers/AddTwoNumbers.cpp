#include <iostream>


 struct ListNode {
  int val;
  ListNode *next;
  ListNode() : val(0), next(nullptr) {}
  ListNode(int x) : val(x), next(nullptr) {}
  ListNode(int x, ListNode *next) : val(x), next(next) {}
};
 

ListNode *addTwoNumbers(ListNode *l1, ListNode *l2) {
  ListNode *result(new ListNode(0));
  ListNode *curr(result);
  int keeping(0);
  while (l1 != nullptr || l2 != nullptr || keeping != 0) {
    int x(l1 != nullptr ? l1->val : 0);
    int y(l2 != nullptr ? l2->val : 0);
    int sum = x + y + keeping;
    keeping = sum / 10;
    curr->next = new ListNode(sum % 10);
    curr = curr->next;
    l1 = l1 != nullptr ? l1->next : nullptr;
    l2 = l2 != nullptr ? l2->next : nullptr;
  }

  return result->next;
}

int main()
{
    std::cout << "Hello World!\n";
}

