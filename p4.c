#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define START 50
#define INCREMENT 50

char * nextLine(FILE *fp);
void prpat(char *pat, char *fp);
void first(int nLines, char *fname);
void diff(char *file1,char *file2);
void stat(char *fp);
void last(int num_lines, char *fp);
void trunc(int num_chars, char *fp); 

int main(int argc, char *argv[]){

  FILE *cf; // holds the command file
  char *c; //  holds each line from the command file
  char *firstC = NULL;
  char *secondC = NULL;
  char *tptr; // holds the string seperated using strtok
 
  //check to see if the correct format is used when the program was run
  if(argc != 2)
  {
    printf("Invalid number of arguments; format should be program name 
    followed by the command file name \n");
    fflush(stdout);
    exit(1);
  }
  //check to see if the command file can be opened for reading
  if((cf = fopen(argv[1],"r")) == NULL)
  {
    printf("Cant open command file %s \n", argv[1]);
    fflush(stdout);
    exit(1);
  }

  while((c = nextLine(cf)) != NULL)
  {
    tptr = strtok(c," \t\n");
    
    if(strcmp(tptr, "stat") == 0)
    {
     firstC = strtok(NULL," \n");
     stat(firstC);
    }
    if(strcmp(tptr, "prpat") == 0)
    { 
     firstC = strtok(NULL," \n");
     secondC = strtok(NULL," \n");
     prpat(firstC, secondC);
    }
    if(strcmp(tptr, "first") == 0)
    {
     firstC = strtok(NULL," \n");
     secondC = strtok(NULL," \n");
     first(atoi(firstC), secondC);
    }
    if(strcmp(tptr, "last") == 0)
    {
     firstC = strtok(NULL," \n");
     secondC = strtok(NULL," \n");
     last(atoi(firstC), secondC);
    }
    if(strcmp(tptr, "trunc") == 0)
    {
     firstC = strtok(NULL," \n");
     secondC = strtok(NULL," \n");
     trunc(atoi(firstC), secondC);
    }
    if(strcmp(tptr, "diff") == 0)
    {
     firstC = strtok(NULL," \n");
     secondC = strtok(NULL," \n");
     diff(firstC, secondC);
    }
  }
 
 fclose(cf);
 return 0;
}

/* Dilan Samarasinghe.This function takes in a string thats the pattern and a string thats the file name and prints each line where the pattern occurs in the file*/
void prpat(char *pat,char *fp){
  char *p; // holds the string that is the next line
  FILE *f; // file pointer
  int flag = 0; // when flag == 0 the pattern has not occured, when flag == 1 the pattern has occured 
  //checks to see if you can open file and assigns it to f which is the file pointer
  if((f = fopen(fp, "r"))== NULL){
    printf("Cant open file %s  \n", fp);
    fflush(stdout);
    return;
  }
  // we loop through till the end of the file by checking if nextLine returns null
  while((p = nextLine(f)) != NULL){
    if(strstr(p,pat)){ // we use strstr to check if the pattern is in the current string
      printf( "%s \n", p); // if it is we print the current string
      fflush(stdout);
      flag = 1;
    }
  }
  if(flag == 0){ // we check if the flag has been tripped if not it means the pattern didnt occur anywhere in the file
    printf("The specified pattern does not occur in the file\n");
    fflush(stdout);
  }
  fclose(f);
  return;
}
/* Dilan Samarasinghe.This function takes in a number thats bigger than 0 and a string thats the file name and prints the first n number of lines. */  
void first(int nLines, char *fname){
  FILE *f;
  char *p;
  int count = 0;
  // we check if the number of lines to be printed is bigger than zero
  if(nLines <= 0){ 
    printf("The number of lines to be printed must be bigger than 0");
    fflush(stdout);
    return;
  }
  if((f = fopen(fname,"r")) == NULL){
    printf("Cant open file %s \n", fname);
    fflush(stdout);
    return;
  }
  // we loop through until we print n number of lines, we use count to keep track of which line we're on
  while(count < nLines){
    if((p = nextLine(f)) == NULL){
      fclose(f);
      return;
    }
       printf("%s \n", p);
       fflush(stdout);
       count++;
  }
  fclose(f);
}
/* Dilan Samarasinghe. This function takes in two strings which are the names of the files and then compares each file to see if they are similar */
void diff(char *file1, char *file2){
  FILE *f1, *f2;
  int line =0;
  char *c1, *c2;
  int same = 1; // same = 1 means the file is the same so far, same = 0 means the file is different at this point
  int ended = 0; // ended = 1 means one of the files has ended while the other has not

  // in the following two if statements we check to see if we can open the two files
  if((f1 = fopen(file1,"r"))== NULL){
    printf("Cant open file %s \n", file1);
    fflush(stdout);
    return;
  }
  if((f2= fopen(file2,"r")) == NULL){
    printf("Cant open file %s \n", file2);
    fflush(stdout);
    return;
  }
  // we loop until we hit null on both files 
  while(((c1 = nextLine(f1)) != NULL) && ((c2 = nextLine(f2)) != NULL)){
    if(strcmp(c1,c2) == 0){ // if the current two strings are similar we increment the line
      line++;
    }
    else if(strcmp(c1,c2)!= 0){ // otherwise we set the same to 0 meaning the file is no longer the same
      same = 0;
    }
  }
  // we check if the first file ended and the second file did not, if so we set ended to 1 meaning one of the files ended before the other and increment line
  if(((c1 = nextLine(f1)) == NULL) &&((c2 = nextLine(f2)) != NULL)){
    ended = 1;
    same = 0;
    line++;
  }
  // same as previous if statement but check to see if the second file ended before the first and if so set ended to 1 and increment line.
  if(((c1 = nextLine(f1)) != NULL) &&((c2 = nextLine(f2)) == NULL)){
    ended = 1;
    same = 0;
    line++;
  }
  // if one file ended before the other and the lines before it are not the same we print the line where the file ended
  if(same == 0 && ended == 1){
    printf("The files are different at line %i \n", line);
    fflush(stdout);
  }
  // if one file didnt end before the other and the files arent the same print the line where the first file differed
  else if(same == 0 && ended == 0){
    printf("The files are dfferent at line %i \n", line);
    fflush(stdout);
  }
  // if one file ended before the other but the rest of the files were identical still print the line where one file ended
  else if(same == 1 && ended == 1){
    printf("The files are different at line %i \n", line);
    fflush(stdout);
  }

  // if one file didnt end before the other and the rest of the lines are all the same print the files are identical.
  else{
    printf("The files are identical \n");
    fflush(stdout);
  }
  fclose(f1);
  fclose(f2);
  return;
}

void stat(char *fp)
{
 FILE *finp;
 char *c;
 int num_chars, total_char, num_lines, max_line, min_line, avg_line;
 
 if((finp = fopen(fp, "r")) == NULL)
 {
  printf("Could not open %s \n", fp);
  fflush(stdout);
  return;
 }
 num_chars = total_char = num_lines = max_line = min_line = avg_line = 0;
 while((c = nextLine(finp)) != NULL)
 {
  num_lines++;
  if(num_lines < 2)
  {
   min_line = strlen(c)+1;
  }

  num_chars = strlen(c)+1;
  total_char += num_chars;
  
  if(max_line < num_chars)
  {
   max_line = num_chars;
  }
  if(num_chars < min_line)
  {
   min_line = num_chars;
  }
 }
 avg_line = total_char/num_lines;

 printf("Number of lines: %d Longest Line: %d Shortest Line: %d Avg Length: %d \n", num_lines, max_line, min_line, avg_line);
 fflush(stdout);
 fclose(finp);
 return;
}

void last(int num_lines, char *fp)
{
 FILE *finp;
 char *c;
 char *ptrs[num_lines];
 int num, count = 0;

 if(num_lines <= 0)
 {
  printf("Number of lines can't be negative or zero.");
  fflush(stdout);
  return;
 }
 if((finp = fopen(fp,"r")) == NULL)
 {
  printf("Can't open %s \n", fp);
  fflush(stdout);
  return;
 }

 while((c = nextLine(finp)) != NULL)
 {
  ptrs[count] = c;
  count++;
  if(count > num_lines-1)
  {
   count = 0;
  }
 }
 for(num = 0; num < num_lines; num++)
 {
  printf("%s \n", ptrs[num]);
 }
 fclose(finp);
}

void trunc(int num_chars, char *fp)
{
 FILE *finp;
 char *p;
 int string_len;
 char copy[num_chars]; 
 int i;

 if(num_chars < 2)
 {
  printf("Length needs to be longer then 2. \n");
  fflush(stdout);
  return;
 }
 if((finp = fopen(fp, "r")) == NULL)
 {
  printf("Can't open %s \n", fp);
  fflush(stdout);
  return;
 }
 while((p = nextLine(finp)) != NULL)
 {
  string_len = strlen(p);
  strncpy(copy, p, num_chars);
  copy[num_chars] = '\n';
 } 
  for(i=0;i > num_chars;i++)
  {
   printf("%c", copy[i]); 
  }
 
 fclose(finp);
}

char * nextLine(FILE *fp){
  int c;
  char *p = NULL;
  int size = START;
  int count = 0;
  if((c = getc(fp)) == EOF)
    {
      return NULL;
    }
  if((p = (char *) malloc(size * sizeof(char))) == NULL)
    {
      printf("malloc returned null \n");
      exit(1);
    }
  p[count] = c; 
  count++;
 while(count < size)
  {
   c = getc(fp);
   if(c == '\n')
   {
    p[count] = '\0';
    return p;
   }
   p[count] = c;
   count++;
  }
 while(1)
 {
  size += INCREMENT;
  if(( p = (char *) realloc(p, size * sizeof(char))) == NULL)
  {
   printf("realloc returned null \n");
   exit(1);
  }
  while(count < size)
  {
   c = getc(fp);
   if( c == '\n')
   {
    p[count] = '\0';
    return p;
   }
   p[count] = c;
   count++;
  }
 } 
  
}
