#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<math.h>
#include<unistd.h>
#include<ncurses.h>
#include "project.h"


typedef long long int ll;
typedef double db;

#define f first
#define s second
#define all(v) (v).begin(),(v).end()
#define print(x) printf("%d \n", x);
#define printll(x) printf("%lld \n", x);
#define get(n) scanf("%d", &n);
#define Dis 3
#define Max_Line_Size 40

int st_selectx, st_selecty;
char slct[N];
bool found[N][N], mark[N][N];


int max(int x, int y)
{
   if(x > y)
      return x;
   return y;   
}

int min(int x, int y)
{
   if(x < y)
      return x;
   return y;   
}
void swap(int *x, int *y)
{
   int tmp = *x;
   *x = *y;
   *y = tmp;
}

struct Editor
{
   int x, y, line, save, st_line;
   char mode;      // i -> insert    m | n -> noemal    v->visual   x->exit
   char file_name[N]; 
   char Buffer[N][N];
   char Bar[N];
};
struct Editor ed, ed_copy, tmp;



void copy_ed()
{
   ed_copy = ed;
}

void curses_init()
{
   initscr();                  
   noecho();                       
   cbreak();                       
   keypad(stdscr, true);     
   start_color();      
}

void navigation(int input)
{
   if(input == 'k' || input == KEY_UP)         // up
   {
      if(ed.y == 0)
         ed.st_line = max(ed.st_line-1, 0);
      ed.y = max(0, ed.y-1);
   }
   else if(input == 'j' || input == KEY_DOWN)   //down
   {

      if(ed.y == 9)
         ed.st_line++;
      
      ed.y = min(9, ed.y+1);
      //ed.y = ed.y + 1;
      //ed.st_line = max(ed.st_line, ed.x-10);
      //printf("%d", strlen(ed.Buffer[y])+Dis-1);
   }
   else if(input == 'h' || input == KEY_LEFT)   // left
   {
      ed.x--;
   }
   else if(input == 'l' || input == KEY_RIGHT)   // righy
   {
      ed.x++;
   }
   
   int y = ed.y + ed.st_line;
   ed.x = min(ed.x, strlen(ed.Buffer[y])+Dis);
   ed.x = max(Dis, ed.x);
   //clrtoeol();
   
}

void Put_st_in_ed(char* st)
{
   tab_to_space(st);
   for(int i = 0; i < N; i++)
      memset(ed.Buffer[i], '\0', sizeof ed.Buffer[i]);
   ed.line = 0;   
   //return;
   for(int i = 0; i < strlen(st); i++)
   {
      int cnt = 0;
      while(i < strlen(st) && st[i] != '\n')
      {
         if(cnt == Max_Line_Size)
         {
            ed.line++;
            cnt = 0;
         }
         ed.Buffer[ed.line][cnt++] = st[i];
         i++;
      }
      
      ed.line++;
   }
}

void init(char *fn)
{
   //print(-1);

   ed.file_name[0] = ' ';
   ed.file_name[1] = ' ';
   ed.file_name[2] = '\0';
   
   
   if(fn != NULL)
   {
      memset(ed.file_name, '\0', sizeof ed.file_name);
      for(int i = 0; i < strlen(fn); i++)
      {
         ed.file_name[i] = fn[i];
         //printf("%c", fn[i]);
      }
   }

   ed.mode = 'n';
   ed.save = 1;
   ed.x = Dis;
   memset(ed.Bar, '\0', sizeof ed.Bar); 
   ed.st_line = ed.y = ed.line = 0;
   if(fn == NULL)
   {
      return;
   }   
   if(fn[0] == '/')
      fn++;
   //printf("%s\n", fn);
   FILE *fp;
   fp = fopen(fn, "r");

   char st[N] = {"\0"};
   get_all_char(st, fp);
   Put_st_in_ed(st);
   fclose (fp);
}

bool is_select(int y, int x)
{
   
   if(ed.mode != 'v')
      return 0;
   //int y, x;
   //getyx(stdscr, y, x);
   int mny = ed.y;
   int mxy = st_selecty - ed.st_line;
   int mnx = ed.x;
   int mxx = st_selectx;
   if(mny > mxy)
   {
      swap(&mny, &mxy);
      swap(&mnx, &mxx);
   }
   if(mny < y && y < mxy)
      return 1;

   if(mny == mxy)
   {
      if(mnx > mxx)
         swap(&mnx, &mxx);
      return (y == mny && x >= mnx && x <= mxx);
   }   
   if(y == mny && x >= mnx)   
      return 1;
   if(y == mxy && x <= mxx)
      return 1;   
   return 0;      
}

void printBuff()
{
   init_pair(1, COLOR_GREEN, COLOR_YELLOW);
   init_pair(2, COLOR_BLACK, COLOR_RED);
   if(ed.mode == 'v')
      memset(slct, '\0', sizeof slct);
   for(int i = ed.st_line; i < ed.st_line + 10; i++)
   {
      //if(select())
      move(i-ed.st_line, 0);
      printw("%2d ", i);//, ed.Buffer[i]);
      for(int j = 0; j < strlen(ed.Buffer[i]); j++)
      {
         int y, x;
         getyx(stdscr, y, x);
         if(is_select(y, x))
         {
            attron(COLOR_PAIR(1));
            addch(ed.Buffer[i][j]);
            slct[strlen(slct)] = ed.Buffer[i][j];
            if(j == strlen(ed.Buffer[i]) - 1)
               slct[strlen(slct)] = '\n';
            //printf("%d %d %d    ", y, st_selecty, ed.y);
            attroff(COLOR_PAIR(1));
         }
         else if(found[i][j] && ed.mode == 'f')      // -> y = i-ed.st_line   x = j+Dis-1
         {
            attron(COLOR_PAIR(2));
            addch(ed.Buffer[i][j]);
            attroff(COLOR_PAIR(2));
         }    
         else
         {
            printw("%c", ed.Buffer[i][j]);
            //printw("%d", (ed.y >= y && y <= 2));
            //printw("%d", y);
         }
         
      }
      printw("\n");   
   }


   int y, x;
   getmaxyx(stdscr, y, x);

   WINDOW * Mode = newwin(3, x-1, y-5, 0), *Bar = newwin(2, x-1, y-2, 0);          
   wclear(Mode);
   //wclear(Bar);
   refresh();
   box(Mode, 0, 0);
   wattron(Mode, A_REVERSE);
   if(ed.mode == 'i')
      mvwprintw(Mode, 1, 1, "INSERT");
   else if(ed.mode == 'v')
      mvwprintw(Mode, 1, 1, "VISUAL");  
   else
      mvwprintw(Mode, 1, 1, "NORMAL");
   wattroff(Mode, A_REVERSE);   
   wprintw(Mode, " %s",  ed.file_name);
   if(!ed.save)
   {
      wprintw(Mode, " +");
   }
   wrefresh(Mode);

   //box(Bar, 0, 0);
   //wattron(Bar, A_REVERSE);
   wmove(Bar, 1, 1);
   if(ed.Bar != NULL)
      wprintw(Bar, "%s", ed.Bar);
   wrefresh(Bar);
   //wattroff(Bar, A_REVERSE);
   


   if(ed.mode != 'm')
   {
      
      move(ed.y, ed.x);
      //printf("pos : %d %d  ", ed.x, ed.y);
   }
 
}

void insert_input(int input)
{
   ed.save = 0;
   int len = 0;
   copy_ed();
   char st[N] = {"\0"};
   int y = ed.y + ed.st_line;
   //getchar();
   for(int i = 0; i < y; i++)
   {
      for(int j = 0; j < strlen(ed.Buffer[i]); j++)
         st[len++] = ed.Buffer[i][j];
      st[len++] = '\n';   
   }
   
   int cnt = 0;
   for(int i = 0; i < ed.x-Dis; i++)
   {
      st[len++] = ed.Buffer[y][i];
      cnt++;
   }
   if(input == 263)
   {
      //printf("%s-", st);
      len--;
      cnt--;
   }
   else
   {
      st[len++] = input;
      cnt++;
   }
   for(int i = ed.x-Dis; i < strlen(ed.Buffer[y]); i++)
   {
      st[len++] = ed.Buffer[y][i];
      cnt++;
   }
   if(y+1 < ed.line)
      st[len++] = '\n';
 
   for(int i = y+1; i < ed.line; i++)
   {
      for(int j = 0; j < strlen(ed.Buffer[i]); j++)
         st[len++] = ed.Buffer[i][j];
      st[len++] = '\n';   
   }
   //st[len++] = (ed.line) + '0';
   //Put_in_clipboard(st);
   //printf("%s--", st);
   //exit(0);
   
   Put_st_in_ed(st);

   if(input != 263 && ed.x + 1 == Max_Line_Size + Dis)
   {
      ed.y++;
      ed.x = Dis;
   }
   else if(input != 263)
     ed.x++;
   else
      ed.x--;  
   //ed.line = max(ed.line, y+1);
   memset(ed.Bar, '\0', sizeof ed.Bar); 
}

void remove_slct()
{
   ed.save = 0;
   copy_ed();
   //if(mny < y && y < mxy)
     // return 1;
   char st[N] = {"\0"};  
   for(int i = ed.st_line; i < ed.st_line + 10; i++)
   {

      bool fl = false;
      for(int j = 0; j < strlen(ed.Buffer[i]); j++)
      {
         int y, x;
         getyx(stdscr, y, x);
         if(!is_select(i-ed.st_line, j+Dis))
         {
            st[strlen(st)] = ed.Buffer[i][j];
            fl |= (j == strlen(ed.Buffer[i])-1);
         }
      }
      if(fl)
         st[strlen(st)] = '\n';
   }
   //printf("%s", ed.file_name);
   Put_st_in_ed(st);
   ed.y = 0;
   ed.x = Dis;
}

void past_clipboard()
{
   ed.save = 0;
   copy_ed();
   char st[N] = {"\0"}; 
   
   FILE *fp;
   fp = fopen("clipboard.txt", "r");
   get_all_char(st, fp);
   fclose (fp);
   
   for(int i = 0; i < strlen(st); i++)
      insert_input(st[i]);

   //ed.y = 0;
   //ed.x = Dis;

}

void error()
{
   memset(ed.Bar, '\0', sizeof ed.Bar); 
   char error[] = "invalid command";
   for(int i = 0; i < strlen(error); i++)
      ed.Bar[i] = error[i];
}

void get_all_ed(char* st)
{
   int cnt = 0;
   for(int i = 0; i < ed.line; i++)
   {
      for(int j = 0; j < strlen(ed.Buffer[i]); j++)
         st[cnt++] = ed.Buffer[i][j];
      st[cnt++] = '\n';   
   }

   return;
}


void save()
{
   if(ed.file_name == NULL)
   {
      error();
      return;   
   }
   char st[N] = {'\0'};
   get_all_ed(st);
 
   Put(st, ed.file_name);
   ed.save = 1;
   
   memset(ed.Bar, '\0', sizeof ed.Bar); 

}

void saveas(char *pt)
{
   char st[N] = {"\0"};
   int len = strlen(pt);
   if(!creatfile(pt))
   {
      error();
      return;
   }
   for(int i = 0; i < len; i++)
      pt[i] = ed.Bar[i+9];
   pt[len] = '\0';   
   get_all_ed(st);
   Put(st, pt);
   ed.save = 1;

   //ed.file_name = pt; 
   for(int i = 0; i < strlen(pt); i++)
      ed.file_name[i] = pt[i];
;
   memset(ed.Bar, '\0', sizeof ed.Bar); 
}


void open(char *pt)
{
   if(pt[0] == '/')
      pt++;
     
   if(!valid(pt) || !exists(pt))
   {
      error();
      return;
   }

   if(ed.file_name != NULL)
      save();

   init(pt);
}


void undo_ed()
{
   tmp = ed;
   ed = ed_copy;
   ed_copy = tmp;
   ed.mode = 'n';
   ed.save = 0;
   memset(ed.Bar, '\0', sizeof ed.Bar);

}

void clear_found()
{
   for(int i = 0; i < ed.line; i++)
   {
      memset(found[i], 0, sizeof found[i]);
      memset(mark[i], 0, sizeof mark[i]);
   }
}

void find_expression(char* q)
{
   if(q[0] == '/')
      q++;
   ed.mode = 'f';
 
   bool fl = true;
   for(int i = ed.st_line; i < ed.st_line + 10; i++)
   {
      char *st = ed.Buffer[i];
     
      
      for(int j = 0; j < strlen(ed.Buffer[i]); j++)
      {
         if(!match(st+j, q))
            continue;
         fl = false;   
         mark[i][j] = 1;  
         //printf("%d %d  ", i, j); 
         for(int k = j; k < j+strlen(q); k++)
            found[i][k] = 1;   
      }
   }  
   if(fl)
   {
      error();
      return;
   }

   memset(ed.Bar, '\0', sizeof ed.Bar); 
}


void go_to_next_match()
{

   int y = ed.y, x = ed.x;
   y += ed.st_line;
   x -= Dis-1;
   //printf("%d %d  ", x, y);
   for(int i = y; i < ed.st_line + 10; i++)
   {
      int start = 0;
      if(i == y)
         start = x+1;
      for(int j = start; j < strlen(ed.Buffer[i]); j++)
      {
         if(mark[i][j])
         {
            ed.y = i-ed.st_line;
            ed.x = j+Dis;
            //printf("goto : %d %d  ", ed.x, ed.y);
            return;
         }
      }
   }  
   for(int i = ed.st_line; i < ed.st_line + 10; i++)
   {
      for(int j = 0; j < strlen(ed.Buffer[i]); j++)
      {
         
         if(mark[i][j])
         {
            ed.y = i-ed.st_line;
            ed.x = j+Dis;
            //printf("goto : %d %d  ", ed.x, ed.y);
            return;
         }
      }
   }  
}


void auto_indent_editor()
{
   ed.save = 0;
   copy_ed();
   //print(-1);
   char st1[N], st2[N] = {"\0"};
   get_all_ed(st1);
   //printf("%s-", st1);
   auto_indent(st1, st2);
   Put_st_in_ed(st2);
   memset(ed.Bar, '\0', sizeof ed.Bar); 
}


void process(char *q)
{
   if(q[0] == '/')
   {
      find_expression(q);
      return;
   }
   if(q[0] == ':')
      q++;
   //printf("%s", q);
   
   if(strcmp(q, "save") == 0)
   {
      save();
      return;
   }
   if(strlen(q) > 9 && q[0] == 's' && q[1] == 'a' && q[2] == 'v' && q[3] == 'e' && q[4] == 'a' && q[5] == 's')
   {
      char pt[N];
      for(int i = 0; i < strlen(q); i++)
         pt[i] = ed.Bar[i+9];
      saveas(pt);
      return;
   }
   if(strlen(q) > 5 && q[0] == 'o' && q[1] == 'p' && q[2] == 'e' && q[3] == 'n')
   {
      char pt[N];
      for(int i = 0; i < strlen(q); i++)
         pt[i] = ed.Bar[i+6];
      open(pt);
      return;
   }
   if(strcmp(q, "undo") == 0)
   {
      undo_ed();
      return;
   }
   if(strcmp(q, "auto-indent") == 0)
   {
      auto_indent_editor();
      return;
   }
   
   char ans[N] = "operation successful";
   char inval[N] = "invalid command";
   if(!query(q, ans))
   {
      for(int i = 0; i < N; i++)
         ans[i] = inval[i];
   }
   Put_st_in_ed(ans);
   
   

   memset(ed.Bar, '\0', sizeof ed.Bar); 
}

void handel(int input)
{
   if(ed.mode == 'f')
   {
      if(input == 'n')
      {
         go_to_next_match();
      }
      else if(input == 'x')
      {
         ed.mode = 'x';
      }
      else
      {
         clear_found();
         ed.mode = 'n';
      }
      return;
   }
   if(ed.mode == 'v')
   {
      if(input == 'x')
      {
         ed.mode = 'x';
         return;
      }
      if(input == 'i')
      {
         ed.mode = 'i';
         return;
      }
      if(input == 27)
      {
         ed.mode = 'n';
         return;
      }
      if(input == 'd')
      {
         Put_in_clipboard(slct);
         remove_slct();
         ed.mode = 'n';
         return;      
      }  
      if(input == 'y')
      {
         ed.mode = 'n';
         Put_in_clipboard(slct);
         return;
      }
      if(input == 'f')
      {
         ed.y = 0;
         ed.x = Dis;
         return;
      }
      if(input == 'e')
      {
         int last_line = 0;
         for(int i = ed.st_line; i < ed.st_line + 10; i++)
         {
            if(strlen(ed.Buffer[i]))
               last_line = i;
         }
         ed.y = last_line;
         ed.x = strlen(ed.Buffer[ed.y]) + Dis;
         return;
      }
     
      navigation(input);   
      return;
   }

   if(ed.mode == 'n')
   {
      if(input == 'x')
      {
         ed.mode = 'x';
         return;
      }
      if(input == 'i')
      {
         ed.mode = 'i';
         return;
      }
      if(input == 'v')
      {
         ed.mode = 'v';
         st_selectx = ed.x;
         st_selecty = ed.y+ed.st_line; 
         return;
      }
      if(input == '=')
      {
         auto_indent_editor();
         return;
      }
      if(input == ':' || input == '/')
      {
         ed.mode = 'm';
         memset(ed.Bar, '\0', sizeof ed.Bar);         
         ed.Bar[0] = input;
         return;
      }
      if(input == 'p')        // past
      {
         past_clipboard();
         return;
      }
      if(input == 'u')
      {
         undo_ed();
         return;
      }
      if(input == 'f')
      {
         ed.y = 0;
         ed.x = Dis;
         return;
      }
      if(input == 'e')
      {
         int last_line = 0;
         for(int i = ed.st_line; i < ed.st_line + 10; i++)
         {
            if(strlen(ed.Buffer[i]))
               last_line = i;
         }
         ed.y = last_line;
         ed.x = strlen(ed.Buffer[ed.y]) + Dis;
         return;
      }
      navigation(input);   
      return;
   }

   if(ed.mode == 'm')
   {
      if(input == 10)
      {
         ed.mode = 'n';
         process(ed.Bar);
         return;
      }
      ed.Bar[strlen(ed.Bar)] = input;
      return;
   }

   // insert
   //print(input);
   if(input == 27)
   {
      ed.mode = 'n';
      return;
   }
   insert_input(input);
}




int main(int argc, char* argv[])
{

   if(argc > 1)
   {          
      init(argv[1]);
   }
   else
   {
      init(NULL);
   }
   //("%s\n", ed.Buffer);
   curses_init();                

   while(ed.mode != 'x')
   {
      printBuff();
      int input = getch();
      handel(input);
   }

   refresh();                      
   endwin();                      

   return 0;
}