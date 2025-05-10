#include <iostream>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <chrono>
#include <iomanip>

using namespace std;
using namespace std::chrono;


bool existaDeja(int* clauza, int dim, int literal)
{
    for (int i = 0; i < dim; i++)
    {
        if (clauza[i] == literal) return true;
    }
    return false;
}

void genereazaClauze(int**& clauze, int*& dimClauze, int& nrClauze, int maxClauze, int maxLiteraliPerClauza, int maxLiteral)
{
    srand(time(0));
    for (int i = 0; i < maxClauze; i++)
    {
        int numarLiterali = rand() % maxLiteraliPerClauza + 1;
        clauze[i] = new int[numarLiterali];
        dimClauze[i] = numarLiterali;

        int j = 0;
        while (j < numarLiterali)
        {
            int literal = rand() % (2 * maxLiteral + 1) - maxLiteral;
            if (literal != 0 && !existaDeja(clauze[i], j, literal))
            {
                clauze[i][j] = literal;
                j++;
            }
        }

        sort(clauze[i], clauze[i] + dimClauze[i]);
        nrClauze++;
    }
}

int aplicaRegula1(int**& clauze, int*& dimClauze, int& nrClauze)
{
    int verificat = 1;

    while (verificat)
    {
        //cout<<"\nAplicam regula 1 ";
        verificat = 0;

        for (int i = 0; i < nrClauze; ++i)
        {
            if (dimClauze[i] == 1)
            {
                int L = clauze[i][0];
                verificat = 1;

                int newNrClauze = 0;
                for (int j = 0; j < nrClauze; ++j)
                {
                    bool contineL = false;
                    for (int k = 0; k < dimClauze[j]; ++k)
                    {
                        if (clauze[j][k] == L)
                        {
                            contineL = true;
                            break;
                        }
                    }
                    if (!contineL)
                    {
                        clauze[newNrClauze] = clauze[j];
                        dimClauze[newNrClauze] = dimClauze[j];
                        ++newNrClauze;
                    }
                    else
                    {
                        delete[] clauze[j];
                    }
                }
                nrClauze = newNrClauze;
                int Lc = -L;
                for (int j = 0; j < nrClauze; ++j)
                {
                    int nouaDim = 0;
                    for (int k = 0; k < dimClauze[j]; ++k)
                    {
                        if (clauze[j][k] != Lc)
                        {
                            clauze[j][nouaDim++] = clauze[j][k];
                        }
                    }
                    dimClauze[j] = nouaDim;

                    if (nouaDim == 0)
                    {

                        return 0;
                    }
                }
                break;
            }
        }
    }

    return 1; // regula 1 nu mai poate fi aplicată, continuă cu regula 2 sau 3
}



int aplicaRegula2(int**& clauze, int*& dimClauze, int& nrClauze,int maxLiteral)
{
    int verificat = 1;

    while (verificat)
    {
        verificat = 0;
        //cout<<"\nAplicam regula 2\n";
        int maxLit = maxLiteral + 1;
        int* pozitive = new int[maxLit];
        int* negative = new int[maxLit];

        for (int i = 0; i < maxLit; ++i)
        {
            pozitive[i] = 0;
            negative[i] = 0;
        }
        for (int i = 0; i < nrClauze; ++i)
        {
            for (int j = 0; j < dimClauze[i]; ++j)
            {
                int lit = clauze[i][j];
                int val = abs(lit);
                if (val < maxLit)
                {
                    if (lit > 0) pozitive[val]++;
                    else negative[val]++;
                }
            }
        }

        int literalPur = 0;
        for (int i = 1; i < maxLit; ++i)
        {
            if (pozitive[i] > 0 && negative[i] == 0)
            {
                literalPur = i;
                break;
            }
            if (negative[i] > 0 && pozitive[i] == 0)
            {
                literalPur = -i;
                break;
            }
        }

        if (literalPur != 0)
        {
            verificat = 1;
            //cout<<"Pentru literalul pur "<<literalPur<<endl;
            int newNrClauze = 0;
            for (int i = 0; i < nrClauze; ++i)
            {
                bool contineLiteral = false;
                for (int j = 0; j < dimClauze[i]; ++j)
                {
                    if (clauze[i][j] == literalPur)
                    {
                        contineLiteral = true;
                        break;
                    }
                }
                if (!contineLiteral)
                {
                    clauze[newNrClauze] = clauze[i];
                    dimClauze[newNrClauze] = dimClauze[i];
                    ++newNrClauze;
                }
                else
                {
                    delete[] clauze[i];
                }
            }

            nrClauze = newNrClauze;
            if (nrClauze == 0)
            {
                delete[] pozitive;
                delete[] negative;
                return 0;
            }
        }

        delete[] pozitive;
        delete[] negative;
    }

    return 1;
}



void copiazaFormula(int**& dst, int*& dimDst, int& nrDst, int** src, int* dimSrc, int nrSrc)
{
    nrDst = nrSrc;
    dst = new int*[nrDst];
    dimDst = new int[nrDst];
    for (int i = 0; i < nrDst; ++i)
    {
        dimDst[i] = dimSrc[i];
        dst[i] = new int[dimDst[i]];
        for (int j = 0; j < dimDst[i]; ++j)
            dst[i][j] = src[i][j];
    }
}

void adaugaClauzaUnu(int**& clauze, int*& dimClauze, int& nrClauze, int literal)
{
    int** nouClauze = new int*[nrClauze + 1];
    int* nouDim = new int[nrClauze + 1];
    for (int i = 0; i < nrClauze; ++i)
    {
        nouDim[i] = dimClauze[i];
        nouClauze[i] = clauze[i];
    }

    nouClauze[nrClauze] = new int[1];
    nouClauze[nrClauze][0] = literal;
    nouDim[nrClauze] = 1;
    ++nrClauze;

    delete[] clauze;
    delete[] dimClauze;
    clauze = nouClauze;
    dimClauze = nouDim;
}

void elibereazaMemorie(int** clauze, int* dimClauze, int nrClauze)
{
    for (int i = 0; i < nrClauze; ++i)
        delete[] clauze[i];
    delete[] clauze;
    delete[] dimClauze;
}


bool dpll(int** clauze, int* dimClauze, int nrClauze, int maxClauze,int maxLiteral)
{
    int rezultat1;
    rezultat1 = aplicaRegula1(clauze, dimClauze, nrClauze);
    if (rezultat1 == 0)
        return 0;
    if (nrClauze == 0)
        return 1;
    int rezultat2 = aplicaRegula2(clauze, dimClauze, nrClauze, maxLiteral);
    if (rezultat2 == 0)
        return 0;
    if (nrClauze == 0)
        return 1;

    int L = clauze[0][0];
    int** clauze1;
    int* dim1;
    int nr1;
    copiazaFormula(clauze1, dim1, nr1, clauze, dimClauze, nrClauze);
    adaugaClauzaUnu(clauze1, dim1, nr1, L);

    aplicaRegula1(clauze1, dim1, nr1);
    if (nr1 == 0)
    {
        elibereazaMemorie(clauze1, dim1, nr1);
        return true;
    }

    aplicaRegula2(clauze1, dim1, nr1, maxLiteral);
    if (nr1 == 0)
    {
        elibereazaMemorie(clauze1, dim1, nr1);
        return true;
    }
    int** clauze2;
    int* dim2;
    int nr2;
    copiazaFormula(clauze2, dim2, nr2, clauze, dimClauze, nrClauze);
    adaugaClauzaUnu(clauze2, dim2, nr2, -L);

    int rez1=aplicaRegula1(clauze2, dim2, nr2);
    if(rez1==0)
        return false;
    else
    {
        if (nr2 == 0)
        {
            elibereazaMemorie(clauze2, dim2, nr2);
            elibereazaMemorie(clauze1, dim1, nr1);
            return true;
        }
    }
    int rez2=aplicaRegula2(clauze2, dim2, nr2, maxLiteral);
    if(rez2==0)
        return false;
    else
    {
        if (nr2 == 0)
        {
            elibereazaMemorie(clauze2, dim2, nr2);
            elibereazaMemorie(clauze1, dim1, nr1);
            return true;
        }
    }

    elibereazaMemorie(clauze1, dim1, nr1);
    elibereazaMemorie(clauze2, dim2, nr2);
    return true;
}


void elibereazaMemorie(int** clauze, int nrClauze, int* dimClauze)
{
    for (int i = 0; i < nrClauze; i++)
    {
        delete[] clauze[i];
    }
    delete[] clauze;
    delete[] dimClauze;
}


int main()
{
    int maxClauze, maxLiteraliPerClauza, maxLiteral;
    cout << "Introduceti numarul maxim de clauze: ";
    cin >> maxClauze;
    cout << "Introduceti numarul maxim de literali per clauza: ";
    cin >> maxLiteraliPerClauza;
    maxLiteral = maxLiteraliPerClauza;

    int** clauze = new int*[maxClauze];
    int* dimClauze = new int[maxClauze];
    int nrClauze = 0;

    genereazaClauze(clauze, dimClauze, nrClauze, maxClauze, maxLiteraliPerClauza, maxLiteral);

    auto start = high_resolution_clock::now();
    bool satisfiabil = dpll(clauze, dimClauze, nrClauze, maxClauze,maxLiteral);
    auto stop = high_resolution_clock::now();

    auto duration = chrono::duration_cast<chrono::duration<double>>(stop - start);
    if(satisfiabil!=0)
    {
        cout << "Formula este satisfiabila.\n";
    }
    else
    {
        cout << "Formula nu este satisfiabila.\n";
    }
    cout << fixed << setprecision(6);
    cout << "Timp: " << duration.count() << " secunde\n";

    elibereazaMemorie(clauze, maxClauze, dimClauze);

    return 0;
}
