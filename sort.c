#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include "time.h"
#include "windows.h"

#define TEST_DATA_NUM      (5000000)
#define TEST_DATA_RANGE    (1000000)   //test data is not larger than it
#define TEST_TIMES         (5)

#define CONFIG_PRINT_DATA  (0)
#define CONFIG_BUBBLE_SORT_ENABLE   (0)
#define CONFIG_SELECT_SORT_ENABLE   (0)
#define CONFIG_INSERT_SORT_ENABLE   (0)
#define CONFIG_SHELL_SORT_ENABLE    (0)
#define CONFIG_MERGE_SORT_ENABLE    (1)
#define CONFIG_QUICK_SORT_ENABLE    (1)

const char* sortFuncName[] = {"bubble","select","insert","shell","merge","quick"}; 
int test_data[] = {12,123,45,67,5,43,234,64,77,2};
/* bubble sort */
void bubbleSort(int *array,int size)
{
    for(int i=0;i<size-1;i++){
        for(int j=size-1;j>i;j--){
            if(array[j]<array[j-1]){
                int tmp = array[j];
                array[j] = array[j-1];
                array[j-1] = tmp;
            }
        }
    }
}
/* selection sort */
void selectionSort(int *array,int size)
{
    int minIndex;
    for(int i=0;i<size-1;i++){
        minIndex = i;
        for(int j=size-1;j>i;j--){
            if(array[j]<array[minIndex]){
                minIndex = j;
            }
        }
        int tmp = array[minIndex];
        array[minIndex] = array[i];
        array[i] = tmp;
    }
}

/* insert sort */
void insertSort(int *array,int size)
{
    for(int i=1;i<size;i++){
        int tmp = array[i];
        int index = i-1;
        while(array[index]>tmp && index>=0){//注意 tmp 不能用 array[i] 代替，array[i] 值可能会改变
            array[index+1] = array[index];
            index--;
        }
        array[index+1] = tmp;
    }
}

/* Shell sort */
static int dkHibbard(int t,int k)
{
	return (int)(pow(2,t-k+1)-1);
}

static void shellInsert(int *array,int n,int dk)
{
	int i,j,temp;
    for(i=dk;i<n;i++)
    {
        temp=array[i];
        for(j=i-dk;(j>=i%dk)&&array[j]>temp;j-=dk)
            array[j+dk]=array[j];
        if(j!=i-dk)
            array[j+dk]=temp;
    }
}

void shellSort(int *array,int n,int t)
{
    int i;
    for(i=1;i<=t;i++)
        shellInsert(array,n,dkHibbard(t,i));
}

/* merge sort */
static void merge_array(int *array,int first,int mid,int last,int *tmp)
{
	int i = first,j = mid + 1;
	int m = mid,n = last;
	int k = 0;
	while(i<=m && j<=n){
		if(array[i] <= array[j])
			tmp[k++] = array[i++];
		else
			tmp[k++] = array[j++];
	}
	while(i <= m){
		tmp[k++] = array[i++];
	}

	while(j <= n){
		tmp[k++] = array[j++];
	}

	for(i=0;i<k;i++){
		array[first+i] = tmp[i];
	}
}

static void merge_sort(int *array,int first,int last,int *tmp)
{
	if(first<last){
		int mid = (first + last)/2;
		merge_sort(array,first,mid,tmp);
		merge_sort(array,mid+1,last,tmp);
		merge_array(array,first,mid,last,tmp);
	}
}

void mergeSort(int *array,int size)
{
	int *p = (int*)malloc(size*sizeof(int));
	if(p == NULL)
		return;
	merge_sort(array,0,size-1,p);
	free(p);
}

/* quick sort */

static void quick_sort(int *array,int left,int right)
{
	if(left >= right)
		return;
	int i = left;
	int j = right;
	int key = array[left];
	while(i<j){
		while(i<j && key <= array[j]){
			j--;
		}
		array[i] = array[j];
		while(i<j && key >= array[i]){
			i++;
		}
		array[j] = array[i];	
	}
	array[i] = key;
	quick_sort(array,left,i-1);
	quick_sort(array,i+1,right);
}

void quickSort(int *array,int size)
{
	quick_sort(array,0,size-1);
}

/* heap sort */
void heapSort(int *array,int size)
{
	
}

/* main function */
int main(void)
{
	int i;
	int sortArgTime[6] = {0};
	int testTime = TEST_TIMES;
	int start,end;
	int *data,*dataBackup;
	data = (int*)malloc(TEST_DATA_NUM*sizeof(int));
	if(data == NULL){
		printf("malloc failed!\n");
		return -1;
	}
	dataBackup = (int*)malloc(TEST_DATA_NUM*sizeof(int));
	if(dataBackup == NULL){
		printf("malloc failed\n");
		free(data);
		return -1;
	}
	#if 0
	printf("before:\n");
	for(i=0;i<10;i++)
		printf("%d\t",test_data[i]);
	quickSort(test_data,10);
	printf("\nafter:\n");
	for(i=0;i<10;i++)
		printf("%d\t",test_data[i]);
	printf("\n\n");
	while(1);
    #endif
	/* generate test data randomly */
	while(testTime--){
		printf("current time:%ld\n",time(NULL));
		srand(time(NULL)+testTime);
		for(i=0;i<TEST_DATA_NUM;i++)
			*(data+i) = rand()%TEST_DATA_RANGE + 1;
		memcpy(dataBackup,data,TEST_DATA_NUM*sizeof(int));
		#if CONFIG_PRINT_DATA
			printf("\nbefore sort:\n");
			for(i=0;i<TEST_DATA_NUM;i++){
				if(i%10==0)
					printf("\n");
				printf("%d\t",data[i]);
			}
		#endif
        #if CONFIG_BUBBLE_SORT_ENABLE
			start = GetTickCount();
			bubbleSort(data,TEST_DATA_NUM);
			end = GetTickCount();
			sortArgTime[0] += (end - start);
		#endif
		#if CONFIG_SELECT_SORT_ENABLE
		memcpy(data,dataBackup,TEST_DATA_NUM*sizeof(int));
		start = GetTickCount();
		selectionSort(data,TEST_DATA_NUM);
		end = GetTickCount();
		sortArgTime[1] += (end - start);
		#endif
		#if CONFIG_INSERT_SORT_ENABLE
		memcpy(data,dataBackup,TEST_DATA_NUM*sizeof(int));
		start = GetTickCount();
		insertSort(data,TEST_DATA_NUM);
		end = GetTickCount();
		sortArgTime[2] += (end - start);
		#endif
		#if CONFIG_SHELL_SORT_ENABLE
		memcpy(data,dataBackup,TEST_DATA_NUM*sizeof(int));
		start = GetTickCount();
		shellSort(data,TEST_DATA_NUM,3);
		end = GetTickCount();
		sortArgTime[3] += (end - start);
		#endif
		#if CONFIG_MERGE_SORT_ENABLE
		memcpy(data,dataBackup,TEST_DATA_NUM*sizeof(int));
		start = GetTickCount();
		mergeSort(data,TEST_DATA_NUM);
		end = GetTickCount();
		sortArgTime[4] += (end - start);
		#endif
		#if CONFIG_QUICK_SORT_ENABLE
		memcpy(data,dataBackup,TEST_DATA_NUM*sizeof(int));
		start = GetTickCount();
		quickSort(data,TEST_DATA_NUM);
		end = GetTickCount();
		sortArgTime[5] += (end - start);
		#endif
		#if CONFIG_PRINT_DATA
		    printf("\n\nafter sort:\n");
			for(i=0;i<TEST_DATA_NUM;i++){
				if(i%10 == 0)
					printf("\n");
				printf("%d\t",data[i]);
			}
		printf("\n");
		#endif
	}
	printf("\nbenchmark\ntest condition:   data num: %d ,test times: %d \n",TEST_DATA_NUM,TEST_TIMES);
	printf("----sort function----time waste----\n");
	for(i=0;i<6;i++){
		sortArgTime[i] /= TEST_TIMES;
		printf("----%s------%d ms-----\n",sortFuncName[i],sortArgTime[i]);
	}
	free(data);
	free(dataBackup);
	for(;;);
}