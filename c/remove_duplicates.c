/*Problem Statement
------------------------------------------------------------------------------------
Given an integer array nums sorted in non-decreasing order, remove the duplicates in-place such that each unique element appears only once. The relative order of the elements should be kept the same.

Consider the number of unique elements in nums to be k​​​​​​​​​​​​​​. After removing duplicates, return the number of unique elements k.

Example 1:
Input: nums = [1,1,2]
Output: 2, nums = [1,2,_]

Example 2:
Input: nums = [0,0,1,1,1,2,2,3,3,4]
Output: 5, nums = [0,1,2,3,4,_,_,_,_,_]
-----------------------------------------------------------------------------------
*/

#include<stdio.h>
void main()
{
	int a[10],ele,i,j,k,c=0;
	printf("Enter number of elements:\n");
	scanf("%d",&ele);
	printf("Enter the array elements:\n");
	for(i=0;i<ele;i++)
		scanf("%d",&a[i]);

	for(i=0;i<ele;i++)
	{
		for(j=i+1;j<ele;j++)
		{
			if(a[i]==a[j])
			{
				for(k=j;k<ele-1;k++)
				{
				a[k]=a[k+1];
				}
				ele--;
				j--;
				c++;
			}
		}
	}
	for(i=0;i<ele;i++)
		printf("%d ",a[i]);
	printf("\n");

	printf("Count=%d\n",c);
}
