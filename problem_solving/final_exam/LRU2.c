//LRU with AVL tree & Doubly linked list
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int printQuery = 0; // 1-> true, 0-> false 채점시 0
int printCurrentCache = 1; // 1-> true, 0-> false 채점시 1 IRUCacheGet의 리턴값을 출력함.

int Capacity = 0, num = 0;

typedef struct LRUCache {
    int key;
    int value;
    struct LRUCache* next;
    struct LRUCache* prev;
} LRUCache;
LRUCache* head;
LRUCache* tail;

typedef struct tnode {//avl트리는 왼,오른쪽 자식 노드의 높이 차이가 최대 +1, 최소 -1의 차이까지만 허용함  
    int key;
    int value;
    int height; //트리의 높이를 표현, 해당 노드부터 leaf 노드까지 가장 긴 경로의 길이 
    struct tnode* left; //왼쪽 자식 노드 포인팅
    struct tnode* right; //오른쪽 자식 노드 포인팅
    struct LRUCache* cnode; //캐시를 포인팅
}tnode;
tnode* root;
tnode* cur;

LRUCache* lRUCacheCreate(int capacity) {
    Capacity = capacity;
    LRUCache* p = (LRUCache*)malloc(sizeof(LRUCache*));
    head = (LRUCache*)malloc(sizeof(LRUCache*));
    tail = (LRUCache*)malloc(sizeof(LRUCache*));
    head = p;
    tail = p;
    p->next = NULL;
    p->prev = NULL;
    return p;
}

int height(tnode* n) {
    if (n == NULL)
        return 0;
    return n->height;
}

int bigger(int a, int b) {
    return (a > b) ? a : b;
}

tnode* newNode(int key, int value) {
    tnode* newnode = (tnode*)malloc(sizeof(tnode));
    newnode->key = key;
    newnode->value = value;
    newnode->left = NULL;
    newnode->right = NULL;
    newnode->height = 1;  // new node is initially added at leaf// 근데 왜 1이지? 0아닌가 
    if (num == 0)
        root = newnode;
    return newnode;
}

tnode* rightRotate(tnode* y) {//왼쪽 자식 노드의 높이가 오른쪽 자식 노드보다 1이상 커서 왼쪽으로 치우친 상태, 오른쪽으로 회전해야함
    tnode* x = y->left;
    tnode* T2 = x->right;
    x->right = y;
    y->left = T2;
    y->height = bigger(height(y->left), height(y->right)) + 1;
    x->height = bigger(height(x->left), height(x->right)) + 1;
    if (y == root)
        root = x;
    return x;
}

tnode* leftRotate(tnode* x) {//오른쪽 자식 노드의 높이가 왼쪽 자식 노드보다 1이상 커서 오른쪽으로 치우친 상태, 왼쪽으로 회전해야함
    tnode* y = x->right;
    tnode* T2 = y->left;
    y->left = x;
    x->right = T2;
    x->height = bigger(height(x->left), height(x->right)) + 1;
    y->height = bigger(height(y->left), height(y->right)) + 1;
    if (x == root)
        root = y;
    return y;
}

int getBalance(tnode* n) {
    if (n == NULL)
        return 0;
    return height(n->left) - height(n->right);
}

tnode* insert(tnode* node, int key, int value) {
    if (node == NULL)
        return(newNode(key, value));

    if (key < node->key)
        node->left = insert(node->left, key, value);
    else if (key > node->key)
        node->right = insert(node->right, key, value);
    else
        return node;

    node->height = 1 + bigger(height(node->left), height(node->right));

    int balance = getBalance(node);
    if (balance > 1 && key < node->left->key)
        return rightRotate(node);
    if (balance < -1 && key > node->right->key)
        return leftRotate(node);
    if (balance > 1 && key > node->left->key) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }
    if (balance < -1 && key < node->right->key) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }
    return node;
}

tnode* minValueNode(tnode* node) {
    tnode* current = node;

    while (current->left != NULL)
        current = current->left;
    return current;
}

tnode* deleteNode(tnode* root, int key) {
    if (root == NULL)
        return NULL;

    if (key == root->key) {
        root->cnode = NULL; //연결 끊음
        return root;
    }
    if (key < root->key)
        root->left = deleteNode(root->left, key);
    else if (key > root->key)
        root->right = deleteNode(root->right, key);
    else { //삭제할 노드 찾음
        return NULL;
        if ((root->left == NULL) || (root->right == NULL)) { //자식 노드 1개거나 아예 없거나
            tnode* temp = root->left ? root->left : root->right;
            if (temp == NULL) {
                temp = root;
                root = NULL;
            }
            else {
                *root = *temp;
            }
            free(temp);
        }
        else {// 자식 노드가 둘 다 있음
            tnode* temp = minValueNode(root->right);
            root->key = temp->key;
            root->right = deleteNode(root->right, temp->key);
        }
    }

    if (root == NULL)
        return root;

    root->height = 1 + bigger(height(root->left), height(root->right));
    int balance = getBalance(root);

    if (balance > 1 && getBalance(root->left) >= 0)
        return rightRotate(root);
    if (balance > 1 && getBalance(root->left) < 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }
    if (balance < -1 && getBalance(root->right) <= 0)
        return leftRotate(root);
    if (balance < -1 && getBalance(root->right) > 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }
    return root;
}

LRUCache* getvalue(tnode* Cur, int key) {
    tnode* p = root;
    while (p != NULL) {
        if (key < p->key)
            p = p->left;
        else if (key > p->key)
            p = p->right;
        else if (key == p->key)
            return p->cnode;
    }
    return NULL;//못찾음
}

int lRUCacheGet(LRUCache* obj, int key) {
    LRUCache* po = getvalue(root, key);

    if (po == NULL)
        return -1;

    if (po == head) {//head노드 접근시 업데이트 필요 없음
        return po->value;
    }
    else if (po == tail) {//tail노드 접근시
        po->next->prev = NULL;
        tail = po->next;
        po->next = NULL;
        head->next = po;
        po->prev = head;
        head = po;
        return po->value;
    }
    else { //중간에 낀 노드들 접근시
        po->next->prev = po->prev;
        po->prev->next = po->next;
        po->next = NULL;
        head->next = po;
        po->prev = head;
        head = po;
        return po->value;
    }
}

void lRUCachePut(LRUCache* obj, int key, int value) {
    LRUCache* po = getvalue(root, key);

    if (po != NULL) {//이미 캐시 안에 있는 노드
        lRUCacheGet(obj, key);
        return;
    }

    if (num >= Capacity) { //tail 노드 삭제
        cur = deleteNode(cur, tail->key);
        LRUCache* rnode = tail;
        (tail->next)->prev = NULL;
        tail = tail->next;
        free(rnode);
        num--;
    }

    if (num == 0) {//첫 노드 -> p에다가 값 저장하기 
        obj->key = key;
        obj->value = value;
        cur = insert(cur, key, value);
        root->cnode = obj;
        num++;
    }
    else {
        LRUCache* temp = (LRUCache*)malloc(sizeof(LRUCache*));
        temp->key = key;
        temp->value = value;
        head->next = temp;
        temp->prev = head;
        temp->next = NULL;
        head = temp;
        cur = insert(cur, key, value);
        tnode* p = root;
        while (p != NULL) {
            if (key < p->key)
                p = p->left;
            else if (key > p->key)
                p = p->right;
            else if (key == p->key) {
                p->cnode = temp;
                break;
            }
        }
        num++;
    }
}

void lRUCacheFree(LRUCache* obj) {
    free(obj);
}

void GetLRUCache(int key) {
    printf("GET (%d)\n", key);
}

void PutLRUCache(int key, int value) {
    printf("PUT (%d,%d)\n", key, value);
}

void PrintLRUCache() {
    printf("Print Cache\n");
}
