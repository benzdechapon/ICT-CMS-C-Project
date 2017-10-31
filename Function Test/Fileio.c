#include<stdio.h>
struct userINFO {
    char id[7];
    char name[20];
    char lastName[20];
};
int main()
{
    FILE *fdata;
    struct userINFO allUser[3];
    int i=0;
    fdata = fopen("textdata.txt","r");

    if (fdata == NULL)
    {
        printf("ERROR!");
        return 0;
    }

    while (fscanf(fdata,"%s %s %s", allUser[i].id, allUser[i].name, allUser[i].lastName)!=EOF)
    {
        i++;
    }

    for (i = 0; i < 3 ; i++)
    {
        printf("ID: %s\n", allUser[i].id);
        printf("Name: %s\n", allUser[i].name);
        printf("LastName: %s\n", allUser[i].lastName);
    }
    fclose(fdata);

    return 0;
}
