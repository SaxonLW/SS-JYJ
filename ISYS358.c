/*
 ============================================================================
 Name        : ISYS358.c
 Author      : Saxon Lysaght-Wheeler
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <windows.h>
#include "ISYS358.h"


typedef int uint;

FILE * listOut;

FILE * groupOut;

FILE * input;

unsigned int blInt = 0;

unsigned int tolerance = 3;

char clearLine [] = "                              ";

struct timeval start, randomAssigned, listSorted, groupsAssigned, personsAssigned, groupsAllocated;

void bl (){
	blInt++;
	printf ("\nbreakLine\t%d\n", blInt);
	fflush(stdout);
}



static char *rand_string(char *str, size_t size)
{
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJK";
    if (size) {
        --size;
        for (size_t n = 0; n < size; n++) {
            int key = rand() % (int) (sizeof charset - 1);
            str[n] = charset[key];
        }
        str[size] = '\0';
    }
    return str;
}

void printSortedList (PersonRefList list){
	for (unsigned int i = 0; i < list.size; i++){
		//printf("%d\t%s\t%s\t%d\t%d\n",list.list[i]->ID,list.list[i]->name,list.list[i]->address,list.list[i]->postcode,list.list[i]->group);
		fprintf(listOut,"%d\t%s\t%s\t%d\n",list.list[i]->ID,list.list[i]->name,list.list[i]->address,list.list[i]->postcode);
	}
}

void printGroups (GroupList * list){
	for (int i = 0; i < list->size; i++){
		if (list->list->memberCount){

			printf("\nGroup %d", i);
			fprintf(groupOut,"\nGroup %d", i);

			for (int j = 0; j < list->list->memberCount; j++){
				printf("\nName;\t%s\t\tPostCode\t%u", list->list[i].members[j]->name,list->list[i].members[j]->postcode);

				fprintf(groupOut,"\nName;\t%s\t\tPostCode\t%u", list->list[i].members[j]->name,list->list[i].members[j]->postcode);

			}
		}
	}
}

void printGroup (Group * group){
	for (int i = 0; i < group->memberCount; i++){

	}
}

void assignRandom (PersonList * baseList){
	for (int i = 0; i < baseList->size; i++){

		baseList->list[i].name = calloc(20, sizeof(char));
		rand_string(baseList->list [i].name,20);

		baseList->list[i].address = calloc(20,sizeof(char));
		rand_string(baseList->list[i].address, 20);

		baseList->list[i].postcode = rand()%1000 + 2000;

		baseList->list[i].ID = i;

		baseList->list[i].group = -1;
	}
}

void createShallowCopy (PersonList * original, PersonRefList * shallow){
	shallow->size = original->size;
	shallow->list = (Person**)calloc(shallow->size,sizeof(Person*));
	register Person * tmp = &original->list[0];
	unsigned int size = &original->list[1] - &original->list[0];
	for (int i = 0; i < shallow->size; i++){
		shallow->list[i] = tmp;
		tmp += size;
	}
}

int partition (PersonRefList * list, int l, int r){
	int pivot, i, j;
	Person * t;
	pivot = list->list[l]->postcode;
	i = l;
	j = r +1;

	while (1){
		do{
			i++;
			/*printf("\nA\tI\t%d\tJ\t%d\tL\t%d\tR\t%d\n", i,j,l,r);
			fflush(stdout);*/
		} while (list->list[i]->postcode <= pivot && i < r);

		do {
			if (j == 0){
				break;
			}
			j--;
			/*printf("\nB\tI\t%d\tJ\t%d\tL\t%d\tR\t%d\n", i,j,l,r);
			fflush(stdout);*/
		} while (list->list[j]->postcode > pivot && j > 0);

		if (i >=j){
			break;
		}

		/*printf("\nC\tI\t%d\tJ\t%d\tL\t%d\tR\t%d\n", i,j,l,r);
		fflush(stdout);*/

		t = list->list[i];
		list->list[i] = list->list[j];
		list->list[j] = t;
	}
	t = list->list[l];
	list->list[l] = list->list[j];
	list->list[j] = t;
	return j;
}

void quickSort (PersonRefList * list, int l, int r){
	int j;

	if (l < r){
		j = partition(list,l,r);
		quickSort(list,l,j-1);
		quickSort(list,j+1,r);
	}
}

void sortListPostCode (PersonRefList * list){
	quickSort(list, 0, list->size-1);
}

//EVERYTHING IS LISTS ALL I SEE IS LISTS FUCKING END EVERYTHING AAAAAAAAAHHHHHHHHHHH

GroupList* assignGroups (PersonRefList * baseList){

	GroupList * groupList;

	uint memberCount = 0;

	uint groupIndex = 0;

	uint targetPC = baseList->list[0]->postcode;

	Person* cPerson = NULL;

	for (uint i = 0; i < baseList->size; i++){
		cPerson = baseList->list[i];

		if (memberCount < 4 && ((cPerson->postcode - targetPC) <= tolerance)){
			cPerson->group = groupIndex;
			cPerson->memberIndex = memberCount;
			memberCount+=1;
		}
		else{
			memberCount = 0;
			targetPC = cPerson->postcode;
			groupIndex+=1;

			cPerson->group = groupIndex;
			cPerson->memberIndex = memberCount;
			memberCount++;
		}
	}

	groupList = calloc(1,sizeof(GroupList));
	groupList->size = groupIndex+1;
	groupList->list = malloc((groupIndex+1) * sizeof(Group));

	gettimeofday(&personsAssigned,NULL);

	uint group = 0;
	uint mem = 0;

	for (uint i = 0; i < baseList->size; i++){
		cPerson = baseList->list[i];
		group = cPerson->group;
		mem = cPerson->memberIndex;
		groupList->list[group].members[mem] = cPerson;
		groupList->list[group].memberCount = mem+1;
	}

	gettimeofday(&groupsAllocated,NULL);

	return groupList;
}

int main(int argv, char** argc) {
	srand(time(NULL));

	PersonList baseList;

	PersonRefList listCopy;

	GroupList * groupList;

	listOut = fopen ("listOut.txt", "w");
	groupOut = fopen("groupOut.txt", "w");

	gettimeofday(&start,NULL);

	unsigned int length = 1000000;

	switch (argv){
		case (3):
			if((tolerance = atoi(argc[2])) == 0){
				tolerance = 3;
			}
		case (2):
			if((length = atoi(argc[1])) == 0){
				length = 100;
			}
			break;
	}

	baseList.list = (Person*) calloc (length,sizeof(Person));
	baseList.size = length;

	assignRandom(&baseList);

	gettimeofday(&randomAssigned,NULL);
	createShallowCopy (&baseList,&listCopy);

	sortListPostCode(&listCopy);
	gettimeofday(&listSorted,NULL);

	groupList = assignGroups (&listCopy);

	gettimeofday(&groupsAssigned,NULL);

	printf("\nTime to assign random;\t%ld\nTime to sort;\t%ld\nTime to assign groups to person;\t%ld\nTime to assign person to groups;\t%ld\nTime to assign groups;\t%ld\n",randomAssigned.tv_sec-start.tv_sec,listSorted.tv_sec-start.tv_sec,personsAssigned.tv_sec-start.tv_sec, groupsAllocated.tv_sec-start.tv_sec, groupsAssigned.tv_sec-start.tv_sec);
	printf("\nWriting Groups\r");
	fflush(stdout);

	for (int i = 0; i < groupList->size; i++){
		fprintf(groupOut,"\nGroup %d\n", i);
		fprintf(groupOut,"\tNumber of Members;\t%d\n", groupList->list[i].memberCount);
		for (int j = 0; j < groupList->list[i].memberCount; j++){
			fprintf (groupOut,"\t\tID; %d\tName; %s\tAddress; %s\tPostCode; %d\n",groupList->list[i].members[j]->ID, groupList->list[i].members[j]->name, groupList->list[i].members[j]->address,groupList->list[i].members[j]->postcode);
		}
	}
	printf(clearLine);
	printf("\rWriting Lists");
	fflush(stdout);
	printSortedList(listCopy);

	fflush(groupOut);

	fclose(listOut);

	fclose(groupOut);

	return EXIT_SUCCESS;
}
