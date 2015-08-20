/*
 * ISYS358.h
 *
 *  Created on: 13 Aug 2015
 *      Author: Whooza
 */

#ifndef ISYS358_H_
#define ISYS358_H_

/*
 * Name		Person
 * Datatype	Struct
 * Purpose	Stores the variables ascociated with a Person
 */
typedef struct person {
	char* name;
	char* address;
	unsigned int postcode;
	unsigned int ID;
	unsigned int group;
	unsigned int memberIndex;
	struct person * relationships;


}Person;
/*
 * Name		PersonList
 * Datatype	Struct
 * Purpose	Stores a list of Person
 */
typedef struct personList{
	Person * list;
	unsigned int size;
}PersonList;
/*
 * Name		personRefList
 * Datatype	Struct
 * Purpose	Stores a list of references to person
 */
typedef struct personRefList{
	Person ** list;
	unsigned int size;
}PersonRefList;
/*
 * Name		Group
 * Datatype	Struct
 * Purpose	Stores the variables ascociated with a Group
 */
typedef struct group{
	Person* members [4];
	int memberCount;
}Group;
/*
 * Name		Person
 * Datatype	Struct
 * Purpose	Stores a list of Groups
 */
typedef struct groupList{
	Group * list;
	unsigned int size;
}GroupList;

/*
 * Name		rand_string
 * Datatype	function
 * Passed Values;
 * 		char* str		The string that will be assigned a value
 * 		size_t size		The length of the random string
 * Return Value
 * 		static char*	The pointer tot he randomly generated string
 * Purpose	Generates a random string
 */
static char *rand_string(char *str, size_t size);
/*
 * Name			print""
 * Datatype		function
 * Passed Values;
 * 		The data to be printed
 * Return Value
 * 		void
 * Purpose
 * 		Printing the various data types
 */
void printSortedList (PersonRefList list);

void printGroups (GroupList * list);

void printGroup (Group * group);
/*
 * Name			Assign Random
 * Datatype		Function
 * Passed Values;
 * 		PersonList * baseList		This is a pointer to a list of people
 * Return Value
 * 		void
 * Purpose
 * 		Assigns the values of each person in the list
 */
void assignRandom (PersonList * baseList);
/*
 * Name		createShallowCopy
 * Datatype	Function
 * Passed Values;
 * 		PersonList * original		Is the original list of Persons
 * 		PersonRefList * shallow		Is the shallow copy of the list of Persons
 * Return Value
 * 		void
 * Purpose
 * 	To generate a shallow shallow copy of the list of Persons. (The double shallow is purposeful)
 */
void createShallowCopy (PersonList * original, PersonRefList * shallow);
/*
 * Name		sortListPostCode, QuickSort and partition
 * Datatype	Function
 * Passed Values;
 * 		PersonRefList * list	A shallow shallow copy of Persons
 * 		int l					The lowest value to be sorted
 * 		int r					The largest value to be sorted
 * Return Value
 * 		void
 * Purpose
 * 		To sort the list in nlog(n) operations
 */
int partition (PersonRefList * list, int l, int r);

void quickSort (PersonRefList * list, int l, int r);

void sortListPostCode (PersonRefList * list);
/*
 * Name		assignGroups
 * Datatype	functions
 * Passed Values;
 * 		PersonRefList * baseList		A reference to shallow shallow copy of Persons sorted by post code
 * Return Value
 * 		GroupList *						A reference to a list of groups
 * Purpose
 */
GroupList* assignGroups (PersonRefList * baseList);

#endif /* ISYS358_H_ */


