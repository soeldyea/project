#include <stdio.h>
#include <stdlib.h>
#include <time.h>

long long count_comp_1, count_swap_1, count_comp_2, count_swap_2; 

// Функция для сортировки методом Шелла
void Shell_sort(double *a, int n) {
    int i, j, step;
    double tmp;
    for (step = n / 2; step > 0; step /= 2) { // Выбираем шаг (каждый раз он уменьшается в 2 раза)
        for (i = step; i < n; i++) {
            tmp = a[i]; //берем элемент с индексом равным шагу
            for (j = i; j >= step; j -= step) {//смотрим есть ли слева от него на расстоянии шага какие-то элементы
                count_comp_1++; 
                if (tmp < a[j - step]) {//если текущий элемент меньше чем, элемент, стоящий на расстоянии длины шага слева
                    a[j] = a[j - step]; //то ставим на место текущего, стоящий слева на расст. длины шага и ищем место для предыдущего текущего
                    count_swap_1++; 
                } else {
                    break;
                }
            }
            a[j] = tmp; //нашли место для текущего, теперь слева на расст. длины шага эл-т меньший данного
        }
    }
}

// Функция быстрой сортировки
static void QuickSort(double *a, int left, int right) {
    double comp, tmp;
    int i, j;
    i = left, j = right;
    comp = a[(left + right) / 2]; // Находим значение компаранда, относительно него мы в первый раз будем сортировать элементы

    do { // Выполняем действия, пока индексы i и j не пересеклись
        while (a[i] < comp) { // Если элемент меньше компаранда, и он слева от указателя right, то все хорошо, переходим к следующему
            count_comp_2++; 
            i++;
        }
        // Сейчас элемент a[i] не меньше компаранда, нам нужно найти элемент a[j], который меньше компаранда, но стоит правее, чем a[i] 

        // Если условие while не выполнилось, но было сравнение двух элементов массива
        if (a[i] >= comp)
            count_comp_2++; 

        while (comp < a[j]) { // Аналогично, если элемент больше компаранда, и он справа от указателя left, то все хорошо, идем к следующему
            count_comp_2++; 
            j--;
        }
        // В результате этого цикла нашли подходящий элемент a[j]

        // Если условие while не выполнилось, но было сравнение двух элементов массива  
        if (a[j] <= comp)
            count_comp_2++;

        if (i <= j) { // Проверяем, что элемент a[i] действительно стоит левее a[j] и меняем эти элементы местами
            tmp = a[i];
            a[i] = a[j];
            a[j] = tmp;
            if (a[i] != a[j])
                count_swap_2++; // Если i = j, то элемент заменяется сам на себя
            i++, j--;
        }
    } while (i <= j);

    // Между left и i теперь числа, которые меньше компаранда, а между j и right числа, которые больше компаранда

    // Если i или j не дошли до противоположной границы, то рекурсивно вызываем функцию для соответствующей части
    if (left < j) {
        QuickSort(a, left, j);
    }
    if (right > i) {
        QuickSort(a, i, right);
    }
}

// Функция для вызова быстрой сортировки
void quicksort(double *a, int n) {
    QuickSort(a, 0, n - 1);
}

// Функция для генерации массива
void generate_array(double *array, int n) {
    for (int i = 0; i < n; i++) {
        array[i] = (double)rand() / RAND_MAX * RAND_MAX;
    }
}

//функция, разворачивающая массив
void reverse_array(double *array, int n) {
    //идем до середины массива и меняем текущий элемент с симметричным ему, относительно этой середины
    for (int i = 0; i < n / 2; i++) {
        double tmp = array[i];
        array[i] = array[n - i - 1];
        array[n - i - 1] = tmp;
    }
}

// Функция для вывода массива
void print_array(double *array, int n) {
    for (int i = 0; i < n; i++) {
        printf("%.2f ", array[i]);
    }
    printf("\n");
}

int main(void) {
    srand(time(NULL)); // инициализация генератора случайных чисел

    int n;
    printf("Введите размер массива n: ");
    scanf("%d", &n);

    double **arrays = malloc(4 * sizeof(double *));
    for (int k = 0; k < 4; k++) {
        arrays[k] = malloc(n * sizeof(double));
    }

    for (int k = 0; k < 4; k++) {
        generate_array(arrays[k], n);

        // В 3 раз массив должен быть обязательно отсортирован, в 4 раз он должен быть развернут
        if (k > 1) {
            Shell_sort(arrays[k], n); // Сортируем массив
            if (k == 3) {
                reverse_array(arrays[k], n); // Разворачиваем отсортированный массив
            }
        }
        
        printf("Массив %d до сортировки:\n", k + 1);
        //print_array(arrays[k], n);
        printf("\n");

        // Сохраняем исходный массив массив, для сортировки вторым методом
        double *original = malloc(n * sizeof(double));
        for (int i = 0; i < n; i++) {
            original[i] = arrays[k][i];
        }

        count_comp_1 = 0;
        count_swap_1 = 0;
        Shell_sort(arrays[k], n);   
        printf("Сортировка Шелла число сравнений: %lld, число обменов: %lld\n", count_comp_1, count_swap_1);

        count_comp_2 = 0;
        count_swap_2 = 0;
        quicksort(original, n); 
        printf("Быстрая сортировка число сравнений: %lld, число обменов: %lld\n", count_comp_2, count_swap_2);
        
        printf("\n\n"); 
        free(original);
    }

    for (int k = 0; k < 4; k++) {
        free(arrays[k]);
    }
    free(arrays);

    return 0;
}
