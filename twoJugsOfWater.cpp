#include <cstdio>
#include <stack>
#include <map>
#include <algorithm>
using namespace std;

//Estructura que representa un estado, x y y siendo los litros puestos en la jarra por el momento
struct estado {
    int x, y;

    // Verificar en formato mapa de los estados si la capacidad de uno es menor a la de otro
    bool operator < (const estado& that) const {
        if (x != that.x) return x < that.x;
        return y < that.y;
    }
};

// Capacidades a determinar de las dos jarras y el objetivo
int capacidad_jx, capacidad_jy, objetivo;

void dfs(estado start, stack <pair <estado, int> >& ruta)   {
    stack <estado> s;
    estado meta = (estado) {-1, -1};

    //Mapa para guardar los estados para no regresar a aquellos
    //ya visitados y mantiene sus estados raiz para encontrar un camino con ellos
    //La llave es el estado, y el valor el estado padre y la regla que lo llevó a él
    map <estado, pair <estado, int> > padres;

    s.push(start);
    padres[start] = make_pair(start, 0);

    while (!s.empty())    {
        //En lo que la pila no esté vacía, tomar el estado desde el frente de la pila
        estado top = s.top();
        s.pop();

        // Terminar el programa si se llegó al objetivo
        if (top.x == objetivo || top.y == objetivo) {
            meta = top;
            break;
        }

        // Para encontrar los sucesores posibles de este estado, se utilizan reglas para determinar las
        // acciones realizadas y remover posibles bifurcaciones ya visitadas

        // Regla 1: (x, y) -> (capacidad_jx, y) if x < capacidad_jx
        // Llenar la primera jarra
        if (top.x < capacidad_jx)  {
            estado child = (estado) {capacidad_jx, top.y};
            // Sólo considerar este estado si no ha sido visitado
            if (padres.find(child) == padres.end()) {
                s.push(child);
                padres[child] = make_pair(top, 1);
            }
        }

        // Regla 2: (x, y) -> (x, capacidad_jy) if y < capacidad_jy
        // Llenar la segunda jarra
        if (top.y < capacidad_jy)  {
            estado child = (estado) {top.x, capacidad_jy};
            if (padres.find(child) == padres.end()) {
                s.push(child);
                padres[child] = make_pair(top, 2);
            }
        }

        // Regla 3: (x, y) -> (0, y) if x > 0
        // Vaciar la jarra 1
        if (top.x > 0)  {
            estado child = (estado) {0, top.y};
            if (padres.find(child) == padres.end()) {
                s.push(child);
                padres[child] = make_pair(top, 3);
            }
        }

        // Regla 4: (x, y) -> (x, 0) if y > 0
        // Vaciar la jarra 2
        if (top.y > 0)  {
            estado child = (estado) {top.x, 0};
            if (padres.find(child) == padres.end()) {
                s.push(child);
                padres[child] = make_pair(top, 4);
            }
        }

        // Regla 5: (x, y) -> (min(x + y, capacidad_jx), max(0, x + y - capacidad_jx)) if y > 0
        // Llenar agua desde la jarra 2 a la primera hasta que esta esté llena o la jarra 2 se vacíe
        if (top.y > 0)  {
            estado child = (estado) {min(top.x + top.y, capacidad_jx), max(0, top.x + top.y - capacidad_jx)};
            if (padres.find(child) == padres.end()) {
                s.push(child);
                padres[child] = make_pair(top, 5);
            }
        }

        // Regla 6: (x, y) -> (max(0, x + y - capacidad_jy), min(x + y, capacidad_jy)) if x > 0
        // Llenar agua desde la jarra 1 a la segunda hasta que esta esté llena o la jarra 1 se vacíe
        if (top.x > 0)  {
            estado child = (estado) {max(0, top.x + top.y - capacidad_jy), min(top.x + top.y, capacidad_jy)};
            if (padres.find(child) == padres.end()) {
                s.push(child);
                padres[child] = make_pair(top, 6);
            }
        }
    }

    // No se llegó a encontrar el objetivo 
    if (meta.x == -1 || meta.y == -1)
        return;

    // Generando el camino a partir del estado con backtraking
    ruta.push(make_pair(meta, 0));
    while (padres[ruta.top().first].second != 0)
        ruta.push(padres[ruta.top().first]);
}

int main()  {
    stack <pair <estado, int> > ruta;
    printf("Ingresa las capacidades de las dos jarras : ");
    scanf("%d %d", &capacidad_jx, &capacidad_jy);
    printf("Ingresa el objetivo de litros: ");
    scanf("%d", &objetivo);

    dfs((estado) {0, 0}, ruta);
    if (ruta.empty())
        printf("\nNo se puede llegar al objetivo\n");
    else    {
        printf("\nNumero de movimientos realizados para llegar al objetivo : %d\nUn camino para llegar al objetivo :\n", ruta.size() - 1);
        while (!ruta.empty())   {
            estado top = ruta.top().first;
            int rule = ruta.top().second;
            ruta.pop();

            switch (rule)   {
                case 0: printf("Estado : (%d, %d)\n#\n", top.x, top.y);
                        break;
                case 1: printf("Estado : (%d, %d)\nAcción : Llenar la primera jarra\n", top.x, top.y);
                        break;
                case 2: printf("Estado : (%d, %d)\nAcción : Llenar la segunda jarra\n", top.x, top.y);
                        break;
                case 3: printf("Estado : (%d, %d)\nAcción : Vaciar la primera jarra\n", top.x, top.y);
                        break;
                case 4: printf("Estado : (%d, %d)\nAcción : Vaciar la segunda jarra\n", top.x, top.y);
                        break;
                case 5: printf("Estado : (%d, %d)\nAcción : Agregar desde la segunda jarra a la primera jarra\n", top.x, top.y);
                        break;
                case 6: printf("Estado : (%d, %d)\nAcción : Agregar desde la primera jarra a la segunda jarra\n", top.x, top.y);
                        break;
            }
        }
    }

    return 0;
}