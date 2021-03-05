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
vector<vector<int>>s;// ��������� ������ ��� ������ ������� �� �����
int *s1;//���������� ������ ��� �������
char *bb1;// ���������� ������ ��� ������
int L; //������ (������� �������)
int int_max=INT_MAX;
void north(int x, int y);
void south(int x, int y);
void west(int x, int y);
void east(int x, int y);
int m;
int q=2; //�����, ������������ �������� �� ������� ��������������
float p;

void inMatrix ()//������ �������� �����
{
    ifstream fi(inputFileName);//���������� ��� ������ �����
    string str;// ���������� ��� ������ ��������� ������ �����
    for (int y=0;getline(fi,str);y++)//���� �������� ���� ���� ������ � �����
    {
        stringstream ss;//��������� ���������� ��� ��������� �����
        ss << str;//�������� ����������� ������ � ����
        int tmp;// ���������� ��� ���������� ��������
        for (int x=0; ss >> tmp; x++)// ���� ������������ ��� �������� � ������
        {
            if(y==0)// ���� ������ ������
                s.push_back(vector<int>(0));// ������� ����� ������ ������ ��� ������� �������� ������ ������
            s[x].push_back(tmp);// �������� ��������� ������� � ��������� ������, ��� ������������, ��� x �������� ��������, ��� y
        }
    }
    L = s.size();//���������� ������ �������
    for (int x=0;x<L;x++)// ���������� ��� x
        reverse (s[x].begin(),s[x].end());// �������������� ������, ����� �������� ����������� ���������� ����� ������ y=0
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
    fill(bb1,bb1+L*L, char (0));
    m=0;
    q=2;
}

int main()
{
    inMatrix();
    s1=new int [L*L];
    bb1=new char [L*L];
    initalg();
    for (int x=0;x<L;x++)
            north(x,0);
    printf("backbone mass= %d\n", m);
    if(m==0)
        printf ("cluster does not percolate !\n");
    outBB1Matrix(bb1, bbFileName);
    delete []s1;
    delete []bb1;
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
		printf ("cluster percolates !\n");
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
    if (y>=0)//� ������ ������������ �������� y>0
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
    if (x>=0)//� ������ ������������ �������� y>0
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