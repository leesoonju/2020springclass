//LRU with simple binary search tree & doubly linked list
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int printQuery = 1; // 1-> true, 0-> false 채점시 0
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
    struct tnode* left; //왼쪽 자식 노드 포인팅
    struct tnode* right; //오른쪽 자식 노드 포인팅
    struct LRUCache* cnode; //캐시를 포인팅
}tnode;
tnode* root;
tnode* cur;
LRUCache* lRUCacheCreate(int capacity) {
    root = (tnode*)malloc(sizeof(tnode));
    cur = root;
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

tnode* newNode(int key, int value) {
    tnode* newnode = (tnode*)malloc(sizeof(tnode));
    newnode->key = key;
    newnode->value = value;
    newnode->left = NULL;
    newnode->right = NULL;
    return newnode;
}

tnode* insert(tnode* node, int key, int value) {
    if (node == NULL)
        return newNode(key, value);

    if (key < node->key)
        node->left = insert(node->left, key, value);
    else
        node->right = insert(node->right, key, value);
    return node;
}

tnode* minValueNode(tnode* node) {
    tnode* current = node;
    while (current && current->left != NULL)
        current = current->left;

    return current;
}

tnode* deletenode(tnode* root, int key) {
    if (root == NULL)
        return root;

    if (key < root->key)
        root->left = deletenode(root->left, key);
    else if (key > root->key)
        root->right = deletenode(root->right, key);
    else {
        if (root->left == NULL) {
            tnode* temp = root->right;
            root->cnode = NULL;
            free(root);
            return temp;
        }
        else if (root->right == NULL) {
            tnode* temp = root->left;
            root->cnode = NULL;
            free(root);
            return temp;
        }
        tnode* temp = minValueNode(root->right);
        root->key = temp->key;
        root->right = deletenode(root->right, temp->key);
    }
    return root;
}

LRUCache* searchNode(int key) {
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
    LRUCache* po = searchNode(key);

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
    LRUCache* po = searchNode(key);
    if (po != NULL) {//이미 캐시 안에 있는 노드
        lRUCacheGet(obj, key);
        return;
    }

    if (num >= Capacity) { //tail 노드 삭제
        cur = deletenode(cur, tail->key);
        LRUCache* rnode = tail;
        (tail->next)->prev = NULL;
        tail = tail->next;
        free(rnode);
        num--;
    }

    if (num == 0) {//첫 노드 -> p에다가 값 저장하기 
        obj->key = key;
        obj->value = value;
        root->key = key;
        root->value = value;
        root->cnode = obj;
        root->left = NULL;
        root->right = NULL;
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
