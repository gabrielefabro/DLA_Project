#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct
{
    int x; // position on x axis
    int y; // position on y axis

    int vel;  // velocity
    int dire; // direction

    int stuck; // 0 = not stuck, 1 = stuck
} particle;

// DA IMPLEMENTARE
//  typedef int (*start_dla)(int n, int m, int matrix[n][m], particle *p);
//  typedef void (*move)(particle p);
//  typedef int (*check_position)(int n, int m, int matrix[n][m], particle *p);

void print_matrix(int n, int m, int matrix[n][m]);
void move(particle *part, int n, int m, int matrix[n][m]);
int gen_particles(int num_particles, particle *particles_list, char *particle_arg);
int start_DLA(int num_particles, particle *particles_list, int n, int m, int matrix[n][m]);
int check_position(int n, int m, int matrix[n][m], particle *p);

/*
 * check_position controlla tutti i possibili movimenti che potrebbe fare la particella in una superficie 2D.
 * La funzione ritorna un intero che indica se la particella è rimasta bloccata o meno.
 * Se la particella è rimasta bloccata, la funzione ritorna -1, altrimenti ritorna 0.
 * La funzione riceve in input le dimensioni della matrice, la matrice e la particella interessata.
 * La funzione modifica la matrice e la particella SOLO se la particella è rimasta bloccata.
*/
int check_position(int n, int m, int matrix[n][m], particle *p)
{
    if (p->stuck == 1)
    {
        return -1;
    }

    int directions[] = {0, 1, 0, -1, 1, 0, -1, 0, 1, 1, 1, -1, -1, 1, -1, -1};

    for (int i = 0; i < 8; i += 2)
    {
        int near_y = p->y + directions[i];
        int near_x = p->x + directions[i + 1];
        if (near_x > 0 && near_x < n && near_y > 0 && near_y < m)
        {
            if (matrix[near_y][near_x] == 1)
            {
                printf("Particle stuck at (%d, %d) \n",p->y, p->x);
                matrix[p->y][p->x] = 1;
                p->stuck = 1;
                return -1;
            }
        }
    }
    return 0;
}

/*
 * move muove la particella in una direzione pseudocasuale.
 * La funzione riceve in inpute la particella, le dimensioni della matrice e la matrice.
 * La funzione modifica la matrice e la particella a ogni chiamata simulando il movimento della particella.
 * Quest'ultima funzionalità penso sia solo a scopo di TEST.
 * La funzione non ritorna nulla.
*/
void move(particle *p, int n, int m, int matrix[n][m])
{
    if (p->x > 0 && p->x < n && p->y > 0 && p->y < m)   
        matrix[p->y][p->x] = 0;
    // move particle
    p->x += rand() % 3 - 1;
    p->y += rand() % 3 - 1;
    if (p->x > 0 && p->x < n && p->y > 0 && p->y < m)
        matrix[p->y][p->x] = 2;

}

/*
 * gen_particles genera una lista di particelle a partire da una stringa.
 * La funzione riceve in input il numero di particelle da generare, la lista di particelle e la stringa.
 * La stringa deve essere formattata nel seguente modo: "i,j,v,i,j,v,i,j,v,..." dove i e j sono le coordinate della particella e v è la velocità.
 * La funzione ritorna 0 se la generazione è andata a buon fine, altrimenti ritorna 1.
 * La funzione modifica la lista di particelle.
*/
int gen_particles(int num_particles, particle *particles_list, char *particle_arg)
{
    // get data from particle_arg
    char *token = strtok(particle_arg, ",");
    int index = 0;
    while (token != NULL)
    {
        if (index == num_particles)
        {
            printf("Error: too many particles\n");
            return 1;
        }

        // get particle data
        int i = (int)atoi(token);
        token = strtok(NULL, ",");
        int j = (int)atoi(token);
        token = strtok(NULL, ",");
        int v = (int)atoi(token);
        token = strtok(NULL, ",");

        // create particle
        particle *p = malloc(sizeof(particle));
        p->x = i;
        p->y = j;
        p->vel = v;

        // add particle to list
        particles_list[index] = *p;
        index++;
    }
    return 0;
}


/*
 * print_matrix stampa la matrice.
 * La funzione riceve in input le dimensioni della matrice e la matrice.
 * La funzione non ritorna nulla.    
*/
void print_matrix(int n, int m, int matrix[n][m])
{
    int a, b;
    for (a = 0; a < n; a++)
    {
        for (b = 0; b < m; b++)
        {
            printf("%d ", matrix[a][b]);
        }
        printf("\n");
    }
    printf("-----------------------------------------------------\n");
}

/*
 * start_DLA simula l'algoritmo DLA.
 * Printa la matrice una prima volta e poi simula il movimento di ogni particella per un valore t.
 * La simulazione del movimento consiste:
 *  - in un ciclo che scorre tutte le particelle
 *  - per ogni particella viene chiamata la funzione check_position che controlla se la particella è in prossimità di un cristallo,
 *    in caso affermativo blocca la particella e da quel momento inizia a far parte del cristallo. Altrimenti la particella si muove.
 *  - nel caso in cui la particella non sia rimasta bloccata, viene chiamata la funzione move che si occupa di muoverla.
 * 
 * L'algoritmo termina al raggiungimento di un valore t.
 * La funzione riceve in input il numero di particelle, la lista di particelle, le dimensioni della matrice e la matrice.
 * La funzione ritorna 0 se l'esecuzione è andata a buon fine, altrimenti ritorna 1.
 * 
*/
int start_DLA(int num_particles,
              particle *particles_list,
              int n, int m,
              int matrix[n][m])
{
    printf("Starting DLA\n");
    int t;
    srand(time(NULL));
    for (t = 0; t < 100; t++)
    {
        int i;
        for (i = 0; i < num_particles; i++)
        {
            particle *p = &particles_list[i];
            if (p->stuck == 0)
            {
                int isStuck = check_position(n, m, matrix, p);
                if (isStuck == 0)
                {
                    move(p, n, m, matrix);
                }
            }
        }
    }
    printf("Finished DLA\n");
    return 0;
}

int main(int argc, char *argv[])
{
    int n, m;    // matrix dimensions
    int seed[2]; // seed position

    // get matrix dimensions
    char *sizes = argv[1];
    char *token = strtok(sizes, ",");
    n = (int)atoi(token);
    token = strtok(NULL, ",");
    m = (int)atoi(token);

    // get seed position
    char *seed_pos = argv[2];
    token = strtok(seed_pos, ",");
    seed[0] = (int)atoi(token);
    token = strtok(NULL, ",");
    seed[1] = (int)atoi(token);

    // create matrix
    int matrix[n][m];
    memset(matrix, 0, sizeof(matrix[0][0]) * n* m);
    matrix[seed[0]][seed[1]] = 1;

    // get particle data
    char *particle_arg = argv[3];
    int num_particles = (int)atoi(argv[4]);
    particle *particles_list = malloc(sizeof(particle) * num_particles);
    // create particles and check for errors
    if (gen_particles(num_particles, particles_list, particle_arg))
    {
        printf("Error: could not create particles\n");
        exit(1);
    }

    // first print 
    print_matrix(n, m, matrix);
    fflush(stdout);
    // start DLA
    start_DLA(num_particles, particles_list, n, m, matrix);

    print_matrix(n, m, matrix);
    fflush(stdout);

    FILE *fptr;

   // use appropriate location if you are using MacOS or Linux
   fptr = fopen("matrix.txt","w");

   if(fptr == NULL)
   {
      printf("Error!");   
      exit(1);             
   }

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            fprintf(fptr,"%d ", matrix[i][j]);
        }
        fprintf(fptr,"\n");
    }

    // the list is fr
    free(particles_list);

    return 0;
}