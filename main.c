//**************************************
// In the name of GOD
// Compiler for BASIC Language
// Written by Arman Malekzade
// Student No. : 9313027
// Amirkabir University of Technology
//**************************************

#include<stdio.h>
#include<ctype.h>
#include<string.h>
#define MAX1 500
#define FILENAME_BUFFER 100
#define ID_BUFFER 30
#define NUM_BUFFER 10
void keyw(char *p);
void lexical_analysis();
void syntax_analysis();
void new_line();
void line_number();
void command_let();
void command_print();
void command_next();
void command_return();
void command_end();
void command_for();
void command_ifinput();
void command_if();
void command_input();
void command_gotogosub();
void command_goto();
void command_gosub();
void skip_space();
void identifier();
void number();
void error_detection(char str[]);
char linenumber[10];
char file_name[FILENAME_BUFFER];
char fileinput[MAX1];
int global_counter;
void code_generation();
void semantic_analysis();
int semantic_count_end=0;
int semantic_count_for=0;
int semantic_count_next=0;
int semantic_count_return=0;
char *command_now="";
char *command_prev="";
char *command_dprev="";
void command_iterator(char *s);
char assembly_data[MAX1];
char assembly_code[MAX1];
int  assembly_data_counter=-1;
int  assembly_code_counter=-1;
void write_assembly_data(char s);
void write_data(char *s);
void write_assembly_code(char s);
void write_code(char *s);
char idbuffer[ID_BUFFER];
char numbuffer[NUM_BUFFER];
char for_buffer[ID_BUFFER];
char for_lbuffer[NUM_BUFFER];
char if_buffer[ID_BUFFER];
char if_lbuffer[NUM_BUFFER];
int i=0,id=0,kw=0,num=0,op=0;
char keys[32][10]={"print","let","input","for","to","next","if","then","goto","gosub","return","end"};
void welcome(){
    printf("-----------Compiler for BASIC Language------------\n");
    printf("__________________________________________________\n");
    printf("  |||  ||||| |       | |||||| | |     ||||| ||||||\n");
    printf(" |     |   | ||     || |    | | |     |     |    |\n");
    printf(" |     |   | | |   | | |    | | |     ||||| ||||||\n");
    printf(" |     |   | |  | |  | |||||| | |     |     ||||  \n");
    printf("  |||  ||||| |   |   | |      | ||||| ||||| |  |||\n");
    printf("__________________________________________________\n");
    printf("-------Written By Arman Malekzade (9313027)-------\n");
}
main()
{
    welcome();
    lexical_analysis();
    syntax_analysis();
    semantic_analysis();
    code_generation();
}
/************************************* lexical-analysis begin *************************************/
void lexical_analysis(){
    printf("------------------LEXICAL-ANALYSIS----------------\n");
    printf("__________________________________________________\n");
    char ch,str[25],seps[15]=" \t\n,;(){}[]#\"<>",oper[]="!%^&*-+=~|.<>/?";
    int j;
    char fname[FILENAME_BUFFER];
    FILE *f1;
    printf("---------------- Enter File Path :----------------\n");
    scanf("%s",fname);
    for(int x=0;x<=strlen(fname);x++) file_name[x]=fname[x];
    f1 = fopen(fname,"r");
    if(f1==NULL)
    {
        printf("---------------- File Not Found :-----------------");
        exit(0);
    }
    while((ch=fgetc(f1))!=EOF)
    {
        for(j=0;j<=14;j++)
        {
            if(ch==oper[j])
            {
                printf("%c is an operator\n",ch);
                op++;
                str[i]='\0';
                keyw(str);
            }
        }
        for(j=0;j<=14;j++)
        {
            if(i==-1)
                break;
            if(ch==seps[j])
            {
                if(ch=='"')
                {
                    do
                    {
                        ch=fgetc(f1);
                        if(ch!='"')
                            printf("%c",ch);
                    }while(ch!='"');
                    printf("\b is an argument\n");
                    i=-1;
                    break;
                }
                str[i]='\0';
                keyw(str);
            }
        }
        if(i!=-1)
        {
            str[i]=ch;
            i++;
        }
        else
            i=0;
    }
    printf("Keywords: %d\nIdentifiers: %d\nOperators: %d\nNumbers: %d\n",kw,id,op,num);
    printf("__________________________________________________\n");
}
void keyw(char *p)
{
    int k,flag=0;
    for(k=0;k<=sizeof(keys);k++)
    {
        if(strcmp(keys[k],p)==0)
        {
            //printf("%d",*p);
            if(*p!=0){
                printf("%s is a keyword\n",p);
                kw++;
                flag=1;
                break;
            }else break;
        }
    }
    if(flag==0)
    {
        if(isdigit(p[0]))
        {
            printf("%s is a number\n",p);
            num++;
        }
        else
        {
            //if(p[0]!=13&&p[0]!=10)
            if(p[0]!='\0')
            {
                printf("%s is an identifier\n",p);
                id++;
            }
        }
    }
    i=-1;
}
/************************************* lexical-analysis end *************************************/
/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ syntax-analysis begin ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
void syntax_analysis(){
    printf("------------------SYNTAX-ANALYSIS-----------------\n");
    printf("__________________________________________________\n");
    FILE *f1;
    f1 = fopen(file_name,"r");
    char ch;
    for(int x=0;(ch=fgetc(f1))!=EOF;x++) fileinput[x]=ch;
    global_counter=0;
    new_line();
    printf("__________________________________________________\n");
}
void new_line(){
    line_number();
}
void line_number(){
    // linenumber begin //
    int x=0;
    while(isnumber(fileinput[global_counter])) {
        linenumber[x]=fileinput[global_counter];
        x++;
        global_counter++;
    }
    linenumber[x]='\0';
    // linenumber end   //
    skip_space();
    if(fileinput[global_counter]=='l') command_let();
    else if(fileinput[global_counter]=='p') command_print();
    else if(fileinput[global_counter]=='n') command_next();
    else if(fileinput[global_counter]=='r') command_return();
    else if(fileinput[global_counter]=='e') command_end();
    else if(fileinput[global_counter]=='f') command_for();
    else if(fileinput[global_counter]=='i') command_ifinput();
    else if(fileinput[global_counter]=='g') command_gotogosub();
}
void command_print(){
    command_iterator("print");
    global_counter++;
    if(fileinput[global_counter]!='r') {
        printf("LINE %s",linenumber);error_detection(" - SYNTAX ERROR - PRINT COMMAND MISTYPE\n");
    }
    else{
        global_counter++;
        if(fileinput[global_counter]!='i') {
            printf("LINE %s",linenumber);error_detection(" - SYNTAX ERROR - PRINT COMMAND MISTYPE\n");
        }else{
            global_counter++;
            if(fileinput[global_counter]!='n') {
                printf("LINE %s",linenumber);error_detection(" - SYNTAX ERROR - PRINT COMMAND MISTYPE\n");
            }else{
                global_counter++;
                if(fileinput[global_counter]!='t') {
                    printf("LINE %s",linenumber);error_detection(" - SYNTAX ERROR - PRINT COMMAND MISTYPE\n");
                }else{
                    global_counter++;
                    skip_space();
                    if(fileinput[global_counter]!='"') {
                        printf("LINE %s",linenumber);error_detection(" - SYNTAX ERROR - EXPECTED \" AFTER PRINT COMMAND\n");
                    }else{
                        global_counter++;
                        write_code("PRINT '");
                        while(fileinput[global_counter]!='"') {
                            write_assembly_code(fileinput[global_counter]);
                            if(fileinput[global_counter]!='\n') global_counter++;
                            else {printf("LINE %s",linenumber);error_detection(" - SYNTAX ERROR - EXPECTED \" AT THE END OF PRINT COMMAND\n");}
                        }
                        write_code("'\n");
                        global_counter=global_counter+2;
                        if(strcmp(command_prev,"for")==0) {
                            write_code("JMP Loop");write_code(for_lbuffer);
                            write_code("\nEndLoop");write_code(for_lbuffer);write_code(":\n");
                        }
                        if(strcmp(command_prev,"if")==0) {
                            write_code("EndIF");write_code(if_lbuffer);write_code(":\n");
                        }
                        new_line();
                    }
                }
            }
        }
    }
}
void command_let(){
    command_iterator("let");
    global_counter++;
    if(fileinput[global_counter]!='e') {
        printf("LINE %s",linenumber);error_detection(" - SYNTAX ERROR - LET COMMAND MISTYPE\n");

    }
    else{
        global_counter++;
        if(fileinput[global_counter]!='t') {
            printf("LINE %s",linenumber);error_detection(" - SYNTAX ERROR - LET COMMAND MISTYPE\n");
        }else{
            global_counter++;
            skip_space();
            identifier();
            if(fileinput[global_counter]=='='){
                global_counter++;
                number();
                if(fileinput[global_counter]=='\n') {
                    write_data("MOV DX,");
                    write_data(numbuffer);
                    write_data("\nMOV ");
                    write_data(idbuffer);
                    write_data(",DX\n");
                    if(strcmp(command_prev,"for")==0) {
                        write_code("JMP Loop");write_code(for_lbuffer);
                        write_code("\nEndLoop");write_code(for_lbuffer);write_code(":\n");
                    }
                    if(strcmp(command_prev,"if")==0) {
                        write_code("EndIF");write_code(if_lbuffer);write_code(":\n");
                    }
                    global_counter++;new_line();
                }
            }else{
                printf("LINE %s",linenumber);error_detection(" - SYNTAX ERROR - EXPECTED =\n");
            }
        }
    }
}
void command_next(){
    command_iterator("next");
    global_counter++;
    if(fileinput[global_counter]!='e') {
        printf("LINE %s",linenumber);error_detection(" - SYNTAX ERROR - NEXT COMMAND MISTYPE\n");
    }else{
        global_counter++;
        if(fileinput[global_counter]!='x'){
            printf("LINE %s",linenumber);error_detection(" - SYNTAX ERROR - NEXT COMMAND MISTYPE\n");
        }else{
            global_counter++;
            if(fileinput[global_counter]!='t'){
                printf("LINE %s",linenumber);error_detection(" - SYNTAX ERROR - NEXT COMMAND MISTYPE\n");
            }else{
                semantic_count_next++;
                global_counter++;
                skip_space();
                identifier();
                global_counter++;
                if(strcmp(command_prev,"for")==0) {
                    write_code("JMP Loop");write_code(for_lbuffer);
                    write_code("\nEndLoop");write_code(for_lbuffer);write_code(":\n");
                }
                if(strcmp(command_prev,"if")==0) {
                    write_code("EndIF");write_code(if_lbuffer);write_code(":\n");
                }
                new_line();
            }
        }
    }
}
void command_return(){
    command_iterator("return");
    global_counter++;
    if(fileinput[global_counter]!='e'){
        printf("LINE %s",linenumber);error_detection(" - SYNTAX ERROR - RETURN COMMAND MISTYPE\n");
    }else{
        global_counter++;
        if(fileinput[global_counter]!='t'){
            printf("LINE %s",linenumber);error_detection(" - SYNTAX ERROR - RETURN COMMAND MISTYPE\n");
        }else{
            global_counter++;
            if(fileinput[global_counter]!='u'){
                printf("LINE %s",linenumber);error_detection(" - SYNTAX ERROR - RETURN COMMAND MISTYPE\n");
            }else{
                global_counter++;
                if(fileinput[global_counter]!='r'){
                    printf("LINE %s",linenumber);error_detection(" - SYNTAX ERROR - RETURN COMMAND MISTYPE\n");
                }else{
                    global_counter++;
                    if(fileinput[global_counter]!='n'){
                        printf("LINE %s",linenumber);error_detection(" - SYNTAX ERROR - RETURN COMMAND MISTYPE\n");
                    }else{
                        write_code("RET\n");
                        if(strcmp(command_prev,"for")==0) {
                            write_code("ADD AX,1\n");
                            write_code("JMP Loop");write_code(for_lbuffer);
                            write_code("\nEndLoop");write_code(for_lbuffer);write_code(":\n");
                        }
                        if(strcmp(command_prev,"if")==0) {
                            write_code("EndIF");write_code(if_lbuffer);write_code(":\n");
                        }
                        semantic_count_return++;
                        global_counter=global_counter+2;
                        new_line();
                    }
                }
            }
        }
    }
}
void command_end(){
    command_iterator("end");
    global_counter++;
    if(fileinput[global_counter]!='n'){
        printf("LINE %s",linenumber);error_detection(" - SYNTAX ERROR - END COMMAND MISTYPE\n");
    }else{
        global_counter++;
        if(fileinput[global_counter]!='d'){
            printf("LINE %s",linenumber);error_detection(" - SYNTAX ERROR - END COMMAND MISTYPE\n");
        }else{
            write_code("END\n");
            semantic_count_end++;
            global_counter=global_counter+2;
            new_line();
        }
    }
}
void command_for(){
    command_iterator("for");
    global_counter++;
    if(fileinput[global_counter]!='o'){
        printf("LINE %s",linenumber);error_detection(" - SYNTAX ERROR - FOR COMMAND MISTYPE\n");
    }else{
        global_counter++;
        if(fileinput[global_counter]!='r'){
            printf("LINE %s",linenumber);error_detection(" - SYNTAX ERROR - FOR COMMAND MISTYPE\n");
        }else{
            write_code("MOV ");
            global_counter++;
            skip_space();
            identifier();
            strcpy(for_buffer,idbuffer);
            strcpy(for_lbuffer,linenumber);
            write_code(for_buffer);
            write_assembly_code(',');
            skip_space();
            if(fileinput[global_counter]=='=') {
                global_counter++;
                skip_space();
                if(isalpha(fileinput[global_counter])) {identifier();write_code(idbuffer);write_code("\n");}
                else if(isnumber(fileinput[global_counter])) {number();write_code(numbuffer);write_code("\n");}
                skip_space();
                if(fileinput[global_counter]!='t'){
                    printf("LINE %s",linenumber);error_detection(" - SYNTAX ERROR - EXPECTED to AFTER IDENTIFIER IN FOR COMMAND\n");
                }else{
                    global_counter++;
                    if(fileinput[global_counter]!='o'){
                        printf("LINE %s",linenumber);error_detection(" - SYNTAX ERROR - EXPECTED to AFTER IDENTIFIER IN FOR COMMAND\n");
                    }else{
                        semantic_count_for++;
                        global_counter++;
                        skip_space();
                        if(isalpha(fileinput[global_counter])) {
                            identifier();
                            write_code("Loop");write_code(linenumber);
                            write_code(":\nMOV AX,");write_code(for_buffer);
                            write_code("\nCMP AX,");write_code(idbuffer);
                            write_code("\nJG EndLoop");write_code(linenumber);
                            write_code("\nLoopBody:\nMOV BX,");write_code(for_buffer);write_code("\n");
                            skip_space();
                            global_counter++;
                            skip_space();
                            new_line();
                        }
                        else if(isnumber(fileinput[global_counter])) {
                            number();
                            write_code("Loop");write_code(linenumber);
                            write_code(":\nMOV AX,");write_code(for_buffer);
                            write_code("\nCMP AX,");write_code(numbuffer);
                            write_code("\nJG EndLoop");write_code(linenumber);
                            write_code("\nLoopBody:\nMOV BX,");write_code(for_buffer);write_code("\n");
                            global_counter++;
                            skip_space();
                            new_line();
                        }
                        else{
                            printf("LINE %s",linenumber);error_detection(" - SYNTAX ERROR - EXPECTED IDENTIFIER OR NUMBER AFTER = IN FOR COMMAND\n");
                        }
                    }
                }
                
            }else{
                printf("LINE %s",linenumber);error_detection(" - SYNTAX ERROR - EXPECTED = AFTER IDENTIFIER IN FOR COMMAND\n");
            }
        }
    }
}
void command_ifinput(){
    global_counter++;
    if(fileinput[global_counter]=='f') command_if();
    else if(fileinput[global_counter]=='n') command_input();
    else{
        printf("LINE %s",linenumber);error_detection(" - SYNTAX ERROR - EXPECTED input OR if COMMAND\n");
    }
}
void command_if(){
    command_iterator("if");
    strcpy(if_lbuffer,linenumber);
    write_code("IF");write_code(linenumber);write_code(":\nMOV AX,");
    if(fileinput[global_counter]!='f'){
        printf("LINE %s",linenumber);error_detection(" - SYNTAX ERROR - IF COMMAND MISTYPE\n");
    }else{
        global_counter++;
        skip_space();
        if(isalpha(fileinput[global_counter]))  {
            identifier();
            write_code(idbuffer);write_code("\nCMP AX,");
        }
        else if(isnumber(fileinput[global_counter]))  {
            number();write_code(numbuffer);write_code("\nCMP AX,");
        }
        skip_space();
        if(fileinput[global_counter]=='>'||fileinput[global_counter]=='<'||fileinput[global_counter]=='=') global_counter++;
        else{
            printf("LINE %s",linenumber);error_detection(" - SYNTAX ERROR - EXPECTED COMPARISON OPERATOR AFTER NUMBER OF IDENTIFIER IN IF COMMAND\n");
        }
        skip_space();
        if(isalpha(fileinput[global_counter]))  {
            identifier();
            write_code(idbuffer);
        }
        else if(isnumber(fileinput[global_counter]))  {
            number();
            write_code(numbuffer);
        }
        write_code("\nJG EndIF");write_code(if_lbuffer);write_code("\n");
        skip_space();
        if(fileinput[global_counter]!='t'){
            printf("LINE %s",linenumber);error_detection(" - SYNTAX ERROR - EXPECTED then AFTER PREDICATE IN IF COMMAND\n");
        }else{
            global_counter++;
            if(fileinput[global_counter]!='h'){
                printf("LINE %s",linenumber);error_detection(" - SYNTAX ERROR - EXPECTED then AFTER PREDICATE IN IF COMMAND\n");
            }else{
                global_counter++;
                if(fileinput[global_counter]!='e'){
                    printf("LINE %s",linenumber);error_detection(" - SYNTAX ERROR - EXPECTED then AFTER PREDICATE IN IF COMMAND\n");
                }else{
                    global_counter++;
                    if(fileinput[global_counter]!='n'){
                        printf("LINE %s",linenumber);error_detection(" - SYNTAX ERROR - EXPECTED then AFTER PREDICATE IN IF COMMAND\n");
                    }else{
                        global_counter++;
                        skip_space();
                        if(fileinput[global_counter]=='l') command_let();
                        else if(fileinput[global_counter]=='p') command_print();
                        else if(fileinput[global_counter]=='n') command_next();
                        else if(fileinput[global_counter]=='r') command_return();
                        else if(fileinput[global_counter]=='e') command_end();
                        else if(fileinput[global_counter]=='f') command_for();
                        else if(fileinput[global_counter]=='i') command_ifinput();
                        else if(fileinput[global_counter]=='g') command_gotogosub();
                    }
                }
            }
        }
    }
}
void command_input(){
    command_iterator("input");
    global_counter++;
    if(fileinput[global_counter]!='p'){
        printf("LINE %s",linenumber);error_detection(" - SYNTAX ERROR - INPUT COMMAND MISTYPE\n");
    }else{
        global_counter++;
        if(fileinput[global_counter]!='u'){
            printf("LINE %s",linenumber);error_detection(" - SYNTAX ERROR - INPUT COMMAND MISTYPE\n");
        }else{
            global_counter++;
            if(fileinput[global_counter]!='t'){
                printf("LINE %s",linenumber);error_detection(" - SYNTAX ERROR - INPUT COMMAND MISTYPE\n");
            }else{
                global_counter++;
                skip_space();
                identifier();
                skip_space();
                global_counter++;
                if(strcmp(command_prev,"for")==0) {
                    write_code("ADD AX,1\n");
                    write_code("JMP Loop");write_code(for_lbuffer);
                    write_code("\nEndLoop");write_code(for_lbuffer);write_code(":\n");
                }
                if(strcmp(command_prev,"if")==0) {
                    write_code("EndIF");write_code(if_lbuffer);write_code(":\n");
                }
                new_line();
            }
        }
    }
}
void command_gotogosub(){
    global_counter++;
    if(fileinput[global_counter]!='o'){
        printf("LINE %s",linenumber);error_detection(" - SYNTAX ERROR - GOTO OR GOSUB COMMAND MISTYPE\n");
    }else{
        global_counter++;
        if(fileinput[global_counter]=='t') command_goto();
        else if(fileinput[global_counter]=='s') command_gosub();
        else{
            printf("LINE %s",linenumber);error_detection(" - SYNTAX ERROR - GOTO OR GOSUB COMMAND MISTYPE\n");
        }
    }
    
}
void command_goto(){
    command_iterator("goto");
    if(fileinput[global_counter]!='t'){
        printf("LINE %s",linenumber);error_detection(" - SYNTAX ERROR - GOTO COMMAND MISTYPE\n");
    }else{
        global_counter++;
        if(fileinput[global_counter]!='o'){
            printf("LINE %s",linenumber);error_detection(" - SYNTAX ERROR - GOTO OR GOSUB COMMAND MISTYPE\n");
        }else{
            global_counter++;
            skip_space();
            number();
            skip_space();
            global_counter++;
            if(strcmp(command_prev,"for")==0) {
                write_code("ADD AX,1\n");
                write_code("JMP Loop");write_code(for_lbuffer);
                write_code("\nEndLoop");write_code(for_lbuffer);write_code(":\n");
            }
            if(strcmp(command_prev,"if")==0) {
                write_code("EndIF");write_code(if_lbuffer);write_code(":\n");
            }
            new_line();
        }
    }
}
void command_gosub(){
    command_iterator("gosub");
    global_counter++;
    if(fileinput[global_counter]!='u'){
        printf("LINE %s",linenumber);error_detection(" - SYNTAX ERROR - GOSUB COMMAND MISTYPE\n");
    }else{
        global_counter++;
        if(fileinput[global_counter]!='b'){
            printf("LINE %s",linenumber);error_detection(" - SYNTAX ERROR - GOSUB COMMAND MISTYPE\n");
        }else{
            global_counter++;
            skip_space();
            number();
            skip_space();
            global_counter++;
            if(strcmp(command_prev,"for")==0) {
                write_code("ADD AX,1\n");
                write_code("JMP Loop");write_code(for_lbuffer);
                write_code("\nEndLoop");write_code(for_lbuffer);write_code(":\n");
            }
            if(strcmp(command_prev,"if")==0) {
                write_code("EndIF");write_code(if_lbuffer);write_code(":\n");
            }
            new_line();
        }
    }
}
void identifier(){
    if(isalpha(fileinput[global_counter])){
        int id_counter=0;
        idbuffer[id_counter]=fileinput[global_counter];
        id_counter++;
        global_counter++;
        while(isalnum(fileinput[global_counter])) {idbuffer[id_counter]=fileinput[global_counter];id_counter++;global_counter++;}
        idbuffer[id_counter]='\0';
        //printf("\nid found : %s\n",idbuffer);
    }else {printf("LINE %s",linenumber);error_detection(" - SYNTAX ERROR - EXPECTED IDENTIFIER\n");}
}
void number(){
    if(isnumber(fileinput[global_counter])){
        int num_counter=0;
        while(isnumber(fileinput[global_counter])) {
            numbuffer[num_counter]=fileinput[global_counter];num_counter++;global_counter++;
        }
        numbuffer[num_counter]='\0';
        //printf("\nnumber found : %s\n",numbuffer);
    }else {printf("LINE %s",linenumber);error_detection(" - SYNTAX ERROR - EXPECTED NUMERIC VALUE\n");}
}
void error_detection(char str[]){
    printf("%s",str);
    while(fileinput[global_counter]!='\n') global_counter++;
    global_counter++;
    new_line();
}
void skip_space(){
    while(fileinput[global_counter]==' '&&global_counter<MAX1) global_counter++;//skip space
}
/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ syntax-analysis end ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
/*$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ semantic-analysis begin $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$*/
void semantic_analysis(){
    printf("-----------------SEMANTIC-ANALYSIS----------------\n");
    printf("__________________________________________________\n");
    if(semantic_count_end>1) printf("SEMANTIC ERROR - MORE THAN 1 end COMMANDS WERE FOUND!\n");
    if(semantic_count_end==0) printf("SEMANTIC ERROR - NO end COMMANDS WERE FOUND!\n");
    if(semantic_count_for!=semantic_count_next) printf("SEMANTIC ERROR - THE NO. OF for COMMANDS SHOULD BE EQUAL TO THE NO. OF next COMMANDS!\n");
    printf("__________________________________________________\n");
}
/*$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ semantic-analysis end   $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$*/
/* ----------------------------------- code-generation begin ---------------------------------*/
void code_generation(){
    printf(".data");
    printf("\n%s",assembly_data);
    printf(".code\nMAIN  PROC  NEAR");
    printf("\n%s\n",assembly_code);
    printf("Finish:\nMOV AX,4C00H\nINT 21H\nMAIN ENDP\n");
}
void write_assembly_data(char s){
    assembly_data_counter++;
    assembly_data[assembly_data_counter]=s;
}
void write_assembly_code(char s){
    assembly_code_counter++;
    assembly_code[assembly_code_counter]=s;
}
void write_data(char *s){
    for(int i=0;i<strlen(s);i++) write_assembly_data(s[i]);
}
void write_code(char *s){
    for(int i=0;i<strlen(s);i++) write_assembly_code(s[i]);
}
void command_iterator(char *s){
    command_dprev=command_prev;
    command_prev=command_now;
    command_now=s;
}
/* -----------------------------------  code-generation end  ---------------------------------*/