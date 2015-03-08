#include<stdio.h>
#include<stdlib.h>
#define MAXLEVEL 10

/***************************************************************************************
 *  From the pseudo code of search algorithm it can be deduced that list is a structure*
 *  which contains header and the current max level indicated by var level.            *
 *  Header is a pointer to a structure that contains,                                  *
 *  dynamically allocated array of pointers to pointers called forward.                *
 *  which intern points to a self referential structure called node, containing        *
 *  the forward pointer itself and the information stored in a variable data.          *
 ***************************************************************************************
 */


int pointer_binary_insert = 0;
int pointer_binary_delete = 0;
int pointer_binary_search = 0;

int comparison_binary_insert = 0;
int comparison_binary_delete = 0;
int comparison_binary_search = 0;

int pointer_skip_insert = 0;
int pointer_skip_delete = 0;
int pointer_skip_search = 0;

int comparison_skip_insert = 0;
int comparison_skip_delete = 0;
int comparison_skip_search = 0;

typedef struct node
{
   int data;
   struct node** forward; // its a pointer pointing to another pointer
}node;

typedef struct SL
{
    node* header;
    int level;
}SL;


/************************************************
 *Initialization block which allocates          *
 *memory for header and forward pointers which  *
 *point to the header                           *
 ************************************************
 */

SL *initialize(SL* list)
{
    list->level = 1;
    int i;
    node *head = (node*)malloc(sizeof(struct node));  //allocating memory for header..
    list->header = head;
    head->forward = (node**)malloc(sizeof(node*)*(MAXLEVEL+1)); // allocating forward pointers
    for(i=0;i<=MAXLEVEL;i++)
    {
        list->header->forward[i] = list->header;// all forward pointers of the header are pointing to NIL
    }
    return list;
}

int search_list(SL* list, int key)
{
    int i;
    node* temp;
    temp = list->header;
    for(i=list->level;i>=1;i--)
    {
        while((temp->forward[i]->data)<key)
        {
            temp = temp->forward[i];
            pointer_skip_search++;
            comparison_skip_search++;
        }
    }
    if(temp->forward[1]->data==key)
    {
        comparison_skip_search++;
        return 1;
    }

    return 0;
}
int randomlevel()
{
   float p = 5;
   int num_level=1;


   while(rand()%10>p&&num_level<MAXLEVEL)
    {
        num_level++;
    }
    return num_level;
}

int insert(SL* list, int val)
{
    node* NIL = list->header;
    int level_value;
    node* update[MAXLEVEL];
    node* temp;
    temp = list->header;
    int i;
    for(i=list->level;i>=1;i--)
    {
        while((temp->forward[i]->data)<val)
           {
            pointer_skip_insert++;
            comparison_skip_insert++;
            temp = temp->forward[i];
           }
        update[i] = temp; //holds the pointers of all the values lesser than
    }
    temp = temp->forward[1];
    pointer_skip_insert++;
    if(temp->data == val) // The value is already present
        {
            comparison_skip_insert++;
            return 1;
        }

    else
    {
        level_value = randomlevel();
        if(level_value>list->level)
        {
            comparison_skip_insert++;
            for(i=list->level+1;i<=level_value;i++)
            {
                update[i]= NIL; // Assigning NIL values for the new pointers
            }
            list->level = level_value;
        }
 //creating new node to insert the data
temp = (node*)malloc(sizeof(node));
temp->data = val;
//allocating memory for forward pointers..
temp->forward = (node**)malloc(sizeof(node*)*(level_value+1));
for(i=1;i<=level_value;i++)
{
    temp->forward[i] = update[i]->forward[i];
    update[i]->forward[i] = temp;
    pointer_skip_insert++;
}
return 0;

}
}

int delete_node(SL* list,int key)
{

    node* NIL = list->header;
    node* update[MAXLEVEL];
    node* temp;
    temp = list->header;
    int i;
    for(i=list->level;i>=1;i--)
    {

        while(temp->forward[i]->data<key)
        {
            comparison_skip_delete++;
            pointer_skip_delete++;
            temp = temp->forward[i];
        }
        update[i] = temp;
    }
    temp = temp->forward[1];
    pointer_skip_delete++;
    if(temp->data!=key)
    {
        comparison_skip_delete++;
        return 1;
    } //element is not found


    /*correcting the forward pointers*/

    for(i=1;i<list->level;i++)
    {
        if(update[i]->forward[i]!=temp)
        {
            comparison_skip_delete++;
            break;
        }
        update[i]->forward[i] = temp->forward[i];
        pointer_skip_delete++;
    }
/*
    while(temp)
    {
        free(temp->forward);
        free(temp);
    }*/

    //Checking if the level needs to be decreased..
    while((list->level>1)&&(list->header->forward[list->level]==NIL))
        {
            comparison_skip_delete++;
            list->level--;
        }


    return 0; // successfully deleted..

}

void display(SL* list)
{
    node *x = list->header;
            while (x && x->forward[1] != list->header)
            {
                printf("%d,",x->forward[1]->data);
                x = x->forward[1];
            }
            printf("NIL\n");
}
struct BT
{
    int value;
    struct BT *left;
    struct BT * right;
};

struct BT *newNode(int item)
{
    struct BT *temp =  (struct BT *)malloc(sizeof(struct BT));
    temp->value = item;
    temp->left = temp->right = NULL;
    return temp;
}

struct BT* insert_BT(int ele,struct BT* root)
{
    if((root)==NULL)
    {
        comparison_binary_insert++;
        return newNode(ele);
    }
    else if(ele<(root)->value)
    {
        comparison_binary_insert++;
        pointer_binary_insert++;
       root->left = insert_BT(ele,(root)->left);
    }
    else if(root->value == ele)
    {
        comparison_binary_insert++;
        //printf("%d was not inserted",ele);

    }
    else
    {
        comparison_binary_insert++;
        pointer_binary_insert++;
        root->right = insert_BT(ele,(root)->right);

    }
      return root;
}

struct BT * FindMin(struct BT* root)
{
    struct BT * current = root;
    while((current)->left != NULL)
    {
        comparison_binary_delete++;
        pointer_binary_delete++;
        (current) = (current)->left;
    }


    return current;
}

struct BT* Delete(struct BT* root,int data)
{

   if(root == NULL)
   {
       comparison_binary_delete++;
       return root;
   }

   else if(data < (root)->value)
       {
        comparison_binary_delete++;
        pointer_binary_delete++;
        (root)->left = Delete((root->left),data);
   }
   else if(data > (root)->value)
   {

        comparison_binary_delete++;
        pointer_binary_delete++;
        (root)->right = Delete(((root)->right),data);

   }
   else //root is found
   {
        //case 1...
    if((root)->left == NULL && (root)->right == NULL)
    {
        comparison_binary_delete += 2;

        free(root);
        root = NULL;
    }
      //case 2 one child
    else if((root)->left == NULL)
    {
        comparison_binary_delete++;
        pointer_binary_delete++;
        struct BT * temp = root;
        (root) = (root)->right;
        free(temp);
    }
    else if((root)->right == NULL)
    {
        comparison_binary_delete++;
        pointer_binary_delete++;
        struct BT *temp = root;
        (root) = (root)->left;
        free(temp);
    }
        //case 3: 2 child
    else
    {
       struct BT  *temp;
       temp = FindMin((root)->right);
       (root)->value = (temp)->value;
       pointer_binary_delete++;
       (root)->right = Delete(((root)->right),(temp)->value);

    }
   }
  return root;
}

void Search(struct BT* ptr,int num)
{
    if(ptr==NULL)
      {
          comparison_binary_search++;
          //printf("Element not found\n");
          return;
      }

    if(ptr->value==num)
    {
        comparison_binary_search++;
        //printf("Element Found\n");
    }
    else if(num<(ptr->value))
        {
            comparison_binary_search++;
            pointer_binary_search++;
            Search(ptr->left,num);
        }
    else if(num>ptr->value)
    {
        comparison_binary_search++;
            pointer_binary_search++;
        Search(ptr->right,num);
    }
    return;
}

void display_BT(struct BT* root)
{
    if(root!=NULL)
    {
        display_BT(root->left);
        printf("%d,",root->value);
        display_BT(root->right);
    }
}

void create(int *a)
{
    int o;
    int t;
    int th;

    int i;
    for(i=0;i<1000;i++)
    {
        a[i]= (rand()%3+1);
        if(a[i]==1)
        {
            o++;

        }
        else if(a[i]==2)
        {
            t++;
        }
        else if(a[i]==3)
        {
            th++;
        }
    }
    printf("Inserts = %d \nDelete = %d \nSearch = %d\n",o,t,th);
}


int main()
{
    int randomarray[1000];
    create(&randomarray);
    FILE *myFile;
    int numbers[1000]={0},i=0;
    if ((myFile = fopen("randomnumbers.txt", "r"))==NULL)
    {
    printf("failed to open\n");
    return 1;
    }
    else
    while((fscanf(myFile,"%d",&numbers[i]))!=EOF) //scanf and check EOF
    i++;

   int num;
   SL list;
   int ele;
   initialize(&list);
   struct BT *root = NULL;
   for(i=0;i<1000;i++)
   {
      insert(&list,numbers[i]);

   }
   for(i=0;i<1000;i++)
   {
       root = insert_BT(numbers[i],root);
   }


  for(i=0;i<1000;i++)
  {

        switch(randomarray[i])
        {
        case 1:
            if(insert(&list,(numbers[i]+1000)))
                {
                //    printf("Element already present\n");
                }

            else
            {
               //  printf("Element was inserted\n");
            }

            break;

        case 2:
            if(delete_node(&list,numbers[i]))
              {
                  //  printf("Element not present\n");
              }
            else
               //  printf("Element found and deleted\n");
            break;
        case 3:
            if(search_list(&list,numbers[i]))
               {
                 //  printf("Element found\n");
               }
            else
            {
                // printf("Element not present\n");
            }

            break;


       }
 }


for(i=0;i<1000;i++)
{
        switch(randomarray[i])
        {
        case 1:
            root = insert_BT((numbers[i]+1000),root);
            break;
        case 2:
            Delete(root,numbers[i]);
            break;

        case 3:
            Search(root,numbers[i]);
            break;
        }

}
display(&list);
display_BT(root);



printf("\nThe number of pointer traversals for binary tree insert are %d \n",pointer_binary_insert);
printf("The number of pointer traversals for binary tree Delete are %d \n",pointer_binary_delete);
printf("The number of pointer traversals for binary tree search are %d \n",pointer_binary_search);
printf("The number of pointer traversals for skip list insert are %d \n",pointer_skip_insert);
printf("The number of pointer traversals for skip list Delete are %d \n",pointer_skip_delete);
printf("The number of pointer traversals for skip list search are %d \n",pointer_skip_search);
printf("The number of comparisons for binary tree insert are %d \n",comparison_binary_insert);
printf("The number of comparisons for binary tree Delete are %d \n",comparison_binary_delete);
printf("The number of comparisons for binary tree search are %d  \n",comparison_binary_search);
printf("The number of comparisons for skip list insert are %d \n",comparison_skip_insert);
printf("The number of comparisons for skip list Delete are %d \n",comparison_skip_delete);
printf("The number of comparisons for skip list search are %d \n",comparison_skip_search);


   return 0;
}

