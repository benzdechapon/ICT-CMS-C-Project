#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <curses.h>

//Function Prototype
void update_data();
void update_file();
void id_arrange();
WINDOW *banner_win(int x, int y, int width);
void select_menuH(WINDOW *menu_win, int hilight, int width, int height, char *choices[], int n);
void print_description(WINDOW *bottom_win, int width,char des[]);
void win_delete(WINDOW *win);
void menu_Main();
void login_page();
void student_menu(int user_index);
void admin_menu(int user_index);
void show_all(int role, int user_index);
void searchinfo(int role, int user_index);
void delete_info(int user_index);
void edit_info(int role, int user_index);
void clean_er(WINDOW *win, int startx,int line, int width);
void user_register();
int idCheck(WINDOW *win, int width, int y,char st[]);
int nameCheck(WINDOW *win, int width, int y, char st[]);
int phoneCheck(WINDOW *win, int width, int y, char st[]);
int mailCheck(WINDOW *win, int width, int y, char st[]);
int passCheck(WINDOW *win, int width, int y, char st[],char st2[]);

//Global Variables
struct userInfo {
    char id[8];
    char firstName[31];
    char lastName[31];
    char phoneNumber[11];
    char email[51];
    char password[16];
};
struct userInfo user_all[50];
int total_user=0;
char file_name[] = "userInfo.txt";

//Main
int main()
{
    menu_Main();
    return 0;
}


//Functions
void update_data()
{
    FILE *freader;
    freader = fopen(file_name, "a+");
    int i=0;
    while (fscanf(freader,"%7s %30s %30s %10s %50s %15s", user_all[i].id, user_all[i].firstName, user_all[i].lastName,
        user_all[i].phoneNumber, user_all[i].email, user_all[i].password) != EOF)
    {
        i++;
    }

    total_user = i;
    fclose(freader);
}

void update_file()
{
    FILE *fwriter;
    int i;
    fwriter = fopen(file_name, "w");
    for (i = 0; i < total_user; i++)
    {
        fprintf(fwriter,"%s %s %s %s %s %s\n", user_all[i].id, user_all[i].firstName, user_all[i].lastName,
            user_all[i].phoneNumber, user_all[i].email, user_all[i].password);
    }
    fclose(fwriter);
}

void id_arrange()
{
    //Bubblesort
    int i, j;
    struct userInfo temp;

    for (i = 0; i < total_user-1; i++)
    {
        for (j = 0; j < total_user-i-1; j++)
        {
            if (strcmp(user_all[j].id,user_all[j+1].id) > 0)
            {
                temp = user_all[j];
                user_all[j] = user_all[j+1];
                user_all[j+1] = temp;
            }
        }
    }
    update_file();
}

WINDOW *banner_win(int x, int y, int width)
{
    WINDOW *localtop_menu;
    int i;
    char toptext[] = "ICT Contact Management Systems";
    int posi = (width-strlen(toptext))/2;
    localtop_menu = newwin(3, width, y, x);
    //wprintw(localtop_menu,"%d", posi);
    for(i = 0; i < width; i++) wprintw(localtop_menu,"=");
    mvwprintw(localtop_menu,1,posi, "%s\n", toptext);
    for(i = 0; i < width; i++) wprintw(localtop_menu,"=");
    wrefresh(localtop_menu);
    return localtop_menu;

}

void select_menuH(WINDOW *menu_win, int hilight, int width, int height, char *choices[], int n)
{
    int choiceH, choiceA, i;
    wclear(menu_win);
    choiceH = (2*n) -1;
    choiceA = (height - choiceH)/2;
    for (i = 0; i < n ; i++)
    {
        if (hilight == i+1)
        {
            wattron(menu_win,A_REVERSE);
            mvwprintw(menu_win, choiceA, (width-strlen(choices[i]))/2, "%s", choices[i]);
            wattroff(menu_win,A_REVERSE);
            choiceA += 2;
        }
        else {
            mvwprintw(menu_win, choiceA, (width-strlen(choices[i]))/2, "%s", choices[i]);
            choiceA += 2;
        }

    }
    wrefresh(menu_win);

}

void print_description(WINDOW *bottom_win, int width,char des[])
{
    int i;
    wclear(bottom_win);
    for(i = 0; i < width; i++) wprintw(bottom_win,"-");
    wprintw(bottom_win, "%s", des);
    wrefresh(bottom_win);
}

void win_delete(WINDOW *win)
{
    wclear(win);
    wrefresh(win);
    delwin(win);
}

void menu_Main()
{
    WINDOW *menu_top, *menu_choice, *menu_bottom;
    int i, loop_con=1, hilight = 1, window_width, choice_height, choice;
    int wch;
    char *menuM_choices[] = {
        "User Registration",
        "Login",
        "Exit"
    };
    char *menuM_des[] = {
        "Register to ICT Contact Management Systems.",
        "Login to ICT Contact Management Systems.",
        "Exit the program."
    };
    int n_choices = sizeof(menuM_choices) / sizeof(menuM_choices[0]);
    update_data();
    id_arrange();
    //Star Curses Mode
	initscr();
    noecho();
    cbreak();
    curs_set(0);
    window_width = COLS-2;
    choice_height = LINES - 8;
    menu_top = banner_win(1,1,window_width);
    menu_choice = newwin(choice_height, window_width,4,1);
    menu_bottom = newwin(3,window_width, LINES-4,1);
    keypad(menu_choice, TRUE);
    select_menuH(menu_choice, hilight, window_width, choice_height, menuM_choices, n_choices);
    print_description(menu_bottom, window_width, menuM_des[hilight-1]);
    while (loop_con)
    {
        wch = wgetch(menu_choice);
        switch (wch) {
            case KEY_UP:
                if (hilight == 1) hilight = n_choices;
                else hilight-=1;
                break;
            case KEY_DOWN:
                if (hilight == 3) hilight = 1;
                else hilight+=1;
                break;
            case 10:
                choice = hilight;
                loop_con = 0;
                break;
            }
        select_menuH(menu_choice, hilight, window_width, choice_height, menuM_choices, n_choices);
        print_description(menu_bottom, window_width, menuM_des[hilight-1]);
    }
    switch (choice)
    {
        case 1:
            win_delete(menu_top);
            win_delete(menu_choice);
            win_delete(menu_bottom);
            curs_set(1);
            endwin();
            user_register();
            break;
        case 2:
            win_delete(menu_top);
            win_delete(menu_choice);
            win_delete(menu_bottom);
            curs_set(1);
            endwin();
            login_page();
            break;
        case 3:
            win_delete(menu_top);
            win_delete(menu_choice);
            win_delete(menu_bottom);
            curs_set(1);
            char bye[] = "Good Bye!! :D";
            mvprintw(LINES/2, (COLS - strlen(bye))/2, "%s", bye);
            getch();
            endwin();
            break;
    }
}

void login_page()
{
    WINDOW *login_top, *login_main;
    int loop_con = 1, choice, window_width,login_height,foundid=0,foundpass=0,i,index;
    char id[10], pass[15];
    char logintext[] = "Login to ICT Contact Management Systems";
    char iderror[] = "\"ID Incorrect!\"";
    char passerror[] = "\"Password Incorrect!\"";
    //Start Curses Mode
    initscr();
    cbreak();
    window_width = COLS-2;
    login_height = LINES-5;
    login_top = banner_win(1,1,window_width);
    login_main = newwin(login_height, window_width, 4, 1);
    while (foundid != 1 || foundpass != 1)
    {
        mvwprintw(login_main, 0, (COLS-strlen(logintext))/2, "%s", logintext);
        mvwprintw(login_main, (login_height/2)-1, window_width/3+5, "%8s: ", "ID");
        mvwprintw(login_main, (login_height/2), window_width/3+5, "Password: ");
        wrefresh(login_main);
        if (foundid == 0) mvwscanw(login_main, (login_height/2)-1, window_width/3+15,"%s", id);
        if (foundpass == 0) mvwscanw(login_main, (login_height/2), window_width/3+15,"%s", pass);
        for(i=0;i<total_user;i++)
        {
            if (strcmp(id,user_all[i].id)==0)
            {
                foundid = 1;
                index = i;
                if (strcmp(pass,user_all[i].password)==0)
                {
                    foundpass = 1;
                    break;
                }
            }
        }
        if(foundid==0)
        {
            wclear(login_main);
            mvwprintw(login_main, login_height-10, (window_width-strlen(iderror))/2, "%s", iderror);
        }
        else if (foundpass == 0)
        {
            wclear(login_main);
            mvwprintw(login_main, login_height-10, (window_width-strlen(passerror))/2, "%s", passerror);
            mvwprintw(login_main, (login_height/2)-1, window_width/3+15,"%s", user_all[index].id);
        }
    }
    win_delete(login_top);
    win_delete(login_main);
    endwin();
    if (user_all[index].id[0] == '0' && user_all[index].id[1] == '0') admin_menu(index);
    else student_menu(index);
}

void student_menu(int user_index)
{
    WINDOW *student_top, *student_choice, *student_bottom;
    int i, loop_con=1, hilight = 1, window_width, choice_height, choice, wch;
    char *login_choices[] = {
            "Edit My Contact Information",
            "Show all Contacts",
            "Search for a Contact",
            "Logout"
    };
    char *login_des[] = {
            "Edit your contact information in ICT Contact Management Systems.",
            "Show all contacts information in ICT Contact Management Systems.",
            "Search for a contact information in ICT Contact Management Systems by using firstName.",
            "Logout from ICT Contact Management Systems and return to mainmenu."
    };
    char welcom[] = "Welcome! you are logged in as a STUDENT";
    int n_choices = sizeof(login_choices) / sizeof(login_choices[0]);
    int name_length = strlen(user_all[user_index].firstName) + strlen(user_all[user_index].lastName) + 1;
    //Start Curses Mode
    initscr();
    noecho();
    cbreak();
    curs_set(0);
    window_width = COLS-2;
    choice_height = LINES - 8;
    student_top = banner_win(1,1,window_width);
    student_choice = newwin(choice_height, window_width,4,1);
    student_bottom = newwin(3,window_width, LINES-4,1);
    keypad(student_choice, TRUE);
    select_menuH(student_choice, hilight, window_width, choice_height, login_choices, n_choices);
    print_description(student_bottom, window_width, login_des[hilight-1]);
    mvwprintw(student_choice, 0, (window_width-strlen(welcom))/2, "%s", welcom);
    mvwprintw(student_choice, 1, (window_width-name_length)/2, "%s %s", user_all[user_index].firstName, user_all[user_index].lastName);
    wrefresh(student_choice);
    while (loop_con)
    {
        wch = wgetch(student_choice);
        switch (wch) {
            case KEY_UP:
                if (hilight == 1) hilight = n_choices;
                else hilight-=1;
                break;
            case KEY_DOWN:
                if (hilight == n_choices) hilight = 1;
                else hilight+=1;
                break;
            case 10:
                choice = hilight;
                loop_con = 0;
                break;
            }
        select_menuH(student_choice, hilight, window_width, choice_height, login_choices, n_choices);
        print_description(student_bottom, window_width, login_des[hilight-1]);
        mvwprintw(student_choice, 0, (window_width-strlen(welcom))/2, "%s", welcom);
        mvwprintw(student_choice, 1, (window_width-name_length)/2, "%s %s", user_all[user_index].firstName, user_all[user_index].lastName);
        wrefresh(student_choice);
    }
    switch (choice)
    {
        case 1:
            win_delete(student_top);
            win_delete(student_choice);
            win_delete(student_bottom);
            curs_set(1);
            endwin();
            edit_info(1, user_index);
            break;
        case 2:
            win_delete(student_top);
            win_delete(student_choice);
            win_delete(student_bottom);
            curs_set(1);
            endwin();
            show_all(1, user_index);
            break;
        case 3:
            win_delete(student_top);
            win_delete(student_choice);
            win_delete(student_bottom);
            curs_set(1);
            endwin();
            searchinfo(1, user_index);
            break;
        case 4:
            win_delete(student_top);
            win_delete(student_choice);
            win_delete(student_bottom);
            curs_set(1);
            char bye[] = "Good Bye!! :D";
            mvprintw(LINES/2, (COLS - strlen(bye))/2, "%s", bye);
            getch();
            endwin();
            menu_Main();
            break;
    }
}

void admin_menu(int user_index)
{
    WINDOW *admin_top, *admin_choice, *admin_bottom;
    int i, loop_con=1, hilight = 1, window_width, choice_height, choice, wch;
    char *login_choices[] = {
        "Edit User's Contact",
        "Show all Contacts",
        "Search for a Contact",
        "Delete User's Contact",
        "Logout"
    };
    char *login_des[] = {
        "Edit user contact information in ICT Contact Management Systems.",
        "Show all contacts information in ICT Contact Management Systems.",
        "Search for a contact information in ICT Contact Management Systems by using firstName.",
        "Delete user's contact information from ICT Contact Management Systems.",
        "Logout from ICT Contact Management Systems and return to mainmenu."
    };
    char welcom[] = "Welcome! you are logged in as a ADMIN";
    int n_choices = sizeof(login_choices) / sizeof(login_choices[0]);
    int name_length = strlen(user_all[user_index].firstName) + strlen(user_all[user_index].lastName) + 1;
    //Start Curses Mode
    initscr();
    noecho();
    cbreak();
    curs_set(0);
    window_width = COLS-2;
    choice_height = LINES - 8;
    admin_top = banner_win(1,1,window_width);
    admin_choice = newwin(choice_height, window_width,4,1);
    admin_bottom = newwin(3,window_width, LINES-4,1);
    keypad(admin_choice, TRUE);
    select_menuH(admin_choice, hilight, window_width, choice_height, login_choices, n_choices);
    print_description(admin_bottom, window_width, login_des[hilight-1]);
    mvwprintw(admin_choice, 0, (window_width-strlen(welcom))/2, "%s", welcom);
    mvwprintw(admin_choice, 1, (window_width-name_length)/2, "%s %s", user_all[user_index].firstName, user_all[user_index].lastName);
    wrefresh(admin_choice);
    while (loop_con)
    {
        wch = wgetch(admin_choice);
        switch (wch) {
            case KEY_UP:
                if (hilight == 1) hilight = n_choices;
                else hilight-=1;
                break;
            case KEY_DOWN:
                if (hilight == n_choices) hilight = 1;
                else hilight+=1;
                break;
            case 10:
                choice = hilight;
                loop_con = 0;
                break;
            }
        select_menuH(admin_choice, hilight, window_width, choice_height, login_choices, n_choices);
        print_description(admin_bottom, window_width, login_des[hilight-1]);
        mvwprintw(admin_choice, 0, (window_width-strlen(welcom))/2, "%s", welcom);
        mvwprintw(admin_choice, 1, (window_width-name_length)/2, "%s %s", user_all[user_index].firstName, user_all[user_index].lastName);
        wrefresh(admin_choice);
    }
    switch (choice)
    {
        case 1:
            win_delete(admin_top);
            win_delete(admin_choice);
            win_delete(admin_bottom);
            curs_set(1);
            endwin();
            edit_info(0, user_index);
            break;
        case 2:
            win_delete(admin_top);
            win_delete(admin_choice);
            win_delete(admin_bottom);
            curs_set(1);
            endwin();
            show_all(0, user_index);
            break;
        case 3:
            win_delete(admin_top);
            win_delete(admin_choice);
            win_delete(admin_bottom);
            curs_set(1);
            endwin();
            searchinfo(0, user_index);
            break;
        case 4:
            win_delete(admin_top);
            win_delete(admin_choice);
            win_delete(admin_bottom);
            curs_set(1);
            endwin();
            delete_info(user_index);
            break;
        case 5:
            win_delete(admin_top);
            win_delete(admin_choice);
            win_delete(admin_bottom);
            curs_set(1);
            char bye[] = "Good Bye!! :D";
            mvprintw(LINES/2, (COLS - strlen(bye))/2, "%s", bye);
            getch();
            endwin();
            menu_Main();
            break;
    }
}

void show_all(int role, int user_index)
{
    WINDOW *show_top, *show_main;
    int i, window_width, show_height;
    show_height = total_user + 7;
    //Start Curses Mode
    initscr();
    window_width = COLS-2;
    show_top = banner_win(1,1, window_width);
    show_main = newwin(show_height, window_width,4,1);
    wprintw(show_main, "All Contacts\n");
    wprintw(show_main, "==============================");
    mvwprintw(show_main, 3, 0, "ID");
    mvwprintw(show_main, 3, window_width/7, "FirstName");
    mvwprintw(show_main, 3, (window_width/7)*2, "LastName");
    mvwprintw(show_main, 3, (window_width/7)*3, "Role");
    mvwprintw(show_main, 3, (window_width/7)*4, "Phone Number");
    mvwprintw(show_main, 3, (window_width/7)*5, "Email");
    wprintw(show_main, "\n");
    for (i = 0; i<window_width;i++) wprintw(show_main, "=");
    for (i = 0; i < total_user; i++)
    {
        mvwprintw(show_main, 5+i, 0, "%s", user_all[i].id);
        mvwprintw(show_main, 5+i, window_width/7, "%s", user_all[i].firstName);
        mvwprintw(show_main, 5+i, (window_width/7)*2, "%s", user_all[i].lastName);
        if (user_all[i].id[0] == '0' && user_all[i].id[1] == '0') mvwprintw(show_main, 5+i, (window_width/7)*3, "Admin");
        else mvwprintw(show_main, 5+i, (window_width/7)*3, "Student");
        mvwprintw(show_main, 5+i, (window_width/7)*4, "%s", user_all[i].phoneNumber);
        mvwprintw(show_main, 5+i, (window_width/7)*5, "%s", user_all[i].email);
    }
    wprintw(show_main, "\n");
    for (i = 0; i<window_width;i++) wprintw(show_main, "-");
    wprintw(show_main, "Press any key to return to %s page...", (role == 1)? "Student":"Admin");
    wgetch(show_main);
    win_delete(show_top);
    win_delete(show_main);
    curs_set(0);
    endwin();
    if (role == 1) student_menu(user_index);
    else admin_menu(user_index);
}

void searchinfo(int role, int user_index)
{
    WINDOW *s_top, *s_main;
    int i, j=0,f=0, index[total_user];
    char input[50], er[100] = "There is no person Name '", er2[]="' in the system.", *s;

    //Start Curses Mode
    initscr();
    s_top = banner_win(1, 1, COLS-2);
    s_main = newwin(LINES-5, COLS-2, 4,1);
    while (f==0)
    {
        strcpy(er, "There is no person Name '");
        mvwprintw(s_main,(LINES-5)/2, COLS/3,"Enter FirstName: ");
        wrefresh(s_main);
        wscanw(s_main, "%s", input);
        for (i = 0; i < total_user; i++)
        {
            if (s = strstr(user_all[i].firstName, input))
            {
                f++;
                index[j] = i;
                j++;
            }
        }
        if (f >= 1)
        {
            wclear(s_main);
            wprintw(s_main, "Result of Searching for \"%s\"\n", input);
            wprintw(s_main, "---------------------------------------------------------------");
            mvwprintw(s_main, 3, 0, "ID");
            mvwprintw(s_main, 3, (COLS-2)/7, "FirstName");
            mvwprintw(s_main, 3, ((COLS-2)/7)*2, "LastName");
            mvwprintw(s_main, 3, ((COLS-2)/7)*3, "Role");
            mvwprintw(s_main, 3, ((COLS-2)/7)*4, "Phone Number");
            mvwprintw(s_main, 3, ((COLS-2)/7)*5, "Email");
            for (i = 0; i < f; i++)
            {
                mvwprintw(s_main, 5+i, 0, "%s", user_all[index[i]].id);
                mvwprintw(s_main, 5+i, (COLS-2)/7, "%s", user_all[index[i]].firstName);
                mvwprintw(s_main, 5+i, ((COLS-2)/7)*2, "%s", user_all[index[i]].lastName);
                if (user_all[index[i]].id[0] == '0' && user_all[index[i]].id[1] == '0') mvwprintw(s_main, 5+i, ((COLS-2)/7)*3, "Admin");
                else mvwprintw(s_main, 5+i, ((COLS-2)/7)*3, "Student");
                mvwprintw(s_main, 5+i, ((COLS-2)/7)*4, "%s", user_all[index[i]].phoneNumber);
                mvwprintw(s_main, 5+i, ((COLS-2)/7)*5, "%s", user_all[index[i]].email);
            }
            wprintw(s_main, "\n");
            for (i = 0; i < COLS-2; i++) wprintw(s_main, "-");
            wprintw(s_main, "Press any key to return to %s page...", (role == 1)? "Student":"Admin");
            wgetch(s_main);
        }
        else
        {
            strcat(er, input);
            strcat(er, er2);
            wclear(s_main);
            mvwprintw(s_main, (LINES-5)/2+2, (COLS-strlen(er)-2)/2, "%s", er);
        }
    }
    win_delete(s_top);
    win_delete(s_main);
    endwin();
    if (role == 1) student_menu(user_index);
    else admin_menu(user_index);

}

void delete_info(int user_index)
{
    WINDOW *d_top, *d_main;
    int i, index=-1, de_width, startx, idC=1, highlight = 2, choice=0, ch, now_aindex=user_index;
    char input[50], id[8];
    strcpy(id, user_all[user_index].id);
    //Start Curses Mode
    initscr();
    de_width = COLS-2;
    startx = COLS/3;
    d_top = banner_win(1, 1, de_width);
    d_main = newwin(LINES-5, de_width, 4,1);
    mvwprintw(d_main, 0, (de_width-strlen("Please enter ID to delete user"))/2, "Please enter ID to delete user");
    REGIS:
    while(idC)
    {
        mvwprintw(d_main, 2, de_width/2-6,"ID: ");
        wrefresh(d_main);
        wscanw(d_main, "%s", input);
        idC = idCheck(d_main, de_width, 4, input);
        if (idC == 1) clean_er(d_main, de_width/2-2, 2, de_width);
    }
    clean_er(d_main, 0, 4, de_width);
    if (strcmp(input, user_all[user_index].id) == 0)
    {
        mvwprintw(d_main, 4, (de_width-strlen("You can not delete yourself!"))/2, "You can not delete yourself!");
        idC = 1;
        clean_er(d_main, de_width/2-2, 2, de_width);
        goto REGIS;
    }
    for (i = 0; i < total_user ; i++)
    {
        if(strcmp(input,user_all[i].id) == 0)
        {
            index = i;
            break;
        }
    }
    if (index!=-1)
    {
        mvwprintw(d_main, 6, startx, "          ID: %s", user_all[index].id);
        mvwprintw(d_main, 7, startx, "   FirstName: %s", user_all[index].firstName);
        mvwprintw(d_main, 8, startx, "    LastName: %s", user_all[index].lastName);
        mvwprintw(d_main, 9, startx, "Phone Number: %s", user_all[index].phoneNumber);
        mvwprintw(d_main,10, startx, "       Email: %s", user_all[index].email);
        mvwprintw(d_main,11, startx, "    Password: %s", user_all[index].password);
        wrefresh(d_main);
        curs_set(0);
        mvwprintw(d_main, 13, (de_width-strlen("Do you want to delete this user?"))/2, "Do you want to delete this user?");
        while (choice == 0)
        {
            if (highlight == 1)
            {
                wattron(d_main,A_REVERSE);
                mvwprintw(d_main, 14, de_width/2-6, "Yes");
                wattroff(d_main,A_REVERSE);
                wprintw(d_main, "   ");
                wprintw(d_main, "No");
                wrefresh(d_main);
            }
            else if (highlight == 2)
            {
                mvwprintw(d_main, 14, de_width/2-6, "Yes");
                wprintw(d_main, "   ");
                wattron(d_main,A_REVERSE);
                wprintw(d_main, "No");
                wattroff(d_main,A_REVERSE);
                wrefresh(d_main);
            }
            noecho();
            keypad(d_main, TRUE);
            ch = wgetch(d_main);
            switch (ch) {
                case KEY_LEFT:
                    if (highlight == 1) highlight = 2;
                    else highlight--;
                    break;
                case KEY_RIGHT:
                    if (highlight == 2) highlight = 1;
                    else highlight++;
                    break;
                case 10:
                    choice = highlight;
                    break;
            }
        }
    }
    else
    {
        clean_er(d_main,0,4, de_width);
        mvwprintw(d_main, 4, (de_width-strlen("ID not founded!"))/2, "ID not founded!");
        clean_er(d_main, de_width/2-2, 2, de_width);
        idC = 1;
        goto REGIS;
    }
    switch (choice) {
        case 1:
            for(i=index;i<total_user;i++)
            user_all[i]=user_all[i+1];
            total_user--;
            mvwprintw(d_main, 16, (de_width-strlen("Delete Success!"))/2, "Delete Success!");
            if (strcmp(id, user_all[user_index].id)!=0) now_aindex = user_index-1;
            break;
        case 2:
            mvwprintw(d_main, 16, (de_width-strlen("Deletion was cancelled!"))/2, "Deletion was cancelled!");
            break;
    }
    mvwprintw(d_main, 18, (de_width-strlen("Press any key to return to admin page..."))/2, "Press any key to return to admin page...");
    wrefresh(d_main);
    curs_set(1);
    wgetch(d_main);
    win_delete(d_top);
    win_delete(d_main);
    endwin();
    update_file();
    admin_menu(now_aindex);
}

void edit_info(int role, int user_index)
{
    WINDOW *e_top, *e_main;
    int i, e_width, j, highlight = 1, choice, e_index, idC=1, ch, loop_con= 1, f = 0;
    char input[50], input2[50];
    char *choices[] = {
        "FirstName",
        "LastName",
        "Phone Number",
        "Email",
        "Password"
    };
    //Start Curses Mode
    initscr();
    e_width = COLS-2;
    e_top = banner_win(1,1, e_width);
    e_main = newwin(LINES-5, e_width, 4,1);
    if (role == 0)
    {
        mvwprintw(e_main, 0, (e_width-strlen("Please enter ID to edit user information"))/2, "Please enter ID to edit user information");
        EID:
        while(idC)
        {
            mvwprintw(e_main, 2, e_width/2-6,"ID: ");
            wrefresh(e_main);
            wscanw(e_main, "%s", input);
            idC = idCheck(e_main, e_width, 4, input);
            if (idC == 1) clean_er(e_main, e_width/2-2, 2, e_width);
        }
        for (i = 0; i < total_user ; i++)
        {
            if (strcmp(input, user_all[i].id) == 0)
            {
                f = 1;
                e_index = i;
                break;
            }
        }
        if (f == 0)
        {
            clean_er(e_main, 0, 4, e_width);
            mvwprintw(e_main, 4, (e_width-strlen("ID not founded!"))/2, "ID not founded!");
            clean_er(e_main, e_width/2-2, 2, e_width);
            idC = 1;
            goto EID;
        }
        clean_er(e_main, 0, 4, e_width);
    }
    else
    {
        e_index = user_index;
    }
    wclear(e_main);

    select_menuH(e_main, highlight, e_width, LINES-5, choices, 5);
    mvwprintw(e_main, 0, e_width/2-11, "[%s] ID: %s", (user_all[e_index].id[0] == '0' && user_all[e_index].id[1] == '0')? "ADMIN": "STUDENT", user_all[e_index].id);
    mvwprintw(e_main, 2, (e_width-strlen("Please select information that you want to edit"))/2, "Please select information that you want to edit");
    wrefresh(e_main);
    noecho();
    keypad(e_main, TRUE);
    curs_set(0);
    while (loop_con)
    {
        ch = wgetch(e_main);
        switch (ch) {
            case KEY_UP:
                if (highlight == 1) highlight =5;
                else highlight-=1;
                break;
            case KEY_DOWN:
                if (highlight == 5) highlight = 1;
                else highlight+=1;
                break;
            case 10:
                choice = highlight;
                loop_con = 0;
                break;
        }
        select_menuH(e_main, highlight, e_width, LINES-5, choices, 5);
        mvwprintw(e_main, 0, e_width/2-11, "[%s] ID: %s", (user_all[e_index].id[0] == '0' && user_all[e_index].id[1] == '0')? "ADMIN": "STUDENT", user_all[e_index].id);
        mvwprintw(e_main, 2, (e_width-strlen("Please select information that you want to edit"))/2, "Please select information that you want to edit");
        wrefresh(e_main);
    }
    echo();
    curs_set(1);
    wclear(e_main);
    mvwprintw(e_main, 0, e_width/2-11, "[%s] ID: %s", (user_all[e_index].id[0] == '0' && user_all[e_index].id[1] == '0')? "ADMIN": "STUDENT", user_all[e_index].id);
    mvwprintw(e_main, 2, (e_width-strlen("Please select information that you want to edit"))/2, "Please select information that you want to edit");
    wrefresh(e_main);
    int firstC, lastC, phoneC, emailC, passC, startx = COLS/2-15;
    switch (choice) {
        case 1:
            while (firstC)
            {
                mvwprintw(e_main, 4, startx, "FirstName: ");
                wrefresh(e_main);
                wscanw(e_main, "%s", input2);
                firstC = nameCheck(e_main, e_width, 6, input2);
                if (firstC == 1) clean_er(e_main, 0, 4, e_width);
            }
            clean_er(e_main, 0, 6, e_width);
            strcpy(user_all[e_index].firstName, input2);
            break;
        case 2:
            while (lastC)
            {
                mvwprintw(e_main, 4, startx, "LastName: ");
                wrefresh(e_main);
                wscanw(e_main, "%s", input2);
                lastC = nameCheck(e_main, e_width, 6, input2);
                if (lastC == 1) clean_er(e_main, 0, 4, e_width);
            }
            clean_er(e_main, 0, 6, e_width);
            strcpy(user_all[e_index].lastName, input2);
            break;
        case 3:
            while (phoneC)
            {
                mvwprintw(e_main, 4, startx, "PhoneNumber: ");
                wrefresh(e_main);
                wscanw(e_main, "%s", input2);
                phoneC = phoneCheck(e_main, e_width, 6, input2);
                if (phoneC == 1) clean_er(e_main, 0, 4, e_width);
            }
            clean_er(e_main, 0, 6, e_width);
            strcpy(user_all[e_index].phoneNumber, input2);
            break;
        case 4:
            while (emailC)
            {
                mvwprintw(e_main, 4, startx, "Email: ");
                wrefresh(e_main);
                wscanw(e_main, "%s", input2);
                emailC = mailCheck(e_main, e_width, 6, input2);
                if (emailC == 1) clean_er(e_main, 0, 4, e_width);
            }
            clean_er(e_main, 0, 6, e_width);
            strcpy(user_all[e_index].email, input2);
            break;
        case 5:
            while (passC)
            {
                mvwprintw(e_main, 4, startx, "Password: ");
                wrefresh(e_main);
                wscanw(e_main, "%s", input2);
                passC = passCheck(e_main, e_width, 6, input2, input2);
                if (passC == 1) clean_er(e_main, 0, 4, e_width);
            }
            clean_er(e_main, 0, 6, e_width);
            strcpy(user_all[e_index].password, input2);
            break;
    }
    char ex[] = "Press any key to return to ";
    strcat(ex,(role == 0)? "admin page...":"student page...");
    mvwprintw(e_main, 6, (e_width-strlen("Information edit success!"))/2, "Information edit success!");
    mvwprintw(e_main, 8, (e_width-strlen(ex))/2, "%s", ex);
    wgetch(e_main);
    win_delete(e_top);
    win_delete(e_main);
    endwin();
    update_file();
    if (role == 0) admin_menu(user_index);
    else student_menu(user_index);

}

void clean_er(WINDOW *win, int startx,int line, int width)
{
    int i;
    wmove(win, line, startx);
    for (i = startx; i < width ; i++)
    {
        wprintw(win, " ");
    }
    wrefresh(win);
}

void user_register()
{
    WINDOW *re_top, *re_main;
    char input[50], input2[50];
    int idC = 1,firstC = 1,lastC = 1,phoneC = 1,emailC = 1,passC = 1, i, re_width;
    //Start Curses Mode
    initscr();
    re_width = (COLS/3)+15;
    re_top = banner_win(1,1,COLS-2);
    re_main = newwin(LINES-5, re_width, 4, COLS/3-5);
    mvwprintw(re_main, 0, (re_width-strlen("User Registration"))/2, "User Registration");
    wprintw(re_main, "\n\n\n");
    wprintw(re_main, "              ID: \n");
    wprintw(re_main, "       FirstName: \n");
    wprintw(re_main, "        LastName: \n");
    wprintw(re_main, "    Phone Number: \n");
    wprintw(re_main, "           Email: \n");
    wprintw(re_main, "        Password: \n");
    wprintw(re_main, "Confirm Password: \n");
    wrefresh(re_main);

    //ID
    while (idC)
    {
        mvwscanw(re_main, 3, 18, "%s", input);
        idC = idCheck(re_main, re_width, 11, input);
        if (idC == 1) clean_er(re_main, 18, 3, re_width);
    }
    clean_er(re_main, 0, 11, re_width);
    strcpy(user_all[total_user].id, input);

    //FirstN
    while (firstC)
    {
        mvwscanw(re_main, 4, 18, "%s", input);
        firstC = nameCheck(re_main, re_width, 11, input);
        if (firstC == 1) clean_er(re_main, 18, 4, re_width);
    }
    clean_er(re_main, 0, 11, re_width);
    strcpy(user_all[total_user].firstName, input);

    //LastN
    while (lastC)
    {
        mvwscanw(re_main, 5, 18, "%s", input);
        lastC = nameCheck(re_main, re_width, 11, input);
        if (lastC == 1) clean_er(re_main, 18, 5, re_width);
    }
    clean_er(re_main, 0, 11, re_width);
    strcpy(user_all[total_user].lastName, input);

    //PhoneN
    while (phoneC)
    {
        mvwscanw(re_main, 6, 18, "%s", input);
        phoneC = phoneCheck(re_main, re_width, 11, input);
        if (phoneC == 1) clean_er(re_main, 18, 6, re_width);
    }
    clean_er(re_main, 0, 11, re_width);
    strcpy(user_all[total_user].phoneNumber, input);

    //Email
    while (emailC)
    {
        mvwscanw(re_main, 7, 18, "%s", input);
        emailC = mailCheck(re_main, re_width, 11, input);
        if (emailC == 1) clean_er(re_main, 18, 7, re_width);
    }
    clean_er(re_main, 0, 11, re_width);
    strcpy(user_all[total_user].email, input);

    //Password
    while (passC)
    {
        if (passC == 1)
        {
            mvwscanw(re_main, 8, 18, "%s", input);
            mvwscanw(re_main, 9, 18, "%s", input2);
        }
        else if (passC == 2)
        {
            mvwscanw(re_main, 9, 18, "%s", input2);
        }
        passC = passCheck(re_main, re_width, 11, input, input2);
        if (passC == 1)
        {
            clean_er(re_main, 18, 8, re_width);
            clean_er(re_main, 18, 9, re_width);
        }
        else if (passC == 2) clean_er(re_main, 18, 9, re_width);
    }
    clean_er(re_main, 0, 11, re_width);
    strcpy(user_all[total_user].password, input);

    //Register Succes
    total_user++;
    char re_success[] = "Congratulation!! ";
    char re_success2[] = "Now you can login to ICT Contact Management System";
    strcat(re_success, user_all[total_user-1].firstName);
    mvwprintw(re_main, 11, (re_width-strlen("Registration Successful!!"))/2, "Registration Successful!!");
    mvwprintw(re_main, 12, (re_width-strlen(re_success))/2, "%s", re_success);
    mvwprintw(re_main, 13, (re_width-strlen(re_success2))/2, "%s", re_success2);
    mvwprintw(re_main, 15, (re_width-strlen("Press Any key to return to the main menu..."))/2, "Press Any key to return to the main menu...");
    wrefresh(re_main);
    wgetch(re_main);
    win_delete(re_top);
    win_delete(re_main);
    endwin();
    update_file();
    menu_Main();
}

int idCheck(WINDOW *win,int width ,int y,char st[])
{
    int i = 0;
    char e1[] = "You can use only 7-digit number.";
    if (strlen(st)!=7)
    {
        clean_er(win, 0, y, width);
        mvwprintw(win, y,  (width-strlen(e1))/2,"%s", e1);
        return 1;
    }
    for (i = 0; i < 7; i++) {
        if (isdigit(st[i]) == 0)
        {
            clean_er(win, 0, y, width);
            mvwprintw(win, y,  (width-strlen(e1))/2,"%s", e1);
            return 1;
        }
    }
    return 0;
}

int nameCheck(WINDOW *win, int width, int y, char st[])
{
    int i=0;
    char e1[] = "You can use only alphabet letter.";
    for (i = 0; i < strlen(st); i++) {
        if (isalpha(st[i]) == 0)
        {
            clean_er(win, 0, y, width);
            mvwprintw(win, y,  (width-strlen(e1))/2,"%s", e1);
            return 1;
        }
    }
    return 0;
}

int phoneCheck(WINDOW *win, int width, int y, char st[])
{
    int i=0;
    char e1[] = "Must start with 0.";
    char e2[] = "You can use only 10-digit number.";
    if(st[0] != '0')
    {
         clean_er(win, 0, y, width);
         mvwprintw(win, y,  (width-strlen(e1))/2,"%s", e1);
         return 1;
    }
    if (strlen(st)!=10)
    {
         clean_er(win, 0, y, width);
         mvwprintw(win, y,  (width-strlen(e2))/2,"%s", e2);
         return 1;
    }
    for (i = 0; i < 10; i++) {
        if (isdigit(st[i]) == 0)
        {
            clean_er(win, 0, y, width);
            mvwprintw(win, y,  (width-strlen(e2))/2,"%s", e2);
            return 1;
        }
    }
    return 0;
}

int mailCheck(WINDOW *win, int width, int y, char st[])
{
    int i;
    char *e1, *e2;
    char er1[] = "Email must contain \"@\".";
    char er2[] = "Email must contain \".\" after \"@\".";
    if (e1 = strstr(st,"@"))
    {
        if (e2 = strstr(e1,"."))
        {
            return 0;
        }
        else
        {
            clean_er(win, 0, y, width);
            mvwprintw(win, y,  (width-strlen(er2))/2,"%s", er2);
            return 1;
        }
    }
    else
    {
        clean_er(win, 0, y, width);
        mvwprintw(win, y,  (width-strlen(er1))/2,"%s", er1);
        return 1;
    }
}

int passCheck(WINDOW *win, int width, int y, char st[], char st2[])
{
    char e1[] = "You must use at least 8 characters for password.";
    char e2[] = "Password Not Match.";
    if (strlen(st)<8)
    {
        clean_er(win, 0, y, width);
        mvwprintw(win, y,  (width-strlen(e1))/2,"%s", e1);
        return 1;
    }
    else
    {
        if (strcmp(st,st2)!=0)
        {
            clean_er(win, 0, y, width);
            mvwprintw(win, y,  (width-strlen(e2))/2,"%s", e2);
            return 2;
        }
        else
        {
            return 0;
        }
    }
}
