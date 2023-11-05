
#ifndef PROGECT_H
#define PROGECT_H

#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <unistd.h>

#define print(x) printf("%d \n", x);
#define N 10005

char CWD[100];

/*
void invalid_command()
{
    printf("invalid command\n");
}
*/

bool exists(char *path)
{
    FILE *file;
    if ((file = fopen(path, "r")))
    {
        fclose(file);
        return 1;
    }
    return 0;
}
bool valid(char * path)
{
    return(path != NULL && strlen(path) >= 5 && path[0] == 'r' && path[1] == 'o' && path[2] == 'o' && path[3] == 't' && path[4] == '/');
}
void fix(char* token)
{
    if(token[0] == '"')
    {
        //printf("fix\n");
        
        for(int i = 0; i < strlen(token)-1; i++)
            token[i] = token[i+1];
        token[strlen(token)-2] = '\0';
       
    }
}
void goto_root()
{
    getcwd(CWD, 100);
    int sz = strlen(CWD);
    while(CWD[sz-1] != 't' || CWD[sz-2] != 'o' || CWD[sz-3] != 'o' || CWD[sz-4] != 'r' || CWD[sz-5] != '/')
    {
        chdir("..");
        getcwd(CWD, 100);
        sz = strlen(CWD);
    }
    chdir("..");
}

void goto_path(char * path) // staet with : /root
{
    //printf("%s-\n", path);
    path = strtok(path, "/");
  //  path = strtok(NULL, "/");
    while(path != NULL) 
    {
        //char *st = token;
        //printf("%s-\n", path);
        mkdir(path, S_IRWXU);
        chdir(path);
        path = strtok(NULL, "/");
        
    }   
}
void get_all_char(char* string, FILE *fp)
{
   
    char c;
    while((c = getc(fp)) != EOF)
    {
        //printf("%c !!!! \n", c);
        string[strlen(string)] = c;
    }
}
void get_all_char_pos(char* string, FILE *fp, int x, int y)
{
    char c;
    for(int i = 1; i < x; i++)
    {
        while((c = getc(fp)) != '\n') 
        {
            string[strlen(string)] = c;
        }
        string[strlen(string)] = c;
    }
    for(int j = 0; j < y; j++)
    {
        c = getc(fp);
        string[strlen(string)] = c;
      //  printf("%c\n", c);
    }

}

void Put_in_clipboard(char* st)
{
    FILE *fp;
    fp = fopen("clipboard.txt", "w");
    fputs(st, fp);
    fclose (fp);
}

void copy_file(char *path)
{
    FILE *fp, *fpcp;
    fp = fopen(path, "r");
    path[strlen(path)] = '*';
    fpcp = fopen(path, "w");
    char stt[10000] = {"\0"};
    get_all_char(stt, fp);
    //printf("%s\n", stt);
    fputs(stt, fpcp);
    fputs(stt, fp);
    path[strlen(path)-1] = '\0';
   
    fclose (fpcp);
    fclose(fp);
}


void Put(char* st, char* path)
{
    if(path[0] == '/')
        path++;
    //printf("%s\n", path);    
    //copy_file(path);
    FILE *fp;
    fp = fopen(path, "w");
    fputs(st, fp);
    fclose (fp);
    return;

}

bool creatfile(char *path)
{
    if(!valid(path) || exists(path))
    {
        return 0;   
    }
    
    int stfile = 0;
    for(int i = strlen(path)-1; i >= 0; i--)
    {
        if(path[i] == '/')
        {
            stfile = i;
            break;
        }
    }
    
    //printf("%d %s-\n", stfile, path);
    path[stfile] = '\0';
    //print(-1);
    goto_path(path);
  

    path[stfile] = '/';
    path += stfile+1;
    FILE *fp;
    fp = fopen(path, "w+");
    fclose (fp);

    path[strlen(path)] = '*';
    fp = fopen(path, "w+");
    fclose (fp);

    goto_root();
    return 1;
    //path[strlen(path)] = '\0';
   
}


bool match(char* st1, char* st2)
{
    if(strlen(st2) > strlen(st1))
        return 0;
    for(int i = 0; i < strlen(st2); i++)
    {
        if(st1[i] != st2[i])
            return 0;
    }
    return 1;
}

bool match2(char* a, char* b, int len)
{
    if(strlen(b) > len)
        return 0;

    if(b[0] == '*')
    {
        b++;
        int j = 0;
        for(int i = len-strlen(b); i < len; i++)
        {
            if(b[j++] != a[i])
                return 0;

        }
        return 1;
    }
    if(b[strlen(b)-1] == '*' && b[strlen(b)-2] != '\\')
    {
        for(int i = 0; i < strlen(b)-1; i++)
        {
            if(a[i] != b[i])
                return 0;
        }
        return 1;
    }
    if(strlen(b) != len)
        return 0;
    
    //for(int i = 0; i < len; i++)
      //  printf("%c", a[i]);
    //printf("\n");
    
    for(int i = 0; i < len; i++)
    {
        if(b[i] != a[i])
            return 0;
    }    
    
    return 1;

}

void tab_to_space(char* st)
{
    char st2[N] = {"\0"};
    int cnt = 0;
    for(int i = 0; i < strlen(st); i++)
    {
        if(st[i] == '\t')
        {
            st2[cnt++] = ' ';
            st2[cnt++] = ' ';
            st2[cnt++] = ' ';
            st2[cnt++] = ' ';
        }
        else
            st2[cnt++] = st[i];
    }
   
    for(int i = 0; i < strlen(st2); i++)
    {
        st[i] = st2[i];
    }
}

void auto_indent(char *st1, char *st2)
{
    int i = 0, cnt_tab = 0;
    while(i < strlen(st1))
    {
        //printf("%d\n%s", i, st2);
        bool fl_char = 0;
        while(st1[i] == ' ' || st1[i] == '\t')  
            i++;
        if(st1[i] == '\n')
            goto hell;
        // insert tabs
        for(int t = 0; t < cnt_tab; t++)
            st2[strlen(st2)] = '\t';

        while(i < strlen(st1) && st1[i] != '\n')
        {
            if(st1[i] == '{')
            {
                while(strlen(st2) && fl_char && (st2[strlen(st2)-1] == ' ' || st2[strlen(st2)-1] == '\t'))
                    st2[strlen(st2)-1] = '\0';
                if(fl_char)        
                    st2[strlen(st2)] = ' ';   
                st2[strlen(st2)] = '{';
                st2[strlen(st2)] = '\n';   
                cnt_tab++;    
                goto hell;
            }    
            else if(st1[i] == '}')
            {
                st2[strlen(st2)] = '\n';
                cnt_tab--;
                // insert tabs
                for(int t = 0; t < cnt_tab; t++)
                    st2[strlen(st2)] = '\t';

                st2[strlen(st2)] = '}';
                st2[strlen(st2)] = '\n';
                goto hell;   
            }
            else if(st1[i] != ' ' && st1[i] != '\t')
                fl_char = 1;

            st2[strlen(st2)] = st1[i];
            i++;
        }
        hell:
        i++;
    }
    tab_to_space(st2);
  
    
}

bool closing_pairs(char* path)
{
    if(!valid(path) || !exists(path))
    {
        //invalid_command();
        return 0;
    }
    char st2[10000] = {"\0"}, st1[1000] = {"\0"};

    FILE *fp;
    fp = fopen(path, "r");
    get_all_char(st1, fp);
    fclose (fp);

    auto_indent(st1, st2);

    //printf("%s\n", path);

    copy_file(path);
    fp = fopen(path, "w");
    //printf("%s", string);
    fputs(st2, fp);
    //for(int i = 0; i < strlen(st2); i++)
      //  ans[i] = st2[i];
    fclose (fp);
    return 1;
}


bool insert(char *path, char *str, int x, int y)
{

    if(!valid(path) || !exists(path) || x == -1 || y == -1 || str == NULL)
    {
        //invalid_command();
        return 0;
    }
   
    copy_file(path);

    FILE *fp;
    fp = fopen(path, "r");

    char string[10000] = {"\0"};

    get_all_char_pos(string, fp, x, y);
    for(int i = 0; i < strlen(str); i++)
    {
        //if(str[i] != '\"')
        string[strlen(string)] = str[i];
    }

    get_all_char(string, fp);
    //printf("%s", string);
    fclose (fp);
    fp = fopen(path, "w");
    fputs(string, fp);
    fclose (fp);
    return 1;
}

bool cat(char *path, char *ans)
{
    //printf("%s\n", path);
    if(!valid(path) || !exists(path))
    {
        return 0;
    }

    FILE *fp;
    fp = fopen(path, "r");
    char string[10000] = {"\0"};
    get_all_char(string, fp);
    

    for(int i = 0; i < strlen(string); i++)
        ans[i] = string[i];

    fclose (fp);
    return 1;
    
}

bool removestr(char *path, int x, int y, int dir, int size_num)
{

    if(!valid(path) || !exists(path) || x == -1 || y == -1 || size_num == -1 || dir == -1)
    {
        return 0;
    }
    FILE *fp;
    fp = fopen(path, "r");

    char string[10000] = {"\0"};
    get_all_char_pos(string, fp, x, y);
    if(dir)
    {
        if(strlen(string) < size_num)
        {
            return 0;
        }
        for(int i = 0; i < size_num; i++)
          string[strlen(string)-1] = '\0';
        
    }
    else
    {
        char c;
        for(int i = 0; i < size_num; i++)
        {
            c = getc(fp);
            if(c == EOF)
            {
                //invalid_command();
                return 0;
            }
        }

    }

    get_all_char(string, fp);
    fclose (fp);

    copy_file(path);

    fp = fopen(path, "w");
    //printf("%s", string);
    fputs(string, fp);
    fclose (fp);
    return 1;


}


bool copy(char *path, int x, int y, int dir, int size_num)
{
   // printf("%s %d %d %d %d \n", path, x, y, size_num, dir);
    if(!valid(path) || !exists(path) || x == -1 || y == -1 || size_num == -1 || dir == -1)
    {
        //invalid_command();
        return 0;
    }

    FILE *fp;
    fp = fopen(path, "r");

    char tmp[10000] = {"\0"}, string[10000] = {"\0"};
    get_all_char_pos(tmp, fp, x, y);
    

    if(dir)
    {
        if(strlen(tmp) < size_num)
        {
            //invalid_command();
            return 0;
        }
        int k = size_num;
        for(int i = 0; i < size_num; i++)
        {
          string[i] = tmp[strlen(tmp)-k];
          k--;
        }
    }
    else
    {
       
        for(int i = 0; i < size_num; i++)
        {
            string[i] = getc(fp);
            if(string[i] == EOF)
            {
                //invalid_command();
                return 0;
            }
        }

    }
    //printf("%s", string);
    fclose (fp);
    Put_in_clipboard(string);
    return 1;

}

bool cut(char *path, int x, int y, int dir, int size_num)
{
    return (copy(path, x, y, dir, size_num) && removestr(path, x, y, dir, size_num));
}

bool paste(char *path, int x, int y)
{
    if(!valid(path) || !exists(path))
    {
        //invalid_command();
        return 0;
    }
    FILE *fp;
    fp = fopen("clipboard.txt", "r");
    char string[10000] = {"\0"};
    //printf("%s\n", str);
    get_all_char(string, fp);
    fclose (fp);
    insert(path, string, x, y);
    return 1;

}

bool find(char *path, char *str, bool all, bool count, bool byword, bool at)     // count (at n) byword all              //find -str hiva -file /root/a.txt -all -byword
{
    if(!valid(path) || !exists(path) || str == NULL)
    {
        //invalid_command();
        return 0;
    }

    FILE *fp;
    fp = fopen(path, "r");
    char string[10000] = {"\0"};
    get_all_char(string, fp);
    fclose (fp);

    //printf("%s\n\n\n", string);

    if(count + (at != 0) + all > 1)
    {
        //invalid_command();
        return 0;
    }
    if(!count && !all)
        at = 1;

    int cnt = 0, cnt_vaj = 0, ind_vaj = 0;
    
    for(int i = 0; i < strlen(string); i++)
    {
        while(string[i] == ' ' || string[i] == '\n')
            continue;
        //printf("start of word : %d\n", i);

        int j = i; 
        bool fl = 0;   
        ind_vaj++;    
        while(string[j] != '\n' && string[j] != ' ' && j < strlen(string))
        {
            //*
            for(int k = i; k <= j; k++)
            {
                if(match2(string+k, str, j-k+1))
                {
                    fl = 1;
                    cnt++;
                    if(!byword && cnt == at)
                    {
                        print(k);
                        return 1;
                    }

                    if(!byword && all)
                    {
                        //printf("%d %d\n", k, j);
                        if(cnt > 1)
                            printf(", ");
                        printf("%d", k);
                    }
                }

            }
            //*/
            //printf("%d %d %c\n", (int)(strlen(string)), j, string[j]);
            j++;
        }
        cnt_vaj += fl;
        if(byword && cnt_vaj == at)
        {
            print(ind_vaj);
            return 1;
        }
        if(byword && all && fl)
        {
            if(cnt_vaj > 1)
                printf(", ");
            printf("%d", ind_vaj);
        }

        i = j;
    }

    if(byword)
    {
        if(count)
            print(cnt_vaj);
        if(at != 0 && cnt_vaj < at)
            print(-1);
    }
    else
    {
        if(count)
            print(cnt);
        if(at != 0 && cnt < at)
            print(-1);
    }

 
    if(all)
        printf("\n");

    return 1; 
}


bool find_match(char *l, char* str)
{
    for(int i = 0; i < strlen(l); i++)
    {
        if(match(l+i, str))
        {
            //printf("find : %s---%d---%s\n", l, i, str);
            return 1;
        }
    }
    return 0;
}

bool grep(char *path, char *str, bool L, bool C, char *ans)
{
    int ans_len = 0;
    ans[1] = ans[0] = '\0';
    if(str == NULL)
    {
        //invalid_command();
        return 0;
    }
    
    char copy_path[100] = {"\0"};
    for(int i = 0; i < strlen(path); i++)
        copy_path[i] = path[i];

    char *token = strtok(path, " ");
    while(token != NULL)
    {
        token++;
        //printf("%s-\n", token);
        if(!valid(token) || !exists(token))
        {
            //invalid_command();
            return 0;
        }
        token = strtok(NULL, " ");
    }

    
    path = copy_path;
    //printf("%s-\n", path);
    int cnt = 0;
    char* token2 = strtok(path, " ");
    while(token2 != NULL)
    {
        token2++;

        //printf("%s--\n", token2);
        
        FILE *fp;
        fp = fopen(token2, "r");
        char string[10000] = {"\0"};
        get_all_char(string, fp);
        fclose (fp);
        //printf("%s--\n", string);

        int st = 0;
        for(int i = 0; i < strlen(string); i++)
        {
            char line[N] = {"\0"};
            while(i < strlen(string) && string[i] != '\n')
            {
                line[strlen(line)] = string[i];
                i++;
            }
            //printf("line : %s\n", line);
            if(find_match(line, str))
            {
                cnt++;
                if(!C)
                {
                    for(int j = 0; j < strlen(token2); j++)
                        ans[ans_len++] = token2[j];   
                    //printf("%s", token2);
                }
                if(!C && !L)
                {
                    ans[ans_len++] = ':';
                    ans[ans_len++] = ' ';
                    //printf(": ");        
                }
                if(!C && !L)
                {
                    for(int j = 0 ; j < strlen(line); j++)
                        ans[ans_len++] = line[j]; 
                }
                if(!C)
                {
                    ans[ans_len++] = '\n';
                    //printf("\n");
                }
            }
        }

        token2 = strtok(NULL, " ");
    }
    if(C)
    {
        while(cnt)
        {
            int x = cnt%10;
            char y = (char)(x + '0');
            ans[ans_len++] = y;
            cnt /= 10;

        }
        //print(cnt);
    }

    return 1;


}

bool undo(char *path)
{
    if(!valid(path) || !exists(path))
    {
        //invalid_command();
        return 0;
    }

    FILE *fp, *fpcp;
    
    //printf("%s\n", path);
    int len = strlen(path);
    path[len] = '*';
    path[len + 1] = '\0';
    //printf("%s\n", path);


    fpcp = fopen(path, "r");
    char stt[10000] = {"\0"};
    get_all_char(stt, fpcp);
    path[strlen(path)-1] = '\0';

    fclose (fpcp);

   // print(-1);

    copy_file(path);
   
    fp = fopen(path, "w");
    fputs(stt, fp);
    fclose (fp);
    return 1;


}

int get_cnt_line(char *st)
{
    int ans = 0;
    for(int i = 0; i < strlen(st); i++)
        ans += (st[i] == '\n');
    return ans;    
}

bool compare(char *path, char *ans)
{
    int ans_len = 0;
    ans[1] = ans[0] = '\0';
    char p1[10000] = {"\0"}, p2[10000] = {"\0"};
    int ii = 0;
    while(path[ii] != ' ')
    {
        p1[strlen(p1)] = path[ii];
        //printf("%c\n", path[ii]);
        ii++;
    }
    path += 2;
    while(ii < strlen(path))
    {
        p2[strlen(p2)] = path[ii];
        ii++;
    }

    if(!valid(p1) || !exists(p1) || !valid(p2) || !exists(p2))
    {
        //invalid_command();
        return 0;
    }

    char st1[10000] = {"\0"}, st2[10000] = {"\0"};
    FILE *fp;
    fp = fopen(p1, "r");
    get_all_char(st1, fp);
    fclose (fp);

    fp = fopen(p2, "r");
    get_all_char(st2, fp);
    fclose (fp);
    //printf("%s-\n%s-\n", st1, st2);
    //return; 
    int i = 0, j = 0, l = 1; 
    while(l < strlen(st1) && i < strlen(st1) && j < strlen(st2))
    {
        bool fl = true;
        int t_i = i, t_j = j;
        while(fl && st1[t_i] != '\n' && st2[t_j] != '\n')
        {
            if(st1[t_i] != st2[t_j])
                fl = false;     

            t_i++;
            t_j++;
        }    
        if(st1[t_i] != '\n' || st2[t_j] != '\n')
            fl = false;
        if(st1[t_i] == '\n' && st2[t_j] == '\n')
            fl = true;    

        if(!fl)
        {
            char *tmp = "============ #";
            char indline = (char)('0' + l);
            char *tmp2 = " ============\n";
            //tmp[14] = indline;
            for(int i = 0; i < strlen(tmp); i++)
                ans[ans_len++] = tmp[i];
            ans[ans_len++] = indline;
            for(int i = 0; i < strlen(tmp2); i++)
                ans[ans_len++] = tmp2[i]; 
            //printf("%s%c%s\n", tmp, indline, tmp2);

        }

        while(i < strlen(st1) && st1[i] != '\n')
        {
            if(!fl)
                ans[ans_len++] = st1[i]; //printf("%c", st1[i]);
            i++;
        }
        i++;
        if(!fl)
            ans[ans_len++] = '\n'; //printf("\n");

        while(j < strlen(st2) && st2[j] != '\n')
        {
            if(!fl)
                ans[ans_len++] = st2[j]; //printf("%c", st2[j]);
            j++;
        }
        j++;
        if(!fl)
            ans[ans_len++] = '\n'; //printf("\n");
        l++;
       
    }
    //printf("%d %d\n", i, j);
    if(i != strlen(st1))
    {
        //printf("<<<<<<<<<<< #%d - #%d <<<<<<<<<<<\n", l, get_cnt_line(st1));
        char *tmp = "<<<<<<<<<<< #";
        char indline = (char)('0' + l);
        char indline2 = (char)('0' + get_cnt_line(st1));
        char *tmp2 = " <<<<<<<<<<<\n";
        
        for(int i = 0; i < strlen(tmp); i++)
            ans[ans_len++] = tmp[i];
        ans[ans_len++] = indline;
        ans[ans_len++] = ' ';
        ans[ans_len++] = '-';
        ans[ans_len++] = ' ';
        ans[ans_len++] = '#';
        ans[ans_len++] = indline2;
        for(int i = 0; i < strlen(tmp2); i++)
            ans[ans_len++] = tmp2[i]; 


        while(i < strlen(st1))
        {
            while(i < strlen(st1))
                ans[ans_len++] = st1[i++]; //printf("%c", st1[i++]);
            l++;
            i++;
        }
        ans[ans_len++] = '\n'; //printf("\n");

    }


    if(j != strlen(st2))
    {
        //printf(">>>>>>>>>>>> #%d - #%d >>>>>>>>>>>>n", l, get_cnt_line(st2));
        
        char *tmp = ">>>>>>>>>>>>#";
        char indline = (char)('0' + l);
        char indline2 = (char)('0' + get_cnt_line(st2));
        char *tmp2 = " >>>>>>>>>>>>\n";
        
        for(int i = 0; i < strlen(tmp); i++)
            ans[ans_len++] = tmp[i];
        ans[ans_len++] = indline;
        ans[ans_len++] = ' ';
        ans[ans_len++] = '-';
        ans[ans_len++] = ' ';
        ans[ans_len++] = '#';
        ans[ans_len++] = indline2;
        for(int i = 0; i < strlen(tmp2); i++)
            ans[ans_len++] = tmp2[i]; 
       
       
       
        while(j < strlen(st2))
        {
            //printf("============ #%d ===========\n", l);
            while(j < strlen(st2))
                ans[ans_len++] = st2[j++];//printf("%c", st2[j++]);
            l++;
            j++;
        }
        ans[ans_len++] = '\n'; 
    }
    return 1;
}


////////////////////////////////////////////////



int _file_(char** token, char** path)
{
    if((*token)[0] != 'f' || (*token)[1] != 'i' || (*token)[2] != 'l' || (*token)[3] != 'e')
        return 0;
    (*token) += 5;
    if((*token)[strlen((*token))-1] == ' ')
        (*token)[strlen((*token))-1] = '\0';
    fix((*token));
    (*token)++;
    (*path) = (*token);
    //printf("path : %s- \n", (*path));
    return 1;
}

int _pos_(char** token, int *x, int *y)
{
    //printf("%s\n", (*token));
    if((*token)[0] != 'p' || (*token)[1] != 'o' || (*token)[2] != 's')
        return 0;
    (*token) += 4;
    *x = (int)((*token)[0] - '0');
    *y = (int)((*token)[2] - '0');
    return 1;

}

int _size_(char** token, int *size_num)
{
    if((*token)[0] != 's' || (*token)[1] != 'i' || (*token)[2] != 'z' || (*token)[3] != 'e')
        return 0;
    (*token) += 5;
    *(size_num) = (int)((*token)[0] - '0');
    return 1;

}

int _borf_(char** token, int *dir)
{
    if((*token)[0] == 'b')
        *dir = 1;
    else if((*token)[0] == 'f')
        *dir = 0;
    else
        return 0;
    return 1;    
}


int _str_(char** token, char** str)
{
    if((*token)[0] != 's' || (*token)[1] != 't' || (*token)[2] != 'r')
        return 0;
    (*token)[strlen((*token))-1] = '\0';
    (*token) += 4;
    fix((*token));    
    (*str) = (*token);
    return 1;
}

int _C_(char** token, bool *C)
{
    if((*token)[0] != 'C')
        return 0;
    return *C = 1;
}

int _L_(char** token, bool *L)
{
    if((*token)[0] != 'l')
        return 0;
    return *L = 1;
}

int _find_(char** token, bool *count, bool *byword, bool *all, int *at)
{
    if((*token)[0] == 'c' && (*token)[1] == 'o' && (*token)[2] == 'u' && (*token)[3] == 'n' &&  (*token)[4] == 't')
    {
        *count = 1;
        return 1;
    }

    if((*token)[0] == 'b' && (*token)[1] == 'y' && (*token)[2] == 'w' && (*token)[3] == 'o' && (*token)[4] == 'r' && (*token)[5] == 'd')
    {
        *byword = 1;
        return 1;
    }
    if((*token)[0] == 'a' && (*token)[1] == 'l' && (*token)[2] == 'l')
    {
        *all = 1;
        return 1;
    }
    if((*token)[0] != 'a' || (*token)[1] != 't' || (*token)[2] != ' ')
    {
        return 0;
    }
    //printf("fuck %s\n", (*token));
    //printf("%c\n", (*token)[0]);
    (*token) += 3;
    *at = (int)((*token)[0] - '0');

    return 0;
}


bool query(char *q, char* ans)
{
    char *str, *path, *type;
    int dir, x, y, size_num, at;
    bool C, L, count, byword, all;
    str = path = NULL;
    dir = x = y = size_num = -1;
    count = at = all = byword = C = L = 0;
    char* token = strtok(q, " ");
    type = token;
    token++;
    token = strtok(NULL, "-");
    
    while(token != NULL)
    {
        //printf("tolen : %s-\n", token);
        if(_file_(&token, &path))
            goto hell;
        else if(_find_(&token, &count, &byword, &all, &at))
            goto hell;            
        else if(_str_(&token, &str))
            goto hell;
        else if(_size_(&token, &size_num))
            goto hell;
        else if(_pos_(&token, &x, &y))
            goto hell;
        else if(_borf_(&token, &dir))
            goto hell;  
        else if(_C_(&token, &C))
            goto hell;
        else if(_L_(&token, &L))
            goto hell;
        //printf("path : %s- \n", path);
        hell:
        token = strtok(NULL, "-");
    }

    //printf("/////////////////// \ntype : %s\nstr : %s-\npath : %s-\nx : %d\ny : %d\ndir : %d\nsizre : %d\nC : %d\nL : %d\n",type, str, path, x, y, dir, size_num, C, L);
    //printf("conut : %d\nat : %d\nbyword : %d\nall : %d\n////////////////////////////////\n", count, at, byword, all);

    if(strcmp(type, "createfile") == 0)             // createfile -file /root/t.txt
    {
        return creatfile(path);
    }
    else if(strcmp(type, "insertstr") == 0)         // insertstr -file /root/a.txt -str "akfj; ajijak  alfkj" -pos 1:0
    {
        return insert(path, str, x, y);
    }
    else if(strcmp(type, "cat") == 0)               // cat -file /root/dir1/dir2/file.txt
    {                                               // cat -file /root/a.txt
        //printf("%s\n\n", ans);                        
        return cat(path, ans);
    }   
    else if(strcmp(type, "removetstr") == 0)        // removetstr -file /root/a.txt -pos 2:3 -b -size 9
    {
        return removestr(path, x, y, dir, size_num); 
    }
    else if(strcmp(type, "copy") == 0)              // copy -file /root/dir1/dir2/file.txt -pos 2:5 -b -size 3
    {                                               // copy -file /root/a.txt -pos 2:5 -b -size 3
        return copy(path, x, y, dir, size_num);
    }
    else if(strcmp(type, "cut") == 0)               // cut -file /root/dir1/dir2/file.txt -pos 2:5 -b -size 3
    {   
        return cut(path, x, y, dir, size_num);             
    }
    else if(strcmp(type, "pastestr") == 0)          // pastestr -file /root/dir1/dir2/a.txt -pos 1:0
    {                                               // pastestr -file /root/a.txt -pos 1:0
        return paste(path, x, y);
    }
    else if(strcmp(type, "find") == 0)              // find -str hiva -file /root/a.txt -all
    {
        return find(path, str, all, count, byword, at);
    }
    else if(strcmp(type, "grep") == 0)              //  grep -str salam -files /root/b.txt /root/a.txt
    {
        return grep(path, str, L, C, ans);
    }
    else if(strcmp(type, "undo") == 0)              // undo -file /root/a.txt
    {
        return undo(path);
    }  
    else if(strcmp(type, "compare") == 0)           // compare -file /root/a.txt /root/t.txt
    {
        return compare(path, ans);
    }
    else if(strcmp(type, "auto-indent") == 0)       // auto-indent -file /root/a.txt
    {
        return closing_pairs(path);
    }
    return 0;
}






#endif
