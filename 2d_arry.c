#include<stdio.h>
void main()
{

	int a[2][2][2]={{{1,2},{3,4}},{{5,6},{7,8}}};


	int ((*p)[2])[1]=a;


	for(int i=0;i<2;i++)
	{

		for(int j=0;j<2;j++)
		{
			for(int k=0;k<2;k++)
				printf("%d",*((*((*p)+k))));
                      
                        p++;
                       
		}

			//			p++;
     }
}
