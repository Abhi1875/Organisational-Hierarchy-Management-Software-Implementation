#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// The following is a employee in the organisation, it has the pointer to two more employees a subordinate_1 and a subordinate_2
struct Employee
{
    int emp_id; // emp_ids will be unique
    struct Employee *subordinate_1;
    struct Employee *subordinate_2;
};

// The following function creates a employee and returns its pointer
struct Employee *create_employee(int x)
{
    struct Employee *ptr = (struct Employee *)malloc(sizeof(struct Employee));
    ptr->emp_id = x;
    ptr->subordinate_1 = NULL;
    ptr->subordinate_2 = NULL;
    return ptr;
}

// The following code creates a organisation from scanning the input file
struct Employee *create_company()
{
    int x;
    scanf("%d", &x);

    if (x == -1)
        return NULL; // -1 is used when there is a NULL pointer ie when no value is present
    struct Employee *par = create_employee(x);

    par->subordinate_1 = create_company();
    par->subordinate_2 = create_company();

    return par;
}

// The following function
void print_company_helper(struct Employee *ceo)
{
    // take input
    if (!ceo)
    {
        printf("%d ", -1);
        return;
    }
    printf("%d ", ceo->emp_id);
    print_company_helper(ceo->subordinate_1);
    print_company_helper(ceo->subordinate_2);
    return;
}

void print_company(struct Employee *ceo)
{
    print_company_helper(ceo);
    printf("\n");
    return;
}

// --------------------------------------------------- YOU CAN EDIT BELOW THIS LINE -----------------------------------

struct Employee *CEO = NULL;
// CEO is a global pointer that points to the CEO of the company

/*  In this function you have to print all the employees at a given level, Note that ceo is at level 0
In any of the functions which involve printing you need not print -1 for NULL pointers */

void help_EmployeesAtSameLevel(struct Employee *head, int des, int cur)
{
    if (head)
    {
        if (des == cur)
            printf("%d ", head->emp_id);
        else
        {
            help_EmployeesAtSameLevel(head->subordinate_1, des, cur + 1);
            help_EmployeesAtSameLevel(head->subordinate_2, des, cur + 1);
        }
    }
}

void EmployeesAtSameLevel(int level)
{
    // The level may be any integer
    struct Employee *head = CEO;
    int cur=0;
    help_EmployeesAtSameLevel(head, level, cur);
    return;
}

// You have to print the employees as you search the organization look for the examples in the pdf and the input.txt and output.txt for details
// Note: You do not have to print -1 for NULL pointers

void help(struct Employee *head)
{
    if (head == NULL)
        return;
    printf("%d ", head->emp_id);
    help(head->subordinate_1);
    help(head->subordinate_2);
    return;
}
void get_employees()
{
    help(CEO);
    return;
}

/* In the following function you have to print the immediate team of a employee - it includes their boss and their subordinates
Note: You do not have to print -1 for NULL pointers */

int Boss(int emp_id);
int max(int a, int b);

void immediate(int id, struct Employee *root)
{
    if (root == NULL)
        return;
    if (root->emp_id == id)
    {
        printf("%d ", Boss(id));
        if (root->subordinate_1)
        {
            printf("%d ", root->subordinate_1->emp_id);
        }
        if (root->subordinate_2)
        {
            printf("%d ", root->subordinate_2->emp_id);
        }
        return;
    }
    immediate(id, root->subordinate_1);
    immediate(id, root->subordinate_2);
}
void ImmediateTeam(int emp_id)
{
    immediate(emp_id, CEO);
}

int getlevel(int id, struct Employee *root, int c)
{
    if (root == NULL)
        return -10000000;
    if (root->emp_id == id)
    {
        return c;
    }
    else
    {
        return max(getlevel(id, root->subordinate_1, c + 1), getlevel(id, root->subordinate_2, c + 1));
    }
}
int Level(int emp_id)
{
    return getlevel(emp_id, CEO, 0);
}

struct Employee *common_node(struct Employee *root, int n1, int n2)
{
    if (!root)
        return NULL;
    if (root->emp_id == n1 || root->emp_id == n2)
        return root;
    struct Employee *left = common_node(root->subordinate_1, n1, n2);
    struct Employee *right = common_node(root->subordinate_2, n1, n2);
    if (left && right)
        return root;
    if (left)
        return left;
    else
        return right;
}

int solve(struct Employee *root, int a)
{
    if (!root)
        return 0;
    if (root->emp_id == a)
        return 1;
    int x = solve(root->subordinate_1, a);
    int y = solve(root->subordinate_2, a);
    if (!x && !y)
        return 0;
    else
        return x + y + 1;
}

int help_distance(struct Employee *root, int p, int q)
{
    struct Employee *A = common_node(root, p, q);
    int r = solve(A, p);
    int s = solve(A, q);
    return (r + s - 2);
}

// The following function gives the distance between employees with emp_id1 and emp_id2
struct Employee *findlca(struct Employee *root, int id1, int id2)
{
    if (root == NULL)
        return NULL;
    if (root->emp_id == id1 || root->emp_id == id2)
        return root;
    struct Employee *left = findlca(root->subordinate_1, id1, id2);
    struct Employee *right = findlca(root->subordinate_2, id1, id2);
    if (left && right)
        return root;
    else if (left)
        return left;
    else
        return right;
}
int Distance(int emp_id1, int emp_id2)
{
    // If emp_id1 and emp_id2 are equal the distance is 0, both emp_id's will exist in the organisation
    if (emp_id1 == emp_id2)
        return 0;
    struct Employee *lca = findlca(CEO, emp_id1, emp_id2);
    if (lca == NULL)
        return -1;
    return getlevel(emp_id1, lca, 0) + getlevel(emp_id2, lca, 0);
}

/* The following function takes an emp_id this will belong to a employee in the organisation and your task is to return the emp_id of its boss
Note: If the boss does not exit return -1 */

int max(int a, int b)
{
    if (a > b)
        return a;
    else
        return b;
}
int getBoss(int id, struct Employee *prev, struct Employee *root)
{
    if (root == NULL)
        return -1;
    if (root->emp_id == id)
    {
        if (prev == NULL)
            return -1;
        else
            return prev->emp_id;
    }
    return max(getBoss(id, root, root->subordinate_1), getBoss(id, root, root->subordinate_2));
}
int Boss(int emp_id)
{
    struct Employee *prev = NULL, *root = CEO;
    return getBoss(emp_id, prev, root);
}

/* The following function returns the diameter of a Organisation -
a diameter is the maximum distance between any two emp_ids in the organisation. You can use the distance function implemented above */

int maxHeight(struct Employee *root)
{
    if (root == NULL)
        return 0;
    return 1 + max(maxHeight(root->subordinate_1), maxHeight(root->subordinate_2));
}
int maxdis(struct Employee *root)
{
    if (root == NULL)
        return 0;
    int left = maxdis(root->subordinate_1);
    int right = maxdis(root->subordinate_2);
    int included = maxHeight(root->subordinate_1) + maxHeight(root->subordinate_2) + 1;
    return max(left, max(right, included));
}
int Diameter()
{
    return maxdis(CEO) - 1;
}

/* The following function takes an emp_id of a employee and returns the number of employees directly connected to it.
NULL pointers are not included */

int count(struct Employee *root)
{
    if (root == NULL)
        return 0;
    return 1 + count(root->subordinate_1) + count(root->subordinate_2);
}

int Team_size(int emp_id, struct Employee *root)
{
    if (root == NULL)
        return 0;
    if (root->emp_id == emp_id)
        return count(root);
    return max(Team_size(emp_id, root->subordinate_1), Team_size(emp_id, root->subordinate_2));
}
int TeamSize(int emp_id)
{
    return Team_size(emp_id, CEO);
}

// --------------------------------------------------- YOU CAN EDIT ABOVE THIS LINE -----------------------------------

/* The following driver code creates a organisation for you and based on the input file
it will call all the functions that are necessary, your job is to edit the functions
above the line. Their descriptions are in the pdf and in the comments in the code. */

int main(int argc, char const *argv[])
{
    CEO = create_company();
    print_company(CEO);

    int T;
    scanf("%d", &T);

    char operation_type[20];

    while (T--)
    {
        scanf("%s", operation_type);

        if (strcmp(operation_type, "level") == 0)
        {
            int x;
            scanf("%d", &x);
            int d = Level(x);
            printf("%d\n", d);
        }

        if (strcmp(operation_type, "distance") == 0)
        {
            int x, y;
            scanf("%d %d", &x, &y);
            int d = Distance(x, y);
            printf("%d\n", d);
        }

        if (strcmp(operation_type, "employees_at_same_level") == 0)
        {
            int x;
            scanf("%d", &x);
            EmployeesAtSameLevel(x);
            printf("\n");
        }

        if (strcmp(operation_type, "get_employees") == 0)
        {
            get_employees();
            printf("\n");
        }

        if (strcmp(operation_type, "boss") == 0)
        {
            int x;
            scanf("%d", &x);
            int d = Boss(x);
            printf("%d\n", d);
        }

        if (strcmp(operation_type, "diameter") == 0)
        {
            int d = Diameter();
            printf("%d\n", d);
        }

        if (strcmp(operation_type, "immediate_team") == 0)
        {
            int x;
            scanf("%d", &x);
            ImmediateTeam(x);
            printf("\n");
        }

        if (strcmp(operation_type, "team_size") == 0)
        {
            int x;
            scanf("%d", &x);
            int d = TeamSize(x);
            printf("%d\n", d);
        }
    }

    return 0;
}
