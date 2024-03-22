#include <stdio.h>
#include <string.h>
#include <malloc.h>


struct Passenger{   //make a passenger struct with its properties
    int passengerId;
    int travelDate;
    char travelTime[1000];
    char travelFrom[1000];
    char travelTo[1000];
    struct Passenger *next;
};

typedef struct Passenger *LinkedList;
typedef struct Passenger  *Node;

struct Bus{
    int busNumber;    //make a bus struct with its properties
    int Date;
    char departureTime[1000];
    char travelFrom[1000];
    char travelTo[1000];
    int ticketPrice;
    int capacity;
    LinkedList passengerList;
};

int insertAtLast(LinkedList list,Node new,int  capacity);
void printLinkedList(LinkedList L);
void loadTheBusInformationFile(struct Bus busses[]);
void loadPassengerInformationFile(LinkedList allPassengers);
void assignPassengersAndPrintAssign(LinkedList allPassengers, struct Bus busses[], int *bussesSize, LinkedList unmatchedPassengers);
void printSpecifcBus(struct Bus busses[],int size);
void bussesPrint(struct Bus  busses [],int *bussesSize);
        void addPassenger(LinkedList allPassengers);
void deletePassenger (LinkedList linkedList, int idValue);
void deleteBus(struct Bus busses[],int *bussesSize,LinkedList unMatchedPassengers);
void busPrint(struct Bus  bus);
Node cloneNode(Node node);
void makeEmpty(LinkedList linkedList);
int bussesNumber(int currentBusses);


int main() {

    int initially =0;
    int *bussesSize;    //pointer to change the size of the bus when deleting a bus or add
    int temp=bussesNumber(initially);     //function to count the busses loaded from the file
    bussesSize= &temp;
    struct Bus busses[*bussesSize];   //creating the busses array




    LinkedList allPassengers = (LinkedList) malloc(sizeof(struct Passenger));           //creating all passengers linked list
    allPassengers->next = NULL;


    LinkedList unmatchedPassengers = (LinkedList) malloc(sizeof(struct Passenger));     //creating unmatched passengers linked list
    unmatchedPassengers->next = NULL;



    int chosenNumber;           //number to chose from

    while(1){
        printf("please chose one number between 1-9 only:\n"                      // make the menu to choose from it
               "1. Load the bus information file\n"
               "2. Load the passenger information file\n"
               "3. Assign passengers and print assignment information of all\n"
               "busses\n"
               "4. Print a specific bus information along with its passengers\n"
               "information (names and IDs)\n"
               "5. Print unmatched passengers\n"
               "6. Add new passenger\n"
               "7. Delete passenger\n"
               "8. Delete bus number\n"
               "9. Exit\n");

        scanf("%d",&chosenNumber);    //get the number chosen

        if(chosenNumber==1){
            loadTheBusInformationFile(busses);
            printf("        ----------------------------------------------------------  \n");
            printf("             file loaded\n");
            printf("        ----------------------------------------------------------  \n");

            bussesPrint(busses,bussesSize);

        }//calling the function to do for each number chosen

        if(chosenNumber==2){
            loadPassengerInformationFile(allPassengers);
            printf("        ----------------------------------------------------------  \n");
            printf("             file loaded\n");
            printf("        ----------------------------------------------------------  \n");

            printLinkedList(allPassengers);

        }

        if(chosenNumber==3){
            makeEmpty(unmatchedPassengers);
            for (int i = 0; i <*bussesSize ; ++i) {               // because of assign clicked another time dont added the same passenger another time
                makeEmpty(busses[i].passengerList);

            }

            assignPassengersAndPrintAssign(allPassengers,busses,bussesSize,unmatchedPassengers);
        }

        if(chosenNumber==4){
            printSpecifcBus(busses,*bussesSize);
        }

        if(chosenNumber==5){
            printf("        ----------------------------------------------------------  \n");
            printf("         unmatched passenger list:  \n");
            printf("        ----------------------------------------------------------  \n");
            printLinkedList(unmatchedPassengers);
        }

        if(chosenNumber==6){
            addPassenger(allPassengers);
            printf("        ----------------------------------------------------------  \n");

            printf("           passenger is added\n");
            printf("        ----------------------------------------------------------  \n");

        }

        if(chosenNumber==7){
            int idValue;
            printf("please enter the passenger id want to delete\n");       // deleting the passenger using id
            scanf("%d",&idValue);
            deletePassenger(allPassengers,idValue);
            deletePassenger(unmatchedPassengers,idValue);                   // delete the passenger from all linked list matched
            for (int i = 0; i < 4; ++i) {
                deletePassenger(busses[i].passengerList,idValue);
            }


        }

        if(chosenNumber==8){
            deleteBus(busses,bussesSize,unmatchedPassengers);
        }

        if(chosenNumber==9){
            return 0;
        }

    }
}


void loadTheBusInformationFile(struct Bus busses[]){
    FILE *fptr;
    char tempBussesLine[1000];                           //open the file ,reading  each line  and separate it with #
    int currentBus=0;                                    //variable to access evey bus in the busses array
    fptr = fopen("busses.txt", "r");
    if (fptr == NULL) {
        printf("there is no  file with that name");     //checking if file not found
        return ;
    }
    while(fgets(tempBussesLine, 200, fptr)!= NULL){                 //checking if null or empty
        if (strcmp(tempBussesLine, "") == 0 || strcmp(tempBussesLine, "\n") == 0) {
            break;
        }
        busses[currentBus].busNumber= atoi(strtok(tempBussesLine,"#"));
        busses[currentBus].Date= atoi(strtok(NULL,"#"));                       //saving the properties of every line in new bus in the busses array
        strcpy(busses[currentBus].departureTime, strtok(NULL,"#"));
        strcpy(busses[currentBus].travelFrom, strtok(NULL,"#"));
        strcpy(busses[currentBus].travelTo , strtok(NULL,"#"));
        busses[currentBus].ticketPrice= atoi(strtok(NULL,"#"));
        busses[currentBus].capacity= atoi(strtok(NULL,"#"));
        busses[currentBus].passengerList = NULL;
        currentBus++;
    }
}


void loadPassengerInformationFile(LinkedList allPassengers){
    FILE *fptr;
    char tempPassengerLine[1000];                                //open file ,reading each line and separate it with #
    fptr = fopen("passengers.txt", "r");
    if (fptr == NULL) {
        printf("there is no  file with that name");
        return ;
    }

    while(fgets(tempPassengerLine, sizeof(tempPassengerLine), fptr)!= NULL){   //checking if null or empty
        if (strcmp(tempPassengerLine, "") == 0 || strcmp(tempPassengerLine, "\n") == 0) {
            break;
        }
        // remove trailing new line
        tempPassengerLine[strlen(tempPassengerLine) - 1] = '\0';
        Node new = (Node) malloc(sizeof (struct Passenger));
        new->passengerId= atoi(strtok(tempPassengerLine,"#"));          //saving each line properties in a node and insert the node to all passengers linked list
        new->travelDate=atoi(strtok(NULL,"#"));
        strcpy(new->travelTime, strtok(NULL,"#"));
        strcpy(new->travelFrom,strtok(NULL,"#"));
        strcpy(new->travelTo,strtok(NULL,"#"));
        insertAtLast(allPassengers, new, 1000);          //insert the node
    }
}


void assignPassengersAndPrintAssign(LinkedList allPassengers, struct Bus busses[], int *bussesSize, LinkedList unmatchedPassengers){
    if(allPassengers == NULL){
        return ;            //checking if linked is null
    }
    Node passenger=allPassengers->next;
    while (passenger != NULL ){
        Node clonedPassenger = cloneNode(passenger);            // moving in the all passenger linked list getting the passenger node properties in new one to not send the same node
        int isPassengerAdded = 0;                                   // to check if the passenger is added to one of the busses

        for (int i = 0; i < *bussesSize; i++) {
            if (passenger->travelDate == busses[i].Date &&
            strcmp(passenger->travelFrom, busses[i].travelFrom) == 0 &&
            strcmp(passenger->travelTime, busses[i].departureTime) == 0 &&
                    strcmp(passenger->travelTo, busses[i].travelTo) == 0)                        // comparing the passenger travel date , time  , from and to destination  to each bus and insert it to its linked list passengers if its matched
             {
                if(busses[i].passengerList == NULL) {                                   //checking if its null and make one
                    busses[i].passengerList = (LinkedList) malloc(sizeof(struct Passenger));
                    busses[i].passengerList -> next = NULL;
                }
                isPassengerAdded = insertAtLast(busses[i].passengerList, clonedPassenger, busses[i].capacity);
                break;
            }
        }
        if(isPassengerAdded == 0) {                                       // if the passenger did not added to any of the busses will be added to unmatched linked list
            insertAtLast(unmatchedPassengers, clonedPassenger, 1000);
        }
        passenger=passenger->next;
    }
    for (int i = 0; i < *bussesSize ; i++) {   // the busses linked list will be printed wth the bus information
        busPrint(busses[i]);
        printf("   its passengers : \n");
        printLinkedList(busses[i].passengerList);
        printf("----------------------------------------------------------------------------------------------------------\n");
    }
}


int insertAtLast(LinkedList list,Node new,int  capacity){
    int isAdded = 0;            //determine if the node is added
    if(list ==NULL|| new==NULL){  //check if linked is null or the node wanted to add is null
        return 0;
    }
    Node temp;
    temp=list;
    int counter=0;   // to determine the number of nodes in the linked list
    while(temp -> next !=NULL){    //move in the linked list to find the last node and add the new one after it
        counter++;
        temp=temp->next;
    }
    if(counter < capacity) {
        temp->next=new; //add after the last node and  then add null to the new node
        new->next=NULL;
        isAdded = 1;
    }
    return isAdded;
}


void printSpecifcBus(struct Bus busses[],int size){
    int busNumber;
    int busIsFound=0; ///checking if the bus found
        printf("please enter the number of the bus wanted to print its informations and passengers informations  \n");
        scanf("%d",&busNumber);             // choosing the bus wanted from its number
        for (int i = 0; i < size; ++i) {
            if(busses[i].busNumber==busNumber){// if found print its information
                busPrint(busses[i]);
                printf("   ***    its passengers : \n");
                printLinkedList(busses[i].passengerList);

                busIsFound=1;
            }
        }
        if(busIsFound==0){   // if not found print that
            printf("        ----------------------------------------------------------  \n");
            printf("                     bus is not found\n");
            printf("        ----------------------------------------------------------  \n");

        }

}


void printLinkedList(LinkedList L){
    if(L==NULL ){         //checking if the list is null
        printf("         has no passenger \n     ");
        return;
    }
    if(L->next==NULL){                     //checking if the list is empty to print this
        printf("           has no passenger \n     ");
        return;

    }

    Node temp=L->next  ;  // moving in all list passengers

    while(temp != NULL){
        printf("  ### id= %d, date= %d , time =%s , from= %s , to= %s \n",temp->passengerId, temp->travelDate,temp->travelTime,temp->travelFrom,temp->travelTo);
        temp=temp->next;
    }
}


void addPassenger(LinkedList allPassengers){
    Node new=(Node) malloc(sizeof (allPassengers));
    printf("please enter passenger number\n");

    scanf("%d",&new->passengerId);                          // add new passenger with the properties of a passenger

    printf("please enter passenger travel date\n");

    scanf("%d",&new->travelDate);

    printf("please enter passenger travel time\n");

    scanf("%s",new->travelTime);

    printf("please enter where the passenger traveling from\n");

    scanf("%s",new->travelFrom);

    printf("please enter where the passenger traveling to\n");

    scanf("%s",new->travelTo);

    new -> next = NULL;

    insertAtLast(allPassengers,new,100);  //  added the new passenger to all passenger list


}


void deletePassenger (LinkedList linkedList, int idValue){    //deleting passenger from a linked list
        if(linkedList == NULL){         //checking if list is null
            return;
        }
        Node previous ;
        previous=linkedList;

        while(previous->next!=NULL && previous->next->passengerId!=idValue){         //finding the previous node of the node we want to delete
            previous=previous->next;
        }

        if(previous -> next != NULL && previous->next->passengerId == idValue){     // checking if the node found not the last node match the wanted one
            Node wantedToDelete;
            wantedToDelete=previous->next;      // deleting the wanted one by connect the previous  to its next one
            previous->next=wantedToDelete->next;
            free(wantedToDelete);

        } else {
            return ;                // it will be moved all the passengers and not found the wanted one or the list is empty
        }
    }


void deleteBus(struct Bus busses[],int *bussesSize,LinkedList unMatchedPassengers){
    int numWantedToDelete;
    printf("please enter the bus number want to delete\n");         //deleting the bus from its number
    scanf("%d",&numWantedToDelete);
    int pos = -1;
    for (int i = 0; i < *bussesSize; i++) {
       if( busses[i].busNumber==numWantedToDelete){              //finding the index number of the bus wanted to delete
           pos=i;
           break;
       }
    }
    if(pos < 0||pos>*bussesSize-1) {         // bus not found
        printf("        ----------------------------------------------------------  \n");
        printf("         bus  is not found\n");
        printf("        ----------------------------------------------------------  \n");
        return;
    }

    if(busses[pos].passengerList!=NULL && busses[pos].passengerList->next!=NULL){
        Node temp=busses[pos].passengerList->next;
        while(temp!=NULL){
            insertAtLast(unMatchedPassengers,temp,100);   //adding the passengers in that bus to unmatched linked list
            temp=temp->next;
        }
    }
    else{
        printf("        ----------------------------------------------------------  \n");
        printf("          it has no passenger to delete      \n");
        printf("        ----------------------------------------------------------  \n");

    }
    if(pos==*bussesSize-1){
        *bussesSize=*bussesSize-1;   //to delete the last element the array

    }

    else{
        for (int i = pos; i <*bussesSize-1; ++i) {            //shifting the array to delete the bus wanted
            busses[i]=busses[i+1];
        }
        *bussesSize=*bussesSize-1;   //decrease the size of busses array
    }
    printf("        ----------------------------------------------------------  \n");
    printf("           bus is deleted\n");
    printf("        ----------------------------------------------------------  \n");

}


void busPrint(struct Bus  bus){
    printf(" bus : bus number = %d, date= %d , time =%s , from= %s , to= %s ,ticket price= %d  capacity = %d \n",bus.busNumber,bus.Date,bus.departureTime,bus.travelFrom,bus.travelTo,bus.ticketPrice,bus.capacity);
}


void bussesPrint(struct Bus  busses [],int *bussesSize){
    for (int i = 0; i <*bussesSize; ++i) {
        printf(" id= %d, date= %d , time =%s , from= %s , to= %s ,ticket price= %d  capacity = %d \n",busses[i].busNumber,busses[i].Date,busses[i].departureTime,busses[i].travelFrom,busses[i].travelTo,busses[i].ticketPrice,busses[i].capacity);
    }
}


Node cloneNode(Node node){
    Node new=(Node) malloc(sizeof (struct Passenger));
    new->passengerId=node->passengerId;     //make new node to save the properties in it
    new->travelDate=node->travelDate;
    strcpy(new->travelTime,node->travelTime);
    strcpy(new->travelFrom,node->travelFrom);
    strcpy(new->travelTo,node->travelTo);
    new->next=NULL;
    return new;
}


void makeEmpty(LinkedList linkedList) {
    if (linkedList == NULL) {   //check if list null
        return;                   // to make linked list empty of all nodes
    }
    Node current = linkedList->next;
    linkedList->next=NULL;
    while (current != NULL ) {
        free(current);  //delete every node
        current=current->next;
    }

}


int bussesNumber(int currentBus){
    FILE *fptr;
    char tempBussesLine[200];                           //open the file ,reading  each line
    fptr = fopen("busses.txt", "r");
    if (fptr == NULL) {
        printf("there is no  file with that name");//checking if file not found
    }
    while(fgets(tempBussesLine, 200, fptr)!= NULL){                 //checking the line if null or empty
        if (strcmp(tempBussesLine, "") == 0 || strcmp(tempBussesLine, "\n") == 0) {
            break;
        }

       
        currentBus++;
    }


    return currentBus ;

    }
