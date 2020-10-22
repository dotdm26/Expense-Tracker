#include <stdio.h>
#include <string.h>
#pragma warning (disable:4996)

#define RECORD "user_report.bin"
#define TOTAL 512
#define DESC_LENGTH 80
#define FULLDATE 11

/*
Created by Minh Do, student of Metropolia University of Applied Sciences
Smart IoT Systems - C Programming
Year 2
*/

typedef struct record {
    float income, expense;
    char description[DESC_LENGTH];
    char date[FULLDATE];
}record;

void add_transaction(int choice, int i, record* transactions);
void append_to_file(int i, record* transactions);

int main() {
    //i is the number of transactions made
    record transactions[TOTAL];
    int user_option, i;
    float money_income = 0, money_expense = 0, money_balance = 0;
    char temp; //to clear newlines for fgets that succeed scanf to work
    FILE* file_ptr;

    printf("======================================\n           Expense Tracker            \n     Created by Minh Do (1911396)     \n======================================\n");

    //Opens file to check, creates file if it doesn't exist. Also checks the number of transactions that are already in the file.
    file_ptr = fopen(RECORD, "ab+");
    i = fread(&transactions, sizeof(record), TOTAL, file_ptr); //checks the number of "record" structures saved in the file
    fclose(file_ptr);

    do {
        printf("1: Add income\n2: Add expense\n3: Print report\n4: Clear records\n5: Exit the program\n");
        printf("Please select an option: ");
        scanf("%d", &user_option);
        printf("\n");
        switch (user_option) {
        case 1: //add money
            add_transaction(user_option, i, transactions);
            append_to_file(i, transactions);
            i++;
            break;

        case 2: //spend money
            add_transaction(user_option, i, transactions);
            append_to_file(i, transactions);
            i++;
            break;

        case 3: //print report
            if (i == 0) {
                printf("====================\nThe record is empty.\n====================\n");
            }
            else {
                printf("There are %d transactions in the record.\n", i);
                file_ptr = fopen(RECORD, "rb+");
                for (int j = 0; j < i; j++) {
                    printf("\n%s", transactions[j].description);
                    if (transactions[j].expense == 0) {
                        money_income = money_income + transactions[j].income;
                        printf("+%.2f", transactions[j].income);
                    }
                    else if (transactions[j].income == 0) {
                        money_expense = money_expense + transactions[j].expense;
                        printf("-%.2f", transactions[j].expense);
                    }
                    printf("\n%s \n", transactions[j].date);
                }
            }
            money_balance = money_income - money_expense;
            printf("\n----------------------------\n");
            printf("%-8s: %.2f\n%-8s: %.2f\n%-8s: %.2f\n", "Income", money_income, "Expense", money_expense, "Balance", money_balance);
            printf("----------------------------\n");
            fclose(file_ptr);
            break;

        case 4: //initialize
            memset(transactions, 0, sizeof(transactions)); //empties the array of structures
            money_balance = 0, money_income = 0, money_expense = 0;
            i = 0;
            fclose(fopen(RECORD, "wb"));
            printf("Record has been cleared.\n");
            break;

        case 5: //exit program
            printf("Exiting program. Thank you for using Expense Tracker.");
            break;
            return 0;

        default:
            printf("Please enter a valid option.\n");
            break;
        }
        printf("\n");
    } while (user_option != 5);
    return 0;
}

//Create a transactionS
void add_transaction(int choice, int i, record* transactions) {
    char temp, day[FULLDATE], month[FULLDATE], year[FULLDATE];
    printf("Enter date (DD/MM/YYYY): ");
    scanf("%s %s %s", day, month, year);

    //Appends day, month and year strings to date string
    strcpy(transactions[i].date, "");
    strcat(transactions[i].date, day);
    strcat(transactions[i].date, "/");
    strcat(transactions[i].date, month);
    strcat(transactions[i].date, "/");
    strcat(transactions[i].date, year);

    printf("Enter description (80 characters max.): ");
    scanf("%c", &temp); //clear newline from scanf for fgets to work
    fgets(transactions[i].description, sizeof(transactions[i].description), stdin);

    printf("Enter the amount of money (EUR): ");
    //if the user selected "Add Money" (1) or "Spend Money" (2)
    if (choice == 1) {
        scanf("%f", &transactions[i].income);
        transactions[i].expense = 0;
        printf("\nThe following information has been added:\n%s+%.2f\n%s\n", transactions[i].description, transactions[i].income, transactions[i].date);
    }
    else if (choice == 2) {
        scanf("%f", &transactions[i].expense);
        transactions[i].income = 0;
        printf("\nThe following information has been added:\n%s-%.2f\n%s\n", transactions[i].description, transactions[i].expense, transactions[i].date);
    }
}

//Adds all the transactions, AKA array of "record" structures into the record file. 
void append_to_file(int i, record* transactions) {
    FILE* file_ptr;
    file_ptr = fopen(RECORD, "ab+");
    fwrite(&transactions[i], sizeof(record), 1, file_ptr);
    fclose(file_ptr);
}
