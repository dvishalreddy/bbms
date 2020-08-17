#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
struct date
{
  int date;
  int month;
  int year;
}cdate;
struct donor
{
  int id;
  char name[20];
  int age;
  char gender;
  char bloodgroup[4];
  char phoneNO[15];
  struct date lastDonated;
};
int z=0;
int flag=0;
void add();
void displayAll();
void search();
void update();
void pat_req(struct date);
void delete_dnr();
int login();
int bloodGroupVerify(char[4]);
int dateVerify(int, int, int);
int validpno(char[15]);
int countLeapYears(struct date);
int date_diff(struct date,struct date);
char *xgets(char *dest, int size, FILE *fp)
{
  /* read a line from the user */
  if (!fgets(dest, size, fp))
  {
    /* read failed, end of file detected */
    printf("Premature end of file\n");
    exit(1);
  }
  /* strip the \n if present */
  dest[strcspn(dest, "\n")] = '\0';
  return dest;
}

int main()
{ 
  if(flag==0){
    time_t t = time(NULL);
    struct tm *tm;
    tm = localtime(&t);
    printf("Today's date: %d-%d-%d\n",tm->tm_mday,tm->tm_mon+1,tm->tm_year+1900);
    // struct date cdate;
    cdate.date=tm->tm_mday;
    cdate.month=tm->tm_mon+1;
    cdate.year=tm->tm_year+1900;
    while(1){
      int result;
      result=login();
      if(result==1){
        break;
      }
      else{
        printf("Invalid Credentials, Please try again\n");
        continue;
      }
    }
    flag=1;
  }
  while (1)
  {
    printf("===============================================================\n");
    printf("\t\tBlood Bank Management System\n");
    printf("===============================================================\n");
    printf("1)Add a new donor\n");
    printf("2)Search for a donor\n");
    printf("3)Update existing donor\n");
    printf("4)Display all donors\n");
    printf("5)Patient request\n");
    printf("6)Delete details\n");
    printf("7)To exit the program\n");
    int menu;
    scanf("%d", &menu);
    if (menu == 1)
    {
      add();
    }
    else if (menu == 2)
    {
      search();
    }
    else if (menu == 3)
    {
      update();
    }
    else if (menu == 4)
    {
      displayAll();
    }
    else if (menu == 5)
    {
      pat_req(cdate);
    }
    else if (menu == 6)
    {
      delete_dnr();
    }
    else if (menu == 7)
    {
      exit(1);
    }    
    else
    {
      main();
    }
  }
}

void add()
{
  FILE *fp;
  int s;
  //open file for writing
  fp = fopen("person.dat", "ab+");
  if (fp == NULL)
  {
    fprintf(stderr, "\nError opend file\n");
    exit(1);
  }
  s = 1;
  struct donor input;
  while (fread(&input, sizeof(input), 1, fp))
  {
    s = 1 + input.id;
  }
  char contagiuos[2];
  printf("Are you suffering from any contagious diseases?(Y/N)\n");
  scanf("%s",contagiuos);
  if(strcmp(contagiuos,"Y")==0 || strcmp(contagiuos,"y")==0){
    printf("Sorry, you are not eligible to donate blood\n");
    exit(1);
  }
  input.id = s;
  printf("Your ID is %d", input.id);
  fflush(stdin);
  printf("\nEnter Name:");
  scanf("%s",input.name);
  while (1)
  {
    printf("\nEnter age:");
    scanf("%d", &input.age);
    if (input.age < 18 || input.age > 65)
    {
      printf("\nYou are not eligibile to donate blood");
      printf("\n1)Main Menu\n2)Re enter age\n3)Exit the program\n");
      int invalid_age;
      scanf("%d", &invalid_age);
      switch (invalid_age)
      {
      case 1:
        main();
      case 2:
        continue;
      case 3:
        exit(1);
      default:
        exit(1);
      }
    }
    else
    {
      break;
    }
  }
  getchar();
  while (1)
  {
    printf("\nEnter Gender(M/F/O):");
    scanf("%c", &input.gender);
    getchar();
    if (input.gender == 'M' || input.gender == 'F' || input.gender == 'O')
    {
      break;
    }
    else
    {
      printf("\nEnter a valid gender(M/F/O)");
    }
  }
  while (1)
  {
    printf("\nEnter Bloodgroup:");
    xgets(input.bloodgroup, 4, stdin);
    int x;
    x = bloodGroupVerify(input.bloodgroup);
    if (x == 1)
    {
      break;
    }
    else
    {
      continue;
    }
  }
  while (1)
  {
    if(strcmp(input.bloodgroup,"AB+")==0||strcmp(input.bloodgroup,"AB-")==0){
      getchar();
    }
    printf("\nEnter PhoneNo:");
    fflush(stdin);
    xgets(input.phoneNO, 15, stdin);
    int x;
    x = validpno(input.phoneNO);
    if (x == 1)
    {
      break;
    }
    else
    {
      continue;
    }
  }
  while (1)
  {
    printf("\nEnter the last donanted date(DD MM YYYY):");
    scanf("%2d%2d%4d", &input.lastDonated.date, &input.lastDonated.month, &input.lastDonated.year);
    if (dateVerify(input.lastDonated.date, input.lastDonated.month, input.lastDonated.year))
    {
      break;
    }
    else
    {
      printf("\nInvalid date,please enter again");
      continue;
    }
  }
  fwrite(&input, sizeof(struct donor), 1, fp);
  fclose(fp);
  main();
}

void displayAll()
{
  FILE *fp;
  struct donor input;
  fp = fopen("person.dat", "rb");
  if (fp == NULL)
  {
    fprintf(stderr, "\nError opening file\n");
    exit(1);
  }

  // read file contents till end of file
  printf("\n");
  printf("===============================================================");
  printf("\n\t\t\tAll Donar Details\n");
  printf("===============================================================\n");
  printf("ID\tName\tAge\tGender\tBlood Group\tPhone Number\tDate\n");
  while (fread(&input, sizeof(input), 1, fp))
    printf("%d\t%s\t%d\t%c\t%s\t\t%s\t%d-%d-%d\n", input.id,
           input.name, input.age, input.gender, input.bloodgroup, input.phoneNO, input.lastDonated.date, input.lastDonated.month, input.lastDonated.year);

  // close file
  fclose(fp);
}

void search()
{
  FILE *fp;
  fp = fopen("person.dat", "rb");
  struct donor input;
  int verify;
  printf("\n1)Search using ID\n2)Search by Bloodgroup\n3)Main menu\n");
  scanf("%d", &verify);
  getchar();
  switch (verify)
  {
  case 1:
  {
    int id_verify;
    printf("\nEnter ID");
    scanf("%d", &id_verify);
    while (fread(&input, sizeof(input), 1, fp))
    {
      if (input.id == id_verify)
      {
        printf("===============================================================");
        printf("\n\t\t\tDonar Details\n");
        printf("===============================================================\n");
        printf("ID\tName\tAge\tGender\tBlood Group\tPhone Number\tDate\n");
        printf("%d\t%s\t%d\t%c\t%s\t\t%s\t%d-%d-%d\n", input.id,
               input.name, input.age, input.gender, input.bloodgroup, input.phoneNO, input.lastDonated.date, input.lastDonated.month, input.lastDonated.year);
        main();
      }
    }
    printf("\nNo such Record exits");
    printf("\n1)Main menu\n2)Search another record");
    int fail_search;
    scanf("%d", &fail_search);
    if (fail_search == 1)
    {
      main();
    }
    else
    {
      search();
    }
  }
  case 2:
  {
    char bg_verify[4], bg_verify1[3];
    printf("\nEnter Bloodgroup:");
    xgets(bg_verify, sizeof(bg_verify), stdin);
    strcpy(bg_verify1, bg_verify);
    int temp = 0;
    printf("===============================================================");
    printf("\n\t\t\tDonar Details\n");
    printf("===============================================================\n");
    while (fread(&input, sizeof(input), 1, fp))
    {
      if (strcmp(input.bloodgroup, bg_verify) == 0 ||
          strcmp(input.bloodgroup, bg_verify1) == 0)
      {
        if (temp == 0)
        {
          printf("ID\tName\tAge\tGender\tBlood Group\tPhone Number\tDate\n");
        }
        printf("%d\t%s\t%d\t%c\t%s\t\t%s\t%d-%d-%d\n", input.id,
               input.name, input.age, input.gender, input.bloodgroup, input.phoneNO, input.lastDonated.date, input.lastDonated.month, input.lastDonated.year);
        temp = 1;
      }
    }
    if (temp == 0)
    {
      printf("\nNo such Record exits");
      printf("\n1)Main Menu\n2)Search for another record");
      int fail_search;
      scanf("%d", &fail_search);
      if (fail_search == 1)
      {
        main();
      }
      else
      {
        search();
      }
    }
  }
  }
}

void update()
{
  FILE *fp;
  int cnt = 0;
  fp = fopen("person.dat", "rb+");
  struct donor input;
  int verify;
  printf("\nEnter ID to update:");
  scanf("%d", &verify);
  getchar();
  while (fread(&input, sizeof(input), 1, fp))
  {
    cnt = cnt + 1;
    if (verify == input.id)
    {
      int menu_input;
      printf("1)Phone Number\n2)Blood Group\n3)Lastdonation\n4)Age\n5)Name\n");
      scanf("%d", &menu_input);
      switch (menu_input)
      {
      case 1:
        while (1)
        {
          printf("\nEnter PhoneNo:");
          fflush(stdin);
          xgets(input.phoneNO, 15, stdin);
          int x;
          x = validpno(input.phoneNO);
          if (x == 1)
          {
            break;
          }
          else
          {
            continue;
          }
        }
        rewind(fp);
        fseek(fp, (cnt - 1) * sizeof(input), SEEK_SET);
        fwrite(&input, sizeof(input), 1, fp);
        break;
      
      case 2:
        while (1)
        {
          printf("\nEnter Bloodgroup:");
          fflush(stdin);
          xgets(input.bloodgroup, 4, stdin);
          int x;
          x = bloodGroupVerify(input.bloodgroup);
          if (x == 1)
          {
            break;
          }
          else
          {
            continue;
          }
        }
        rewind(fp);
        fseek(fp, (cnt - 1) * sizeof(input), SEEK_SET);
        fwrite(&input, sizeof(input), 1, fp);
        break;
      
      case 3:
        while (1)
        {
          printf("\nEnter the last donanted date(dd mm yyyy):");
          fflush(stdin);
          scanf("%2d%2d%4d", &input.lastDonated.date, &input.lastDonated.month, &input.lastDonated.year);
          if (dateVerify(input.lastDonated.date, input.lastDonated.month, input.lastDonated.year))
          {
            break;
          }
          else
          {
            printf("\nInvalid date,please enter again");
            continue;
          }
        }
        rewind(fp);
        fseek(fp, (cnt - 1) * sizeof(input), SEEK_SET);
        fwrite(&input, sizeof(input), 1, fp);
        break;
      
      case 4:
        while (1)
        {
          printf("\nEnter Gender(M/F/O):");
          fflush(stdin);
          scanf("%c", &input.gender);
          getchar();
          if (input.gender == 'M' || input.gender == 'F' || input.gender == 'O')
          {
            break;
          }
          else
          {
            printf("\nEnter a valid gender(M/F/O)");
          }
        }
        rewind(fp);
        fseek(fp, (cnt - 1) * sizeof(input), SEEK_SET);
        fwrite(&input, sizeof(input), 1, fp);
        break;

      case 5:
        printf("\nEnter Name:");
        fflush(stdin);
        xgets(input.name, 20, stdin);
        rewind(fp);
        fseek(fp, (cnt - 1) * sizeof(input), SEEK_SET);
        fwrite(&input, sizeof(input), 1, fp);
        break;

      default:
        break;
      }
      
    }
    else
    {
      continue;
    }
    fclose(fp);
  }
}

void pat_req(struct date da)
{
  FILE *fp;
  struct donor input;
  char bgrp[4];

  while (1)
  {
    printf("\nEnter Bloodgroup:");
    fflush(stdin);
    xgets(bgrp, 4, stdin);
    int x;
    x = bloodGroupVerify(bgrp);
    if (x == 1)
    {
      break;
    }
    else
    {
      continue;
    }
  }
  
  fp = fopen("person.dat", "rb");

  if (fp == NULL)
  {
    printf("\n cannot open the record file");
    exit(1);
  }
  printf("ID\tName\tAge\tGender\tBlood Group\tPhone Number\tDate\n");

  if ((strcmp(bgrp, "O-") == 0) || (strcmp(bgrp, "O+") == 0))
  {
    while (fread(&input, sizeof(input), 1, fp))
    {
      if (((strcmp(input.bloodgroup, "O-") == 0)) && (date_diff(da, input.lastDonated) > 90) && (input.age > 18) && (input.age < 65))
      {
        printf("%d\t%s\t%d\t%c\t%s\t\t%s\t%d-%d-%d\n", input.id,
               input.name, input.age, input.gender, input.bloodgroup, input.phoneNO, input.lastDonated.date, input.lastDonated.month, input.lastDonated.year);
      }
    }
    rewind(fp);
    if ((strcmp(bgrp, "O+")) == 0)
    {
      while (fread(&input, sizeof(input), 1, fp))
      {
        if ((strcmp(input.bloodgroup, "O+") == 0) && (date_diff(da, input.lastDonated) > 90) && (input.age > 18) && (input.age < 65))
        {
          printf("%d\t%s\t%d\t%c\t%s\t\t%s\t%d-%d-%d\n", input.id,
                 input.name, input.age, input.gender, input.bloodgroup, input.phoneNO, input.lastDonated.date, input.lastDonated.month, input.lastDonated.year);
        }
      }
    }
    rewind(fp);
  }

  else if ((strcmp(bgrp, "A+") == 0) || (strcmp(bgrp, "A-") == 0))
  {
    while (fread(&input, sizeof(input), 1, fp))
    {
      if (((strcmp(input.bloodgroup, "A-") == 0) || (strcmp(input.bloodgroup, "O-") == 0)) && (date_diff(da, input.lastDonated) > 90) && (input.age > 18) && (input.age < 65))
      {
       printf("%d\t%s\t%d\t%c\t%s\t%s\t%d-%d-%d\n", input.id,
                 input.name, input.age, input.gender, input.bloodgroup, input.phoneNO, input.lastDonated.date, input.lastDonated.month, input.lastDonated.year);
      }
    }
    rewind(fp);

    if (((strcmp(bgrp, "A+") == 0)))
    {
      while (fread(&input, sizeof(input), 1, fp))
      {
        if (((strcmp(input.bloodgroup, "A+") == 0) || (strcmp(input.bloodgroup, "O+") == 0)) && (date_diff(da, input.lastDonated) > 90) && (input.age > 18) && (input.age < 65))
        {
          printf("%d\t%s\t%d\t%c\t%s\t\t%s\t%d-%d-%d\n", input.id,
                 input.name, input.age, input.gender, input.bloodgroup, input.phoneNO, input.lastDonated.date, input.lastDonated.month, input.lastDonated.year);
        }
      }
      rewind(fp);
    }
  }

  else if ((strcmp(bgrp, "AB+") == 0) || (strcmp(bgrp, "AB-") == 0))
  {
    while (fread(&input, sizeof(input), 1, fp))
    {
      if (((strcmp(input.bloodgroup, "AB-") == 0) || (strcmp(input.bloodgroup, "O-") == 0) || (strcmp(input.bloodgroup, "A-") == 0) || (strcmp(input.bloodgroup, "B-") == 0)) && (date_diff(da, input.lastDonated) > 90) && (input.age > 18) && (input.age < 65))
      {
        printf("%d\t%s\t%d\t%c\t%s\t\t%s\t%d-%d-%d\n", input.id,
               input.name, input.age, input.gender, input.bloodgroup, input.phoneNO, input.lastDonated.date, input.lastDonated.month, input.lastDonated.year);
      }
    }
    rewind(fp);

    if (((strcmp(bgrp, "AB+") == 0)))
    {
      while (fread(&input, sizeof(input), 1, fp))
      {
        if (((strcmp(input.bloodgroup, "AB+") == 0) || (strcmp(input.bloodgroup, "O+") == 0) || (strcmp(input.bloodgroup, "A+") == 0) || (strcmp(input.bloodgroup, "B+") == 0)) && (date_diff(da, input.lastDonated) > 90) && (input.age > 18) && (input.age < 65))
        {
          printf("%d\t%s\t%d\t%c\t%s\t\t%s\t%d-%d-%d\n", input.id,
                 input.name, input.age, input.gender, input.bloodgroup, input.phoneNO, input.lastDonated.date, input.lastDonated.month, input.lastDonated.year);
        }
      }
      rewind(fp);
    }
  }

  else
  {
    while (fread(&input, sizeof(input), 1, fp))
    {
      if (((strcmp(input.bloodgroup, "O-") == 0) || (strcmp(input.bloodgroup, "B-") == 0)) && (date_diff(da, input.lastDonated) > 90) && (input.age > 18) && (input.age < 65))
      {
        printf("%d\t%s\t%d\t%c\t%s\t\t%s\t%d-%d-%d\n", input.id,
               input.name, input.age, input.gender, input.bloodgroup, input.phoneNO, input.lastDonated.date, input.lastDonated.month, input.lastDonated.year);
      }
    }
    rewind(fp);
    if (strcmp(bgrp, "B+") == 0)
    {
      while (fread(&input, sizeof(input), 1, fp))
      {
        if (((strcmp(input.bloodgroup, "O+") == 0) || (strcmp(input.bloodgroup, "B+") == 0)) && (date_diff(da, input.lastDonated) > 90) && (input.age > 18) && (input.age < 65))
        {
          printf("%d\t%s\t%d\t%c\t%s\t\t%s\t%d-%d-%d\n", input.id,
                 input.name, input.age, input.gender, input.bloodgroup, input.phoneNO, input.lastDonated.date, input.lastDonated.month, input.lastDonated.year);
        }
      }
    }
    rewind(fp);
  }

  fclose(fp);
}

void delete_dnr()
{
  FILE *fp, *ft, *fd;
  int y, flag;
  int id;
  flag = 0;
  struct donor input;
  printf("Enter your RegNo: ");
  scanf("%d", &id);

  ft = fopen("temp.dat", "wb");
  fp = fopen("person.dat", "rb");
  fd = fopen("del.dat", "ab");

  while (fread(&input, sizeof(input), 1, fp))
  {
    if ((input.id != id))
    {
      fwrite(&input, sizeof(input), 1, ft);
    }
    else
    {
      fwrite(&input, sizeof(input), 1, fd);
      flag = 1;
    }
  }

  fclose(fp);
  fclose(ft);
  fclose(fd);

  remove("person.dat");
  rename("temp.dat", "person.dat");

  if (flag == 1)
  {
    printf("Record Deleted\n");
  }
  else
  {
    printf("Record not found\n");
  }
}

int bloodGroupVerify(char bg[4])
{
  if (strcmp(bg, "O+") == 0 ||
      strcmp(bg, "O-") == 0 ||
      strcmp(bg, "A+") == 0 ||
      strcmp(bg, "A-") == 0 ||
      strcmp(bg, "B+") == 0 ||
      strcmp(bg, "B-") == 0 ||
      strcmp(bg, "AB+") == 0 ||
      strcmp(bg, "AB-") == 0)
  {
    return 1;
  }
  else
  {
    printf("\nInvalid Blood Group");
    printf("\n1)Main menu\n2)Re enter Blood group\n3)Exit the program\n");
    int invalid_bg;
    scanf("%d", &invalid_bg);
    getchar();
    switch (invalid_bg)
    {
    case 1:
      main();
    case 2:
      return 0;
    case 3:
      exit(1);
    default:
      exit(1);
    }
  }
}

int validpno(char pino[15])
{
  if (strlen(pino) == 10)
  {
    if (pino[0] == '9')
    {
      return 1;
    }
    else if (pino[0] == '8')
    {
      return 1;
    }
    else if (pino[0] == '7')
    {
      return 1;
    }
    else if (pino[0] == '6')
    {
      return 1;
    }
    else
    {
      return 0;
    }
  }
  else
  {
    return 0;
  }
}

int dateVerify(int day, int mon, int year)
{
  int is_leap = 0, is_valid = 1;

  if (year >= 1800 && year <= 9999)
  {

    if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))
    {
      is_leap = 1;
    }

    if (mon >= 1 && mon <= 12)
    {
      if (mon == 2)
      {
        if (is_leap && day == 29)
        {
          is_valid = 1;
        }
        else if (day > 28)
        {
          is_valid = 0;
        }
      }

      else if (mon == 4 || mon == 6 || mon == 9 || mon == 11)
      {
        if (day > 30)
        {
          is_valid = 0;
        }
      }

      else if (day > 31)
      {
        is_valid = 0;
      }
    }

    else
    {
      is_valid = 0;
    }
  }
  else if ((day == 0) && (mon == 0) && (year == 0))
  {
    is_valid = 1;
  }
  else
  {
    is_valid = 0;
  }

  return is_valid;
}

int date_diff(struct date d,struct date date){
    int monthDays[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int i;
    long int n = date.year*365 + date.date; 
  
    for (i=0; i<date.month - 1; i++) 
        n += monthDays[i]; 

    n += countLeapYears(date); 
  
    long int m = d.year*365 + d.date; 
    for (i=0; i<d.month - 1; i++) 
        m += monthDays[i]; 
    m += countLeapYears(d); 

    return (m - n); 
}

int countLeapYears(struct date day){ 
    int years = day.year; 
  
    if (day.month <= 2) 
        years--; 

    return years / 4 - years / 100 + years / 400; 
}
int login(){
  char admin[20];
  char *password;
  printf("Username: ");
  fflush(stdin);
  xgets(admin, 20, stdin);
  fflush(stdin);
  password = getpass("Enter Password: ");
  // printf("%s\n",password);
  if (!(strcmp(admin, "admin") || strcmp(password, "admin"))){
    return 1;
  }
  else{
    return 0;
  }
}
