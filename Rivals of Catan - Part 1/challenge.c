#include "challenge.h"


int main(){
    FILE* input;
    char buffer[MAX_SIZE_BUFFER];

    input = fopen("input.txt", "w+");
    if(!input){
        perror("Error opening a file");
        exit(1);
    }
    int sources[NUM_RES]= {0};
    char str[MAX_LENGTH]= {0};           //malloc(sizeof(char)*(MAX_LENGTH));
    if(!str){
        perror("Unable to allocate buffer");
        exit(1);
    }
    if(fgets(buffer, MAX_SIZE_BUFFER, stdin) != NULL){
        if(fputs(buffer , input) == EOF){
            perror("Error writing to file");
            exit(1);
        }
    } else{
        printf("Error reading from stdin\n");
        return 1;
    }

    rewind(input);
    fseek(input, BYTE, SEEK_SET);  //moving the pointer after "{"
    char ch = fgetc(input);
    int i=0;
    while(ch != '\n' && ch != EOF){
        while(ch != ',' && ch != '}'){
            str[i++]=ch;
            ch = fgetc(input);
        }
        str[i]='\0';
        if(strcmp(str , "road")==0){
            sources[BRICK]++;
            sources[WOOD]++;
        }
        if(strcmp(str , "settelment")==0){
            sources[BRICK]++;
            sources[WOOD]++;
            sources[WOOL]++;
            sources[GRAIN]++;
        }
    
        if(strcmp(str , "city")==0){
            sources[GRAIN]++;
            sources[GRAIN]++;
            sources[ORE]++;
            sources[ORE]++;
            sources[ORE]++;
        }
        if(strcmp(str , "development")==0){
            sources[WOOL]++;
            sources[GRAIN]++;
            sources[ORE]++;
        }
        i=0;
        ch = fgetc(input);
    }


    //print the sum of sources needed for the request
    int j=0;
    if(sources[j] < 10){
        printf("0%d\n", sources[j++]);
        printf("BRICK\n");
    } else{
        printf("%d\n", sources[j++]);
        printf("BRICK\n");
    }
    if(sources[j] < 10){
        printf("0%d\n", sources[j++]);
        printf("WOOD\n");
    } else{
        printf("%d\n", sources[j++]);
        printf("WOOD\n");
    }
    if(sources[j] < 10){
        printf("0%d\n", sources[j++]);
        printf("WOOL\n");
    } else{
        printf("%d\n", sources[j++]);
        printf("WOOL\n");
    }
    if(sources[j] < 10){
        printf("0%d\n", sources[j++]);
        printf("GRAIN\n");
    } else{
        printf("%d\n", sources[j++]);
        printf("GRAIN\n");
    }
    if(sources[j] < 10){
        printf("0%d\n", sources[j++]);
        printf("ORE\n");
    } else{
        printf("%d\n", sources[j++]);
        printf("ORE\n");
    }

    fclose(input);
    //free(str);
    return(0);
    
}
