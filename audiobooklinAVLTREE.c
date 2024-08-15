#include <stdio.h>
#include <stdlib.h.>
#include <string.h>
#define SIZE 1000

typedef struct preferences
{
    char author[SIZE];
    char narrator[SIZE];
    char genres[SIZE];
} preferences;

typedef struct Duration
{
    int hours;
    int minutes;
} Duration;

typedef struct audiobooks
{
    int audiobookid;
    char title[SIZE];
    char author[SIZE];
    char narrator[SIZE];
    char genre[SIZE];
    struct Duration length;
    struct Duration watched;
    int sum_rating;
    int count_rating;
    struct audiobooks *left;
    struct audiobooks *right;
    int heightbooks;
} audiobooks;

typedef struct users
{
    int usersid;
    char name[SIZE];
    char email[SIZE];
    struct preferences prefer;
    struct audiobooks *library;
    struct users *left;
    struct users *right;
    int heightusers;
} users;

typedef struct rating
{
    float rate;
    struct rating *left;
    struct rating *right;
    struct audiobooks *ab;
} rating;

int get_abid(char title[], char author[])
{
    int k, t1, a1;
    k = 0;
    t1 = strlen(title);
    a1 = strlen(author);
    for (int i = 0; i < t1; i++)
    {
        k += (int)title[i];
    }
    for (int i = 0; i < a1; i++)
    {
        k += (int)author[i];
    }
    return k % 100000;
}

int max(int a, int b)
{
    return (a > b) ? a : b;
}

int abheight(audiobooks *root)
{
    if (root == NULL)
        return 0;
    else
        return root->heightbooks;
}

int abgetBalance(audiobooks *node)
{
    if (node == NULL)
        return 0;
    return abheight(node->left) - abheight(node->right);
}

void Visitbook(audiobooks *term)
{
    if (term != NULL)
    {
        printf("Details about audiobook\n");
        printf("Id:%d\n", term->audiobookid);
        printf("Title:%s\n", term->title);
        printf("Author:%s\n", term->author);
        printf("NArrator:%s\n", term->narrator);
        printf("Length:%d:%d\n", term->length.hours, term->length.minutes);
        printf("Time watched:%d:%d\n", term->watched.hours, term->watched.minutes);
        printf("Genre:%s\n", term->genre);
        printf("Rating:%f\n\n", term->sum_rating / (float)term->count_rating);
        printf("----------------------------------------------------------------------\n\n");
    }
}

void VisitUsers(users *term)
{

    if (term != NULL)
    {
        printf("Details about User\n");
        printf("Id:%d\n", term->usersid);
        printf("Name:%s\n", term->name);
        printf("Email:%s\n", term->email);
        printf("Prefered Author:%s\n", term->prefer.author);
        printf("Prefered Narrator:%s\n", term->prefer.narrator);
        printf("Prefered Genre:%s\n\n", term->prefer.genres);
        audiobooks *temp;
        temp = term->library;
        while (temp != NULL)
        {
            VisitUsers(term->left);
            Visitbook(temp);
            VisitUsers(term->right);
        }
        printf("----------------------------------------------------------------------\n\n");
    }
}

int usersheight(users *root)
{
    if (root == NULL)
        return 0;
    else
        return root->heightusers;
}

int usersgetBalance(users *node)
{
    if (node == NULL)
        return 0;
    return usersheight(node->left) - usersheight(node->right);
}

audiobooks *abrotate_right(audiobooks *root)
{
    audiobooks *node = root;
    if ((root == NULL) || (root->left == NULL))
        printf("ERROR.\n");
    else
    {
        node = root->left;
        root->left = node->right;
        node->right = root;
    }
    return node;
}

audiobooks *abrotate_left(audiobooks *root)
{
    audiobooks *node = root;
    if ((root == NULL) || (root->right == NULL))
        printf("ERROR.\n");
    else
    {
        node = root->right;
        root->right = node->left;
        node->left = root;
    }
    return node;
}

audiobooks *ab_newnode(int id, char title[], char author[], char narrator[], int hrs, int mins, char genres[], int rate_count, int no_of_rating)
{
    audiobooks *node = (audiobooks *)malloc(sizeof(audiobooks));
    node->audiobookid = id;
    strcpy(node->author, author);
    strcpy(node->narrator, narrator);
    strcpy(node->title, title);
    strcpy(node->genre, genres);
    node->length.hours = hrs;
    node->length.minutes = mins;
    node->watched.hours = 0;
    node->watched.minutes = 0;
    node->heightbooks = 0;
    node->right = NULL;
    node->left = NULL;
    node->count_rating = no_of_rating;
    node->sum_rating = rate_count;
    return node;
}

users *usersrotate_right(users *root)
{
    users *node = root;
    if ((root == NULL) || (root->left == NULL))
        printf("ERROR.\n");
    else
    {
        node = root->left;
        root->left = node->right;
        node->right = root;
    }
    return node;
}

users *usersrotate_left(users *root)
{
    users *node = root;
    if ((root == NULL) || (root->right == NULL))
        printf("ERROR.\n");
    else
    {
        node = root->right;
        root->right = node->left;
        node->left = root;
    }
    return node;
}

users *users_newnode(int id, char name[], char email[], char gen[], char aut[], char narr[])
{
    users *new = (users *)malloc(sizeof(users));
    new->usersid = id;
    strcpy(new->name, name);
    strcpy(new->email, email);
    strcpy(new->prefer.genres, gen);
    strcpy(new->prefer.author, aut);
    strcpy(new->prefer.narrator, narr);
    new->library = NULL;
    new->left = NULL;
    new->right = NULL;
    return new;
}

audiobooks *add_audiobooks(audiobooks *head, int id, char title[], char author[], char narrator[], int hrs, int mins, char genres[], int rate_count, int no_of_rating)
{
    if (head == NULL)
        head = ab_newnode(id, title, author, narrator, hrs, mins, genres, rate_count, no_of_rating);
    else
    {
        if (id < head->audiobookid)
            head->left = (add_audiobooks(head->left, id, title, author, narrator, hrs, mins, genres, rate_count, no_of_rating));
        else if (id > head->audiobookid)
            head->right = (add_audiobooks(head->right, id, title, author, narrator, hrs, mins, genres, rate_count, no_of_rating));
        head->heightbooks = 1 + max(abheight(head->left), abheight(head->right));
        int balance;
        if (head == NULL)
            balance = 0;
        else
            balance = abheight(head->left) - abheight(head->right);
        if (balance > 1 && id < head->left->audiobookid)
            head = abrotate_right(head);
        else if (balance > 1 && id > head->left->audiobookid)
        {
            head->left = abrotate_left(head->left);
            head = abrotate_right(head);
        }
        else if (balance < -1 && id > head->right->audiobookid)
            head = abrotate_left(head);
        else if (balance < -1 && id < head->right->audiobookid)
        {
            head->right = abrotate_right(head->right);
            head = abrotate_left(head);
        }
    }
    return head;
}

audiobooks *edit_audiobooks(audiobooks *head, int id, char title[], char author[], int hrs, int mins)
{
    audiobooks *temp = head;
    while ((temp != NULL) && (temp->audiobookid != id))
    {
        if (id < temp->audiobookid)
        {
            temp = temp->left;
        }
        else if (id > temp->audiobookid)
        {
            temp = temp->right;
        }
    }
    if (temp == NULL)
        printf("Audiobook does not exist.\n");
    else
    {
        strcpy(temp->title, title);
        strcpy(temp->author, author);
        temp->length.hours = hrs;
        temp->length.minutes = mins;
        printf("Audiobook has been updated.\n");
    }
    return head;
}

users *add_users_profiles(users *head, int id, char name[], char email[], char author[], char narrator[], char genres[])
{
    if (head == NULL)
        head = users_newnode(id, name, email, genres, author, narrator);
    else
    {
        if (id < head->usersid)
            head->left = (add_users_profiles(head->left, id, name, email, author, narrator, genres));
        else if (id > head->usersid)
            head->right = (add_users_profiles(head->right, id, name, email, author, narrator, genres));
        head->heightusers = 1 + max(usersheight(head->left), usersheight(head->right));
        int balance;
        if (head == NULL)
            balance = 0;
        else
            balance = usersheight(head->left) - usersheight(head->right);
        if (balance > 1 && id < head->left->usersid)
            head = usersrotate_right(head);
        else if (balance > 1 && id > head->left->usersid)
        {
            head->left = usersrotate_left(head->left);
            head = usersrotate_right(head);
        }
        else if (balance < -1 && id > head->right->usersid)
            head = usersrotate_left(head);
        else if (balance < -1 && id < head->right->usersid)
        {
            head->right = usersrotate_right(head->right);
            head = usersrotate_left(head);
        }
    }
    return head;
}

users *CreateUsers(users *head)
{
    FILE *fp = fopen("users.txt", "r");
    char word[SIZE];
    while (fscanf(fp, "%s", word) == 1)
    {
        int id;
        char name[SIZE], email[SIZE], author[SIZE], narrator[SIZE], genre[SIZE];
        fscanf(fp, "%d", &id);
        fscanf(fp, "%s", name);
        fscanf(fp, "%s", email);
        fscanf(fp, "%s", author);
        fscanf(fp, "%s", narrator);
        fscanf(fp, "%s", genre);
        head = add_users_profiles(head, id, name, email, author, narrator, genre);
    }
    fclose(fp);
    return head;
}

audiobooks *CreateAudio_Book(audiobooks *head)
{
    FILE *fp = fopen("audiobook.txt", "r");
    char word[SIZE];
    while (fscanf(fp, "%s", word) == 1)
    {
        int id, lmins, lhrs, wmins, whrs, rate_count, no_of_rating;
        char title[SIZE], author[SIZE], narrator[SIZE], genre[SIZE];
        fscanf(fp, "%s", title);
        fscanf(fp, "%s", author);
        fscanf(fp, "%s", narrator);
        fscanf(fp, "%d", &lhrs);
        fscanf(fp, "%d", &lmins);
        fscanf(fp, "%d", &whrs);
        fscanf(fp, "%d", &wmins);
        fscanf(fp, "%s", genre);
        fscanf(fp, "%d", &rate_count);
        fscanf(fp, "%d", &no_of_rating);
        id = get_abid(title, author);
        head = add_audiobooks(head, id, title, author, narrator, lhrs, lmins, genre, rate_count, no_of_rating);
    }
    fclose(fp);
    return head;
}

audiobooks *abdelete_audiobooks(audiobooks *root, int id)
{
    if (root != NULL)
    {
        if (id < root->audiobookid)
            root->left = abdelete_audiobooks(root->left, id);
        else if (id > root->audiobookid)
            root->right = abdelete_audiobooks(root->right, id);
        else
        {
            if ((root->left == NULL) || (root->right == NULL))
            {
                audiobooks *temp = root->left ? root->left : root->right;
                if (temp == NULL)
                {
                    temp = root;
                    root = NULL;
                }
                else
                    *root = *temp;

                free(temp);
                printf("Audiobook has been deleted.\n");
            }
            else
            {
                audiobooks *temp = root->right;
                while (temp->left != NULL)
                    temp = temp->left;
                root->audiobookid = temp->audiobookid;
                root->right = abdelete_audiobooks(root->right, temp->audiobookid);
            }
        }
        if (root != NULL)
        {
            root->heightbooks = 1 + max(abheight(root->left), abheight(root->right));
            int balance = abgetBalance(root);
            if (balance > 1 && abgetBalance(root->left) >= 0)
                root = abrotate_right(root);
            if (balance > 1 && abgetBalance(root->left) < 0)
            {
                root->left = abrotate_left(root->left);
                root = abrotate_right(root);
            }
            if (balance < -1 && abgetBalance(root->right) <= 0)
                root = abrotate_left(root);
            if (balance < -1 && abgetBalance(root->right) > 0)
            {
                root->right = abrotate_right(root->right);
                root = abrotate_left(root);
            }
        }
    }
    return root;
}

users *add_users_library(users *head, int id)
{
    users *temp = head;
    while ((temp != NULL) && (temp->usersid != id))
    {
        if (id < temp->usersid)
        {
            temp = temp->left;
        }
        else if (id > temp->usersid)
        {
            temp = temp->right;
        }
    }
    if (temp == NULL)
        printf("User does not exist.\n");
    else
    {
        int i;
        printf("Enter 1 to add an audiobook.\nEnter 2 to remove an audiobook.\n");
        scanf("%d", &i);
        if (i == 1)
        {
            int iid, mins, hrs;
            char title[SIZE], author[SIZE], narrator[SIZE], genres[SIZE];
            printf("Enter title of book:");
            scanf("%s", title);
            printf("Enter author of book:");
            scanf("%s", author);
            printf("Enter narrator of book:");
            scanf("%s", narrator);
            printf("Enter genre of book:");
            scanf("%s", genres);
            printf("Enter length of book in minutes:");
            scanf("%d", &mins);
            iid = get_abid(title, author);
            hrs = mins / 60;
            mins = mins % 60;
            temp->library = add_audiobooks(temp->library, iid, title, author, narrator, hrs, mins, genres, 0, 0);
            printf("User library has been updated.\n");
        }
        else if (i == 2)
        {
            char title[SIZE], author[SIZE];
            int id;
            printf("Enter title of book:");
            scanf("%s", title);
            printf("Enter author of book:");
            scanf("%s", author);
            id = get_abid(title, author);
            temp->library = abdelete_audiobooks(temp->library, id);
            printf("User library has been updated.\n");
        }
        else
            printf("Invalid input.\n");
    }
    return head;
}

void search_audiobooks(audiobooks *head, char criteria[])
{
    if (head != NULL)
    {
        if (strcmp(head->title, criteria) == 0 || strcmp(head->author, criteria) == 0 || strcmp(head->genre, criteria) == 0)
        {
            Visitbook(head);
        }
        else
        {
            search_audiobooks(head->left, criteria);
            search_audiobooks(head->right, criteria);
        }
    }
}

void display_user_library(users *head, int id, char criteria[])
{
    users *temp = head;
    while ((temp != NULL) && (temp->usersid != id))
    {
        if (id < temp->usersid)
        {
            temp = temp->left;
        }
        else if (id > temp->usersid)
        {
            temp = temp->right;
        }
    }
    if (temp == NULL)
        printf("User does not exist.\n");
    else
    {
        audiobooks *curr = temp->library;
        search_audiobooks(curr, criteria);
    }
}

users *listening_progress(users *head, int id, int aid, int hr, int min)
{
    users *temp = head;
    while ((temp != NULL) && (temp->usersid != id))
    {
        if (id < temp->usersid)
        {
            temp = temp->left;
        }
        else if (id > temp->usersid)
        {
            temp = temp->right;
        }
    }
    audiobooks *curr = temp->library;
    while ((curr != NULL) && (curr->audiobookid != aid))
    {
        if (aid < curr->audiobookid)
        {
            curr = curr->left;
        }
        else if (aid > curr->audiobookid)
        {
            curr = curr->right;
        }
    }
    if (curr != NULL)
    {
        Duration T = curr->length;
        T.hours = hr;
        T.minutes = min;
        curr->length = T;
    }
    printf("Listening time is updated.\n");
    return head;
}

void audiobooks_listened(audiobooks *head)
{
    if (head != NULL)
    {
        audiobooks_listened(head->left);
        if (head->watched.hours != 0 && head->watched.minutes != 0)
        {
            Visitbook(head);
        }
        audiobooks_listened(head->right);
    }
}

void listening_history_report_user(users *head, int id)
{
    users *temp = head;
    while ((temp != NULL) && (temp->usersid != id))
    {
        if (id < temp->usersid)
        {
            temp = temp->left;
        }
        else if (id > temp->usersid)
        {
            temp = temp->right;
        }
    }
    if (temp == NULL)
        printf("User does not exist.\n");
    else
    {
        audiobooks_listened(temp->library);
    }
}

audiobooks *rate_audiobooks(users *head, audiobooks *start, int userid, int aid, int rate)
{
    users *temp = head;
    while ((temp != NULL) && (temp->usersid != userid))
    {
        if (userid < temp->usersid)
        {
            temp = temp->left;
        }
        else if (userid > temp->usersid)
        {
            temp = temp->right;
        }
    }

    if (temp != NULL)
    {
        audiobooks *curr = start;
        while ((curr != NULL) && (curr->audiobookid != aid))
        {
            if (aid < curr->audiobookid)
            {
                curr = curr->left;
            }
            else if (aid > curr->audiobookid)
            {
                curr = curr->right;
            }
        }
        if (curr != NULL && curr->audiobookid == aid)
        {
            curr->sum_rating += rate;
            curr->count_rating++;
            printf("audiobook has been rated \n");
        }
    }

    return start;
}

void listening_history_report_audiobook(int aid, audiobooks *head)
{
    audiobooks *curr = head;
    while ((curr != NULL) && (curr->audiobookid != aid))
    {
        if (aid < curr->audiobookid)
        {
            curr = curr->left;
        }
        else if (aid > curr->audiobookid)
        {
            curr = curr->right;
        }
    }
    if (curr == NULL)
        printf("Audiobook does not exist.\n");
    else
    {
        printf("audiobookID is %d.\n", curr->audiobookid);
        printf("title is %s.\n", curr->title);
        Duration temp;
        temp = curr->watched;
        printf("total listening progress is %d hours and %d minutes.\n", temp.hours, temp.minutes);
    }
}

void Range_search_audiobook(int id1, int id2, audiobooks *head)
{
    audiobooks *temp = head;
    if (temp != NULL)
    {

        if (temp->audiobookid >= id1 && temp->audiobookid <= id2)
        {
            printf("title : %s \n", temp->title);
            Range_search_audiobook(id1, id2, temp->left);
            Range_search_audiobook(id1, id2, temp->right);
        }
        else if (temp->audiobookid > id2)
        {
            Range_search_audiobook(id1, id2, temp->left);
        }
        else if (temp->audiobookid < id1)
        {
            Range_search_audiobook(id1, id2, temp->right);
        }
    }
}

rating *insert(rating *head, rating *node)
{
    rating *ptr = head;
    if (head == NULL)
    {
        head = node;
    }
    else
    {
        while (ptr != NULL)
        {
            if (ptr->rate > head->rate)
            {
                if (ptr->left != NULL)
                    ptr = ptr->left;
                else
                {
                    ptr->left = node;
                    break;
                }
            }
            else if (ptr->rate < head->rate)
            {
                if (ptr->right != NULL)
                    ptr = ptr->right;
                else
                {
                    ptr->right = node;
                    break;
                }
            }
            else
            {
                printf("Duplicate,\n");
                break;
            }
        }
    }
    return head;
}

rating *create_rating(audiobooks *head, rating *root)
{
    if (head != NULL)
    {
        root = create_rating(head->right, root);
        rating *new_node = (rating *)malloc(sizeof(rating));
        float t = (float)head->count_rating;
        float rate = (float)head->sum_rating / t;
        new_node->rate = rate;
        new_node->ab = head;
        new_node->left = NULL;
        new_node->right = NULL;
        root = insert(root, new_node);
        root = create_rating(head->left, root);
    }
    return root;
}

void Most_popular_audiobook(rating *head)
{
    if (head != NULL)
    {
        while(head->left!=NULL) head=head->left;
        Visitbook(head->ab);
    }
}

void Display_audiobooks(audiobooks *head)
{
    if (head != NULL)
    {
        Display_audiobooks(head->left);
        Visitbook(head);
        Display_audiobooks(head->right);
    }
}

void Display_users(users *head)
{
    if (head != NULL)
    {
        Display_users(head->left);
        VisitUsers(head);
        Display_users(head->right);
    }
}

int main()
{

    int n;
    audiobooks *head;
    head = NULL;
    head = CreateAudio_Book(head);
    users *start;
    start = NULL;
    start = CreateUsers(start);
    rating *root;
    root = NULL;
    Display_audiobooks(head);
    Display_users(start);
    printf("To add audiobook type 1 \n");
    printf("To edit audiobook type 2 \n");
    printf("To delete audiobook type 3 \n");
    printf("To add user profile type 4 \n");
    printf("To add user library type 5 \n");
    printf("To search audiobook type 6 \n");
    printf("To get listening progress type 7 \n");
    printf("To display user libraries type 8 \n");
    printf("To rate an audiobook type 9 \n");
    printf("To know the most popular audiobook type 10 \n");
    printf("To get listening history of user type 11 \n");
    printf("To get listening history of audiobook type 12 \n");
    printf("To diplay audiobook list according to ratings type 13 \n");
    printf("To print audiobooks list type 14 \n");
    printf("To print users list type 15 \n");

    int id, mins, hrs, uid, rate, flag, uid1, uid2, i;
    char title[SIZE], author[SIZE], narrator[SIZE], genres[SIZE];
    char term[SIZE], email[SIZE], name[SIZE], criteria[SIZE];

    while (n != -1)
    {
        printf("enter the required value of function: \n");
        scanf("%d", &n);

        switch (n)
        {
        case 1:
            printf("Enter title of book: \n");
            scanf("%s", title);
            printf("Enter author of book: \n");
            scanf("%s", author);
            printf("Enter narrator of book: \n");
            scanf("%s", narrator);
            printf("Enter genre of book: \n");
            scanf("%s", genres);
            printf("Enter length of book in minutes: \n");
            scanf("%d", &mins);
            hrs = mins / 60;
            mins = mins % 60;
            id = get_abid(title, author);
            head = add_audiobooks(head, id, title, author, narrator, hrs, mins, genres, 0, 0);
            break;
        case 2:
            printf("Enter audiobooks id: \n");
            scanf("%d", &id);
            printf("Enter new title of book: \n");
            scanf("%s", title);
            printf("Enter new author of book: \n");
            scanf("%s", author);
            printf("Enter new length of book in minutes: \n");
            scanf("%d", &mins);
            hrs = mins / 60;
            mins = mins % 60;
            head = edit_audiobooks(head, id, title, author, hrs, mins);
            break;
        case 3:
            printf("Enter title of book: \n");
            scanf("%s", title);
            printf("Enter author of book: \n");
            scanf("%s", author);
            id = get_abid(title, author);
            head = abdelete_audiobooks(head, id);
            break;
        case 4:
            printf("enter user ID: \n");
            scanf("%d", &uid);
            printf("enter user email: \n");
            scanf("%s", email);
            printf("Enter name of user: \n");
            scanf("%s", name);
            printf("Enter author of book: \n");
            scanf("%s", author);
            printf("Enter narrator of book: \n");
            scanf("%s", narrator);
            printf("Enter genre of book: \n");
            scanf("%s", genres);
            start = add_users_profiles(start, uid, name, email, author, narrator, genres);
            break;
        case 5:
            printf("enter user ID: \n");
            scanf("%d", &uid);
            start = add_users_library(start, uid);
            break;
        case 6:
            printf("Enter title or author or genre of book: \n");
            scanf("%s", term);
            search_audiobooks(head, term);
            break;
        case 7:
            printf("enter user ID: \n");
            scanf("%d", &uid);
            printf("Enter audiobooks id: \n");
            scanf("%d", &id);
            printf("Enter new listening time of book in minutes: \n");
            scanf("%d", &mins);
            hrs = mins / 60;
            mins = mins % 60;
            start = listening_progress(start, uid, id, hrs, mins);
            break;
        case 8:
            printf("enter user ID: \n");
            scanf("%d", &uid);
            printf("if you want to filter with respect to genre type 1 \n");
            printf("if you want to filter with respect to author type 2 \n");
            printf("if you want to filter with respect to narrator type 3 \n");
            scanf("%d", &i);
            if (i == 1)
            {
                printf("Enter genre of book: \n");
                scanf("%s", criteria);
            }
            else if (i == 2)
            {
                printf("Enter author of book: \n");
                scanf("%s", criteria);
            }
            else if (i == 3)
            {
                printf("Enter narrator of book: \n");
                scanf("%s", criteria);
            }
            else
                printf("Wrong input.\n");
            display_user_library(start, uid, criteria);
            break;
        case 9:
            printf("enter user ID: \n");
            scanf("%d", &uid);
            printf("Enter audiobooks id: \n");
            scanf("%d", &id);
            printf("Enter rating (it should be between 1 to 5): \n");
            scanf("%d", &rate);
            head = rate_audiobooks(start, head, uid, id, rate);
            break;
        case 10:
            root = create_rating(head, root);
            Most_popular_audiobook(root);
            break;
        case 11:
            printf("Enter user id:\n");
            scanf("%d", &uid);
            listening_history_report_user(start, uid);
            break;
        case 12:
            printf("Enter audiobooks id: \n");
            scanf("%d", &id);
            listening_history_report_audiobook(id, head);
            break;
        case 13:
            printf("Enter lower range of id:");
            scanf("%d", &uid1);
            printf("Enter higher range of id:");
            scanf("%d", &uid2);
            if (uid1 >= uid2)
                printf("Wrong range input.\n");
            else
                Range_search_audiobook(uid1, uid2, head);
            break;
        case 14:
            Display_audiobooks(head);
            break;
        case 15:
            Display_users(start);
            break;
        default:
            printf("The program has terminated. ");
            n = -1;
        }
    }
}