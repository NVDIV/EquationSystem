#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <ctype.h>
#include <conio.h>

// Function prototypes

bool convergence(int n, double** a);
void solve_system(int n, double* b, double** a, double* xp);
void capture();
int get_number_of_elems();
bool continue_calculations();
void get_element(double* element, int n);

bool input_check;

int main()
{
    // Capture

    capture();

    // Get number of equations in system

    int n;

    do
    {
        n = get_number_of_elems();
    } 
    while (!input_check);

    /* MEMORY ALLOCATION */

    double** A = (double**)malloc(n * sizeof(double*));
    for (int i = 0; i < n; ++i) {
        A[i] = (double*)malloc(n * sizeof(double));
    }

    double* B = (double*)malloc(n * sizeof(double));
    double* result = (double*)malloc(n * sizeof(double));

    do
    {
        // Input coefficients and right-hand side

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                do
                {
                    printf("Enter the coefficient of equation %d (A[%d][%d]), (up to 10 symbols): ", i + 1, i + 1, j + 1);
                    get_element(&A[i][j], n);
                }
                while(!input_check);
            }
            do
            {
                printf("Enter the right-hand side of equation %d (B[%d], (up to 10 symbols)): ", i + 1, i + 1);
                get_element(&B[i], n);
            } 
            while (!input_check);
        }

        // Check convergence

        if (!convergence(n, A)) {
            printf("The system may not converge.\n");
            
            // Free memory and exit
            for (int i = 0; i < n; ++i) {
                free(A[i]);
            }
            free(A);
            free(B);
            free(result);

            return 1;
        }

        // Initialization of the result array

        for (int i = 0; i < n; ++i) {
            result[i] = 0.0;
        }

        solve_system(n, B, A, result);

        // Output the result

        printf("\nSolution of the system: ");
        for (int i = 0; i < n; ++i) {
            printf("%f ", result[i]);
        }
        printf("\n");

    } while (continue_calculations());
    
    // Free memory

    for (int i = 0; i < n; ++i) {
        free(A[i]);
    }
    free(A);
    free(B);
    free(result);

    return 0;
}

void capture()
{
    // Clear console

    system("cls");

    // Author

    printf("\nNadiia Rybak 2023\n");

    // Title

    printf("\nEQSYSTEM\n");

    // Decription

    printf("\nSolves system of linear equations using iteration method.\n\n");
}

int get_number_of_elems()
{
    // Get input

    char input[10];

    printf("\nEnter number of elements in the array (up to 10 symbols): ");
    
    scanf("%s", input);

    // Check input

    // Check if it's a number

    for (int i = 0; input[i] != '\0'; i++)
    {
        if (isdigit(input[i]) == 0)
        {
            printf("\nYour input is invalid.\n");
            input_check = false;
            return -1;
        }
    }

    int result = atof(input);

    if (result == 0)
    {
        printf("\nYour input is invalid.\n");
        input_check = false;
        return -1;
    }

    input_check = true;

    return result;
}

void get_element(double* element, int n)
{
    // Get input

    char input[10];

    scanf("%s", input);

    // Check input

    // Check is that a number

    for (int i = 0; input[i] != '\0'; i++)
    {
        if (isdigit(input[i]) == 0 && input[i] != '.' && input[i] != '-')
        {
            printf("\nYour input is invalid.\n");
            input_check = false;
            return;
        }
    }

    input_check = true;

    *element = atof(input);

    return;
}

bool convergence(int n, double** a)
{
    for (int i = 0; i < n; i++) {
        double sigma = 0.0;
        for (int j = 0; j < n; j++) {
            if (j != i) {
                sigma += a[i][j];
            }
        }
        printf("Equation %d: fabs(a[%d][%d]) = %.6f, sigma = %.6f\n", i + 1, i + 1, i + 1, fabs(a[i][i]), sigma);
        if (a[i][i] <= sigma) {
            return false;
        }
    }
    return true;
}

void solve_system(int n, double* b, double** a, double* xp)
{
    
    // Get initial data

    for (int i = 0; i < n; i++) 
    {
        // Check if diagonal element is zero to avoid division by zero
        if (a[i][i] != 0.0) {
            xp[i] = b[i] / a[i][i];
        } 
        else 
        {
            printf("Error: Division by zero encountered.\n");
            return;
        }
    }
    // Get value of x`es

    double* x = (double*)malloc(n * sizeof(double));   // Final solution array

    double epsilon = 0.00000001;  // Tolerance for convergence

    double max_delta = 2 * epsilon;

    double* delta = (double*)malloc(n * sizeof(double));

    do 
    {
        // Calculate x[i]

        for (int i = 0; i < n; i++) 
        {
            double sigma = 0.0;
            for (int j = 0; j < n; j++) 
            {
                if (j != i) 
                {
                    sigma += a[i][j] * xp[j];
                }
            }
            x[i] = (b[i] - sigma) / a[i][i];
        }

        // Get delta (for each iteration)

        for (int i = 0; i < n; i++) 
        {
            delta[i] = fabs(x[i] - xp[i]);
        }

        // Get max_delta

        max_delta = delta[0];

        for (int i = 1; i < n; i++) 
        {
            if (delta[i] > max_delta) 
            {
                max_delta = delta[i];
            }
        }

        // Update xp for the next iteration
        for (int i = 0; i < n; i++) 
        {
            xp[i] = x[i];
        }

    } while (max_delta > epsilon);

    // Output the final solution

    printf("Solution:\n");

    for (int i = 0; i < n; i++) 
    {
        printf("x[%d] = %.6f\n", i, x[i]);
    }
}

bool continue_calculations()
{
    printf("\nDo you want to enter initial data one more time? (enter/esc) \n");

    while(1)
    {
        char response = getch(); 

        if (response == 13)  
        {
            return true;
        }
        else if (response == 27)  
        {
            return false;
        }
    }
}