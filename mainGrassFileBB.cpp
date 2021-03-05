#include<stdlib.h>
#include<limits.h>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include<algorithm>
using namespace std;
char inputFileName[] = "graph.txt";
char resultFileName[] = "result.txt";
char bbFileName[] = "backbone.txt";
vector<vector<int>>s;// Двумерный вектор для чтения решетки из файла
vector<vector<char>>bb;
int L; //размер (сторона решетки)
int int_max=INT_MAX;
void north(int x, int y);
void south(int x, int y);
void west(int x, int y);
void east(int x, int y);
int m;
int q=2; //метка, показывающая является ли кластер перколяционным
float p;

void inMatrix ()//Чтение входного файла
{
    ifstream fi(inputFileName);//переменная для чтения файла
    string str;// переменная для чтения очередной строки файла
    for (int y=0;getline(fi,str);y++)//цикл работает пока есть строки в файле
    {
        stringstream ss;//потоковая переменная для обработки строк
        ss << str;//помещаем прочитанную строку в файл
        int tmp;// переменная для очередного элемента
        for (int x=0; ss >> tmp; x++)// цикл обрабатывает все элементы в строке
        {
            if(y==0)// если первая строка
                s.push_back(vector<int>(0));// создаем новый пустой вектор для каждого элемента первой строки
            s[x].push_back(tmp);// помещаем очередной элемент в очередной вектор, это обеспечивает, что x меняется медленее, чем y
        }
    }
    L = s.size();//Определяем размер матрицы
    for (int x=0;x<L;x++)// перебираем все x
        reverse (s[x].begin(),s[x].end());// Переворачиваем вектор, чтобы элементы прочитанные последними имели индекс y=0
    int_max=L*L*8;
}

void outBBMatrix()
{
    FILE* fo =fopen(bbFileName, "w");
    for (int y =L-1; y >=0; y--) {
        for (int x = 0; x <L; x++)
            fprintf(fo, "%d ",int (bb[x][y]));
        fprintf(fo, "\n");
    }
    fclose(fo);
}

int main()
{
    inMatrix();
    bb=vector<vector<char>>(L,vector<char>(L));
    for (int x=0;x<L;x++)
            north(x,0);
    printf("backbone mass= %d\n", m);
    if(m==0)
        printf("cluster does not percolate !\n");
    outBBMatrix();
    return 0;
}

void north(int x,int y)
{
	if (y<L)
	{
		if (s[x][y]<2)
		{
		    if (s[x][y]==1)
            {
			    m++;
				s[x][y]=q;
                if ((q&1)==0)
                {
                    west(x-1 ,y);
                    north(x, y+1) ;
                    east(x+1,y);
                }
                else
                {
                    east(x+1,y);
                    north(x, y+1);
                    west(x-1 ,y);
                }
				if (q==s[x][y])
                    bb[x][y]=2, m--;
                else bb[x][y]=1;
            }
            else s[x][y]=int_max;
		}
		else if (q>s[x][y]) q+=2;
	}
	else
	{
	    q++,q+=(q&1)==0;
		printf ("cluster percolates !\n");
	}
}
void east (int x,int y)
{
	if (x<L)
	{
		if (s[x][y]<2)
		{
		    if (s[x][y]==1)
			{
			    m++;
			    s[x][y]=q;
                if ((q&1)==0)
                {
                    north(x , y+1) ;
                    east(x+1,y);
                    south(x ,y-1) ;
                }
                else
                {
                    south(x ,y-1) ;
                    east(x+1,y);
                    north(x , y+1) ;
                }
				if (q==s[x][y])
                    bb[x][y]=2, m--;
                else bb[x][y]=1;
			}
			else s[x][y]=int_max;
		}
		else if (q>s[x][y]) q+=2;
	}
}

void south(int x,int y)
{
    if (y>=0)//В статье Грассбергера ошибочно y>0
    {
        if (s[x][y]<2)
		{
		    if (s[x][y]==1)
			{
                m++;
			    s[x][y]=q;
                if ((q&1)==0)
                {
                    east(x+1,y);
                    south(x ,y-1) ;
                    west(x-1,y);
                }
                else
                {
                    west(x-1,y);
                    south(x ,y-1);
                    east(x+1,y);
                }
				if (q==s[x][y])
                    bb[x][y]=2, m--;
                else bb[x][y]=1;
			}
			else s[x][y]=int_max;
		}
		else if (q>s[x][y]) q+=2;
    }
    else q++,q+=q&1;
}

void west (int x,int y)
{
    if (x>=0)//В статье Грассбергера ошибочно y>0
    {
        if (s[x][y]<2)
		{
		    if (s[x][y]==1)
			{
                m++;
			    s[x][y]=q;
			    if ((q&1)==0)
                {
                    south(x ,y-1) ;
                    west(x-1,y);
                    north(x , y+1);
                }
                else
                {
                    north(x,y+1);
                    west(x-1,y);
                    south(x,y-1);
                }
				if (q==s[x][y])
                    bb[x][y]=2, m--;
                else bb[x][y]=1;
			}
			else s[x][y]=int_max;
		}
		else if (q>s[x][y]) q+=2;
    }
}
