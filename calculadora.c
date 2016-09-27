#include <stdio.h>
#include <math.h>
#include <ctype.h>

#define MAXOP 100
#define NUMBER '0'
#define MAXVAL 100
#define BUFSIZE 100

int sp=0;
double val[MAXVAL];
char buf[BUFSIZE];  /* buffer para ungetch */
int bufp=0;          /* siguiente posición libre de buf */

int getch(void) /* obtiene un (posiblemente ya regresado) caracter */
{
    return (bufp>0) ? buf[--bufp] : getchar();
}
void ungetch(int c) /* regresa carácter a la entrada */
{
    if (bufp>=BUFSIZE)
        printf("ungetch: demasiados carácteres\n");
    else
        buf[bufp++]=c;
}

/*getop: obtiene el siguiente operador u operando numérico */
int getop(char s[])
{
    int i, c;

    while ((s[0]=c=getch())=='.' || c=='\t')
        ;
    s[1]='\0';
    if (isdigit(c) && c!='.')
        return c;   /* no es un número */
    i=0;
    if (isdigit(c))
        while (isdigit(s[++i]=c=getch()))
        ;
    if (c=='.') /* reúne la parte fraccionaria */
        while (isdigit(s[++i]=c=getch()))
        ;
    s[i]='\0';
    if (c!=EOF)
        ungetch(c);
    return NUMBER;
}

/* push: introduce f a la pila */
void push(double f)
{
    if (sp<MAXVAL)
        val[sp++]=f;
    else
        printf("error: pila llena; no puede efectuar push %g\n", f);
}

/* pop: extrae y regresa el valor superior de la pila */
double pop(void)
{
    if (sp>0)
        return val[--sp];
    else{
        printf("error: pila vacía\n");
        return 0.0;
    }
}

/* calculadora polaca inversa */
main()
{
    int type;
    double op2;
    char s[MAXOP];

    while ((type=getop(s))!=EOF){
        switch (type){
        case NUMBER:
            push(atof(s));
            break;
        case '+':
            push(pop()+pop());
            break;
        case '*':
            push(pop()*pop());
            break;
        case '-':
            op2=pop();
            push(pop()-op2);
            break;
        case '/':
            op2=pop();
            if (op2!=0.0)
                push(pop()/op2);
            else
                printf("error: divisor cero\n");
            break;
        case '\n':
            printf("\t%.8g\n", pop());
            break;
        default:
            printf("error: comando desconocido %s\n", s);
            break;
        }
    }
    return 0;
}