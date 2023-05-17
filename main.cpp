#include <iostream>
#include <vector>
#include <fstream>
#include <stdio.h>
#include <sstream>
#include <iterator>
#include <cstdlib>
#include <algorithm>
using namespace std;

vector<int> wierzcholek; //wektor wierzcholkow  z nastepnikami
vector<vector<int>> graf; //graf

void odczyt(){
    fstream plik;
    string linia;
    plik.open("akwb.txt", ios::in | ios::out);

    while(getline(plik, linia)){
        istringstream aktualna_linia(linia);                                                                                                                                    //istringstream zamienia string na strumień
        wierzcholek = vector<int>(istream_iterator<int>(aktualna_linia), istream_iterator<int>());
        graf.push_back(wierzcholek);
    }
    plik.close();
}


bool graf1(){ //sprawdza czy jest 1-grafem
    int sprawdzam1el; //przechowuje nastepnik ktory sprawdzamy
    for(int i=0; i<graf.size(); i++){
        for(int j=0; j<graf[i].size(); j++){
            sprawdzam1el = graf[i][j];                                                                                                                                            //przypisujemy pierwszy nastepnik ktory bedziemy badac
            for(int n=j+1; n<graf[i].size(); n++){
                                                                                                                                                                                    //sprawdzamy czy wierzcholek nie ma dwoch takich samych nastepnikow/lukow wielokrotnych
                if(sprawdzam1el == graf[i][n]){ // jesli graf[i][j]
                    return false;
                }
            }
        }
    }
    return true;
}

bool sprzezony(){ // sprawdzamy czy graf jest sprzezony
    vector<int> przechowaj_nastepniki;
    vector<int> czesc_wspolna;
    for(int i=0; i<graf.size(); i++){
        for(int j=0; j<graf[i].size(); j++){
            przechowaj_nastepniki.push_back(graf[i][j]);
        }
        for(int n=i+1; n<graf.size(); n++){
            czesc_wspolna.clear();
            sort(graf[n].begin(), graf[n].end());//porządkowanie grafu
            sort(przechowaj_nastepniki.begin(), przechowaj_nastepniki.end());//porzadkowanie następników
            set_intersection(graf[n].begin(), graf[n].end(), przechowaj_nastepniki.begin(), przechowaj_nastepniki.end(),
                             back_inserter(czesc_wspolna));
            if(!czesc_wspolna.empty()){
                if(graf[n] != przechowaj_nastepniki){
                    return false;
                }
            }
        }
        czesc_wspolna.clear();
        przechowaj_nastepniki.clear();
    }
    return true;
}

//zwraca nam puste_poprzedniki naszego grafu

vector<vector<int>> zrob_poprzedniki (vector<vector<int>> puste_poprzedniki){                                                                                                                   //f. zwraca wektor wektorów, parametrem jest wektor wektorów
    for(int wierzch=0; wierzch < graf.size(); wierzch++){
        for(auto nastepnik: graf[wierzch]){
            puste_poprzedniki[nastepnik].push_back(wierzch);
        }
    }
    return puste_poprzedniki;
}

bool liniowy(vector<vector<int>> poprzedniki){
    vector<int> przechowaj_nastepniki;
    vector<int> czesc_wspolna_nastepniki;
    vector<int> przechowaj_poprzedniki;
    vector<int> czesc_wspolna_poprzedniki;
    for(int i=0; i<graf.size(); i++){
        for(int j=0; j<graf[i].size(); j++){
            przechowaj_nastepniki.push_back(graf[i][j]);
        }
        for(int x=0; x<poprzedniki[i].size(); x++){
            przechowaj_poprzedniki.push_back(poprzedniki[i][x]);
        }
        for(int n=i+1; n<graf.size(); n++){
            czesc_wspolna_nastepniki.clear();
            czesc_wspolna_poprzedniki.clear();
            sort(graf[n].begin(), graf[n].end());
            sort(poprzedniki[n].begin(), poprzedniki[n].end());
            sort(przechowaj_nastepniki.begin(), przechowaj_nastepniki.end());
            sort(przechowaj_poprzedniki.begin(), przechowaj_poprzedniki.end());

            set_intersection(graf[n].begin(), graf[n].end(), przechowaj_nastepniki.begin(), przechowaj_nastepniki.end(),
                             back_inserter(czesc_wspolna_nastepniki));
            set_intersection(poprzedniki[n].begin(), poprzedniki[n].end(), przechowaj_poprzedniki.begin(), przechowaj_poprzedniki.end(),
                             back_inserter(czesc_wspolna_poprzedniki));
            if(!czesc_wspolna_nastepniki.empty()){
                if(graf[n] != przechowaj_nastepniki || !czesc_wspolna_poprzedniki.empty()){
                    return false;
                }
            }
        }
        czesc_wspolna_nastepniki.clear();
        czesc_wspolna_poprzedniki.clear();
        przechowaj_nastepniki.clear();
        przechowaj_poprzedniki.clear();
    }
    return true;
}

vector<pair<int, int>> zamien_w_oryginalny(vector<vector<int>> oryginalny){
    vector<pair<int, int>> para(oryginalny.size());
    vector<int> pomocniczy;
    int zmienna = 0;
    for(int i=0; i<oryginalny.size(); i++){
        para[i] = make_pair(zmienna, zmienna+1);
        zmienna += 2;
    }

    cout << "\n";
    cout<<"Lista lukow:"<<endl;
    for(int j=0; j < para.size(); j++){
        cout << j << "-->" << "(" << para[j].first <<","<< para[j].second << ")" << endl;
    }

    cout << "\n";


    for(int i=0; i<oryginalny.size()-1; i++){
        for(int j = i + 1; j<oryginalny.size(); j++){
            if(oryginalny[i] == oryginalny[j] && oryginalny[i].size() > 0) {
                pomocniczy.push_back(i);
                pomocniczy.push_back(j);
                para[i].second = para[oryginalny[i][0]].first;
                para[j].second = para[oryginalny[j][0]].first;
                for(int n =1; n<oryginalny[i].size(); n++){
                    para[oryginalny[i][n]].first = para[oryginalny[i][0]].first;
                }
            }
        }
    }

    for(int i=0; i<oryginalny.size(); i++) {
        if (find(pomocniczy.begin(), pomocniczy.end(), i) != pomocniczy.end()) {
            continue;
        }
        for (int j = 0; j < oryginalny[i].size(); j++) {
            para[oryginalny[i][j]].first = para[i].second;
        }
    }

    cout << "Graf oryginalny:" <<endl;
    for(int j=0; j < para.size(); j++){
        cout << j << "-->" << "(" << para[j].first <<","<< para[j].second << ")" << endl;
    }

    return para;
}

vector<pair<int, int>> indeksowanie(vector<pair<int, int>> para){
    int wartosc_sortujaca = 0;
    int obecna_wartosc;
    vector<int> wartosci_uzyte;
    for(int i=0; i<para.size(); i++){
        if(para[i].first > wartosc_sortujaca){
            obecna_wartosc = para[i].first;
            para[i].first = wartosc_sortujaca;
            if(para[i].second == obecna_wartosc){
                para[i].second = wartosc_sortujaca;
            }
            for(int j=i+1; j<para.size(); j++){
                if(para[j].first == obecna_wartosc){
                    para[j].first = wartosc_sortujaca;
                }
                if(para[j].second == obecna_wartosc){
                    para[j].second = wartosc_sortujaca;
                }

            }
            wartosci_uzyte.push_back(wartosc_sortujaca);
            wartosc_sortujaca++;
        }
        else if(find(wartosci_uzyte.begin(), wartosci_uzyte.end(), para[i].first) != wartosci_uzyte.end()){
            wartosc_sortujaca = wartosc_sortujaca;
        }
        else{
            wartosci_uzyte.push_back(para[i].first);
            wartosc_sortujaca++;
        }

        if(para[i].second > wartosc_sortujaca){
            obecna_wartosc = para[i].second;
            para[i].second = wartosc_sortujaca;
            for(int j=i+1; j<para.size(); j++){
                if(para[j].first == obecna_wartosc){
                    para[j].first = wartosc_sortujaca;
                }
                if(para[j].second == obecna_wartosc){
                    para[j].second = wartosc_sortujaca;
                }
            }
            wartosci_uzyte.push_back(wartosc_sortujaca);
            wartosc_sortujaca++;
        }
        else if(find(wartosci_uzyte.begin(), wartosci_uzyte.end(), para[i].second) != wartosci_uzyte.end()){
            wartosc_sortujaca = wartosc_sortujaca;
        }
        else{
            wartosci_uzyte.push_back(para[i].second);
            wartosc_sortujaca++;
        }
    }
    return para;
}

int maks_rozmiar(vector<pair<int, int>> max){
    int x;
    vector<int> wartosci_par;
    for(int i=0; i<max.size(); i++){
        wartosci_par.push_back(max[i].first);
        wartosci_par.push_back(max[i].second);
    }
    x = *max_element(wartosci_par.begin(), wartosci_par.end());
    return x + 1;
}

vector<vector<int>> graf_nowy(vector<pair<int, int>> nowy, vector<vector<int>> n_graf, int rozmiar){
    for(int i=0; i<nowy.size(); i++){
        n_graf[nowy[i].first].push_back(nowy[i].second);
    }
    return n_graf;
}


void zapisz_do_pliku(vector<vector<int>> koncowy_graf){
    fstream file;
    file.open("nowy_graf.txt", ios::out);
    ostream_iterator<int> i (file, " ");
    for(const auto& x: koncowy_graf){
        copy(x.cbegin(), x.cend(), i);
        file << "\n";
    }
}

int main(){
    odczyt();
    vector<vector<int>> poprzedniki(graf.size());
    vector<pair<int, int>> para(graf.size());

    poprzedniki = zrob_poprzedniki(poprzedniki); //uzupelniamy poprzedniki
    bool jest_1graf = graf1();
    bool jest_sprzezony = sprzezony();
    bool jest_liniowy = liniowy(poprzedniki);

    for(int i=0; i<graf.size(); i++) {
        cout << i << " --> ";
        for (int j: graf[i]) {
            cout << j << " --> ";
        }
        cout << "\n";
    }

    cout << "Rozmiar naszego grafu (ilosc wierzcholkow): " << graf.size() << endl;
    if(jest_1graf){
        cout<<"Graf jest 1-grafem"<<endl;
        if(jest_sprzezony){
            cout<<"Graf jest sprzezony"<<endl;
            if(jest_liniowy){
                cout<<"Graf jest liniowy"<<endl;
            }else{
                cout<<"Graf nie jest liniowy"<<endl;
            }
        }else{
            cout<<"Graf nie jest sprzezony"<<endl;}
    }else{
        cout<<"Graf nie jest 1-grafem"<<endl;
    }

    cout << "\n";
    cout<<"Poprzedniki grafu:"<<endl;
    for(int i=0; i<poprzedniki.size(); i++){
        cout << i << " --> ";
        for (int j: poprzedniki[i]) {
            cout << j << " --> ";
        }
        cout << "\n";
    }
    if(jest_1graf && jest_sprzezony){
        para = zamien_w_oryginalny(graf);
        para = indeksowanie(para);
        cout<<'\n';
        cout<<"Indeksowanie:"<<endl;
        for(int j=0; j < para.size(); j++){
            cout << j << "-->" << "(" << para[j].first << "," << para[j].second << ")" << endl;
        }

        cout<<"\n";

        vector<vector<int>> koniec_graf(maks_rozmiar(para));
        int p_rozmiar = maks_rozmiar(para);
        koniec_graf = graf_nowy(para, koniec_graf, p_rozmiar);
        zapisz_do_pliku(koniec_graf);

        cout<<"Nowy graf:"<<endl;
        for (int i = 0; i < koniec_graf.size(); i++) {
            cout << i << " --> ";
            for (int j: koniec_graf[i]) {
                cout << j << " --> ";
            }
            cout << "\n";
        }
    }

    return 0;
}
