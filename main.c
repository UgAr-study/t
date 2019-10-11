#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define TRUE 1
#define FALSE 0

char* Read(char*** index, char** text, int* index_size){

    FILE * Onegin = fopen ("Onegin.txt", "rb");
    //FILE * Onegin_Sort = fopen ("Onegin_Sort.txt", "w");

    long long size_of_file = 0;

    if (Onegin == NULL) printf("Error of file\n");

    else{
        fseek (Onegin, 0, SEEK_END);
        size_of_file = ftell (Onegin);
        fseek (Onegin, 0, SEEK_SET);
    }

    *text = (char*) realloc (*text, (size_of_file + 1) * sizeof(char));
    assert (text != NULL);

    size_t length = fread (*text, sizeof(char), size_of_file, Onegin);
    assert (size_of_file == length);

    printf("size of file = %lld\n", size_of_file);

    *index = (char**) calloc (size_of_file, sizeof(char*));
    assert (index != NULL);

    *index[0] = *text;

    //printf ("%s\n", *text);
    //printf("text[seek] = %p\t char is %c\n", *index[0], **index[0]);

    //fwrite(text, sizeof(char), size_of_file, Onegin_Sort);


    int number_of_strings = 1;

    for(long i = 0; i < size_of_file; i++){

        //printf("I came to 'for' in Read! %c\n", (*text)[i]);

        if((*text)[i] == '\n'){
            //printf("I came to 'if' in 'for' in Read!\n");

            (*text)[i] = '\0';
            (*index)[number_of_strings] = *text + i + 1;

            //char c = *((*index)[number_of_strings]);
            //printf("text[seek] = %p\t char is %c\n", (*index)[number_of_strings], c);

            number_of_strings ++;
        }

        //printf("I came out to 'for' in Read!\n\n");
    }

    *index_size = number_of_strings;

    *index = (char**) realloc (*index, number_of_strings * sizeof(char*));

    //printf("number of strings = %d\n", number_of_strings);

    //printf("index[1] after realloc = %p\n", (*index)[1]);

    fclose(Onegin);

    return *text;
}

int Is_Letter(char c){
    if((c >= 65 && c <= 90) || (c >= 97 && c <= 122)) return TRUE;
    else {
        //printf("%d is not a char\n", c);
        return FALSE;
    }
}

int Compare (int j, int k, char*** index){      //TRUE if j-string > k-string

    //printf("the beginning of 2 string in 'Compare' is %p\n", (*index)[1]);        //!!!!!!!!!!

    int i = 0;      //i for j-string
    for (int r = 0; ; r++){      //r for k - string

        int I = 666, R = 666;

        char c = *((*index)[j] + i);

        if ((I = Is_Letter (c)) == FALSE){
            while ((I = Is_Letter (c)) == FALSE){

                if ( c == '\r' || c == '\0') return FALSE;
                i++;
                c = *((*index)[j] + i);
            }
        }

        c = *((*index)[k] + r);

        if ((R = Is_Letter (c)) == FALSE){
           while ((R = Is_Letter (c)) == FALSE){
                //printf("R\n");
                if (c == '\r' || c == '\0') return TRUE;
                r++;
                c = *((*index)[k] + r);
            }
        }

        if (*((*index)[j] + i) > *((*index)[k] + r)) return TRUE;
        if (*((*index)[j] + i) < *((*index)[k] + r)) return FALSE;
        i++;

    }
}

void Qsort(char*** index, int a_begin, int a_end){
    int arr_size = a_end - a_begin + 1;
    printf("I came to qsort index[arr_size - 1] = %d\t leftboarder = %d\n", (*index)[arr_size - 1], a_begin);

    int left = a_begin;
    int right = a_end;

    printf("index[left] = %d\t index[right] = %d\n", (*index)[left], (*index)[right]);

    if(arr_size == 1) return;

    if (arr_size == 2){
        if (Compare(left, right, index) == TRUE){
            char* t = (*index)[right];
            (*index)[right] = (*index)[left];
            (*index)[left] = t;
        }

        return;
    }

    if (arr_size == 3){

        if (Compare(left, left + 1, index) == TRUE){
            char* t = (*index)[left + 1];
            (*index)[left + 1] = (*index)[left];
            (*index)[left] = t;
        }

        if (Compare(left + 1, left + 2, index) == TRUE){
            char* t = (*index)[left + 2];
            (*index)[left + 2] = (*index)[left + 1];
            (*index)[left + 1] = t;
        }

        if (Compare(left, left + 1, index) == TRUE){
            char* t = (*index)[left + 1];
            (*index)[left + 1] = (*index)[left];
            (*index)[left] = t;
        }

        return;
    }

    int middle = a_begin + arr_size / 2;
    printf("middle = %d\n", middle);

    while (right > left) {
        printf("In high while\n");

        while (Compare(left, middle, index) == FALSE){
            printf("In low while for left\t index[left] = %d\n", (*index)[left]);
            left++;
        }

        while (Compare(right, middle, index) == TRUE){
            printf("In low while for right\t index[right] = %d\n", (*index)[right]);
            right--;
        }

        char* t = (*index)[right];
        (*index)[right] = (*index)[left];
        (*index)[left] = t;
        printf("after swap: index[%d] = %d index[%d] = %d\n", left, (*index)[left], right, (*index)[right]);
        left++;
        right--;
    }

    //printf("before the recursion\n");
    Qsort (index, a_begin, right);
    Qsort (index, left, a_end);

    return;
}

void Writing(char** index, int index_size){
    for(int i = 0; i < index_size; i++)
        puts(index[i]);
}

int main()
{

    //FILE * Onegin_Sort = fopen ("Onegin_Sort.txt", "w");

    char* text = NULL;

    char** index = NULL;

    int index_size = 0;

    Read(&index, &text, &index_size);

//    int y = Compare(1, &index);
//    printf("Let's compare 1 and 2 strings %d\n", y);
    Qsort(&index, 0, index_size - 1);

    Writing(index, index_size);
    //fclose(Onegin_Sort);
    free(text);
    free(index);
    return 0;
}


