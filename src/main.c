// TODO: Breite und Hoehe automatisch auslesen (statt hard zu coden)
#include <stdio.h>
#include <stdlib.h>

#include "../inc/entity.h"
#include "../inc/field.h"
#include "../inc/io.h"

#define WIDTH 64
#define HEIGHT 32

int main(void)
{

    struct field fld = fldCreate(WIDTH, HEIGHT);

    // test
    struct entity e;
    entSetSymbol(&e, 'A');
    printf("%c\n", entGetSymbol(e));
    entSetSymbol(&e, 'a');
    printf("%c\n", entGetSymbol(e));

    fldFree(&fld);

    return 0;
}
