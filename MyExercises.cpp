


struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
     ListNode(int x) : val(x), next(nullptr) {}
     ListNode(int x, ListNode *next) : val(x), next(next) {}
     ~ListNode() {
         cout << "delete " << val << "\n";
     }
 };

ListNode* createList(vector<int>& nums) {
    ListNode* pre=new ListNode(nums[0], nullptr);
    ListNode* head = pre;
    for(int i=1;i<nums.size();++i) {
        ListNode* cur = new ListNode(nums[i], nullptr);
        pre->next = cur;
        pre = cur;
    }
    return head;
}
void freeList(ListNode* head) {
    if (head == nullptr) return;
    ListNode* pre = head;
    head = head->next;
    while(head != nullptr) {
        delete pre; 
        pre = head;
        head = head->next; 
    }
    delete pre;
}

void printList(ListNode* head) {
    while (head != nullptr) {
        cout << head->val << ",";
        head = head->next;
    }
    cout << "\n";
}


    ListNode* reverseList(ListNode* head) {
        if (head == nullptr) return head;

        ListNode *pre=nullptr, *cur=head, *next;
        while (cur != nullptr) {
            next = cur->next;
            cur->next = pre;
            pre = cur;
            cur = next;
        }
        return pre;
    }

    int length(ListNode* head) {
        if (head == nullptr) return 0;
        int len = 0;
        while (head != nullptr) {
            ++len;
            head = head->next;
        }
        return len;
    }

    ListNode* rotateRight(ListNode* head, int k) {
        int len = length(head);
        k %= len;
        if (k == 0) return head;

        head = reverseList(head);
        ListNode *part2=head, *pre=nullptr;
        while (k > 0 && part2 != nullptr) {
            pre = part2;
            part2 = part2->next;
            --k;
        }
        if (part2 == nullptr) {
            return head;
        } else {
            pre->next = nullptr;
            pre = head;
            head = reverseList(head);
            part2 = reverseList(part2);
            pre->next = part2;
            return head;
        }
    }
    
int main() {
    vector<int> nums= {1,2,3,4,5};
    ListNode* head = createList(nums);
    printList(head);
    int len = length(head);
    cout << "length: " << len<<"\n";
    //ListNode* head1 = reverseList(head);
    ListNode* head1=rotateRight(head,2);
    printList(head1);
    freeList(head1);
}




