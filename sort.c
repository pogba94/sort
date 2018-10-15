#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include "time.h"
#include "windows.h"

typedef int data_t;
typedef void(*sort_func_t)(data_t*,int);
typedef enum SORT_FUNC{
	SORT_FUNC_BUBBLE =0,
	SORT_FUNC_SELECT,
	SORT_FUNC_INSERT,
	SORT_FUNC_SHELL,
	SORT_FUNC_MERGE,
	SORT_FUNC_QUICK,
	SORT_FUNC_HEAP,
	SORT_FUNC_TOTAL_NUM,
}SORT_FUNC_T;

#define TEST_DATA_NUM      (5000000)
#define TEST_DATA_RANGE    (1000000)   //test data is not larger than it
#define TEST_TIMES         (3)

#define CONFIG_PRINT_DATA  (0)
#define CONFIG_BUBBLE_SORT_ENABLE   (0)
#define CONFIG_SELECT_SORT_ENABLE   (0)
#define CONFIG_INSERT_SORT_ENABLE   (0)
#define CONFIG_SHELL_SORT_ENABLE    (0)
#define CONFIG_MERGE_SORT_ENABLE    (1)
#define CONFIG_QUICK_SORT_ENABLE    (1)
#define CONFIG_HEAP_SORT_ENABLE     (1)
#define CONFIG_SORT_FUNC_SELECT     ((CONFIG_BUBBLE_SORT_ENABLE<<SORT_FUNC_BUBBLE) | (CONFIG_SELECT_SORT_ENABLE<<SORT_FUNC_SELECT) |    \
	(CONFIG_SELECT_SORT_ENABLE<<SORT_FUNC_SELECT) | (CONFIG_SHELL_SORT_ENABLE<<SORT_FUNC_SHELL) | (CONFIG_MERGE_SORT_ENABLE<<SORT_FUNC_MERGE) | \
	(CONFIG_QUICK_SORT_ENABLE<<SORT_FUNC_QUICK) | (CONFIG_HEAP_SORT_ENABLE<<SORT_FUNC_HEAP))


void bubbleSort(data_t *array,int size);
void selectionSort(data_t *array,int size);
void insertSort(data_t *array,int size);
void shellSort(data_t *array,int size);
void mergeSort(data_t *array,int size);
void quickSort(data_t *array,int size);
void heapSort(data_t *array,int size);

//data_t test_data[] = {1.2,12.3,4.5,6.7,5.0,0.43,2.34,6.4,7.7,2.0};
data_t test_data[] = {12,123,45,67,5,43,234,64,77,2};
const char* sortFuncName[] = {"bubble","select","insert","shell","merge","quick","heap"}; 
sort_func_t sortFunc[] = {bubbleSort,selectionSort,insertSort,shellSort,mergeSort,quickSort,heapSort};

/* bubble sort */
void bubbleSort(data_t *array,int size)
{
    for(int i=0;i<size-1;i++){
        for(int j=size-1;j>i;j--){
            if(array[j]<array[j-1]){
                data_t tmp = array[j];
                array[j] = array[j-1];
                array[j-1] = tmp;
            }
        }
    }
}
/* selection sort */
void selectionSort(data_t *array,int size)
{
    int minIndex;
    for(int i=0;i<size-1;i++){
        minIndex = i;
        for(int j=size-1;j>i;j--){
            if(array[j]<array[minIndex]){
                minIndex = j;
            }
        }
        data_t tmp = array[minIndex];
        array[minIndex] = array[i];
        array[i] = tmp;
    }
}

/* insert sort */
void insertSort(data_t *array,int size)
{
    for(int i=1;i<size;i++){
        data_t tmp = array[i];
        int index = i-1;
        while(array[index]>tmp && index>=0){
            array[index+1] = array[index];
            index--;
        }
        array[index+1] = tmp;
    }
}
/* shell sort */
void shellSort(data_t *array,int size)
{
	int gap,i,j;
	for(gap = size/2;gap>0;gap /= 2){
		for(i = gap;i<size;i++){
			data_t tmp = array[i];
			for(j = i-gap;j>=0 && array[j]>tmp;j-=gap)
				array[j+gap] = array[j];
			array[j+gap] = tmp;
		}
	}
}
/* merge sort */
static void merge_array(data_t *array,int first,int mid,int last,data_t *tmp)
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

static void merge_sort(data_t *array,int first,int last,data_t *tmp)
{
	if(first<last){
		int mid = (first + last)/2;
		merge_sort(array,first,mid,tmp);
		merge_sort(array,mid+1,last,tmp);
		merge_array(array,first,mid,last,tmp);
	}
}

void mergeSort(data_t *array,int size)
{
	int *p = (int*)malloc(size*sizeof(data_t));
	if(p == NULL)
		return;
	merge_sort(array,0,size-1,p);
	free(p);
}

/* quick sort */

static void quick_sort(data_t *array,int left,int right)
{
	if(left >= right)
		return;
	int i = left;
	int j = right;
	data_t key = array[left];
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

void quickSort(data_t *array,int size)
{
	quick_sort(array,0,size-1);
}

/* heap sort */
// adjust heap when root node change
void heap_adjust(data_t *array,int root,int n)
{
	data_t tmp = array[root];
	for(int k= root*2+1;k<n;k = k*2+1){
		if(k+1 < n && array[k] < array[k+1]){
			k++;
		}
		if(array[k] > tmp){
			array[root] = array[k];
			root = k;
		}else{
			break;
		}
	}
	array[root] = tmp;
}
void heapSort(data_t *array,int size)
{
	for(int i=size/2-1;i>=0;i--){
		heap_adjust(array,i,size);
	}
	for(int j=size-1;j>0;j--){
		data_t tmp = array[0];
		array[0] = array[j];
		array[j] = tmp;

		heap_adjust(array,0,j);
	}
}

int benchmark(int data_size,int times)
{
	int i;
	int sortArgTime[SORT_FUNC_TOTAL_NUM] = {0};
	int start,end;
	int test_times = times;
	data_t *data,*dataBackup;
	data = (data_t*)malloc(data_size*sizeof(data_t));
	if(data == NULL){
		printf("malloc failed!\n");
		return -1;
	}
	dataBackup = (data_t*)malloc(data_size*sizeof(data_t));
	if(dataBackup == NULL){
		printf("malloc failed\n");
		free(data);
		return -1;
	}
	#if 0
	printf("before:\n");
	for(i=0;i<10;i++)
		printf("%d\t",test_data[i]);
	sortFunc[4](test_data,10);
	printf("\nafter:\n");
	for(i=0;i<10;i++)
		printf("%d\t",test_data[i]);
	printf("\n\n");
	while(1);
    #endif
    	/* generate test data randomly */
	while(test_times--){
		printf("current time:%ld\n",time(NULL));
		srand(time(NULL)+test_times);
		for(i=0;i<data_size;i++)
			*(data+i) = (data_t)(rand()%TEST_DATA_RANGE + 1);
		memcpy(dataBackup,data,data_size*sizeof(data_t));

		for(int j=0;j<SORT_FUNC_TOTAL_NUM;j++){
			if((CONFIG_SORT_FUNC_SELECT>>j) & 0x01){
				memcpy(data,dataBackup,data_size*sizeof(data_t));
				start = GetTickCount();
				sortFunc[j](data,data_size);
				end = GetTickCount();
				sortArgTime[j] += (end - start);
			}
		}
	}
	printf("\nbenchmark\ntest condition:   data num: %d ,test times: %d \n",data_size,times);
	printf("----sort function----time waste----\n");
	for(i=0;i<SORT_FUNC_TOTAL_NUM;i++){
		sortArgTime[i] /= times;
		printf("----%s------%d ms-----\n",sortFuncName[i],sortArgTime[i]);
	}
	free(data);
	free(dataBackup);
	return 0;
}

/* main function */
int main(void)
{
	benchmark(10000,TEST_TIMES);
/*	benchmark(100000,TEST_TIMES);
	benchmark(500000,TEST_TIMES);
	benchmark(1000000,TEST_TIMES);
	benchmark(5000000,TEST_TIMES);
	benchmark(10000000,TEST_TIMES);
	benchmark(20000000,TEST_TIMES);
	benchmark(50000000,TEST_TIMES);
*/
	for(;;);
}