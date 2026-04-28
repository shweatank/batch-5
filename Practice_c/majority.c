#include<stdio.h>
//majority mean number of elements more than n/2
int major(int *p,int size)
{
	int i,can,cnt=0;
	for(i=0;i<size;i++)
	{
		if(cnt==0)
			can=p[i];
		if(p[i]==can)
			cnt++;
		else
			cnt--;
	}
	return can;

}
/*
int major(int* nums, int size)
{
    int count = 0, candidate = 0;

    // Phase 1: Find candidate
    for (int i = 0; i < size; i++)
    {
        if (count == 0)
            candidate = nums[i];

        if (nums[i] == candidate)
            count++;
        else
            count--;
    }

    // Phase 2: Verify
    count = 0;
    for (int i = 0; i < size; i++)
    {
        if (nums[i] == candidate)
            count++;
    }

    if (count > size / 2)
        return candidate;

    return -1;  // No majority element
}
*/
int main()
{
	int arr[10],i,ret;
	printf("enter the array elements:\n");
	for(int i=0;i<10;i++)
		scanf("%d",(arr+i));
	printf("before:\n");
	for(i=0;i<10;i++)
		printf("%d ",arr[i]);
	printf("\n");
	ret=major(arr,10);
	printf("ret=%d\n",ret);
	//for(i=0;i<10;i++)
	//	printf("%d ",arr[i]);
}
