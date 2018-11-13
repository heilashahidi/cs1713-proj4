/**********************************************************************
p4nra253.c by Shahidi, Heila
(change the previous line for your abc123 ID and your name)
Purpose:
    This program reads user information and a command file.  It
    processes the commands against the user information.
    This is the file containing functions written by the students.

Input:
 User:   same as Programming Assignment #2

 Command:
        This is different from the previous assignment.
        The file contains text in the form of command.

    VEH FILL szUserId1 dPPG iMiles dGallons
            This command will cause processFillup to process the data
    VEH FIX szUserId dPPG iMiles dGallons
            Change the vehicle data by removing a fillup from the data:
               - change the user’s vehicle data and reduce the fillups by 1
               - don’t modify a user with 0 fillups
            This functionality should be placed in the processFillupFix function.
            If a user doesn't exist, show a warning.
    USER NEWVEH szUserId szMake szModel
             Change the make and model of this user’s car and reset the average MPG,
                average PPG, and number of fillups to zero.
                If the user doesn't exist, show a warning.
    USER SHOW szUserId
             Show all the user information for this one user. If the user doesn't exist,
             show a warning.

Notes:
    This file contains functions:
       getUsers - retrieves the users and places them in an array for the specified file.
       sortUsers - sorts the user array by user ID
       printUsers - prints the user array's contents
       processVehCommand - processes the subcommands associated with the VEH command
       processUserCommand - processes the subcommands associated with the USER command
       searchUsers - using a binary search, finds the specified user in the userM array.
       processFillup - processes a fillup data record, checking the updating the user
            information base on the data provided
       processFillupFix - processes a fix to user information, checking the data and
            possibly changing the PPG and MPG data for the user.

**********************************************************************/
// If compiling using visual studio, tell the compiler not to give its warnings
// about the safety of scanf and printf
#define _CRT_SECURE_NO_WARNINGS 1

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "cs1713p4.h"



/********************processUserCommand *****************************
    void processUserCommand(User userM[], int iUserCnt
     , char *pszSubCommand, char *pszRemainingInput)
Purpose:
    Processes the subcommands associated with the USER command:
    USER UPDVEH szUserId szMake szModel
             Change the make and model of this user’s car and reset the average MPG,
                average PPG, and number of fillups to zero.
                If the user doesn't exist, show a warning.
    USER SHOW szUserId
             Show all the user information for this one user. If the user doesn't exist,
             show a warning.
Parameters:
    I/O Node **ppHead                  Pointer to a pointer to an address of the a node
    I   char *pszSubCommand            Pointer pszSubCommand
    I   char *pszRemainingInput        Pointer to the pszRemainingInput
Notes:

**************************************************************************/
void processUserCommand(Node **ppHead, char *pszSubCommand, char *pszRemainingInput)
{
    int iScanfCnt = 0;
    Node *p;
    Node *pPrecedes;
    User user;



    // Determine what to do based on the subCommand
    if (strcmp(pszSubCommand, "UPDVEH") == 0)
    {
      iScanfCnt = sscanf(pszRemainingInput, " %s %s %s",
			user.szUserId,
			user.szMake,
			user.szModel);
      // get the szUserId and szMake and szModel
      // your code


      // Check for bad data
      if (iScanfCnt < 3)
        exitError(ERR_USER_UPDVEH_DATA, pszRemainingInput);

        // your code which uses searchUsers and updates the paid amount

      p = searchLL(*ppHead, user.szUserId, &pPrecedes);
      if (p == NULL)
      {
        printf(WARN_USER_NOT_FOUND, user.szUserId);
          return;
      }



	     strcpy(p->user.szMake, user.szMake);
	     strcpy(p->user.szModel, user.szModel);
	     p->user.dAvgMPG = 0;
	     p->user.dAvgPPG = 0;
	     p->user.iNumFillups = 0;


        // Check for bad data

      }
      else if (strcmp(pszSubCommand, "NEW") == 0)
      {
        iScanfCnt = sscanf(pszRemainingInput, "%6s %10s %10s",
  			user.szUserId,
  			user.szMake,
  			user.szModel);


        p = searchLL(*ppHead, user.szUserId, &pPrecedes);
        if (p == NULL)
        {
          user.dAvgPPG = 0.0;
          insertLL(ppHead, user);

        }else
          printf(WARN_USER_ALREADY_EXISTS, user.szUserId);

      }
      else if (strcmp(pszSubCommand, "CONTACT") == 0)
      {
        iScanfCnt = sscanf(pszRemainingInput, "%6[^,], %30[^,], %20[^,], %13[^,], %12s",
  			user.szUserId,
  			user.szEmailAddr,
  			user.szFullName,
        user.szPhone,
        user.szLoginName);



        p = searchLL(*ppHead, user.szUserId, &pPrecedes);
        if(p == NULL)
           printf(WARN_USER_NOT_FOUND, user.szUserId);
        else
          {
            strcpy(p->user.szUserId, user.szUserId);
            strcpy(p->user.szEmailAddr, user.szEmailAddr);
            strcpy(p->user.szFullName, user.szFullName);
            strcpy(p->user.szPhone, user.szPhone);
            strcpy(p->user.szLoginName, user.szLoginName);
          }

      }
      else if (strcmp(pszSubCommand, "SHOW") == 0)
      {
        // get the szUserId
        iScanfCnt = sscanf(pszRemainingInput, "%6s", user.szUserId);

        // Check for bad data
        if (iScanfCnt < 1)
            exitError(ERR_USER_SHOW_DATA, pszRemainingInput);

        // find the user
        p = searchLL(*ppHead, user.szUserId, &pPrecedes);
        if (p == NULL)
        {
            printf(WARN_USER_NOT_FOUND, user.szUserId);
            return;
        }

        printf("  %-7s %-10s %-10s  %6.2lf  %6.2lf\n"
               , p->user.szUserId
               , p->user.szMake
               , p->user.szModel
               , p->user.dAvgMPG
               , p->user.dAvgPPG);
        printf("     %-20s %-13s   %-12s %-30s\n"
               , p->user.szFullName
               , p->user.szPhone
               , p->user.szLoginName
               , p->user.szEmailAddr);


        }
    else printf(WARN_USER_SUB_COMMAND, pszSubCommand);
}


/********************processVehCommand *****************************
    void processVehCommand(Node **ppHead, char *pszSubCommand,
                              char *pszRemainingInput)
Purpose:
    Processes the subcommands associated with the VEH command:
        VEH FILL szUserId dPPG iMiles dGallons
            uses processFillup to process this command
        VEH FIX szUserId dPPG iMiles dGallons
            Change the vehicle data by removing a fillup from the data:
               - change the user’s vehicle data and reduce the fillups by 1
               - don’t modify a user with 0 fillups
            Note that the users must exist in the user array.  If not, show a warning.

Parameters:
    I   Node **ppHead                  Pointer to a pointer to a node
    I   char *pszSubCommand            Number of elements in userM[]
    I   char *pszRemainingInput        Pointer to the Remaining Input
Notes:

**************************************************************************/
void processVehCommand(Node **ppHead, char *pszSubCommand, char *pszRemainingInput)
{
    Fillup fillup;
    int iScanfCnt;

    // Determine what to do based on the subCommand
    // your code
    if(strcmp(pszSubCommand, "FILL")==0)
    {
      iScanfCnt = sscanf(pszRemainingInput, "%s %lf %d %lf",
      fillup.szUserId,
      &fillup.dPPG, //explicit dereferncing
      &fillup.iMiles,
      &fillup.dGallons);

      if(iScanfCnt < 4)
        exitError(ERR_VEH_FILL_DATA, pszRemainingInput);

      processFillup(*ppHead, fillup);


    }
      else if(strcmp(pszSubCommand, "FIX")==0)
    {
      iScanfCnt = sscanf(pszRemainingInput, "%s %lf %d %lf",
      fillup.szUserId,
      &fillup.dPPG, //explicit dereferncing
      &fillup.iMiles,
      &fillup.dGallons);

      if(iScanfCnt < 4)
        exitError(ERR_VEH_FILL_DATA, pszRemainingInput);

      processFillupFix(*ppHead, fillup);


    } else{


      printf(WARN_VEH_SUB_COMMAND, pszSubCommand);

    }


}


/***********************processFillup******************************
void processFillup(Node *pHead, Fillup fillup)
Purpose:
	Reads functions from header file and if the two strings length don't
	equal then the new calculations for gallons, MPG, and PPG.
Parameters:
	Node *ppHead -- Pointer to the first Node
  Fillup fillup -- Struct fillup datatype Fillup

Notes:
	This function proccess a fillup, array of users, and the user count.

******************************************************************/
void processFillup(Node *ppHead, Fillup fillup)
{
	printFillup(fillup);
  Node *pPrecedes;
  Node *pUser;

  pUser = searchLL(ppHead, fillup.szUserId, &pPrecedes);
  if(pUser == NULL)
  {
		printf(WARN_USER_NOT_FOUND, fillup.szUserId );
    return;

  }else if(fillup.dGallons == 0){

    printf(WARN_GALLS_ZERO);
    return;

  }else {
    double totalMPG = pUser->user.dAvgMPG * (pUser->user.iNumFillups * 1.0);
		double totalPPG = pUser->user.dAvgPPG * (pUser->user.iNumFillups * 1.0);



		double avgMPG = (totalMPG + (fillup.iMiles/fillup.dGallons)) / (pUser->user.iNumFillups + 1.0);
    pUser->user.dAvgMPG = avgMPG;
		double avgPPG = (totalPPG + (fillup.dPPG))/(pUser->user.iNumFillups + 1.0);
		pUser->user.dAvgPPG = avgPPG;
    pUser->user.iNumFillups++;

		printf("\n");
  }
	}





/****************processFillupFix************************

Purpose:
  The purpose of this function is to fix the number amount of
  fillups that a single car is given. But in this function we are
  subtracting the number of fillups from the averages and then
  calculating the new average of MPG and PPG.

Parameters:
  Fillup fillup -- the renamed structure gives the function the data
  attributes for the new calculated averages.
  Node ppHead -- pointer ppHead

Notes:
  This function proccesses the amount of new fillups after the amount is
  fixed using the new number of AvgMPG, avgPPG, iNumFillups.

*********************************************************/
void processFillupFix(Node *ppHead, Fillup fillup)
{
	printFillup(fillup);
  Node *pPrecedes;
  Node *pUser;


  pUser = searchLL(ppHead, fillup.szUserId, &pPrecedes);
  if(pUser == NULL)
    {
      printf(WARN_USER_NOT_FOUND, fillup.szUserId );

    }
    else if(pUser->user.iNumFillups == 0){
     printf(WARN_FILLS_ZERO);

    }
    else if(pUser->user.iNumFillups == 1){
     pUser->user.dAvgMPG = 0;
     pUser->user.dAvgPPG = 0;
     pUser->user.iNumFillups--;

    }
    else {
      double totalMPG = pUser->user.dAvgMPG * (pUser->user.iNumFillups * 1.0);
      double avgMPG = (totalMPG - (fillup.iMiles/fillup.dGallons)) / (pUser->user.iNumFillups);
      pUser->user.dAvgMPG = avgMPG;

      double totalPPG = pUser->user.dAvgPPG * (pUser->user.iNumFillups - 1.0);
  		double avgPPG = (totalPPG - (fillup.dPPG))/(pUser->user.iNumFillups - 1.0);
     	pUser->user.dAvgPPG = avgPPG;
      pUser->user.iNumFillups--;

  		printf("\n");


    }
}

/*****************printFillup*****************************
Purpose:
  The purpose of this function is to format and print the new
  UserId, PPG, Miles, and Gallons after function fillup and
  fillupfix.

Parameters:
  Fillup fillup -- the renamed structure gives the function the data
  attributes for the new calculated averages.

Notes:
  This function only formats and prints while refrencing the struct
  fillup.

**********************************************************/
void printFillup(Fillup fillup)
{
    printf("%6s %11.2lf %6d %7.2lf\n"
           , fillup.szUserId
           , fillup.dPPG
           , fillup.iMiles
           , fillup.dGallons);
}


/******************** printUsers ************************************
   void printUsers(char szHeading[], User userM[], int iUserCnt)
Purpose:
    Prints user information.
Parameters:
    i char szHeading[] -- string describing why the users are printed
    i Node *pHead -- Node pHead points to the front of the list or the current head
Notes:
    Prints two lines of text per user.
**************************************************************************/
void printUsers(char szHeading[], Node *pHead)
{
    Node *p;
    // Print the heading
    printf("%s\n", szHeading);
    // print column headings
    printf("  %-7s %-10s %-10s  %-6s  %-6s\n", "UserID", "Make", "Model", "AvgMPG", "AvgPPG");
    printf("     %-20s %-13s   %-12s %-30s\n", "Name", "Phone", "login", "email");
    // iterate through the userM array
    for (p = pHead; p != NULL; p = p->pNext )
    {
        printf("  %-7s %-10s %-10s  %6.2lf  %6.2lf\n"
               , p->user.szUserId
               , p->user.szMake
               , p->user.szModel
               , p->user.dAvgMPG
               , p->user.dAvgPPG);
        printf("     %-20s %-13s   %-12s %-30s\n"
               , p->user.szFullName
               , p->user.szPhone
               , p->user.szLoginName
               , p->user.szEmailAddr );
    }
    printf("\n");
}
/*******************************insertLL************************************
Node *insertLL(Node **ppHead, User user)
Purpose:
  This function uses searchLL to search for a User in the ordered linked list
  and then inserts the new User if it wasn't already in the list. When the node
  doesn't exist we allocate new memory for that node. We continue to insert the
  new node when pPrecedes equals or doens't equal NULL.

Parameters:
Node **ppHead -- Node ppHead is passed instead of the array and the count
User user -- Datatype for User and user struct

Notes:
  This function first trie sto find if pFind already exists, if so then we return
  pFind. If not found we allocate a new node. Whenever our pPrecedes equals NULL
  we then make our pNew's pNext equal to out ppHead. This also means we are
  inserting at the head of the list. When we are not inserting at the head of the
  list then we are inserting after a node. When inserting after a node we have our
  pNew's next equals to our pPrecedes's next. We finally have to return pNew.

****************************************************************************/
Node *insertLL(Node **ppHead, User user)
{
  Node *pNew = NULL;
  Node *pPrecedes = NULL;
  Node *pFind = NULL;

  //need to see if the node exists already
  pFind = searchLL(*ppHead, user.szUserId, &pPrecedes);
  if (pFind != NULL)
    return pFind;

  //when the node doesn't exist yet
  pNew = allocateNode(user);

  //insert the new node after pPrecedes
  if (pPrecedes == NULL)
  {
    //this inserts at the list Head
    pNew->pNext = *ppHead;
    *ppHead = pNew;

  } else
  {
    //inserts after a Node
    pNew->pNext = pPrecedes->pNext;
    pPrecedes->pNext = pNew;

  }
  return pNew;
}

/******************************searchLL**************************************
Node *searchLL(Node *pHead, char *pszMatchUserId, Node **ppPrecedes)
Purpose:
  This function searches for a User Id in the ordered linked list. When the
  User Id is found it returns a pointer to the node that contains it. When not
  found it returns NULL. It also returns a pointer to the node that precedes
  the node being searched.

Parameters:
  Node *pHead -- Node structure pHead or the current node
  char *pszMatchUserId -- the pointer that is used to compare with Match User ID.
  Node **ppPrecedes -- the pointer that points to the pointer for ppPrecedes.

Notes:
  We intialize our ppPrecedes to equal NULL, as well as p. Through the for loop
  you can pass in p and then string compare to find if p exists.

*****************************************************************************/
Node *searchLL(Node *pHead, char *pszMatchUserId, Node **ppPrecedes)
{
  Node *p;
  *ppPrecedes = NULL;

  for(p = pHead; p != NULL; p = p->pNext)
  {
    //checks to see if UserId is already there
    if(strcmp(pszMatchUserId, p->user.szUserId) == 0)
      return p; //if found return the address

    if(strcmp(pszMatchUserId, p->user.szUserId) < 0)
      return NULL;
    *ppPrecedes = p;
  }
  return NULL;
}
