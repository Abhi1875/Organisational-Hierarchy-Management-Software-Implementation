#include<stdio.h>
#include<stdlib.h>
#include <string.h>

// The following is a employee in the organisation, it has the pointer to two more employees a subordinate_1 and a subordinate_2
struct Employee {
    int emp_id; // emp_ids will be unique
    char* emp_name;
    int emp_salary;
    struct Employee* subordinate_1;
    struct Employee* subordinate_2;
};

// The following function creates a employee and returns its pointer
struct Employee* create_employee(int id, char* name, int salary) {
    struct Employee* ptr = (struct Employee*) malloc(sizeof(struct Employee));
    ptr->emp_id = id;
    ptr->emp_salary = salary;
    ptr->emp_name = strdup(name);
    // strdup() creates a copy of the string or char pointer and stores it in the new char pointer of the employee
    ptr->subordinate_1 = NULL;
    ptr->subordinate_2 = NULL;
    return ptr;
}

// The following code creates a organisation from scanning the input file
struct Employee* create_company() {
    int id, salary;
    char name[100];
    scanf("%d", &id);
    if(id == -1) return NULL; // -1 is used when there is a NULL pointer ie when no value is present

    scanf("%s %d", name, &salary);
    struct Employee* par = create_employee(id, name, salary);

    par->subordinate_1 = create_company();
    par->subordinate_2 = create_company();
    
    return par;
}

// The following function 
void print_company_helper(struct Employee* ceo) {
    // take input
    if(!ceo) {
        printf("%d ", -1);
        return;
    }
    printf("%d %s %d ", ceo->emp_id, ceo->emp_name, ceo->emp_salary);
    print_company_helper(ceo->subordinate_1);
    print_company_helper(ceo->subordinate_2);
    return;
}

void print_company(struct Employee* ceo) {
    print_company_helper(ceo);
    printf("\n");
    return;
} 

// --------------------------------------------------- FUNCTIONS ----------------------------------------------------------

struct Employee* CEO = NULL;
// CEO is a global pointer that points to the CEO of the company

// The below function returns the employee id of the first common boss
int max(int a, int b)
{
    if (a > b)
        return a;
    else
        return b;
}
struct Employee* findlca(struct Employee* root, int id1, int id2){
    if(root==NULL)return NULL;
    if(root->emp_id==id1||root->emp_id==id2)return root;
    struct Employee* left=findlca(root->subordinate_1,id1,id2);
    struct Employee* right=findlca(root->subordinate_2,id1,id2);
    if(left&&right)return root;
    else if(left)return left;
    else return right;
}
int get_first_common_boss(int emp_id1,int emp_id2){
    struct Employee* lca=findlca(CEO,emp_id1,emp_id2);
    if (lca) return lca->emp_id;
    else return -1;
}
//done
//Print the employees with the same last name sperated by a space in the order of their level
int maxLevel(struct Employee* root){
    if(root==NULL)return 0;
    return 1+max(maxLevel(root->subordinate_1),maxLevel(root->subordinate_2));
}
void getnames(struct Employee* root, int level, char* str){
    if(root==NULL)return;
    if(level==0&&strcmp(root->emp_name,str)==0){
        printf("%d ",root->emp_id);
    }
    getnames(root->subordinate_1,level-1,str);
    getnames(root->subordinate_2,level-1,str);
}
void same_last_names(int emp_id){
    struct Employee* cur=findlca(CEO,emp_id,emp_id);
    if(cur==NULL)return;
    char* str=strdup(cur->emp_name);
    int maxl=maxLevel(CEO);
    for(int i=0;i<=maxl;i++){
        getnames(CEO,i,str);
    }
    return;
}

// Print the bosses of the given employee in the order from CEO to immediate boss
void getallboss(struct Employee* head, struct Employee* cur){
    if(cur==NULL||head==NULL||head==cur||head->subordinate_1==NULL||head->subordinate_2==NULL)return;
    struct Employee* temp=findlca(head,head->subordinate_1->emp_id,cur->emp_id);
    printf("%d ",head->emp_id);
    if(temp==head){
        getallboss(head->subordinate_2,cur);
    }
    else{
        getallboss(head->subordinate_1,cur);
    }
}
void get_all_bosses(int emp_id){
    struct Employee* cur=findlca(CEO,emp_id,emp_id);
    if(CEO==cur||cur==NULL){
        printf("-1 ");
        return;
    }
    getallboss(CEO,cur);
    return;
}

// Return the average salary of the team with the given employee as head
int count(struct Employee *root)
{
    if (root == NULL)
        return 0;
    return 1 + count(root->subordinate_1) + count(root->subordinate_2);
}
int getsalary(struct Employee* root){
    if(root==NULL)return 0;
    return root->emp_salary+getsalary(root->subordinate_1)+getsalary(root->subordinate_2);
}
double get_average_salary(int emp_id){
    struct Employee* cur=findlca(CEO,emp_id,emp_id);
    int c=count(cur);
    return ((double)getsalary(cur))/c;;
}

// --------------------------------------------------- MAIN FUNCTION -------------------------------------------------------

/* The following code creates a organisation and based on the input file it will call all the functions that are necessary */

int main(int argc, char const *argv[])
{
    CEO = create_company();
    print_company(CEO);

    int T; 
    scanf("%d", &T);

    char operation_type[100];

    while(T--) {
        scanf("%s", operation_type);

        if(strcmp(operation_type, "get_first_common_boss") == 0) {
            int x,y;
            scanf("%d %d", &x, &y);
            int ans = get_first_common_boss(x,y);
            printf("%d\n", ans);
        } 
        else if(strcmp(operation_type, "same_last_names") == 0) {
            int x;
            scanf("%d", &x);
            same_last_names(x);
            printf("\n");
        } 
        else if(strcmp(operation_type, "get_all_bosses") == 0) {
            int x;
            scanf("%d", &x);
            get_all_bosses(x);
            printf("\n");
        } 
        else if(strcmp(operation_type, "get_average_salary") == 0) {
            int x;
            scanf("%d", &x);
            double ans = get_average_salary(x);
            printf("%.2f\n", ans);
        } 

    }

    return 0;
}