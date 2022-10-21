#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include<string.h>
#include<string>

#define MAX_TREE_HT 1000

using namespace std;

typedef struct hfcode{

    char code[12];
    char ch;
    int dec_value;
    struct hfcode * next;

}HFCODE;

typedef struct binary{

    char holder[100];
    unsigned int length;
    unsigned char ch;
    struct binary * next;

}BINARY;

typedef struct string_binary_holder{

    char binary[100];
    struct string_binary_holder * next;

}HOLDER;

typedef struct reversed_holder{

    char binary[100];
    struct reversed_holder * next;

}RHOLDER;

RHOLDER * rh = NULL;
HOLDER * sbh = NULL;
BINARY * bin = NULL;
HFCODE * hfcd = NULL;

int counter = 0;

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

void insert_to_HFCODE(char c,int dec_val,char bin[], int flag){
    HFCODE *p, *q, *cd = (HFCODE*)malloc(sizeof(HFCODE));
    p = q = hfcd;

    int dec = 0, k;

    if(flag == 0){ /// compress
        for(int i = strlen(bin)-1, j=0;i>=0;i--,j++){
            k = bin[i] - '0';
            k = k << j;
            dec += k;
        }
    }else{ /// decompress
        dec = dec_val;
    }

    cd->ch = c;
    cd->dec_value = dec;
    strcpy(cd->code,bin);

    while(p!=NULL){
        q = p;
        p = p->next;
    }

    if(p == hfcd){
        hfcd = cd;
    }else{
        q->next = cd;
    }

    cd->next = p;
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

        char temp[12] = {'\0'};
        for (int x = 0; x < top; ++x){
            temp[x] = arr[x] + '0';
        }

        insert_to_HFCODE(root->data,0,temp,0);
    }
}

void HuffmanCodes(char data[], int freq[], int size){

    struct MinHeapNode* root = buildHuffmanTree(data, freq, size);

    int arr[MAX_TREE_HT], top = 0;

    generate_codes(root, arr, top);
}

int convert_to_decimal(HFCODE *cd,int bin_length_cd){
    int dec_val = 0, k;
    for(int i = bin_length_cd-1, j=0;i>=0;i--,j++){
        k = cd->code[i] - '0';
        k = k << j;
        dec_val += k;
    }
    return dec_val;
}

char convert_to_char(char dummy[]){

    unsigned int k, dec = 0;
    for(int i=strlen(dummy)-1, j=0;i>=0;i--,j++){
        k = dummy[i] - '0';
        k = k << j;
        dec += k;
    }
    return dec;
}

void insert_binary(char dummy[], int len){

    BINARY *p, *q, *binary = (BINARY*) malloc(sizeof(BINARY));
    p = q = bin;

    strcpy(binary->holder,dummy); /// mag store ng lahat ng decimal values na nakumbert
    binary->length = len;
    binary->ch = convert_to_char(dummy);
    printf(" [%d] ",binary->ch);

    while(p!=NULL){
        q = p;
        p = p->next;
    }
    if(p==bin){
        bin = binary;
    }else{
        q->next = binary;
    }
        binary->next = p;
}

HFCODE * search_code(int ch){

    HFCODE * cd;
    cd = hfcd;

    while(cd!=NULL && cd->ch != ch){
        cd = cd->next;
    }

    if(cd == NULL){
        return NULL;
    }else{
        return cd;
    }

}

char * generate_extension_name(char fn[], int flag){

    char extension_name[4] = {'\0'};
    if(flag == 1){
        strcpy(extension_name,"cmp");
    }else{
        strcpy(extension_name,"txt");
    }

    int x = 0, fn_len = strlen(fn);
    fn_len-=3;

    for(int i=fn_len;i<strlen(fn);i++)
        fn[i] = extension_name[x++]; ///eto pang kumbert to new extension name hehe try.txt ---> try.cmp

    return fn;
}

void insert_to_final_holder(HFCODE * temp){
    HOLDER *p, *q, *fh = (HOLDER*)malloc(sizeof(HOLDER));
    p = q = sbh;

    printf("\t%s-",temp->code);

    strcpy(fh->binary,temp->code);
    fh->next = NULL;

    while(p!=NULL){
      //  printf("\nLAMAN: %s",p->binary);
        q = p;
        p = p->next;
    }

    if(sbh == NULL){
        sbh = fh;
    }else{
        q->next = fh;
    }

    fh->next = p;
    counter++;
}

HOLDER * extract_last_binary(){

    HOLDER *z, *current;
    z = current = sbh;
    while(z->next!=NULL){
        current = z;
        z = z->next;
    }
    current->next = NULL;
    return z;
}

void invert_list(){
    printf("counter: %d",counter);getch();
    for(int i=0;i<counter;i++){ /// REREVERSE YUNG LIST

        RHOLDER *cptr, *nptr, *newNode = (RHOLDER*)malloc(sizeof(RHOLDER));
        cptr = nptr = rh;

        HOLDER * temp;
        temp = extract_last_binary();
        printf("\nLAST BIN: %s",temp->binary);
        strcpy(newNode->binary,temp->binary);

        while(nptr!=NULL){
            cptr = nptr;
            nptr = nptr->next;
        }

        if(nptr == rh){
            rh = newNode; ///PUTANGINANGYAN ETO YUNG ERROR NA INIISIP KO 2HRS TANGINA, HINDI KO PALA PINOPOINT SA ROOT SORRY
        }else{
            cptr->next = newNode;
        }

        newNode->next = nptr;
    }

}

void compress(char fn[]){

    char file_name[50] = {'\0'};
    strcpy(file_name,fn);

    //printf langs
    HFCODE *z = hfcd;
    while(z!=NULL)
    {
        printf("\nCH: %c  CODE: %s  DeCI: %d",z->ch,z->code,z->dec_value);
        z= z->next;
    }
    getch();


    printf("\n\n");
    strcpy(fn,generate_extension_name(fn,1));

    printf("new file name: %s", fn);getch();

    FILE * write = fopen(fn,"w+");

        HFCODE *p = hfcd;
        while(p->next!=NULL){ /// fix yung i wwrite nya, so di ma wwrite yung last pointer na naghold nung white space hehe
            fprintf(write,"%c",p->ch);
            p = p->next;
        }
        fprintf(write,"%c",0); ///delimeter
        p = hfcd;
        printf("\n\nTo compress:\n");
        while(p!=NULL){
            printf("[%d|%d]-",p->dec_value,strlen(p->code));
            fprintf(write,"%c%c",p->dec_value,strlen(p->code));
            p = p->next;
        }
        fprintf(write,"%c",0); ///delimeter

        int c, len_dummy = 0, len_binary = 0;
        char dummy[1000] = {'\0'};
        unsigned char ch_temp;


        FILE * read = fopen(file_name, "r+");
        printf("\nto write: \n");
        while(!feof(read)){

            HFCODE *temp;
            int dec_val, bin_length_cd;

            c = fgetc(read);
            temp = search_code(c); ///return address ng code sa structure

            if(temp !=NULL)
            {
                bin_length_cd = strlen(temp->code);
                dec_val = convert_to_decimal(temp,bin_length_cd);
                insert_to_final_holder(temp);
                //printf("\nTEMP->CODE: %c\t%s    %d",temp->ch,temp->code,dec_val);


            }

        }

        printf("\n\nbinary to decode: \n");
        HOLDER *r = sbh;

        while(r!=NULL){
            printf("%s-",r->binary);
            r = r->next;
        }


        r = sbh;
        char holder[1000] = {'\0'};
        char temp[1000] = {'\0'};

        int count = 1, binary_length = 0, binary_code = 0, excess = 0;

        while(r!=NULL){

            printf("\nnode: %s",r->binary);
            strcat(holder,r->binary);

            while(strlen(holder)>=8){
                binary_length = strlen(holder);
                binary_code++;

                char fixed_binary[8] = {'\0'}, to_add[8] = {'\0'};

                //printf("\nbinary: %s\t",holder);
                int index = 0, start = 0, end = strlen(holder);
                strcpy(fixed_binary,holder);
                fixed_binary[8] = '\0'; // cut

                printf("\nFX binary: %s\t",fixed_binary);

                for(int i=8;i<binary_length;i++)
                    to_add[index++] = holder[i];


                printf("\nTO ADD: %s",to_add);

                insert_binary(fixed_binary,strlen(fixed_binary));
                memset(holder,0,sizeof holder);
                memset(holder,0,sizeof fixed_binary);



               // memset(to_add2,0,sizeof to_add2);

char to_add2[1000] = {'\0'};

                if(strlen(to_add)>=8)
                {
                    printf("\nEXCESS >=8 \n");
                  //  getch();
                    int index2=0,to_add_len = strlen(to_add);
                    char to_add2[1000] = {'\0'};

                    strcpy(holder,to_add);
                    for(int i=8;i<binary_length;i++)
                        to_add2[index2++] = holder[i];
                    strcpy(fixed_binary,holder);
                    fixed_binary[8] = '\0'; // cut
                    binary_code++;
                    insert_binary(fixed_binary,strlen(fixed_binary));
                    memset(holder,0,sizeof holder);
                    memset(holder,0,sizeof fixed_binary);



                    strcpy(holder,to_add2);
                    printf("excess: %s\n",holder);

                    memset(to_add,0,sizeof to_add);
                    memset(to_add2,0,sizeof to_add2);

                }
                else{
                    strcpy(holder,to_add);
                    printf("excess: %s\n",holder);
                    memset(to_add,0,sizeof to_add);
                }

            }

            r = r->next;
        }

        if(binary_length!=0){

            binary_code++;

            printf("\nlast binary: %s ",holder);
            insert_binary(holder,strlen(holder));

            excess = strlen(holder);
        }

        fclose(read);

        fprintf(write,"%d",excess);

        int pos = 0;
        printf("counter: %d\n",binary_code);

        for(int i = binary_code - 1; i>=0;i--){

            BINARY * b = bin;
            while(pos!=i){
                b = b->next;
                pos++;
            }
            fprintf(write,"%c",b->ch);
            printf("value to print: %d - %c\n",b->ch,b->ch);
            pos = 0;
        }
    fclose(write);

    //remove(file_name);

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

        if(size < 1){
            return 2;
        }

    }else{
        fclose(fp);
        return 1;
    }
    fclose(fp);


    int freq[count];
    char arr[count];
    strcpy(arr,"");

    int j = -1;

    for(int i=0;i<256;i++){
        if(ch[i]!=0){
            j++;
            freq[j] = ch[i];
            arr[j] = i;
        }
    }

    HuffmanCodes(arr, freq, count);

    return 0;
}

void convert_to_binary(char c,int dec, int length){
    char temp[100] = {'\0'};
    itoa(dec,temp,2);

    if(length!=strlen(temp)){
        char bin_cd[100] = {'\0'};

        for(int i=0;i<length-strlen(temp);i++)
            bin_cd[i] = '0';

        strcat(bin_cd,temp);
        strcpy(temp,bin_cd);
        memset(bin_cd,0,sizeof bin_cd);
    }
    printf("binary: %s\n",temp);

    insert_to_HFCODE(c,dec,temp,1);
    memset(temp,0,sizeof temp);
}

void insert_to_HOLDER(char binary[]){

    HOLDER *p, *q, *fh = (HOLDER*)malloc(sizeof(HOLDER));
    p = q = sbh;

    strcpy(fh->binary,binary);

    while(p!=NULL){
        q = p;
        p = p->next;
    }

    if(p == sbh){
        sbh = fh;
    }else{
        q->next = fh;
    }

    fh->next = p;

}

HFCODE * search_binary_code(char buffer[]){
    HFCODE * call = hfcd;
    while(call!=NULL && strcmp(buffer,call->code)!=0){
        call = call->next;
    }
    if(call == NULL){
        return NULL;
    }else{
        return call;
    }
}

void decompress(char fn[]){
    char file_name[50] = {'\0'};
    strcpy(file_name,fn);

    fn = generate_extension_name(fn,0);

    FILE * read = fopen(file_name,"r+");

    int flag = 0, pos = 0,mod = 0, x, last_getc, index_code = 0, index_length = 0, length = 0, initial_binary, excess = 0, decode = 0;
    unsigned int c;
    char code[256][12] = {'\0'};
    char temp[256] = {'\0'};
    char holder[4] = {'\0'};

    printf("\n\n");

    while(!feof(read)){

        c = fgetc(read);
        if(c == 0 && flag == 0){
            flag = 1;
            printf("\t-end-\n");
            x = last_getc = counter + 1;
            x *= 2;
        }else{
            temp[counter] = c;
            counter++;
        }
        if(flag == 1 && last_getc != counter && c != -1){

            if(mod == 0){

                initial_binary = c;
                mod+=1;
                printf("[%d|",initial_binary);

            }else{

                length = c;
                mod = 0;
                printf("%d]",length);
                printf("\n");
                convert_to_binary(temp[pos++],initial_binary,length);

            }

            x--;
            if(x == 0)
                flag = 2;

        }
        if(flag == 2 && c != 0 && c != -1){
            if(x == 1){                                     /// excess
                excess = c - '0';
                printf("\nexcess: %d",excess);
                x++;
            }else if(x == 2){                               /// first index pag kulang ng zero dito dadagdag
                char buffer[8] = {'\0'}, binary[8] = {'\0'};
                itoa(c,buffer,2);

                for(int i=0; i<excess;i++)
                    binary[i] = '0';

                strcat(binary,buffer);
                printf("\nbin: %s : %d",binary,c);

                insert_to_HOLDER(binary);
                x++;
            }else if(x == 3){                               /// the rest of the codes
                char buffer[8] = {'\0'}, binary[8] = {'\0'};

                itoa(c,buffer,2);
                int add = 8 - strlen(buffer);

                for(int i=0; i<add;i++)
                    binary[i] = '0';

                strcat(binary,buffer);
                insert_to_HOLDER(binary);
                printf("\nbin: %s : %d",binary,c);

                decode++;
            }else{
                x++;                                        /// hop, para iwasan nya yung null character
            }
        }

    }

    fclose(read);

    char buffer[15] = {'\0'};
    int index = 0;
    counter = decode + 1;

    invert_list();

    FILE * write = fopen(fn,"w+");
    RHOLDER *p = rh;
    while(p != NULL){
    int len = strlen(p->binary);
      for(int i=0; i<len; i++){ /// sorry ayoko na hahahaha

            buffer[index++] = p->binary[i];

            HFCODE * call = search_binary_code(buffer);
            if(call != NULL){
                printf("%c",call->ch);
                //fprintf(write,"%c",call->ch);
                memset(buffer,0,sizeof buffer);
                index = 0;
            }

        }
        p = p->next;
    }
    fclose(write);

}

int main(void){

int opt;
    char fn[40];

    printf("MENU:\n[1]Compress\n[2]Decompress\n[3]Exit\nOpt: ");
    scanf("%d",&opt);

        switch(opt){
            case 1:{
            printf("\nFile Name to Compress:");
            scanf("%s",fn);

            int flag = count_frequency(fn);

            if(flag == 1){
                printf("\nNo File Found...");
            }else if(flag == 2){
                printf("\nFile too small...");
            }else{


                compress(fn);

                BINARY * p = bin;
                printf("binary to write: \n");
                while(p!=NULL){
                    printf("%s-",p->holder);
                    p = p->next;
                }
            }
            getch();}break;
            case 2:{
                printf("\nFile Name to Decompress: ");
                scanf("%s",fn);

                int len = strlen(fn), index = len-3, i = 0, flag = 0;
                char extension[4] = "cmp";

                for(index;index<len;index++){
                    if(fn[index] != extension[i++])
                        flag = 1;
                }

                if(flag == 1){
                    printf("extension name incompatible...");
                }else{
                    decompress(fn);

                }

            getch();}break;
            case 3:exit(0);
            default:break;
        }
}
