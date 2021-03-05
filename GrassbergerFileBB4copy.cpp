#include<stdlib.h>
#include<limits.h>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include<algorithm>
#include <ctime>
using namespace std;
char inputFileName[] = "graph.txt";
char resultFileName[] = "result.txt";
char bbFileName[] = "backbone.txt";
vector<vector<int>>s;// Двумерный вектор для чтения решетки из файла
int L; //размер (сторона решетки)
int *s1;
char *bb1, *clearbb1;
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

void outBB1Matrix(char *bb1, const char *bbFileName)
{
    FILE* fo =fopen(bbFileName, "w");
    for (int y =L-1; y >=0; y--) {
        for (int x = 0; x <L; x++)
            fprintf(fo, "%2d ",int (bb1[x*L+y]));
        fprintf(fo, "\n");
    }
    fclose(fo);
}

void initalg()
{
    for (int x=0;x<L;x++)
        copy(s[x].begin(),s[x].end(),s1+x*L);
    fill(bb1,bb1+L*L, char(0));
    m=0;
    q=2;
}

void addclearbb1()
{
    for(int i=0;i<L*L;i++)
        clearbb1[i]*=bb1[i];
}

int countbb1(char* bb1)
{
    m=0;
    for(int i=0;i<L*L;i++)
        m+=bb1[i]==1;
    return m;
}

int main()
{
    clock_t t1, t2;
    inMatrix();
	t1=clock();
    s1=new int [L*L];
    bb1=new char [L*L];

    initalg();
    for (int x=0;x<L;x++)
        north(x,0);
    clearbb1=new char[L*L];
    copy (bb1,bb1+L*L,clearbb1);

    initalg();
    reverse(s1,s1+L*L);// поворот матрицы на 180 градусов
	for (int x=0;x<L;x++)
        north(x,0);
	reverse(bb1,bb1+L*L);// обратный поворот остова на 180 градусов
    addclearbb1();

    initalg();
    for(int i=0;i<L;i++)// вертикальная симметрия матрицы
        {reverse(s1+L*i,s1+L*i+L);}
	for(int x=0;x<L;x++)
        north(x,0);
	for(int i=0;i<L;i++)// обратная вертикальная симметрия остова
        reverse(bb1+L*i,bb1+L*i+L);
    addclearbb1();

    initalg();
    for(int i=0;i<L/2;i++)// горизонтальная симметрия матрицы
        {swap_ranges(s1+L*i,s1+L*i+L,s1+L*(L-i-1));}
	for(int x=0;x<L;x++)
        north(x,0);
	for(int i=0;i<L/2;i++)// обратная горизонтальная симметрия остова
        swap_ranges (bb1+L*i,bb1+L*i+L,bb1+L*(L-i-1));
    addclearbb1();
	t2=clock();
    printf("backbone mass= %d\n", countbb1(clearbb1));
    if(m==0)
        printf("cluster does not percolate !\n");
    else
        printf("cluster percolates !\n");
    printf("runtime = %f\n",(t2-t1)/double(CLOCKS_PER_SEC));
    outBB1Matrix(clearbb1, bbFileName);
    delete[]s1;
    delete[]bb1;
    delete[]clearbb1;
    return 0;
}

void north(int x,int y)
{
	if (y<L)
	{
		if (s1[x*L+y]<2)
		{
		    if (s1[x*L+y]==1)
            {
			    m++;
				s1[x*L+y]=q;
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
				if (q==s1[x*L+y])
                    bb1[x*L+y]=2, m--;
                else bb1[x*L+y]=1;
            }
            else s1[x*L+y]=int_max;
		}
		else if (q>s1[x*L+y]) q+=2;
	}
	else
	{
	    q++,q+=(q&1)==0;
		//printf ("cluster percolates !\n");
	}
}
void east (int x,int y)
{
	if (x<L)
	{
		if (s1[x*L+y]<2)
		{
		    if (s1[x*L+y]==1)
			{
			    m++;
			    s1[x*L+y]=q;
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
				if (q==s1[x*L+y])
                    bb1[x*L+y]=2, m--;
                else bb1[x*L+y]=1;
			}
			else s1[x*L+y]=int_max;
		}
		else if (q>s1[x*L+y]) q+=2;
	}
}

void south(int x,int y)
{
    if (y>=0)//В статье Грассбергера ошибочно y>0
    {
        if (s1[x*L+y]<2)
		{
		    if (s1[x*L+y]==1)
			{
                m++;
			    s1[x*L+y]=q;
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
				if (q==s1[x*L+y])
                    bb1[x*L+y]=2, m--;
                else bb1[x*L+y]=1;
			}
			else s1[x*L+y]=int_max;
		}
		else if (q>s1[x*L+y]) q+=2;
    }
    else q++,q+=q&1;
}

void west (int x,int y)
{
    if (x>=0)//В статье Грассбергера ошибочно y>0
    {
        if (s1[x*L+y]<2)
		{
		    if (s1[x*L+y]==1)
			{
                m++;
			    s1[x*L+y]=q;
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
				if (q==s1[x*L+y])
                    bb1[x*L+y]=2, m--;
                else bb1[x*L+y]=1;
			}
			else s1[x*L+y]=int_max;
		}
		else if (q>s1[x*L+y]) q+=2;
    }
}
