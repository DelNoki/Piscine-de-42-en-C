#include <stdio.h>
#include <stdlib.h>

void findSubsets(int arr[], int n, int index, int current_sum, int target, int subset[], int subset_size) 
{
    int i;
    // Base case: if we have evaluated all elements
    if (index == n)
    {
        if (current_sum == target) 
        {
            i = 0;
            while (i < subset_size)
            {
                printf("%d", subset[i]);
                i++;
                if (i < subset_size)
                    printf(" ");
            }
            printf("\n");
        }
        return ;
    }

    // Choice 1: Exclude the current element
    findSubsets(arr, n, index + 1, current_sum, target, subset, subset_size);

    // Choice 2: Include the current element
    subset[subset_size] = arr[index];
    findSubsets(arr, n, index + 1, current_sum + arr[index], target, subset, subset_size + 1);
}

int main(int argc, char **argv) 
{
    int *arr;
    int target;
    int i;
    int n;
    
    int *subset;

    if (argc < 2)
    {
        printf("Not enough arguments.\n");
    }
    else if (argc == 2)
    {
        printf("No set can be returned since none were given.\n");
    }
    else
    {
        target = atoi(argv[1]);
        i = 2;
        while (i < argc) 
            i++;
        n = i;
        arr = (int *)malloc(sizeof(int) * (n + 1));
        subset = (int *)malloc(sizeof(int) * (n + 1));
        if (!arr || !subset)
        {
            printf("Malloc error.");
            return (-1);
        }
        i = 2;
        while (i < argc)
        {
            arr[i - 2] = atoi(argv[i]);
            i++;
        }
        printf("Subsets matching sum %d:\n", target);
        findSubsets(arr, n, 0, 0, target, subset, 0);
        free(arr);
        free(subset);
    }

    return 0;
}