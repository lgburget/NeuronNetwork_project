#include "Random.h"


RandomNumbers *_RNG;

int main(int argc, char **argv) {    
    if (_RNG) delete _RNG;
    return 0;
}

// pas de gestion d'erreur ou de lien avec simulation pour l'instant, juste utile pour lancer le programme et les tests
