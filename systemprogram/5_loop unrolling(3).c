#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#define WORD_SIZE 40

typedef struct Node {
    char word[WORD_SIZE];
    struct Node* next;
}node;
void FrontBackSplit(node* source, node** frontRef, node** backRef);
void MergeSort(node** headRef);
void MergeSort(node** headRef);

node* CreateNode(char *fileName, node*);
node* findCommmonWords(node*, node*, node*);
node* find2Grams(node*head1, node*head2, node*inputHead);
int NumberOfNodes(node*iterator);
void removeDuplicates(node *head);
void printList(struct Node *head);
void printList2(struct Node *head);


void* Malloc(int num){
 return malloc(num);
}
void Free(void* var){
 free(var);
}
char* Strcpy(char*s1, char*s2){
 return strcpy(s1, s2);
}

int Strcmp(char*s1, char* s2){
 return strcmp(s1, s2);
}
char* Strcat(char*s1, char const*s2){
 return strcat(s1, s2);
}

int main(){

    node *head1,*head2, *commonWordList, *twoGramlist;

    head1 = CreateNode("file1.txt", head1);
    head2 = CreateNode("file2.txt", head2);

    commonWordList = findCommmonWords(head1, head2, commonWordList);
    printf("The number of common words : %d \n", NumberOfNodes(commonWordList));
    printList(commonWordList);

//since we sorted head1 and head2 at commonWordList we need to recreate
    head1 = CreateNode("file1.txt", head1);
    head2 = CreateNode("file2.txt", head2);

    twoGramlist = find2Grams(head1,head2,twoGramlist);
    MergeSort(&twoGramlist);
    removeDuplicates(twoGramlist);
    printf("\nThe number of common 2-grams : %d \n", NumberOfNodes(twoGramlist));
    printList2(twoGramlist);
}
//put words on a file to linked list
 node* CreateNode(char *fileName, node*head){
     unsigned int isEmpty = 1;//if file is empty value stays 1 else 0

    head=(struct Node*)Malloc(sizeof(struct Node));
    node *iterator = head;
    FILE* text = NULL;

    int Space = 0;
    char wordHolder[WORD_SIZE] ;
    int currChar = 0;
    int i = 0;

    //validate file
    if (fileName == "file1.txt"){
	if ((text = fopen("/home/leesoonju/바탕화면/4 - loop unrolling(3)/file1.txt", "r")) == NULL) {
        printf("Can not find file: %s", fileName);
        exit(0);
    	}
    }
    else if (fileName == "file2.txt"){
        if ((text = fopen("/home/leesoonju/바탕화면/4 - loop unrolling(3)/file2.txt", "r")) == NULL) {
        printf("Can not find file: %s", fileName);
        exit(0);
        }
    }


    // read from file until 1st space
    while(currChar != EOF){
        while (((currChar = fgetc (text)) != ' ' && currChar != '.'  && currChar != ','  && currChar != '!'  && currChar != '?'  && currChar != EOF)) {
            iterator->word[i] = currChar;
            wordHolder[i++] = currChar;
            Space=0;
            isEmpty = 0;
        }
        //check for multiple spaces
        if(Space==0){
            Space=1;
            Strcpy(iterator->word,wordHolder);
            memset(wordHolder, 0, sizeof wordHolder);
            iterator->next = (struct Node*)Malloc(sizeof(struct Node));
            iterator = iterator->next;
            i=0;
        }
}
if(isEmpty == 1){
    puts("Your file is empty.");
    exit(0);
}
    iterator->next=NULL;
    return head;
}

 node* findCommmonWords(node*head1, node*head2, node*inputHead){
    MergeSort(&head1);
    MergeSort(&head2);
    unsigned int isEmpty = 1;//if file is empty value stays 1 else 0
    inputHead=(struct Node*)Malloc(sizeof(struct Node));
    node *iterator1;//iterator for first list
    node *iterator2a;//iterator for second list
    node *iterator2b;
    node *iterator2c;
    node *iterator3 = inputHead;
    
    for(iterator1=head1;iterator1->next->word!=NULL;iterator1 = iterator1->next){
        //loop through second list elements
        for(iterator2a = head2, iterator2b = head2->next, iterator2c = head2->next->next; 
	    iterator2c->next->next->next->next->word!=NULL; 
	    iterator2a = iterator2c->next, iterator2b = iterator2a->next, iterator2c = iterator2b->next){
            if (Strcmp(iterator1->word, iterator2a->word) == 0){
                Strcpy(iterator3->word , iterator1->word);
                iterator3->next = (struct Node*)Malloc(sizeof(struct Node));
                iterator3 = iterator3->next;
            isEmpty = 0;
                break;
            }
            if (Strcmp(iterator1->word, iterator2b->word) == 0){
                Strcpy(iterator3->word , iterator1->word);
                iterator3->next = (struct Node*)Malloc(sizeof(struct Node));
                iterator3 = iterator3->next;
            isEmpty = 0;
                break;
            }
            if (Strcmp(iterator1->word, iterator2c->word) == 0){
                Strcpy(iterator3->word , iterator1->word);
                iterator3->next = (struct Node*)Malloc(sizeof(struct Node));
                iterator3 = iterator3->next;
            isEmpty = 0;
                break;
            }

        }

    }


    if(isEmpty == 1){
    puts("There are no common words.");
    exit(0);
}
    iterator3->next = NULL;
    removeDuplicates(inputHead);
    return inputHead;
}

 node* find2Grams(node*head1, node*head2, node*inputHead){
    unsigned int isEmpty = 1;//if file is empty value stays 1 else 0

    inputHead=(struct Node*)Malloc(sizeof(struct Node)*2+1);
    node *iterator1;//iterator for first list
    node *iterator2a;//iterator for second list
    node *iterator2b;
    node *iterator3 = inputHead;
    int i;

    for (iterator1 = head1; iterator1->next->word!=NULL; iterator1 = iterator1->next)
            iterator1->word[i] = tolower(iterator1->word[i]);
    for (iterator2a = head2; iterator2a->next->word!=NULL; iterator2a = iterator2a->next)
            iterator2a->word[i] = tolower(iterator2a->word[i]);

    //loop through first list elements

    for(iterator1=head1; iterator1->next->next->word!=NULL; iterator1 = iterator1->next){
        //loop through second list elements
        for(iterator2a = head2, iterator2b = head2->next; iterator2b->next->next->next->word!=NULL;
            iterator2a = iterator2b->next, iterator2b = iterator2a->next){

            if (Strcmp(iterator1->word, iterator2a->word) == 0  && Strcmp(iterator1->next->word, iterator2a->next->word) ==0){
                Strcpy(iterator3->word , iterator1->word);
                Strcat(iterator3->word," ");
                Strcat(iterator3->word, iterator1->next->word);
                iterator3->next = (struct Node*)Malloc(sizeof(struct Node)*2+1);
                iterator3 = iterator3->next;
                isEmpty = 0;
                break;
            }
            if (Strcmp(iterator1->word, iterator2b->word) == 0 && Strcmp(iterator1->next->word, iterator2b->next->word) ==0){
                Strcpy(iterator3->word ,iterator1->word);
                Strcat(iterator3->word," ");
                Strcat(iterator3->word, iterator1->next->word);
                iterator3->next = (struct Node*)Malloc(sizeof(struct Node)*2+1);
                iterator3 = iterator3->next;
                isEmpty = 0;
                break;

            }

        }
    }



    if(isEmpty == 1){
    puts("There are no common 2-grams.");
    exit(0);
}
    iterator3->next = NULL;
    return inputHead;
}



int NumberOfNodes(node*iterator){
    int i=0;
while(iterator->next!=NULL){
    i++;
    iterator=iterator->next;
}
 return i;
}
 
node* SortedMerge(node* a, node* b)
{
    node* result = NULL;

    /* Base cases */
    if (a == NULL)
        return (b);
    else if (b == NULL)
        return (a);

    /* Pick either a or b, and recur */
    if (strcmp(a->word, b->word) <= 0) {
        result = a;
        result->next = SortedMerge(a->next, b);
    }
    else {
        result = b;
        result->next = SortedMerge(a, b->next);
    }
    return (result);
}

void FrontBackSplit(node* source,node** frontRef, node** backRef)
{
    node* fast;
    node* slow;
    slow = source;
    fast = source->next;

    /* Advance 'fast' two nodes, and advance 'slow' one node */
    while (fast != NULL) {
        fast = fast->next;
        if (fast != NULL) {
            slow = slow->next;
            fast = fast->next;
        }
    }

    /* 'slow' is before the midpoint in the list, so split it in two
    at that point. */
    *frontRef = source;
    *backRef = slow->next;
    slow->next = NULL;
}
void MergeSort(node** headRef) {
    node* head = *headRef;
    node* a;
    node* b;

    for (int i = 0; i < WORD_SIZE; i++)
       (*headRef)->word[i] = tolower((*headRef)->word[i]);

    /* Base case -- length 0 or 1 */
    if ((head == NULL) || (head->next == NULL)) {
        return;
    }

    /* Split head into 'a' and 'b' sublists */
    FrontBackSplit(head, &a, &b);

    /* Recursively sort the sublists */
    MergeSort(&a);
    MergeSort(&b);

    /* answer = merge the two sorted lists together */
    *headRef = SortedMerge(a, b);
} 

void removeDuplicates(node *head){
    node *iterator1;//current node
    node *iterator2;//nodes we will compare
    node *iterator3;//previous node of compare node

    for(iterator1=head;iterator1->next->word!=NULL;iterator1 = iterator1->next){
            iterator3 = iterator1;
        //loop through our first node with the nodes in front
         for(iterator2 = iterator1->next; iterator2->next->word!=NULL;iterator2 = iterator2->next){
            if (Strcmp(iterator1->word, iterator2->word) == 0){
                 iterator3->next = iterator2->next;
            }else{
                iterator3 = iterator3->next;
            }
        }
    }
}
void printList(node *head){
  FILE * fp = fopen("/home/leesoonju/바탕화면/2 - merge sort/result.txt", "w");	
  do{
        fputs(head->word, fp);
	fputs("  ", fp);
        head = head->next;
    }while(head->next != NULL);
}
void printList2(node *head){
  FILE * fp = fopen("/home/leesoonju/바탕화면/2 - merge sort/result2.txt", "w");
  do{
        fputs(head->word, fp);
        fputs("\n ", fp);
        head = head->next;
    }while(head->next != NULL);
}

