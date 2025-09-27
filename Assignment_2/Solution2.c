#include<stdio.h>
#include<stdlib.h>

#define FILENAME "users.txt"//used to store records

struct User{
    int id;
    char name[50];
    int age;
};

void createFileIfNotExists(){//to make sure if file exists
    FILE *fp;

    fp=fopen(FILENAME,"a");//a will open file and append
    if(fp==NULL){
        printf("Error creating the file . \n");
        exit(1);//we will exit if file cant be created
    }
    fclose(fp);//close the file
}

//to add new user
void addUser(){
    FILE *fp;
    struct User u;

    fp=fopen(FILENAME,"a");//this will add at the end
    if(fp==NULL){
        printf("Error opening the file.\n");
        return;
    }

    //we will take user input
    printf("Enter ID: ");
    scanf("%d",&u.id);

    printf("Enter Name: ");
    scanf(" %[^\n]",u.name);//this will allow spaces in name

    printf("Enter Age: ");
    scanf("%d",&u.age);

    fprintf(fp,"%d|%s|%d\n",u.id,u.name,u.age);

    fclose(fp);//close file

    printf("User is added successfully!!\n");
}

//To display the users
void displayUsers(){
    FILE *fp;
    struct User u;
    char line[200];

    fp=fopen(FILENAME,"r");
    if(fp==NULL){
        printf("Error opening the file. \n");
        return ;
    }
    printf("\n List of Users \n");

    //to read each line from file
    while(fgets(line, sizeof(line), fp)){
        //it will split line into parts
        sscanf(line,"%d|%[^|]|%d", &u.id,u.name,&u.age);

        printf("ID: %d, Name: %s, Age: %d\n", u.id,u.name,u.age);
    }
    fclose(fp);
}

//to update the user by id
void updateUser(){
    FILE *fp, *temp;
    struct User u;
    char line[200];
    int id,recordFound=0;//id=ID we want to update, found=to check if record exists

    printf("Enter  id of user to update: ");
    scanf("%d",&id);//take id from user

    fp=fopen(FILENAME,"r");//this will open original file for reading
    if(fp==NULL){
        printf("Error opening the file.\n");
        return;
    }

    temp=fopen("temp.txt","w");//this will open a temporary file forwriting our updated content
    if(temp==NULL){
        printf("Error creating temporary file.\n");
        fclose(fp);
        return;
    }

    //this will read each line from our original file
    while (fgets(line,sizeof(line),fp)){
       if( sscanf(line,"%d|%[^|]|%d",&u.id,u.name,&u.age)==3){

            if(u.id==id){//if this record matches our ID
                recordFound=1;//means that we found the record
                printf("Enter new Name: ");
                scanf(" %[^\n]",u.name);//it will take new name
                printf("Enter new Age: ");
                scanf("%d",&u.age);//it will take new age
            }
            fprintf(temp,"%d|%s|%d\n",u.id,u.name,u.age);

       }else{
            fputs(line,temp);//to prevent data loss
       }
    }

    fclose(fp);//closes original file
    fclose(temp);//closes temp file

    //it will replace old file with updated temp file
    remove(FILENAME);
    rename("temp.txt",FILENAME);

    if(recordFound){
        printf("User updated successfully. \n");
    } else{
        printf("User with ID %d not found. \n",id);
    }
}

//to delete the user by id
void deleteUser(){
    FILE *fp, *temp;
    struct User u;
    char line[200];
    int id,recordFound=0;//id=ID that we want to delete, found=to check if record exists

    printf("Enter id of the user to delete: ");
    scanf("%d",&id);//take the ID from user

    fp=fopen(FILENAME,"r");//this will open original file for reading
    if(fp==NULL){
        printf("Error opening the file.\n");
        return;
    }

    temp=fopen("temp.txt","w");//the temporary file for writing the updated content
    if(temp==NULL){
        printf("Error creating temporary file.\n");
        fclose(fp);
        return;
    }

    // read each line from original file
    while(fgets(line, sizeof(line), fp)){
        if(sscanf(line, "%d|%[^|]|%d", &u.id, u.name, &u.age) == 3){
            if(u.id == id){ // if this record matches the ID we will skip it
                recordFound = 1;  //we found it
                continue;   // 
            }
            fprintf(temp, "%d|%s|%d\n", u.id, u.name, u.age); // write other records
        } else {
            fputs(line, temp); // it will preserve
        }
    }

    fclose(fp);   // close original file
    fclose(temp); // close temp file

    // replace old file with updated temp file
    remove(FILENAME);
    rename("temp.txt", FILENAME);

    if(recordFound){
        printf("User deleted successfully. \n");
    } else{
        printf("User with ID %d not found. \n", id);
    }

}

int main(){
    //we will create file if it does not exist
    createFileIfNotExists();

    int choice;

    do { // start of loop to repeatedly show menu
        printf("\nSelect an option: \n");
        printf("1. Add User\n");
        printf("2. Display Users\n");
        printf("3. Update User\n");
        printf("4. Delete User\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d",&choice);

        if(choice==1){
            addUser(); //it will call add function
        } else if(choice==2){
            displayUsers(); //it will call display function
        } else if(choice==3){
            updateUser(); //it will call update function
        } else if(choice==4){
            deleteUser(); //it will call delete function
        } else if(choice==5){
            printf("Exited \n"); // exit 
        } else {
            printf("Invalid choice! Please try again.\n"); 
        }

    } while(choice != 5); // continue until user selects Exit

    return 0;
}
