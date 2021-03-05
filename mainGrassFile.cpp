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
vector<vector<int>>s;// ��������� ������ ��� ������ ������� �� �����
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

void outMatrix(char* resultFileName)
{
    FILE *fo=fopen(resultFileName, "w");
    for (int y =L-1; y >=0; y--) {
        for (int x = 0; x <L; x++)
            fprintf(fo, "%4d ",s[x][y]);
        fprintf(fo, "\n");
    }
    fclose(fo);
}

int main()
{
    inMatrix();
	for (int x=0;x<L;x++)
            north(x,0);
    printf("backbone mass= %d\n", m);
    if(m==0)
        printf("cluster does not percolate !\n");
    outMatrix(resultFileName);
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
                    m--;
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
                    m--;
			}
			else s[x][y]=int_max;
		}
		else if (q>s[x][y]) q+=2;
	}
}

void south(int x,int y)
{
    if (y>=0)//� ������ ������������ �������� y>0
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
                    m--;
			}
			else s[x][y]=int_max;
		}
		else if (q>s[x][y]) q+=2;
    }
    else q++,q+=q&1;
}

void west (int x,int y)
{
    if (x>=0)//� ������ ������������ �������� y>0
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
                    m--;
			}
			else s[x][y]=int_max;
		}
		else if (q>s[x][y]) q+=2;
    }
}
