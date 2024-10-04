#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int extraMemoryAllocated;

// implement merge sort
// extraMemoryAllocated counts bytes of extra memory allocated
void mergeSort(int pData[], int l, int r)
{	/*problem here I dont know why*/

	int m = l + (r-l) / 2;

	mergeSort(pData, l, m);

	mergeSort(pData, m+1, r);

	int arr1Len = m - l +1;
	int arr2Len = r-m;

	int *leftarr = (int *)malloc(arr1Len * sizeof(int));
	int *rightarr = (int *)malloc(arr2Len * sizeof(int));

	extraMemoryAllocated = extraMemoryAllocated + (arr1Len* sizeof(int)) + (arr2Len * sizeof(int));

	for (int i=0; i<arr1Len; i++){
		leftarr[i] = pData[l+i];
	}

	for(int j=0; j< arr2Len; j++){
		rightarr[j] = pData[1+m+j];
	}

	int a = 0;
	int b = 0;

	int c = l;

	while((a < arr1Len) && (b < arr2Len)){
		if (leftarr[a]<= rightarr[b]){
			pData[c] = leftarr[a];
			c++;
			a++;
		}
		else{
			pData[c] = rightarr[b];
			c++;
			b++;
		}

	}

	/*Insert remaining numbers*/
	while(a < arr1Len){
		pData[c] = leftarr[a];
		c++;
		a++;
	}

	while(b < arr2Len){
		pData[c] = rightarr[b];
		c++;
		b++;
	}

	free(leftarr);
	free(rightarr);

}

// implement insertion sort
// extraMemoryAllocated counts bytes of memory allocated
void insertionSort(int* pData, int n)
{
	for(int i=1; i<n; i++){
		int curr = pData[i];
		int j = i-1;

		while(j>=0 && pData[j]>curr){
			pData[j+1] = pData[j];
			j = j-1 ;

		}
		pData[j+1] = curr;

	}


}

// implement bubble sort
// extraMemoryAllocated counts bytes of extra memory allocated
void bubbleSort(int* pData, int n)
{
	for(int i=0; i<n-1; i++){

		for(int j=0; j<n-1-i; j++){
			
			if(pData[j+1]<pData[j]){

				int temp = pData[j];
				pData[j] = pData[j+1];
				pData[j+1] = temp;
			}

		}

	}
}

// implement selection sort
// extraMemoryAllocated counts bytes of extra memory allocated
void selectionSort(int* pData, int n)
{
	for (int i=0; i<n-1; i++){
		int min = i;

		for(int j = 1+1; j<n; j++){
			if(pData[min]>pData[j]){
				min = j;
			}
		
			if(min != i){
				int temp = pData[min];
				pData[min] = pData[j];
				pData[j] = temp;
			}
		}

	}


}

// parses input file to an integer array
int parseData(char *inputFileName, int **ppData)
{
	FILE* inFile = fopen(inputFileName,"r");
	int dataSz = 0;
	*ppData = NULL;
	
	if (inFile)
	{
		fscanf(inFile,"%d\n",&dataSz);
		*ppData = (int *)malloc(sizeof(int) * dataSz);


		// Implement parse data block
		
		/*if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}*/
		for (int i = 0; i < dataSz; ++i)
		{
			fscanf(inFile, "%d\n", *ppData+i);  
		}

		fclose(inFile);

		 
	}
	
	return dataSz;
}

// prints first and last 100 items in the data array
void printArray(int pData[], int dataSz)
{
	int i, sz = dataSz - 100;
	printf("\tData:\n\t");
	for (i=0;i<100;++i)
	{
		printf("%d ",pData[i]);
	}
	printf("\n\t");
	
	for (i=sz;i<dataSz;++i)
	{
		printf("%d ",pData[i]);
	}
	printf("\n\n");
}

int main(void)
{
	clock_t start, end;
	int i;
    double cpu_time_used;
	char* fileNames[] = {"input1.txt", "input2.txt", "input3.txt"};
	
	for (i=0;i<3;++i)
	{
		int *pDataSrc, *pDataCopy;
		int dataSz = parseData(fileNames[i], &pDataSrc);
		
		if (dataSz <= 0)
			continue;
		
		pDataCopy = (int *)malloc(sizeof(int)*dataSz);
	
		printf("---------------------------\n");
		printf("Dataset Size : %d\n",dataSz);
		printf("---------------------------\n");
		
		printf("Selection Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		selectionSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		printf("Insertion Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		insertionSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);

		printf("Bubble Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		bubbleSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		printf("Merge Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		mergeSort(pDataCopy, 0, dataSz - 1);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		free(pDataCopy);
		free(pDataSrc);
	}
	
}