#include<stdio.h>
#include<string.h>
#include<ctype.h>

#define MAX_LEN 1000

int main(){
    char input[100];//array for our input
    int i;//for looping

    printf("Enter the input: ");
    fgets(input,100,stdin);//read input with spaces

    for(i=0;input[i]!='\0';i++){//this will remove newline at end
        if(input[i]=='\n'){
            input[i]='\0';
            break;//stop loop 
        }
    }

    //we will validate each character
    int valid=1;
    for(int i=0;input[i]!='\0';i++){
        char ch=input[i];

        if((ch>='0' && ch<='9') || ch=='+' || ch=='-' || ch=='*' || ch=='/' || ch==' '){
            //thsi above char is allowed
        } else{
            valid=0;//we found an invalid char
            break;
        }
    }

    if(!valid){
        printf("Error : This is Invalid expression \n");
        return 0;
    }

   
   int numbers[100];//store numbers
   char operators[100];//store operators
   int numCount=0,opCount=0;

    i=0;
    while(input[i]!='\0'){
        if(input[i]==' '){//skip spaces
            i++;
            continue;
        }

        if(isdigit(input[i])){
            int num=0;
            while(isdigit(input[i])){
                num = num * 10 + (input[i]-'0');
                i++;
            }
            numbers[numCount++]=num;
        } else{
            operators[opCount++]=input[i];
            i++;
        }
    }

    //we wil handle * and / first
    for(i=0; i<opCount; i++){
        if(operators[i]=='*' || operators[i]=='/'){
            if(operators[i]=='*')
                numbers[i]=numbers[i]*numbers[i+1];
            else{
                if(numbers[i+1]==0){
                    printf("Error : Divsion by zero \n");
                    return 0;
                }
                numbers[i]=numbers[i]/numbers[i+1];
            }

            //we will shift numbers and operators left 
            int j;
            for(j=i+1;j<numCount-1;j++)
                numbers[j]=numbers[j+1];
            for(j=i;j<opCount-1;j++)
                operators[j]=operators[j+1];
            numCount--;
            opCount--;
            i--;//we will stay at same index to check for next
        }
    }

    //we will handle + and - here
    int result=numbers[0];
    for(i=0; i<opCount; i++){
        if(operators[i]=='+')
          result +=numbers[i+1];
        else if(operators[i] =='-')
        result-=numbers[i+1];
    }


    //print result
    printf("Result is : %d\n",result);

   return 0;
}
