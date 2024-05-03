#include <iostream>
#include <cstdlib> // Para rand() y srand()
#include <ctime>   // Para time()
#include <windows.h>

// Tamaño del mapa
const int WIDTH = 40;
const int HEIGHT = 20;

// Definición de las zonas del mapa
enum Region
{
    PUEBLO_PALETA,
    BOSQUE_VERDE,
    CIUDAD_CELESTE,
    LIGA_POKENTI
};

// Estructura para representar la posición de Ash
struct Ash
{
    int x;
    int y;
};

// Función para imprimir el mapa
void printMap(Ash ashPosition, char map[HEIGHT][WIDTH], int pokedex)
{
    // Limpia la pantalla
    system("cls");

    // Imprime el mapa
    for (int i = 0; i < HEIGHT; ++i)
    {
        for (int j = 0; j < WIDTH; ++j)
        {
            // Verifica la posición de Ash y lo imprime
            if (ashPosition.x == j && ashPosition.y == i)
            {
                std::cout << "A";
            }
            else
            {
                std::cout << map[i][j]; // Imprime el contenido de la celda del mapa
            }
        }
        std::cout << std::endl;
    }

    // Muestra el contador de la Pokédex
    std::cout << "Pokedex: " << pokedex << std::endl;
}

// Función para verificar si Ash puede moverse a una posición específica
bool canMove(Ash ashPosition, int newAshPositionX, int newAshPositionY, char map[HEIGHT][WIDTH])
{
    // Nueva posición
    int newPosX = ashPosition.x + newAshPositionX;
    int newPosY = ashPosition.y + newAshPositionY;

    // Verifica si la nueva posición está dentro de los límites del mapa
    if (newPosX < 0 || newPosX >= WIDTH || newPosY < 0 || newPosY >= HEIGHT)
    {
        return false;
    }

    // Verifica si la nueva posición está bloqueada por una barrera
    if (map[newPosY][newPosX] == 'X')
    {
        // Si el jugador ha capturado todos los Pokémon de la zona, puede cruzar las barreras
        if (map[ashPosition.y][ashPosition.x] == ' ' || map[ashPosition.y][ashPosition.x] == 'P')
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    return true;
}

// Función para generar Pokémon en una zona del mapa
void generatePokemonInZone(char map[HEIGHT][WIDTH], int startX, int startY, int endX, int endY)
{
    for (int i = 0; i < 5; ++i)
    {
        int randX = startX + rand() % (endX - startX + 1);
        int randY = startY + rand() % (endY - startY + 1);
        // Verifica si la celda está vacía antes de colocar un Pokémon
        if (map[randY][randX] == ' ')
        {
            map[randY][randX] = 'P';
        }
        else
        {
            --i; // Vuelve a intentar colocar un Pokémon en otra celda
        }
    }
}

// Función para capturar un Pokémon
void capturePokemon(Ash ashPosition, char map[HEIGHT][WIDTH], int& pokedex)
{
    // Verifica si hay un Pokémon adyacente a la posición de Ash y lo captura
    if (map[ashPosition.y - 1][ashPosition.x] == 'P')
    {
        map[ashPosition.y - 1][ashPosition.x] = ' ';
        pokedex++;
    }
    if (map[ashPosition.y + 1][ashPosition.x] == 'P')
    {
        map[ashPosition.y + 1][ashPosition.x] = ' ';
        pokedex++;
    }
    if (map[ashPosition.y][ashPosition.x - 1] == 'P')
    {
        map[ashPosition.y][ashPosition.x - 1] = ' ';
        pokedex++;
    }
    if (map[ashPosition.y][ashPosition.x + 1] == 'P')
    {
        map[ashPosition.y][ashPosition.x + 1] = ' ';
        pokedex++;
    }
}

// Función para verificar si todos los Pokémon en una zona han sido capturados
bool allPokemonCaptured(char map[HEIGHT][WIDTH], int startX, int startY, int endX, int endY)
{
    for (int y = startY; y <= endY; ++y)
    {
        for (int x = startX; x <= endX; ++x)
        {
            if (map[y][x] == 'P')
            {
                return false;
            }
        }
    }
    return true;
}

// Función para mover a Ash a la siguiente ubicación
void moveToNextZone(Ash& ashPosition, Region& currentZone, int pokedex, char map[HEIGHT][WIDTH])
{
    // Verifica el valor de la Pokédex para determinar la próxima ubicación
    if (pokedex >= 5 && pokedex < 10)
    {
        // Elimina la barrera entre Pueblo Paleta y Bosque Verde
        for (int i = 0; i <= HEIGHT / 2 - 2; ++i)
        {
            for (int j = WIDTH / 2 - 1; j <= WIDTH / 2; ++j)
            {
                map[i][j] = ' ';
            }
        }
        currentZone = BOSQUE_VERDE;
        // Genera Pokémon en Bosque Verde
        generatePokemonInZone(map, WIDTH / 2 + 1, 0, WIDTH - 1, HEIGHT / 2 - 2);
    }
    else if (pokedex >= 10 && pokedex < 15)
    {
        // Elimina la barrera entre Bosque Verde y Ciudad Celeste
        for (int i = 0; i <= HEIGHT / 2; ++i)
        {
            for (int j = WIDTH / 2 + 1; j <= WIDTH; ++j)
            {
                map[i][j] = ' ';
            }
        }
        currentZone = CIUDAD_CELESTE;
        generatePokemonInZone(map, WIDTH / 2 + 1, HEIGHT / 2 + 1, WIDTH - 1, HEIGHT - 1); // Genera Pokémon en Ciudad Celeste
    }
    else if (pokedex >= 15)
    {
        // Elimina la barrera entre Ciudad Celeste y Liga Pokénti
        for (int i = 0; i <= HEIGHT; ++i)
        {
            for (int j = WIDTH / 2 - 1; j <= WIDTH / 2; ++j)
            {
                map[i][j] = ' ';
            }
        }
        currentZone = LIGA_POKENTI;
        // Genera Pokémon en Liga Pokénti
        generatePokemonInZone(map, 0, HEIGHT / 2 + 1, WIDTH / 2 - 2, HEIGHT - 1);
    }
}

int main()
{
    Ash ashPosition = { 0, 0 };        // Posición inicial de Ash
    int pokedex = 0;                       // Inicializa el contador de la Pokédex
    Region currentRegion = PUEBLO_PALETA;      // Zona actual en la que se encuentra Ash

    // Semilla para la generación aleatoria
    srand(time(nullptr));

    char map[HEIGHT][WIDTH]; // Matriz para representar el mapa

    // Inicializa el mapa con espacios en blanco
    for (int i = 0; i < HEIGHT; ++i)
    {
        for (int j = 0; j < WIDTH; ++j)
        {
            map[i][j] = ' ';
        }
    }

    // Genera las barreras entre las cuatro ubicaciones del mapa
    for (int i = 0; i < HEIGHT; ++i)
    {
        for (int j = 0; j < WIDTH; ++j)
        {
            if (j == WIDTH / 2 - 1 || j == WIDTH / 2 || i == HEIGHT / 2 - 1 || i == HEIGHT / 2)
            {
                map[i][j] = 'X';
            }
        }
    }

    // Genera Pokémon en la zona inicial
    generatePokemonInZone(map, 0, 0, WIDTH / 2 - 2, HEIGHT / 2 - 2);

    // Bucle principal del juego
    while (true)
    {
        printMap(ashPosition, map, pokedex); // Imprime el mapa con la posición actual de Ash y el contador de la Pokédex

        // Verifica si todos los Pokémon en la zona actual han sido capturados
        if ((currentRegion == PUEBLO_PALETA && pokedex == 5) ||
            (currentRegion == BOSQUE_VERDE && pokedex == 10) ||
            (currentRegion == CIUDAD_CELESTE && pokedex == 15))
        {
            moveToNextZone(ashPosition, currentRegion, pokedex, map);
        }

        if (pokedex == 20)
        {
            std::cout << "Felicidades, te has hecho con todos" << std::endl;
        }

        // Lee la entrada del teclado
        if (GetAsyncKeyState(VK_UP)) // Arriba
        {
            if (canMove(ashPosition, 0, -1, map))
                ashPosition.y--;
        }
        if (GetAsyncKeyState(VK_DOWN)) // Abajo
        {
            if (canMove(ashPosition, 0, 1, map))
                ashPosition.y++;
        }
        if (GetAsyncKeyState(VK_LEFT)) // Izquierda
        {
            if (canMove(ashPosition, -1, 0, map))
                ashPosition.x--;
        }
        if (GetAsyncKeyState(VK_RIGHT)) // Derecha
        {
            if (canMove(ashPosition, 1, 0, map))
                ashPosition.x++;
        }
        if (GetAsyncKeyState(VK_SPACE)) // Espacio (capturar Pokémon)
        {
            capturePokemon(ashPosition, map, pokedex);
        }
        if (GetAsyncKeyState(VK_ESCAPE)) // Esc (Salir del juego)
        {
            return 0;
        }

        // Hace una pausa de 100 milisegundos antes de volver a imprimir el mapa
        Sleep(100);
    }

    return 0;
}



