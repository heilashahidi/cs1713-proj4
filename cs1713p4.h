/**********************************************************************
cs1713p4.h
Purpose:
    Defines typedefs for
        User - includes user ID, user full name, average miles per
               gallon, average price per gallon, number of fillups,
               email address, phone, contact full name.
        Fillup - includes userID, price per gallon, miles, gallons
                 purchased
        Node - linked list node for one user
    Defines constants for
        boolean values
        maximum line size
        error messages
        program return codes
    Prototypes
Notes:

**********************************************************************/

/**** typedefs ****/

// User information
typedef struct
{
    char        szUserId[7];            // 6 character User ID
    char        szEmailAddr[31];        // User's email address
    char        szLoginName[13];        // User's login name
    char        szPhone[14];            // User contact phone number
                                        //    (999)999-9999
    char        szFullName[21];         // User first and last name
    char        szMake[11];             // Vehicle make
    char        szModel[11];            // Vehicle model
    double      dAvgMPG;                // average miles per gallon
    double      dAvgPPG;                // average price per gallon
    int         iNumFillups;            // number of fillups in average
} User;

// Fillup information
typedef struct
{
    char szUserId[7];                   // 6 character User ID
    double dPPG;                        // price per gallon paid
    int iMiles;                         // miles driven
    double dGallons;                    // gallons purchased
} Fillup;

typedef struct Node
{
    User user;
    struct Node *pNext;
} Node;

/**** constants ****/
// Maximum sizes
#define MAX_LINE_SIZE 100

// boolean
#define FALSE           0
#define TRUE            1

/*  Error Messages */
#define ERR_MISSING_SWITCH              "missing switch "
#define ERR_EXPECTED_SWITCH             "expected switch, found "
#define ERR_MISSING_ARGUMENT            "missing argument for "
#define ERR_USER_FILENAME               "invalid User file name "
#define ERR_COMMAND_FILENAME            "invalid Command file name "
#define ERR_VEHICLE_DATA                "invalid User vehicle data "
#define ERR_USER_CONTACT_DATA           "invalid User contact data "

#define ERR_INVALID_COMMAND             "command name is invalid"
#define ERR_FILLUP_DATA                 "invalid Fillup data"
#define ERR_USER_NOT_FOUND              " *** user (%s) not found\n"

#define ERR_USER_SHOW_DATA              "bad data for USER SHOW command "
#define ERR_USER_NEW_DATA               "bad data for USER NEW command "
#define ERR_USER_UPDVEH_DATA            "bad data for USER UPDVEH command "
#define ERR_VEH_FILL_DATA               "bad data for VEH FILL command "
#define ERR_VEH_FIX_DATA                "bad data for VEH FIX command "
#define ERR_USER_SUB_COMMAND            "invalid/missing subcommand for USER"
#define ERR_VEH_SUB_COMMAND             "invalid/missing subcommand for VEH"

#define WARN_USER_NOT_FOUND             " *** user (%s) not found ***\n"
#define WARN_FILLS_ZERO                 " *** fillups is zero ***\n"
#define WARN_GALLS_ZERO                 " *** zero gallons, not updating ***\n"

#define WARN_USER_SUB_COMMAND           "invalid subcommand for USER: %s\n"
#define WARN_VEH_SUB_COMMAND            "invalid subcommand for VEH: %s\n"
#define WARN_USER_NEW_SUB_COMMAND       "invalid USER NEW subcommand %s\n"
#define WARN_USER_CONTACT_SUB_COMMAND   "invalid USER CONTACT subcommand: %s\n"
#define WARN_USER_ALREADY_EXISTS        " *** user (%s) already exists\n"

/* program return codes */
#define ERR_COMMAND_LINE_SYNTAX     1      // invalid command line syntax
#define USAGE_ONLY                  2      // show usage only
#define ERROR_PROCESSING            3      // exit error

/* prototypes */

/* provided functions */
Node *allocateNode(User user);
Node *getUsers(char *pszUserFileName);
void exitError(char *pszMessage, char *pszDiagnosticInfo);
void exitUsage(int iArg, char *pszMessage, char *pszDiagnosticInfo);
void printFillup(Fillup fillup);

/* functions you must code  */
void printUsers(char szHeading[], Node *pHead);
void processFillup(Node *ppHead, Fillup fillup);
void processFillupFix(Node *ppHead, Fillup fillup);
void processVehCommand(Node **ppHead
                       , char *pszSubCommand, char *pszRemainingInput);
void processUserCommand(Node **ppHead
                        , char *pszSubCommand, char *pszRemainingInput);
Node *searchLL(Node *pHead, char *pszMatchUserId, Node **ppPrecedes);
Node *insertLL(Node **ppHead, User user);
