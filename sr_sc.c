#include<stdio.h>
void main()
{
        int a[3][3],i,j,sum1,sum2;
        for(i=0;i<3;i++)
        {
                for(j=0;j<3;j++)
                {
                        scanf("%d",&a[i][j]);
                }
        }
        for(i=0;i<3;i++)
        {
		sum1=sum2=0;
                for(j=0;j<3;j++)
                {
                        sum1+=a[i][j];
			sum2+=a[j][i];
                }
		printf("sum1=%d,sum2=%d",sum1,sum2);
              
        }

}

