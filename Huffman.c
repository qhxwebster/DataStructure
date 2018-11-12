#include<stdio.h>
#include<stdlib.h>

#define MAXSIZE 1000

int cnt;

typedef char datatype;
typedef struct {
  int weight;
  datatype data;
  int lchild, rchild, parent;
}huffmantree;

typedef struct {
  int *bits;
  int start;
  datatype data;
}codetype;

void HuffmanTree(huffmantree *tree, int *weight);
void HuffmanCode(codetype *code, huffmantree *tree);
void HuffmanDecode(codetype *code, huffmantree *tree, char *k, char *result);


int main(){
    FILE *file1, *file2, *file3, *file4;
    file1 = fopen("input1.txt", "r+");
    file2 = fopen("output1.txt", "r+");
    file3 = fopen("input2.txt", "r+");
    file4 = fopen("output2.txt", "r+");

    huffmantree *tree;
    codetype *code;
    char *k, *result;
    int weight[128] = {0};

    char c = fgetc(file1);
    while(c != EOF){
        if((int)c != 10){
            weight[(int)c]++;
        }
        c = fgetc(file1);
    }

    for(int i = 0; i < 128; i++){
        if(weight[i] != 0){
            cnt++;
        }
    }

    tree = (huffmantree*)malloc((2*cnt - 1)*sizeof(huffmantree));
    code = (codetype*)malloc(cnt*sizeof(huffmantree));
    for(int i = 0; i < cnt; i++){
        code[i].bits = (int*)malloc(cnt*sizeof(int));
    }
    k = (char*)malloc(MAXSIZE);
    result = (char*)malloc(MAXSIZE);

    fscanf(file3, "%s", k);
    printf("%s", k);

    HuffmanTree(tree, weight);
    HuffmanCode(code, tree);
    HuffmanDecode(code, tree, k, result);
    fprintf(file2, "no\tdata\tp\tl\tr\tw\n");
    for(int i = 0; i < ( 2*cnt - 1 ); i++){
        fprintf(file2, "%d\t%c\t\t%d\t%d\t%d\t%d\n", i, tree[i].data, tree[i].parent, tree[i].lchild, tree[i].rchild, tree[i].weight);
    }
    for(int i = 0; i < cnt; i++){
        fprintf(file2, "%c\t", code[i].data);
        for(int j = code[i].start; j < cnt; j++){
            fprintf(file2, "%d" , code[i].bits[j]);
        }
        fprintf(file2, "\n");
    }
    fprintf(file4, "%s", result);

    fclose(file1);
    fclose(file2);
    fclose(file3);
    fclose(file4);

    free(tree);
    free(code);
    return 0;
}
/*
Build HuffmanTree
*/
void HuffmanTree(huffmantree *tree, int *weight){
    //initial
    for(int i = 0; i < ( 2*cnt - 1 ) ; i++){
        tree[i].parent = 0;
        tree[i].lchild = 0;
        tree[i].rchild = 0;
        tree[i].weight = 0;
        tree[i].data = 'X';
    }
    //put non-zero node into tree
    int j = 0;
    int i = 0;
    while(i < cnt){
        if(weight[j] != 0){
            tree[i].weight = weight[j];
            tree[i].data = (datatype)j;
            i++;
        }
        j++;
    }
    // rest child nodes
    for(int i = cnt; i < ( 2*cnt - 1 ); i++){
        int p1 = 0;
        int p2 = 0;
        int small1, small2;
        small1 = small2 = 10000;
        for(int j = 0; j < i; j++){
            if(tree[j].parent == 0){
                if(tree[j].weight < small1){
                    small2 = small1;
                    small1 = tree[j].weight;
                    p2 = p1;
                    p1 = j;
                }else if(tree[j].weight<small2){
                    small2 = tree[j].weight;
                    p2 = j;
                }
            }
        }
        tree[p1].parent = i;
        tree[p2].parent = i;
        tree[i].lchild = p1;
        tree[i].rchild = p2;
        tree[i].weight = tree[p1].weight + tree[p2].weight;
    }
}

//encode
void HuffmanCode(codetype *code, huffmantree *tree){
    int i, c, p, temp;
    for( i = 0; i < cnt; i++){
        temp = cnt;
        c = i;
        p = tree[c].parent;
        code[i].data = tree[c].data;
        while( p != 0 ){
            temp--;
            if(tree[p].lchild == c){
                code[i].bits[temp] = 0;
            }else{
                code[i].bits[temp] = 1;
            }
            c = p;
            p = tree[c].parent;
        }
        code[i].start = temp;
    }
}

//decode
void HuffmanDecode(codetype *code, huffmantree *tree, char *k, char *result){
    int temp = 0, flag = 0;
    for(int i = cnt; i < ( 2*cnt - 1 ); i++){
        if(tree[i].parent == 0){
            temp = flag = i;
        }
    }
    while((*k) != '\0' && (*k) != '\n'){
        if(*(k++) == '0'){
            temp = tree[temp].lchild;
        }else{
            temp = tree[temp].rchild;
        }
        if(tree[temp].lchild == 0 && tree[temp].rchild == 0){
            *(result++) = tree[temp].data;
            printf("%c", tree[temp].data);
            temp = flag;
        }
    }
    *result = '\0';
}
