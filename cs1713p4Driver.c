/**********************************************************************
cs1713p4Driver.c
Purpose:
    This program reads user information and a command file.   It
    processes the commands against the user information.
    This is the driver code for the routines written by the students.
Command Parameters:
    p4 -u userFileName -c commandFileName
Input:
    User	same as Programming Assignment #3 although there may be different data.

    Command	This is is in the form of commands:
         VEH FILL szUserId1 dPPG iMiles dGallons
            This command will cause processFillup to process the data
         VEH FIX szUserId dPPG iMiles dGallons
            Change the vehicle data by removing a fillup from the data:
               - change the user’s vehicle data and reduce the fillups by 1
               - don’t modify a user with 0 fillups
            This functionality should be placed in the processFillupFix function.
                 If a user doesn't exist, show a warning.
         USER UPDVEH szUserId szMake szModel
             Change the make and model of this user’s car and reset the average MPG,
                average PPG, and number of fillups to zero.
                If the user doesn't exist, show a warning.
         USER SHOW szUserId
             Show all the user information for this one user. If the
             user doesn't exist, show a warning.
         USER NEW szUserId    szMake     szModel
                   6s          10s        10s
            Add this new user to the database (linked list).
            Note:  you should check whether the user already exists.
                   Print a warning message if it does.  Do not exit.
         USER CONTACT szUserId szLoginName  szEmailAddr szPhone szFullName
                       6s       12s          30s         13s     20s
            The values are separated by commas.
            This command should change the contact information for a user
              to the specified values.  If the user doesn't exist,
              it should show a warning message, but not exit.

Results:
    Prints the Users prior to processing commands.
    Processes the commands (see above) and shows any errors.
    Prints the resulting Users
Returns:
    0  normal
    1  invalid command line syntax
    2  show usage only
    3  error during processing, see stderr for more information

Notes:
    p4 -?       will provide the usage information.  In some shells,
                you will have to type p4 -\?

**********************************************************************/
// If compiling using visual studio, tell the compiler not to give its warnings
// about the safety of scanf and printf
#define _CRT_SECURE_NO_WARNINGS 1

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "cs1713p4.h"

// prototypes for functions used only in this file
void processCommandSwitches(int argc, char *argv[], char **ppszUserFileName
    , char **ppszCommandFileName);
void processCommands(Node **ppHead, char *pszCommandFileName);
int getSimpleToken(char szInput[], int *piBufferPosition, char szToken[]);

#define MAX_TOKEN_SIZE 20

int main(int argc, char *argv[])
{
    Node      *pHead;                       // points to an ordered linked list of users
    char      *pszUserFileName = NULL;      // User file name
    char      *pszCommandFileName = NULL;   // Command file name

    processCommandSwitches(argc, argv, &pszUserFileName, &pszCommandFileName);

    // get the users list, print it, sort it, and print the sorted list
    pHead = getUsers(pszUserFileName);
    printUsers("Initial Users", pHead);

    printf("\n");

    // process the command input file
    processCommands(&pHead, pszCommandFileName);

    printf("\n");
    // print the user after processing the command file
    printUsers("Resulting Users", pHead);
    printf("\n");
    return 0;
}

/******************** processCommands **************************************
    void processCommands(Node **ppHead, char *pszCommandFileName)
Purpose:
    Reads the Command file to process commands.  There are two major
    types of commands:  USER and VEH.
Parameters:
    I/O Node **ppHead                 Address of the pointer to the head of
                                      an ordered linked list of users
    I   char  **ppszCommandFileName   command file name
Notes:
    This function opens and closes the Command file.
**************************************************************************/
void processCommands(Node **ppHead, char *pszCommandFileName)
{
    FILE *pfileCommand;                     // stream Input for commands
    char szInputBuffer[100];                // input buffer for a single text line
    char szCommand[MAX_TOKEN_SIZE+1];       // command
    char szSubCommand[MAX_TOKEN_SIZE+1];    // subcommand
    int bGotToken;                          // TRUE if getSimpleToken got a token
    int iBufferPosition;                    // This is used by getSimpleToken to
                                            // track parsing position within input buffer

    // open the Command stream data file
    if (pszCommandFileName == NULL)
        exitError(ERR_MISSING_SWITCH, "-c");

    pfileCommand = fopen(pszCommandFileName, "r");
    if (pfileCommand == NULL)
        exitError(ERR_COMMAND_FILENAME, pszCommandFileName);

    /* get command data until EOF
    ** fgets returns null when EOF is reached.
    */
    while (fgets(szInputBuffer, 100, pfileCommand) != NULL)
    {
        printf("%s", szInputBuffer);        // includes \n in the data
        iBufferPosition = 0;                // reset buffer position

        // get the command
        bGotToken = getSimpleToken(szInputBuffer, &iBufferPosition, szCommand);

        // see what the command is
        if (bGotToken && strcmp(szCommand, "USER") == 0)
        {   // User command
            // get the sub comand
            bGotToken = getSimpleToken(szInputBuffer, &iBufferPosition, szSubCommand);
            if (bGotToken)
                processUserCommand(ppHead
                , szSubCommand
                , &szInputBuffer[iBufferPosition]           // address past the subcommand
                );
            else exitError(ERR_USER_SUB_COMMAND, " missing subcommand");
        }
        else  if (bGotToken && strcmp(szCommand, "VEH") == 0)
        {   // VEH command
            bGotToken = getSimpleToken(szInputBuffer, &iBufferPosition, szSubCommand);
            if (bGotToken)
                processVehCommand(ppHead, szSubCommand
                    , &szInputBuffer[iBufferPosition]);
            else exitError(ERR_VEH_SUB_COMMAND, " missing subcommand");
        }
        else
            exitError(ERR_INVALID_COMMAND, szCommand);
    }
    fclose(pfileCommand);
}

/******************** getUsers ***************************************
    Node * getUsers(char * pszUserFileName)
Purpose:
    Retrieves the users from the specified file.
Parameters:
    I   char *pszUserFileName name of the file containing the user data
Returns:
    A pointer to the first node in a linked list.
Notes:
    1. This function opens the file based on the specified file name.
    2. The data has two types of records per user:
        - User Vehicle Information:
            o One line per user (separated by commas)
                szUserId  szMake szModel    dAveMPG    dAvePPG
                 6s        10s    10s        lf          lf
        - User Contact Information:
            o One line per user (separated by commas)
                szLoginNm  szEmail szPhone szContactName
                 12s        30s     13s     20s
**************************************************************************/

Node * getUsers(char * pszUserFileName)
{
    char szInputBuffer[100];      // input buffer for reading data
    int iScanfCnt;                // returned by sscanf
    FILE *pFileUser;              // Stream Input for Users data.
    User user;
    Node *pFind;
    Node *pHead = NULL;

    /* open the Users stream data file */
    if (pszUserFileName == NULL)
        exitError(ERR_MISSING_SWITCH, "-u");

    pFileUser = fopen(pszUserFileName, "r");
    if (pFileUser == NULL)
        exitError(ERR_USER_FILENAME, pszUserFileName);

    /* get user information until EOF
    ** fgets returns null when EOF is reached.
    */

    while (fgets(szInputBuffer, 100, pFileUser) != NULL)
    {
        iScanfCnt = sscanf(szInputBuffer, "%s %s %lf %lf %d"
            , user.szMake
            , user.szModel
            , &user.dAvgMPG
            , &user.dAvgPPG
            , &user.iNumFillups);

        // Check for bad identification data
        if (iScanfCnt < 5)
            exitError(ERR_VEHICLE_DATA, szInputBuffer);

        //read a contact record.  If EOF,  terminate

        if (fgets(szInputBuffer, 100, pFileUser) == NULL)
            exitError(ERR_USER_CONTACT_DATA, szInputBuffer);

        iScanfCnt = sscanf(szInputBuffer, "%6s,%30[^,],%20[^,],%13[^,],%12[^\n]\n"
            , user.szUserId
            , user.szEmailAddr
            , user.szFullName
            , user.szPhone
            , user.szLoginName);

        // Check for bad contact data
        if (iScanfCnt < 5)
            exitError(ERR_USER_CONTACT_DATA, szInputBuffer);

        // Insert the user into the linked list.  Since insertLL might
        // change the beginning of the linked list, we must pass the
        // address of pHead
        pFind = insertLL(&pHead, user);
    }
    fclose(pFileUser);
    return pHead;
}

/******************** allocateNode **************************************
    Node * allocateNode(User user)
Purpose:
    Allocates a new node, placing the parameter in the node.
Parameters:
    I   User user    // User information for one user.
Returns:
    A pointer to the newly allocated node.
Notes:
    If there isn't enough memory available, this function terminates.
**************************************************************************/
Node * allocateNode(User user)
{
    Node *pNew;                     // location of newly allocated a new node
    pNew = malloc(sizeof(Node));    // get some memory
    if (pNew == NULL)               // did malloc work?
        exitError("Memory allocation error", "");
    pNew->user = user;              // load data into new node
    pNew->pNext = NULL;             // initialize pNext
    return pNew;
}

 /******************** getSimpleToken **************************************
 int getSimpleToken(char szInput[], int *piBufferPosition, char szToken[])
 Purpose:
    Returns the next token in a string.  The delimiter for the token is a
    space, a newline or the end of the string.  To help with a
    subsequent call, it also returns the next position in the buffer.
Parameters:
    I   char szInput[]          input buffer to be parsed
    I/O int *piBufferPosition   Position to begin looking for the next token.
                                This is also used to return the next
                                position to look for a token (which will
                                follow the delimiter).
    O   char szToken[]          Returned token.
Returns:
    Functionally:
        TRUE - a token is returned
        FALSE - no more tokens
    iBufferPosition parm - the next position for parsing
    szToken parm - the returned token.  If not found, it will be an empty string.
Notes:
    - If the token is larger than the szToken parm, we return a truncated value.
**************************************************************************/

int getSimpleToken(char szInput[], int *piBufferPosition, char szToken[])
{
    int iDelimPos;                      // found position of delim
    int iCopy;                          // number of characters to copy
    char szDelims[20] = " \n";          // delimiters

    // check for past the end of the string
    if (*piBufferPosition >= (int) strlen(szInput))
    {
        szToken[0] = '\0';              // mark it empty
        return FALSE;                   // no more tokens
    }

    // get the position of the first delim, relative to the iBufferPosition
    iDelimPos = strcspn(&szInput[*piBufferPosition], szDelims);

    // see if we have more characters than target token, if so, trunc
    if (iDelimPos > MAX_TOKEN_SIZE)
        iCopy = MAX_TOKEN_SIZE;             // truncated size
    else
        iCopy = iDelimPos;

    // copy the token into the target token variable
    memcpy(szToken, &szInput[*piBufferPosition], iCopy);
    szToken[iCopy] = '\0';              // null terminate

    // advance the position
    *piBufferPosition += iDelimPos + 1;
    return TRUE;
}

/******************** processCommandSwitches *****************************
    void processCommandSwitches(int argc, char *argv[]
        , char **ppszUserFileName
        , char **ppszCommandFileName)
Purpose:
    Checks the syntax of command line arguments and returns the filenames.
    If any switches are unknown, it exits with an error.
Parameters:
    I   int argc                        count of command line arguments
    I   char *argv[]                    array of command line arguments
    O   char **ppszUserFileName         user file name
    O   char **ppszCommandFileName      command file name
Notes:
    If a -? switch is passed, the usage is printed and the program exits
    with USAGE_ONLY.
    If a syntax error is encountered (e.g., unknown switch), the program
    prints a message to stderr and exits with ERR_COMMAND_LINE_SYNTAX.
**************************************************************************/
void processCommandSwitches(int argc, char *argv[], char **ppszUserFileName
    , char **ppszCommandFileName)
{
    int i;

    for (i = 1; i < argc; i++)
    {
        // check for a switch
        if (argv[i][0] != '-')
            exitUsage(i, ERR_EXPECTED_SWITCH, argv[i]);
        // determine which switch it is
        switch (argv[i][1])
        {
        case 'u':                   // User File Name
            if (++i >= argc)
                exitUsage(i, ERR_MISSING_ARGUMENT, argv[i - 1]);
            // check for too long of a file anme
            else
                *ppszUserFileName = argv[i];
            break;
        case 'c':                   // Command File Name
            if (++i >= argc)
                exitUsage(i, ERR_MISSING_ARGUMENT, argv[i - 1]);
            else
                *ppszCommandFileName = argv[i];
            break;
        case '?':
            exitUsage(USAGE_ONLY, "", "");
            break;
        default:
            exitUsage(i, ERR_EXPECTED_SWITCH, argv[i]);
        }
    }
}

/******************** exitError **************************************
    void exitError(char *pszMessage, char *pszDiagnosticInfo)
Purpose:
    Prints an error message and diagnostic to stderr.  Exits with
    ERROR_PROCESSING.
Parameters:
    I char *pszMessage              error message to print
    I char *pszDiagnosticInfo       supplemental diagnostic information
Notes:
    This routine causes the program to exit.
**************************************************************************/
void exitError(char *pszMessage, char *pszDiagnosticInfo)
{
    fprintf(stderr, "Error: %s %s\n"
        , pszMessage
        , pszDiagnosticInfo);
    exit(ERROR_PROCESSING);
}

/******************** exitUsage *****************************
    void exitUsage(int iArg, char *pszMessage, char *pszDiagnosticInfo)
Purpose:
    If this is an argument error (iArg >= 0), it prints a formatted message
    showing which argument was in error, the specified message, and
    supplemental diagnostic information.  It also shows the usage. It exits
    with ERR_COMMAND_LINE_SYNTAX.

    If this is just asking for usage (iArg will be -1), the usage is shown.
    It exits with USAGE_ONLY.
Parameters:
    I int iArg                      command argument subscript
    I char *pszMessage              error message to print
    I char *pszDiagnosticInfo       supplemental diagnostic information
Notes:
    This routine causes the program to exit.
**************************************************************************/

void exitUsage(int iArg, char *pszMessage, char *pszDiagnosticInfo)
{
    if (iArg >= 0)
        fprintf(stderr, "Error: bad argument #%d.  %s %s\n"
        , iArg
        , pszMessage
        , pszDiagnosticInfo);
    fprintf(stderr, "p4 -u userFileName -c commandFileName\n");
    if (iArg >= 0)
        exit(-1);
    else
        exit(-2);
}
