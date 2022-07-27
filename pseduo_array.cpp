# include <stdio.h>
# include <stdarg.h>
# include <stdlib.h>

int array_offset(int arrayinfo[], ...);
int calculation(int* pt, int* arrayinfo);

int main()

{
    int dimension;
    int result;

    puts("How many dimension you need?");
    scanf("%d", &dimension);

    int* arrayinfo = (int*)malloc((dimension * 2 + 1) * sizeof(int));

    if (arrayinfo == NULL)
    {
        puts("Failed to allocate the memory.");
        exit(1);
    }
    else
        *arrayinfo = dimension;

    for (int i = 1, j = 0; j < dimension; j++, i += 2)  //例如3维，需要6个数值，作为每个维度的上下限；
    {
        printf("The dimension %d ,low_limit:\n", j + 1);
        scanf("%d", &arrayinfo[i]);
        printf("The dimension %d ,high_limit:\n", j + 1);
        scanf("%d", &arrayinfo[i + 1]);
    }

    result = array_offset(arrayinfo, 6, 3, 1);

    printf("the loc is %d. ", result);


    free(arrayinfo);  //free()
    return 0;

}

int array_offset(int* arrayinfo, ...)
{

    if (*arrayinfo < 1 || *arrayinfo >10)
        return -1;

    for (int i = 1; i < sizeof(arrayinfo); i += 2)  //对该维度上下标限制的检查，防止下限大于上限
    {
        if (arrayinfo[i] > arrayinfo[i + 1])
        {
            printf("For the dimension %d,the low_limits is bigger than the relevant high_limits.\n", i);
            return -1;
        }
    }

    va_list var_arg;
    va_start(var_arg, arrayinfo);

    int* pt = (int*)malloc((*arrayinfo) * sizeof(int));  //为下标申请空间
    if (pt == NULL)
    {
        puts("Failed to allocate the memory.");
        exit(1);
    }


    for (int i = 0; i < *arrayinfo; i++)   //存储下标
    {
        pt[i] = va_arg(var_arg, int);
    }

    int k = 0;
    for (int j = 2; k < *arrayinfo; j += 2, k++)        //对下标的检查，防止不在规定的上下限之间
    {
        if (pt[k] < arrayinfo[j - 1] || pt[k] > arrayinfo[j])
            return -1;
    }


    int loc;
    loc = calculation(pt, arrayinfo);
    free(pt);   //free()

    return loc;

}

int calculation(int* pt, int* arrayinfo)     //对偏移量的计算
{

    int loc;
    int i = 4;
    int count = 1;
    int lim = sizeof(pt) - 1;

    loc = (*pt) - arrayinfo[1];
    while (count < lim)
    {
        loc *= (*(arrayinfo + i) - *(arrayinfo + i - 1) + 1);
        loc += pt[count] - *(arrayinfo + i - 1);
        i += 2;
        count++;
    }
    return loc;
}