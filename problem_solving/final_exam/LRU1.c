//LRU with hash table & doubly linked list
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

long long gettimediff(unsigned int nFlag)
{
    const long long NANOS = 1000000000LL;
    static struct timespec startTS, endTS;
    static long long retDiff = 0;
    if (nFlag == 0)
    {
        retDiff = 0;
        if (-1 == clock_gettime(CLOCK_MONOTONIC, &startTS))
            printf("failed\n");

    }
    else
    {
        if (-1 == clock_gettime(CLOCK_MONOTONIC, &endTS))
            printf("failed\n");
        // printf("%ld is %ld \n",endTS.tv_nsec,startTS.tv_nsec);
        retDiff = NANOS * (endTS.tv_sec - startTS.tv_sec) + (endTS.tv_nsec - startTS.tv_nsec);
    }
    return retDiff;

}
int printQuery = 0; // 1-> true, 0-> false 채점시 0
int printCurrentCache = 1; // 1-> true, 0-> false 채점시 lRUCacheGet의 리턴값을 출력함.
int Capacity = 0, num = 0;
////////////////////////////
// leetcode skeleton part //
////////////////////////////
typedef struct LRUCache {
    int key;
    struct LRUCache* next;
    struct LRUCache* prev;
} LRUCache;

typedef struct Hash {
    int key;
    int value;
    LRUCache* nodep;
} Hash;

Hash hash[10000];
LRUCache* head;
LRUCache* tail;

void lRUCacheFree(LRUCache* obj) {
    free(obj);
}

LRUCache* lRUCacheCreate(int capacity) {
    Capacity = capacity;
    memset(hash, NULL, 10000);
    LRUCache* p = (LRUCache*)malloc(sizeof(LRUCache*));

    head = (LRUCache*)malloc(sizeof(LRUCache*));
    tail = (LRUCache*)malloc(sizeof(LRUCache*));
    head = p;
    tail = p;
    p->next = NULL;
    p->prev = NULL;
    return p;
}

int lRUCacheGet(LRUCache* obj, int key) {
    if (hash[key].nodep == NULL) 
        return -1;    

    LRUCache* po = hash[key].nodep;

    if (po == head) {//head노드 접근시 업데이트 필요 없음
        return hash[key].value;
    }
    else if (po == tail) {//tail노드 접근시
        po->next->prev = NULL;
        tail = po->next;
        po->next = NULL;
        head->next = po;
        po->prev = head;
        head = po;
        return hash[key].value;
    }
    else { //중간에 낀 노드들 접근시
        po->next->prev = po->prev;
        po->prev->next = po->next;
        po->next = NULL;
        head->next = po;
        po->prev = head;
        head = po;
        return hash[key].value;
    }
}

void lRUCachePut(LRUCache* obj, int key, int value) {
    hash[key].key = key;
    hash[key].value = value;

    if (hash[key].nodep != NULL) {//캐시 안에 있는 노드
        lRUCacheGet(obj, key);
        return;
    }

    if (num >= Capacity) { //tail 노드 삭제
        LRUCache* rnode = tail;
        hash[tail->key].nodep = NULL;
        (tail->next)->prev = NULL;
        tail = tail->next;
        free(rnode);
        num--;
    }

    if (num == 0) {//첫 노드 -> p에다가 값 저장하기 
        obj->key = key;
        hash[key].nodep = obj;
        num++;
    }
    else {
        LRUCache* temp = (LRUCache*)malloc(sizeof(LRUCache*));
        temp->key = key;
        head->next = temp;
        temp->prev = head;
        temp->next = NULL;
        head = temp;
        hash[key].nodep = temp;
        num++;
    }
}

///////////////
// Upto here //
///////////////

void GetLRUCache(int key){
    printf("GET (%d)\n", key);
}

void PutLRUCache(int key, int value){
    printf("PUT (%d,%d)\n", key, value);
}

// Operation type
// 0 size      --> Setting cache size
// 1 key value --> Put (key, value)
// 2 key       --> Get (key)

int main(int argc, char** argv){
    if (argc != 2)
        return 0;

    char filename_out[25] = {};
    char filename_kv[25] = {};
    strncpy(filename_out, argv[1], strlen(argv[1]));
    strncpy(filename_kv, argv[1], strlen(argv[1]));
    strcat(filename_out, ".out");
    strcat(filename_kv, ".kv");

    FILE* fop = fopen(filename_out, "r");
    FILE* fkv = fopen(filename_kv, "r");
    int* keys = 0;
    int* values = 0;
    int num_pairs = -1;
    gettimediff(0);
    char* operation = (char*)malloc(sizeof(char) * 100);
    LRUCache* myCache = 0;

    while (fgets(operation, 100, fop) != NULL) {
        char* op = strtok(operation, " ");
        int func = atoi(op);
        switch (func) {
        case 0:
        {
            char* c_size = strtok(NULL, " ");
            myCache = lRUCacheCreate(atoi(c_size));
            // Initialize k,v table
            fscanf(fkv, "%d", &num_pairs);
            keys = malloc(sizeof(int) * num_pairs);
            values = malloc(sizeof(int) * num_pairs);
            for (int i = 0; i < num_pairs; i++) {
                fscanf(fkv, "%d %d", &keys[i], &values[i]);
            }
            break;
        }
        case 1:
        {
            char* key = strtok(NULL, " ");
            char* value = strtok(NULL, " ");

            if (printQuery)
                PutLRUCache(atoi(key), atoi(value));

            lRUCachePut(myCache, atoi(key), atoi(value));
            break;
        }
        case 2:
        {
            char* key = strtok(NULL, " ");
            int key_as_num = atoi(key);
            int value = lRUCacheGet(myCache, key_as_num);

            if (printQuery)
                GetLRUCache(key_as_num);

            for (int i = 0; i < num_pairs; i++) {
                if (keys[i] == key_as_num) {
                    // Check for correct return. Naive implementation
                    if (value != -1 && value != values[i]) {
                        fprintf(stderr, "WRONG ANSWER %d %d %d\n", value, atoi(key), values[i]);
                        return 0;
                    }
                }
            }

            break;
        }
        default:
            fprintf(stderr, "ERROR: WRONG OP\n");
            return 1;
        }
    }
    printf("%llu \n", gettimediff(1));
    fclose(fop);
    fclose(fkv);
    free(keys);
    free(values);
    lRUCacheFree(myCache);
    printf("FINISH\n");
    return 0;
}
