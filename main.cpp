#include "matplotlibcpp.h"
#include <bits/stdc++.h>
#include <pthread.h>
#include <ctime>

namespace plt = matplotlibcpp;
using namespace std;
int qtd;
int jump;

vector<double> x;
vector<double> qtd_fast, qtd_naive;

string somar(const string& a, const string& b) { 
    string result = "";
    int s = 0;

    int i = a.size() - 1, j = b.size() - 1; 
    while (i >= 0 || j >= 0 || s == 1) { 
        s += ((i >= 0)? a[i] - '0': 0); 
        s += ((j >= 0)? b[j] - '0': 0); 

        result = char(s % 2 + '0') + result; 

        s /= 2; 

        i--; j--; 
    } 

    return result; 
}

int lengthController(string &str1, string &str2) { 
    const int len1 = str1.size(); 
    const int len2 = str2.size(); 
    if (len1 < len2) {
        const string novo(len2 - len1,'0');
        str1 = novo+str1;
        return len2; 
    } 
    else if (len1 > len2) { 
        const string novo(len1 - len2,'0');
        str2 = novo+str2;
    } 
    return len1; 
}

string DecimalToBinary(long long int number) { 
    string result = ""; 
    if (number <= 0) 
        return "0";
    else { 
        int i = 0; 
        while (number > 0) {    
            stringstream ss; 
            ss<<(number&1); 
            result = ss.str() + result; 
            number = number / 2; 
            i++; 
        } 
        return result; 
          
    } 
} 

string subtrair(string lhs, string rhs) { 
    int length = lengthController(lhs, rhs); 
    long long int diff; 
    string result; 
      
    for (int i = length-1; i >= 0; i--) { 
        diff = (lhs[i]-'0') - (rhs[i]-'0');
        if (diff >= 0) {
            result = DecimalToBinary(diff) + result; 
        } 
        else { 
            for (int j = i-1; j>=0; j--) { 
                lhs[j] = ((lhs[j]-'0') - 1) % 10 + '0'; 
                if (lhs[j] != '1') 
                    break; 
            } 
            result = DecimalToBinary(diff+2) + result; 
        } 
    } 
    return result; 
}

string MakeShifting(const string& str, const int& stepnum) {
    string shifted(stepnum, '0');
    return str + shifted;
} 

string multiplicar_rapido(string X, string Y) { // O(n¹·)
    const int n = lengthController(X, Y); 
      
    if (n == 1) return ((Y[0]-'0' == 1) && (X[0]-'0' == 1)) ? "1" : "0"; 
      
    const int metade = n/2;
       
    string Xl = X.substr(0, metade); 
    string Xr = X.substr(metade); 
      
    string Yl = Y.substr(0, metade); 
    string Yr = Y.substr(metade); 

    const string P1 = multiplicar_rapido(Xl, Yl); 
    const string P2 = multiplicar_rapido(Xr, Yr); 
    const string P3 = multiplicar_rapido(somar(Xl, Xr), somar(Yl, Yr)); 
      
    // return added string version 
    return somar(somar(MakeShifting(P1, 2*(n-metade)),P2),MakeShifting(subtrair(P3,somar(P1,P2)), n-(metade))); 
}

string multiplicar_devagar(const string& str1, const string& str2) { // O(n²)
    string allSum = "";
    const int n = str2.length();
    for (int j = 0; j < n; ++j) { 
        int secondDigit = str2[j] - '0'; 
        if (secondDigit == 1) { 
            allSum = somar(MakeShifting(str1,str2.size()-(j+1)), allSum); 
        } 
        else
            continue; 
          
    } 
    return allSum; 
}

void calcular_matriz_rapido(const string A[2][2], const string B[2][2], string C[2][2]) {
    const string P1 = multiplicar_rapido(A[0][0], subtrair(B[0][1], B[1][1]));
    const string P2 = multiplicar_rapido(B[1][1], somar(A[0][0], A[0][1])); 
    const string P3 = multiplicar_rapido(B[0][0], somar(A[1][0], A[1][1]));
    const string P4 = multiplicar_rapido(A[1][1], subtrair(B[1][0], B[0][0])); 
    const string P5 = multiplicar_rapido(somar(A[0][0], A[1][1]), somar(B[0][0], B[1][1])); 
    const string P6 = multiplicar_rapido(subtrair(A[0][1], A[1][1]), somar(B[1][0], B[1][1]));
    const string P7 = multiplicar_rapido(subtrair(A[0][0], A[1][0]), somar(B[0][0], B[0][1]));
    
    C[0][0] = somar(somar(P5, P4), subtrair(P6, P2)); 
    C[0][1] = somar(P1, P2);
    C[1][0] = somar(P3, P4);
    C[1][1] = subtrair(somar(P5, P1), somar(P3, P7));
}

void calcular_matriz_devagar(const string A[2][2], const string B[2][2], string C[2][2]) {
    for(int i = 0; i < 2; ++i) {
        for(int j = 0; j < 2; ++j) {
            for(int k = 0; k < 2; ++k) {
                C[i][j] = somar(C[i][j], multiplicar_devagar(A[i][k], B[k][j]));
            }
        }
    }
}

void* calcular_rapido(void* v) {
    random_device rd;
    mt19937_64 mt(rd());

    uniform_int_distribution<int> numeros(48, 49);

    for(int x = jump; x <= qtd; x+=jump) {
        string A[2][2], B[2][2], C[2][2];
        for(int i = 2; i--; )
            for(int j = 2; j--; ) {
                A[i][j].reserve(x);
                B[i][j].reserve(x);

                for(int k = x; k--; ) {
                    A[i][j].push_back(numeros(mt));
                    B[i][j].push_back(numeros(mt));                    
                }
            }

        const clock_t tempo = clock();
        calcular_matriz_rapido(A, B, C);
        qtd_fast.push_back(float(clock () - tempo) / CLOCKS_PER_SEC);
    }
    pthread_exit ( NULL ) ;
}

void* calcular_devagar(void* v) {
    random_device rd;
    mt19937_64 mt(rd());

    uniform_int_distribution<int> numeros(48, 49);

    for(int x = jump; x <= qtd; x+=jump) {
        string A[2][2], B[2][2], C[2][2];
        for(int i = 2; i--; )
            for(int j = 2; j--; ) {
                A[i][j].reserve(x);
                B[i][j].reserve(x);

                for(int k = x; k--; ) {
                    A[i][j].push_back(numeros(mt));
                    B[i][j].push_back(numeros(mt));                    
                }
            }

        const clock_t tempo = clock();
        calcular_matriz_devagar(A, B, C);
        qtd_naive.push_back(float(clock () - tempo) / CLOCKS_PER_SEC);
    }
    pthread_exit ( NULL ) ;
}

int main() {
    printf("Insira a quantidade máxima de dígitos: ");
    scanf(" %d", &qtd);

    jump = qtd/10;

    while(qtd < 10 || qtd > 10000) {
        printf("A quantidade deve estar entre 10 e 10000: ");
        scanf(" %d", &qtd);
    }

    for(int i = jump; i <= qtd; i+=jump)
        x.push_back(i);
    
    pthread_t id, id2;

    pthread_create (&id, NULL, calcular_rapido, NULL) ;
    pthread_create (&id2, NULL, calcular_devagar, NULL);

    pthread_join(id, NULL);
    pthread_join(id2, NULL);

    plt::figure_size(1200, 780);
    plt::xlabel("Quantidade de digitos");
    plt::ylabel("Tempo (s)");
    plt::named_plot("Karatsuba + Strassen", x, qtd_fast);
    plt::named_plot("Naive (paralelo)",x, qtd_naive);
    plt::title("Diferenca entre algoritmos");
    plt::legend();
    plt::show();

}