#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include<string>
#include<string.h>

#define MAX_TREE_HT 1000
#define MAX 1000
#define MAX_CH 200000
#define MAX_HOLD 999999

using namespace std;

int last = -1;

struct MinHeapNode {

    char data;
    unsigned freq;

    struct MinHeapNode *left, *right;
};

struct MinHeap {

    unsigned size;
    unsigned capacity;
    struct MinHeapNode** array;
};

typedef struct Codes{
    char ch[MAX] = {NULL};
    char code[MAX][100];

}CODES;
CODES C;

typedef struct code_8bits
{
    long int decimals[12500];
    char bin_8bits[MAX][8];
    char string_decode[MAX_CH];
    char string_code[MAX_CH];

}DECI;
DECI D[MAX];
DECI B[MAX];
DECI DECODE[MAX];

struct MinHeapNode* newNode(char data, unsigned freq){

    struct MinHeapNode* temp = (struct MinHeapNode*)malloc(sizeof(struct MinHeapNode));

    temp->left = temp->right = NULL;
    temp->data = data;
    temp->freq = freq;

    return temp;
}

void swapMinHeapNode(struct MinHeapNode** a,struct MinHeapNode** b){

    struct MinHeapNode* t = *a;

    *a = *b;
    *b = t;
}

void minHeapify(struct MinHeap* minHeap, int idx){

    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < minHeap->size && minHeap->array[left]->freq < minHeap->array[smallest]->freq)
        smallest = left;

    if (right < minHeap->size && minHeap->array[right]->freq < minHeap->array[smallest]->freq)
        smallest = right;

    if (smallest != idx) {
        swapMinHeapNode(&minHeap->array[smallest],&minHeap->array[idx]);
        minHeapify(minHeap, smallest);
    }
}

int isSizeOne(struct MinHeap* minHeap){

    return (minHeap->size == 1);
}

struct MinHeapNode* extractMin(struct MinHeap* minHeap){

    struct MinHeapNode* temp = minHeap->array[0];

    minHeap->array[0] = minHeap->array[minHeap->size - 1];

    --minHeap->size;

    minHeapify(minHeap, 0);

    return temp;
}

void insertMinHeap(struct MinHeap* minHeap,struct MinHeapNode* minHeapNode){

    ++minHeap->size;

    int i = minHeap->size - 1;

    while (i && minHeapNode->freq < minHeap->array[(i - 1) / 2]->freq) {
        minHeap->array[i] = minHeap->array[(i - 1) / 2];
        i = (i - 1) / 2;
    }

    minHeap->array[i] = minHeapNode;
}

void buildMinHeap(struct MinHeap* minHeap){

    int n = minHeap->size - 1;
    for (int i = (n - 1) / 2; i >= 0; --i)
        minHeapify(minHeap, i);
}

int isLeaf(struct MinHeapNode* root){

    return !(root->left) && !(root->right);
}

struct MinHeap* createAndBuildMinHeap(char data[], int freq[], int size){

    struct MinHeap* minHeap = (struct MinHeap*)malloc(sizeof(struct MinHeap));

    minHeap->array = (struct MinHeapNode**)malloc(size * sizeof(struct MinHeapNode*));

    struct MinHeap* mh = minHeap;

    for (int i = 0; i < size; ++i)
        mh->array[i] = newNode(data[i], freq[i]);

    mh->size = size;

    buildMinHeap(mh);

    return minHeap;
}

struct MinHeapNode* buildHuffmanTree(char data[], int freq[], int size){

    struct MinHeapNode *left, *right, *top;

    struct MinHeap* minHeap = createAndBuildMinHeap(data, freq, size);

    while (!isSizeOne(minHeap)) {

        left = extractMin(minHeap);
        right = extractMin(minHeap);

        top = newNode('$', left->freq + right->freq);

        top->left = left;
        top->right = right;

        insertMinHeap(minHeap, top);
    }

    return extractMin(minHeap);
}

void generate_codes(struct MinHeapNode* root, int arr[], int top){

    if (root->left) {
        arr[top] = 0;
        generate_codes(root->left, arr, top + 1);
    }

    if (root->right) {
        arr[top] = 1;
        generate_codes(root->right, arr, top + 1);
    }

    if (isLeaf(root)) {

        last++;

        char code[10];
        for (int i = 0; i < top; ++i){
            C.code[last][i] = arr[i] + '0';
        }

        C.ch[last] = root->data;

    }
}

void HuffmanCodes(char data[], int freq[], int size){

    struct MinHeapNode* root = buildHuffmanTree(data, freq, size);

    int arr[MAX_TREE_HT], top = 0;

    generate_codes(root, arr, top);
}
void decompress(char fn[]){
        int size_of_code, size;
        int length_code;
        FILE * fp = fopen(fn,"r+");

        fscanf(fp,"%d %d %d",&size_of_code,&size,&length_code);

        size++;

        int line = 0, ch[size_of_code],dec[size_of_code],len[size_of_code],index = 0,i=0, cnt = 0;
        int num_of_arr = size /12500;
        int remainder_arr = size%12500;

           if(remainder_arr >0)
                    {
                        num_of_arr += 1;
                    }

          int INDEX = 0,dec_index = 0, counterxx = 0;

        while(!feof(fp)){
            if(line != 0){
                if(line > 0 && index < size_of_code){
                    fscanf(fp,"%d %d %d",&ch[index],&dec[index],&len[index]);
                    index++;

                }

                if(line > size_of_code){

                    fscanf(fp,"%d ",&D[INDEX].decimals[dec_index]);

                    i++;
                    dec_index++;
                    counterxx++;
                    cnt++;
                    if( counterxx == 12500)
                    {
                        INDEX++;

                        dec_index = 0;
                        counterxx = 0;
                    }
                }
            }

            line++;
        }

    fclose(fp);


    int k;

        char temp_holder[MAX][100];
        char bin_holder[MAX][100];
        int length[MAX], bin_len;
        int j;
        temp_holder[0][10] = '\0';

        for(i=0; i<size_of_code; i++){
            itoa(dec[i], temp_holder[i], 2);
            length[i] = strlen(temp_holder[i]);
            int kulang = len[i] - length[i];

            for(j=0; j<kulang; j++){
                bin_holder[i][j] = '0';
            }

            bin_holder[i][kulang] = '\0';
            strcat(bin_holder[i],temp_holder[i]);

        }

        int len_idx0 = length_code % 8;
        int len_bin_code, kulang=0;

        char lack_holder[cnt][8] = {NULL};

        char string_code[length_code] = {NULL};     ///final holder

        getch();

///CONVERT YUNG DECI -8bits BINARY
   ///DECI -BIN FOR REMAINDER
        int kulang2,d;
        itoa(D[0].decimals[0], B[0].bin_8bits[0], 2);

        kulang2 = len_idx0 - strlen(B[0].bin_8bits[0]);

       int dagdag =0;
        for(int j=0;j<kulang2;j++){
             lack_holder[0][j] = '0';
             lack_holder[0][j+1] = '\0';
             dagdag++;
        }

        strcat(lack_holder[0],B[0].bin_8bits[0]);
        strcat(string_code,lack_holder[0]);


        num_of_arr = INDEX;


    for(int i=0,c=1;i<=num_of_arr;i++)
        {


             if (i==num_of_arr)
            {
                while(c < dec_index)
                {

                itoa(D[i].decimals[c],B[i].bin_8bits[c], 2);

               len_bin_code = strlen(B[i].bin_8bits[c]);

               kulang = 8 - len_bin_code;


               for(int j=0;j<kulang;j++){
                lack_holder[i][j] = '0';
                lack_holder[i][j+1] = '\0';

                }


                if(kulang>0){
                    strcat(lack_holder[i],B[i].bin_8bits[c]);

                    strcat(string_code,lack_holder[i]);

                }else{
                    strcat(string_code,B[i].bin_8bits[c]);

                    }


                c++;

            }
        }

            else{

            while(c<12500)
            {
                itoa(D[i].decimals[c],B[i].bin_8bits[c], 2);


               len_bin_code = strlen(B[i].bin_8bits[c]);
               kulang = 8 - len_bin_code;


               for(int j=0;j<kulang;j++){
                lack_holder[i][j] = '0';
                lack_holder[i][j+1] = '\0';
                }

                if(kulang>0){
                    strcat(lack_holder[i],B[i].bin_8bits[c]);

                    strcat(string_code,lack_holder[i]);


                }else{
                    strcat(string_code,B[i].bin_8bits[c]);
                    }

                c++;


            }

                c = 0;
            }


        }


        length_code = strlen(string_code);

       char temp[MAX] ={NULL},string_decode[MAX_CH] ={NULL};
       int indx = -1 ,x=0;

        for(int i=0; i<length_code; i++)   ///kung ilang char meron sa reference code
        {
            temp[++indx] = string_code[i];

            for(int j=0;j<size_of_code;j++)  ///length code = length ng buong binary
            {

                if(strcmp(temp,bin_holder[j]) == 0)
                {
                    indx = -1;
                    memset(temp,0,sizeof temp);
                    string_decode[x++] = ch[j];
                    break;
                }

        }


        }

        fp = fopen(fn, "w+");
        fprintf(fp,"%s",string_decode);
        fclose(fp);

}

void compress(char fn[]){

    char holder[MAX_HOLD] = {NULL};
    int c[100];
    FILE *read;
    read = fopen(fn, "r+");


    int xyz = 0;
    int z = 0;

    while(!feof(read)){
        c[z] = fgetc(read);
        xyz++;
        for(int i=0;i<=last;i++){
            if(c[z] == C.ch[i]){
                strcat(holder, C.code[i]);
            }
        }

        if(z == 200000 * z+1){
            z++;
        }
    }
    long int len_bin = strlen(holder);
    long int size =  len_bin / 8;
    int flag = 0, sobra =0;

    if(len_bin>=8){   ///ETO BINAGO MO TO
        sobra = len_bin % 8;
        if(sobra != 0){
            size++;
            flag = 1;
        }
    }


    int counter = 0, last = 0,k;
    long int  dec = 0;

        long int num_of_arr,remain_arr;
        int j=0;


    for(int i= len_bin - 1,j=0; i >= sobra ; i--){
        k =  holder[i] - '0' ;
        k = k << counter;
        dec = dec + k;
        counter++;

        if(counter == 8){

            D[j].decimals[last] = dec;


            last++;
            counter = 0;
            dec = 0;

                if(size<12500)
                {
                    j=0;
                    num_of_arr = 1;
                }


                if(size>12500)
                {

                    num_of_arr = size/12500;

                    remain_arr = size % 12500;

                    if(remain_arr>0)
                    {
                        num_of_arr += 1;
                    }


                }

                 if(last>=12500)
                {
                    last = 0;
                    j++;
                }


        }
    }


        if(flag == 1){

        for(int i = sobra - 1, j=0; i >=0; i--,j++){
            k =  holder[i] - '0';
            k = k << j;
            dec = dec + k;
        }


        int last_arr = num_of_arr -1;
        D[last_arr].decimals[last] = dec;
        last++;

    }

    int size_of_code = strlen(C.ch);
    int ch_code[size_of_code], bin_code[size_of_code], bin = 0, cd_val = 0;
    char code_arr[MAX] = {NULL};




    for(int i=0; i<size_of_code; i++){
        strcpy(code_arr,C.code[i]);
        for(int j = strlen(code_arr)-1, k=0; j>=0; j--, k++){
            bin  = code_arr[j] - '0';
            bin = bin << k;

            cd_val = cd_val + bin;

        }

        ch_code[i] = C.ch[i];
        bin_code[i] = cd_val;
        cd_val = 0;
    }


    int len_of_binCode[size_of_code];


    FILE *fp;
    fp = fopen(fn,"w+");
    fprintf(fp,"%d %d %d\n",size_of_code,size,len_bin);
    fclose(fp);

    fp = fopen(fn, "a");
    for(int i=0;i<size_of_code;i++){
        len_of_binCode[i] = strlen(C.code[i]);
        fprintf(fp,"%d %d %d\n",ch_code[i],bin_code[i],len_of_binCode[i]);
    }

    for(int i=num_of_arr - 1, j=last-1 ; i >=0 ;i--)
    {

            while(j!=-1)
            {
                fprintf(fp,"%d ",D[i].decimals[j--]);
            }

            if(num_of_arr > 1 )
            {
                j = 12500;
            }

    }
    fclose(fp);

}

int count_frequency(char fn[]){

    int c, size = 0, ch[255], count = 0;

    for(int i=0;i<256;i++){
        ch[i] = 0;
    }

    FILE * fp;
    fp = fopen(fn, "r+");

    if(fp){
        while(!feof(fp)){
            c = fgetc(fp);
            if(ch[c] == 0){
                count++;
            }
            ch[c]++;
            size++;
        }


        if(size < 10){
            return 2;
        }

    }else{
        fclose(fp);
        return 1;
    }
    fclose(fp);


    float weight[count];
    int freq[count];
    char arr[count] = {NULL};
    int j = -1;

    for(int i=0;i<256;i++){
        if(ch[i]!=0){
            j++;
            weight[j] = (float)ch[i] / size;
            freq[j] = ch[i];
            arr[j] = i;
        }
    }

    HuffmanCodes(arr, freq, count);

    return 0;
}

int main(void){


        system("cls");

        int opt;
        printf("\n\t\t\t     ==============================================================");
        printf("\n\t\t\t     || \t\t\t\t\t\t\t ||");
        printf("\n\t\t\t     || \t\t     FILE COMPRESSION \t\t\t ||");
        printf("\n\t\t\t     || \t\t\t\t\t\t\t ||");
        printf("\n\t\t\t     ==============================================================\n");

        printf("\n\t\t\t\t\t\t          MENU\n");
        printf("\n\t\t\t     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
        printf("\n\t\t\t\t\t\t      [1].Compress");
        printf("\n\t\t\t     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
        printf("\n\t\t\t\t\t\t      [2].Decompress");
        printf("\n\t\t\t     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
        printf("\n\t\t\t\t\t\t      [3].Exit");
        printf("\n\t\t\t     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
        printf("\n\t\t\t\t\t\t    Enter Your Choice:");

        scanf("%d",&opt);

        char fn[50];

        switch(opt){
            case 1:{system("cls");
                printf("\n\t\t\t     ==============================================================");
                printf("\n\t\t\t     || \t\t\t\t\t\t\t ||");
                printf("\n\t\t\t     || \t\t      COMPRESSION \t\t\t ||");
                printf("\n\t\t\t     || \t\t\t\t\t\t\t ||");
                printf("\n\t\t\t     ==============================================================\n");
                printf("\n\t\t\t     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
                printf("\n\t\t\t\t\t\tFile name to Compress:");

                scanf("%s",fn);

                int flag = count_frequency(fn);

                if(flag == 1){
                    printf("\n\t\t\t     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
                    printf("\n\t\t\t\t\t\t   No File Found...");
                }else if(flag == 2){
                    printf("\n\t\t\t     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
                    printf("\n\t\t\t\t\t\tFile is too small to compress..");
                }else{
                    compress(fn);
                    printf("\n\t\t\t     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
                    printf("\n\t\t\t\t\t\tFile Successfully Compressed!");

                }
                getch();break;
            }
            case 2:{system("cls");
                printf("\n\t\t\t     ==============================================================");
                printf("\n\t\t\t     || \t\t\t\t\t\t\t ||");
                printf("\n\t\t\t     || \t\t     DECOMPRESSION \t\t\t ||");
                printf("\n\t\t\t     || \t\t\t\t\t\t\t ||");
                printf("\n\t\t\t     ==============================================================\n");
                printf("\n\t\t\t     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
                printf("\n\t\t\t\t\t    File name to Decompress:");
                scanf("%s",fn);
                FILE *fp = fopen(fn,"r");
                if(fp){
                    fclose(fp);
                    decompress(fn);
                    printf("\n\t\t\t     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
                    printf("\n\t\t\t\t\t    File Successfully Decompressed!");
                }else{
                    fclose(fp);
                    printf("\n\t\t\t     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
                    printf("\n\t\t\t\t\t\t   No File Found...");
                }

            }
                getch();break;
            case 3:
                exit(0);
            default:
                break;
        }
}



