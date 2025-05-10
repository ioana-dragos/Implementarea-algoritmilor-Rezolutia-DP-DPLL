#include <iostream>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <math.h>
#include <chrono>

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

    return 1;
}


int aplicaRegula2(int**& clauze, int*& dimClauze, int& nrClauze,int maxLiteral)
{
    int verificat = 1;

    while (verificat)
    {
        verificat = 0;

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




bool suntIdentice(int* clauza1, int* clauza2, int dim1, int dim2)
{
    if(dim1!=dim2)
        return false;

    for(int i=0; i<dim1; ++i)
    {
        if(clauza1[i]!=clauza2[i])
            return false;
    }
    return true;
}

bool existaInMatrice(int** matrice, int* dimensiuni, int nrClauze, int* clauzaNoua, int dimNoua)
{
    for (int i = 0; i < nrClauze; ++i)
    {
        if (dimensiuni[i] != dimNoua) continue;

        bool identica = true;
        for (int j = 0; j < dimNoua; ++j)
        {
            if (matrice[i][j] != clauzaNoua[j])
            {
                identica = false;
                break;
            }
        }
        if (identica) return true;
    }
    return false;
}

bool esteComplementarSauDuplicat(int* clauza, int dimClauza, int literal)
{
    for (int i = 0; i < dimClauza; ++i)
    {
        if (clauza[i] == -literal)
            return true;
        if (clauza[i] == literal)
            return true;
    }
    return false;
}


int rezolutie(int**& clauze, int*& dimClauze, int& nrClauze, int maxClauze)
{


    int incercari;
    incercari = (nrClauze*(nrClauze-1))/2;
    if(incercari>100000)
        incercari/=8;
    else
        if (nrClauze > 50000)
            incercari /= 6;
    else if (nrClauze > 1000)
        incercari /= 4;

    int nr = 0;
    while(nr <= incercari)
    {
        bool aFostNou = false;

        for (int c1 = 0; c1 < nrClauze - 1 && nr <= incercari; ++c1)
        {
            for (int lit1 = 0; lit1 < dimClauze[c1] && nr <= incercari; ++lit1)
            {
                for (int c2 = c1 + 1; c2 < nrClauze && nr <= incercari; ++c2)
                {
                    for (int lit2 = 0; lit2 < dimClauze[c2] && nr <= incercari; ++lit2)
                    {
                        if (clauze[c1][lit1] == -clauze[c2][lit2])
                        {
                            nr++;
                            int* nouaClauza = new int[dimClauze[c1] + dimClauze[c2]];
                            int k = 0;

                            int literalAnulat1 = clauze[c1][lit1];
                            for(int i=0; i<dimClauze[c1]; ++i)
                            {
                                if (clauze[c1][i] !=literalAnulat1)
                                    nouaClauza[k++] = clauze[c1][i];
                            }
                            for (int i = 0; i < dimClauze[c2]; ++i)
                            {
                                if (clauze[c2][i]!=-literalAnulat1)
                                {
                                    if (!esteComplementarSauDuplicat(nouaClauza, k, clauze[c2][i]))
                                    {
                                        nouaClauza[k++] = clauze[c2][i];
                                    }
                                }
                            }

                            if (k== 0)
                            {
                                delete[] nouaClauza;
                                return 0;
                            }

                            sort(nouaClauza, nouaClauza + k);
                            if (!existaInMatrice(clauze, dimClauze, nrClauze, nouaClauza, k))
                            {

                                if (nrClauze >= maxClauze)
                                {

                                    maxClauze *= 2;
                                    int** tempClauze = new int*[maxClauze];
                                    int* tempDimClauze = new int[maxClauze];
                                    for (int j = 0; j < nrClauze; ++j)
                                    {
                                        tempClauze[j] = clauze[j];
                                        tempDimClauze[j] = dimClauze[j];
                                    }

                                    delete[] clauze;
                                    delete[] dimClauze;
                                    clauze = tempClauze;
                                    dimClauze = tempDimClauze;
                                }
                                clauze[nrClauze] = nouaClauza;
                                dimClauze[nrClauze] = k;
                                nrClauze++;
                                aFostNou = true;
                            }
                            else
                            {
                                delete[] nouaClauza;
                            }
                        }
                    }
                }
            }
        }
        if (!aFostNou)
            return 1;
    }

    return 1;
}


int algoritmDP(int**& clauze, int& nrClauze, int*& dimClauze, int& maxClauze,int maxLiteral)
{
    int rezultat1 = aplicaRegula1(clauze, dimClauze, nrClauze);
    if (rezultat1 == 0)
        return 0;
    if (nrClauze == 0)
        return 1;
    else
    {
        int rezultat2 = aplicaRegula2(clauze, dimClauze, nrClauze,maxLiteral);
        if (rezultat2 == 0)
            return 0;
        if (nrClauze == 0)
            return 1;
        if (rezolutie(clauze, dimClauze, nrClauze, maxClauze)==0)
            return 0;

        if (nrClauze == 0)
            return 1;
    }
    return 1;
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
    bool satisfiabil = algoritmDP(clauze, nrClauze, dimClauze, maxClauze,maxLiteral);
    auto stop = high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::duration<double>>(stop - start);
    if(satisfiabil)
    {
        cout << "Formula este satisfiabila.\n";
    }
    else
    {
        cout << "Formula nu este satisfiabila.\n";
    }
    cout << "Timp de executie: " << duration.count() << " secunde\n";
    for (int i = 0; i < nrClauze; i++)
    {
        delete[] clauze[i];
    }
    delete[] clauze;
    delete[] dimClauze;
    return 0;
}
