#include "fecha.h"
#include <ctype.h>

int esBisiesto(int anio)
{
    return (anio % 4 == 0 && anio % 100 != 0) || (anio % 400 == 0);
}

int esFechaValida(int dia, int mes, int anio, int hora, int min)
{
    static const int diasPorMes[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int maxDia;

    if(anio < 1970 || anio > 2100)
        return 0;

    if(mes < 1 || mes > 12)
        return 0;

    maxDia = *(diasPorMes + mes);
    if(mes == 2 && esBisiesto(anio))
        maxDia = 29;

    if(dia < 1 || dia > maxDia)
        return 0;

    if(hora < 0 || hora > 23)
        return 0;

    if(min < 0 || min > 59)
        return 0;

    return 1;
}

int parsearFechaCadena(const char *cadena, int *dia, int *mes, int *anio)
{
    int d = 0, m = 0, a = 0;
    char sep1 = 0, sep2 = 0;

    /* Intentar leer formato con separadores: DD/MM/AAAA o DD-MM-AAAA */
    if(sscanf(cadena, "%d%c%d%c%d", &d, &sep1, &m, &sep2, &a) == 5)
    {
        if((sep1 == '/' && sep2 == '/') || (sep1 == '-' && sep2 == '-'))
        {
            *dia = d;
            *mes = m;
            *anio = a;
            return 1;
        }
    }

    /* Intentar leer formato continuo: DDMMAAAA (8 digitos seguidos) */
    if(sscanf(cadena, "%8d", &d) == 1 && (int)cadena[8] == '\0')
    {
        /* Si puso e.g. 25102026: d = 25102026 */
        if(d >= 1011970) /* minimo 01/01/1970 -> 1011970 */
        {
            a = d % 10000;
            d /= 10000;
            m = d % 100;
            d /= 100;

            *dia = d;
            *mes = m;
            *anio = a;
            return 1;
        }
    }

    return 0;
}

int parsearHoraCadena(const char *cadena, int *hora, int *min)
{
    int h = 0, m = 0;
    char sep = 0;

    /* Formato HH:MM o HH.MM */
    if(sscanf(cadena, "%d%c%d", &h, &sep, &m) == 3)
    {
        if(sep == ':' || sep == '.' || sep == '-')
        {
            *hora = h;
            *min = m;
            return 1;
        }
    }

    /* Formato continuo: HHMM (4 digitos seguidos) */
    if(sscanf(cadena, "%4d", &h) == 1 && (int)cadena[4] == '\0')
    {
        m = h % 100;
        h /= 100;
        *hora = h;
        *min = m;
        return 1;
    }

    return 0;
}

unsigned long long convertirFechaATimestamp(int dia, int mes, int anio, int hora, int min)
{
    struct tm t;
    time_t rawtime;

    t.tm_year = anio - 1900;
    t.tm_mon = mes - 1;
    t.tm_mday = dia;
    t.tm_hour = hora;
    t.tm_min = min;
    t.tm_sec = 0;
    t.tm_isdst = -1;

    rawtime = mktime(&t);
    if(rawtime == (time_t)-1)
        return 0;

    return (unsigned long long)rawtime;
}

void convertirTimestampAFecha(unsigned long long ts, int *dia, int *mes, int *anio, int *hora, int *min)
{
    time_t rawtime = (time_t)ts;
    struct tm *info = localtime(&rawtime);

    if(info != NULL)
    {
        *dia = info->tm_mday;
        *mes = info->tm_mon + 1;
        *anio = info->tm_year + 1900;
        *hora = info->tm_hour;
        *min = info->tm_min;
    }
    else
    {
        *dia = 0;
        *mes = 0;
        *anio = 0;
        *hora = 0;
        *min = 0;
    }
}

void formatearFecha(unsigned long long ts, char *strFecha)
{
    int d, m, a, h, mn;
    convertirTimestampAFecha(ts, &d, &m, &a, &h, &mn);
    sprintf(strFecha, "%02d/%02d/%04d", d, m, a);
}

void formatearHora(unsigned long long ts, char *strHora)
{
    int d, m, a, h, mn;
    convertirTimestampAFecha(ts, &d, &m, &a, &h, &mn);
    sprintf(strHora, "%02d:%02d", h, mn);
}
