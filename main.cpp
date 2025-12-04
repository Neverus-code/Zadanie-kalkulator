#include <iostream>
#include <vector>

// #### & #### Other #### & #### \\

/// @brief Maksymalna wielkość podstawy
const int maxBase = 36;

/// @brief Słownik dozwolonych znaków
const char dictionary[] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','x','y','Z'};

/// @brief Funkcja obsługująca błędy
void error() {
    std::cout << "Get Out!!\n"; 
    exit(-1);
}

/// @brief Funkcja do szybkich potęg
/// @param base podstawa 
/// @param exponent odległość liczona od prawej do lewej
/// @return zwraca base^exponent
unsigned long long fastPow(unsigned long long  base, unsigned long long  exponent) {
    unsigned long long Result = 1;
    while (exponent > 0) {
        if (exponent % 2 == 1)
            Result *= base;
        base *= base;
        exponent /= 2;
    }
    return Result;
}

/// @brief Funkcja zmieniająca małe litery na duże
/// @param c mała litera
/// @return zwraca duży odpowiednik litery
char bigLetters(char c) {
    return (c >= 'a' && c <= 'z') ? c - 32 : c;
}

/// @brief Funkcja do czyszczenia input ze znaków niebędoncych w słowniku, dzieli na pojedyncze symbole i zmienia litery na duże
/// @param value input
/// @return Zwraca wektor symboli
std::vector<char> clearInputTextAndSlice(std::string value) {
    std::vector<char> temp;
    for(int i = 0; i < value.size(); i++) {
        for(int j = 0; j < maxBase;j++) {
            if(bigLetters(value[i]) == dictionary[j]) {
                temp.push_back(bigLetters(value[i]));
            }
        }
    }
    return temp;
}

/// @brief Funkcja do enkodowania liczby z symboli na wartości przypisane danemu symbolowi
/// @param value Liczba do zmiany typu std::vector<char>
/// @return Zwraca typ std::vector<int> z liczbą zapisaną odpowiednimi wartościami
std::vector<int> encode(std::vector<char> value) {
    std::vector<int> temp;
    for(int i = 0; i < value.size(); i++) {
        for(int j = 0; j < maxBase;j++) {
            if(value[i] == dictionary[j]) {
                temp.push_back(j);
            }
        }
    }
    return temp;
}

/// @brief Funkcja do dekodowania liczby z wartości na symbole przypisane danej wartości
/// @param value Liczba do zmiany typu std::vector<int>
/// @return Zwraca typ std::vector<char> z liczbą zapisaną symbolami
std::vector<char> decode(std::vector<int> value) {
    std::vector<char> temp;
    for(int i = 0; i < value.size(); i++) {
        for(int j = 0; j < maxBase;j++) {
            if(value[i] == j) {
                temp.push_back(dictionary[j]);
            }
        }
    }
    return temp;
}

/// @brief Funkcja do sprawdzania czy symbol jest dozwolony w danej podstawie
/// @param base Podstawia liczby
/// @param value Liczba do sprawdzenia
/// @return Zwrawca True jeśli wykryje niedozwolony, zwraca False gdy wszystkie symbole są dozwolone
bool isIllegalSymbol(int base, std::vector<int> value) {
    for(int i = 0; i < value.size(); i++) {
       if(value[i] >= base) { return true; }
    }
    return false;
}



/// @brief Oblicza z wektora cyfr liczbę ull.
/// @param base podstawa liczby
/// @param x wektor cyfr
/// @return zwraca liczbę ull o podstawie 10
unsigned long long changeToBase10Number(int base, std::vector<int> x) {
    // w base10
    unsigned long long result = 0;
    unsigned long long last = x.size()-1;
    for(int i = 0; i < x.size(); i++) {
        result += fastPow(base,i) * x[last];
        last--;
    }
    return result;
}

/// @brief Funkcja do zmiany między podstawami
/// @param base Podstawa liczby
/// @param value liczba
/// @param wantedBase Oczekiwana podstawa
/// @return Liczba w oczekiwanej podstawie
std::vector<int> changeBase(int base, std::vector<int> value, int wantedBase){

    // max base 36
    if(base > maxBase|| base <= 1 || wantedBase > maxBase|| wantedBase <= 1) { error(); }

    if(isIllegalSymbol(base,value)) { error(); }

    std::vector<int> temp;

    unsigned long long test = changeToBase10Number(base,value);
    while(test >= wantedBase) {
        temp.push_back(test % wantedBase);
        test = (test / wantedBase);
    }
    temp.push_back(test);

    std::vector<int> temp2;
    for(int i = temp.size()-1; i >= 0;i--) {
        temp2.push_back(temp[i]);
    }

    return temp2;
}

/// @brief Funkcja do dodawanie 2 liczb o tej samej podstawie, większa liczba zawsze jest górna.
/// @param base Podstawa liczb
/// @param value1 Pierwsza liczba
/// @param value2 Druga lczba
/// @param c Wskaźnik do zwracania wartości przeniesienia 
/// @return Zwraca dodane liczby
std::vector<int> add(int base, std::vector<int> value1, std::vector<int> value2, std::vector<int>* c) {
    if(base > maxBase|| base <= 1) { error(); }
    if(isIllegalSymbol(base,value1) || isIllegalSymbol(base,value2)) { error(); }


    int iter = 0;
    std::vector<int> temp;
    std::vector<int> top;
    std::vector<int> bottom;

    if(value1.size() > value2.size()) { 
        iter = value1.size(); 
        top = value1;

        for(int i = 0; i < value1.size()-value2.size();i++) {
            bottom.push_back(0);
        }

        for(int i = 0; i < value2.size();i++) {
            bottom.push_back(value2[i]);
        }

    }

    if(value1.size() < value2.size()) { 
        iter = value2.size(); 
        top = value2;
        for(int i = 0; i < value2.size()-value1.size();i++) {
            bottom.push_back(0);
        }
        for(int i = 0; i < value1.size();i++) {
            bottom.push_back(value1[i]);
        }
    }

    if(value1.size() == value2.size()) { 
        iter = value1.size();
        top = value1;
        bottom = value2;
    }

    std::vector<int> carry(iter+1, 0);

    for(int i = iter-1; i >= 0;i--) {
        int sum = top[i] + bottom[i] + carry[i];

        if(sum >= base) {
            temp.push_back(sum % base);
            if(i > 0) {
                carry[i-1] += sum / base;
            } else {
                temp.push_back(sum / base);
                // carry[iter-1] += sum / base;

            }

        }else {
            temp.push_back(sum);
        }

    }
    
    c->clear();
    for(int i = 0; i < carry.size(); i++) {
        c->push_back(carry[i]);
    }

    std::vector<int> temp2;
    for(int i = temp.size()-1; i >= 0;i--) {
        temp2.push_back(temp[i]);
    }

    return temp2;

}










// #### & #### GUI #### & #### \\

/// @brief Typ wyliczeniowy definiujący stany menu
enum MenuState {
    MAIN_MENU,
    CALCULATOR_MAIN,
    CALCULATOR_BASE_CHOOSE,
    CALCULATOR_OTHER_BASE,
    CALCULATOR_ADD,
    EXIT
};

/// @brief Stała zmienna szerekości ScreenBuffer
const int width = 79;

/// @brief Stała zmienna wysokości ScreenBuffer
const int height = 19;

/// @brief Zmienna do sprawdzania czy jest to pierwsze wywołanie draw, by uniknąć błędów z usuwaniem złych części konsoli
bool isFirst = true;


/// @brief Tablica zawierająca wyświetlane znaki w konsoli
char ScreenBuffer[width*height];

int base = 10;

/// @brief Funkcja czyszcząca ScreenBuffer i wstawiająca do niego ramkę
void frame() {
    for(int i = 0; i < width*height;i++) { ScreenBuffer[i] = ' '; }
    for(int i = 0; i < width;i++) { ScreenBuffer[i] = '#'; ScreenBuffer[width*(height-1)+i] = '#'; }
    for(int i = 0; i < height;i++) { ScreenBuffer[width*i+(width-1)] = '#'; ScreenBuffer[width*i] = '#'; }
}

/// @brief Funkcja do cofania kursora na początek i używająca funkcji frame do czyszczenia ScreenBuffer
/// @param value ilość dodatkowych lini
void clear(int value) {
    if(isFirst == false) {
        for(int i = 0; i < height+value;i++) {
            std::cout << "\033[F";  
        }
    }
    frame();
}

/// @brief Funkcja do Rysowania ScreenBuffer w konsoli dla lepszego efektu przed użyć funkcji clear
void draw() {
    isFirst = false;
    for(int j = 0; j < height;j++) {
        for(int i = 0; i < width;i++) {
            std::cout << ScreenBuffer[j*width+i];
        }
        std::cout << "\n";
    }
}

/// @brief Funkcja do wstawiania tekstu do ScreenBuffer
/// @param x pozycja X tekstu od lewej
/// @param y pozycja Y tekstu od lewej
/// @param text wartość tekstu do wstawienia
void putText(int x, int y, std::string text) {
    if(x > width || x < 0 || y > height || y < 0 || x+text.size() > width) {
        error();
    }
    for(int i = 0; i < text.size();i++) {
        ScreenBuffer[(y*width+x+i)] = text[i];
    }
}

/// @brief Funkcja do usuwania zer na początku wektora
/// @param value wektor wejściowy
/// @return zwraca wektor wyjściowy
std::vector<char> deleteFirstZero(std::vector<char> value) {
    if(value.size() > 0 && value[0] == '0') {
        std::vector<char> temp;
        int zero = 0;
        for(int i =0; i < value.size();i++) {

            if(value[i] == '0') {
                zero++;
            } else {
                break;
            }
        }
        for(int i = zero; i < value.size();i++) {
            temp.push_back(value[i]);
        }
        return temp;
    }
    return value;
}

/// @brief Funkcja ustawiająca znaki w kształt tabeli z kolumnami do ScreenBuffer
/// @param x pozycja X tabeli względem jej prawego górnego rogu
/// @param y pozycja Y tabeli względem jej prawego górnego rogu
/// @param col0 Wektor zawierający przeniesienia
/// @param col1 Wektor zawierający górną liczbę równania
/// @param col2 Wektor zawierający dolną liczbę równania
/// @param col3 Wektor zawierający wynik działania z col1 i col2
/// @param symbol Symbol działania
void putTable(int x, int y, std::vector<char> col0,std::vector<char> col1,std::vector<char> col2,std::vector<char> col3, char symbol) {

    // std::vector<char> carry  = deleteFirstZero(col0);
    std::vector<char> carry  = col0;
    std::vector<char> top    = deleteFirstZero(col1);
    std::vector<char> bottom = deleteFirstZero(col2);
    std::vector<char> output = deleteFirstZero(col3);

    int biggest = carry.size();
    if(top.size() > biggest) biggest = top.size();
    if(bottom.size() > biggest) biggest = bottom.size();
    if(output.size() > biggest) biggest = output.size();

    if(x > width-2 || x < 1 || (x-(biggest*2+1)) < 1 || y < 1 || y+6 > height) { error();}

    int temp = carry.size()-1;
    for(int i=0; i < carry.size();i++) {
        if(carry[temp] != '0') {
            ScreenBuffer[(((y)*width)+x-i*2)] = carry[temp]; 
            
            
        }
        temp--;
    }

    temp = top.size()-1;
    for(int i=0; i < top.size();i++) {
        ScreenBuffer[(((y+1)*width)+x-i*2)] = top[temp]; 
        temp--;
    }

    temp = bottom.size()-1;
    for(int i=0; i < bottom.size();i++) {
        ScreenBuffer[(((y+2)*width)+x-i*2)] = bottom[temp]; 
        temp--;
    }

    for(int i=0; i < biggest*2;i++) {
        ScreenBuffer[(((y+3)*width)+x-i)] = '='; 
        temp--;
    }
    ScreenBuffer[(((y+3)*width)+x-(biggest*2+1))] = symbol; 

    temp = output.size()-1;
    for(int i=0; i < output.size();i++) {
        ScreenBuffer[(((y+4)*width)+x-i*2)] = output[temp]; 
        temp--;
    }
}

/// @brief Funkcja pomocnicza do ujednolicenia wyświetlania podstawy
void putBaseStatus() {
    putText(68,16, "Base: ");
    putText(74,16, std::to_string(base));
}
 
/// @brief Ustawia, obsługuje i wyświetla MAIN_MENU
/// @param state Stan menu
void mainMenu(MenuState* state) {
    clear(1);
    std::string inpt;
    std::vector<char> cleanInput;

    putText(20,2, "UMSC -- Ultra. Mega. Super. Calculator.");

    putText(20,5, "1 -- Calculator");
    putText(20,6, "0 -- Exit");

    draw();

    for(int i = 0; i < inpt.size()+44;i++) {
        std::cout << " ";  
    }
    std::cout << "\r"; 
    std::cout << ">> ";  
    std::getline(std::cin, inpt);
    cleanInput = clearInputTextAndSlice(inpt);

    if(cleanInput.size() == 1 && cleanInput[0] == '1' ) {
       *state = MenuState::CALCULATOR_MAIN;
    } else if(cleanInput.size() == 1 && cleanInput[0] == '0') {
       *state = MenuState::EXIT;
    }
}

/// @brief Menu, w którym można wybrać tryb kalkulatora
/// @param state Stan menu
void calculatorMain(MenuState* state) {

    clear(1);
    std::string inpt;
    std::vector<char> cleanInput;

    putText(5,2, "Choose mode");

    putText(5,4, "1 - Change base");
    putText(5,6, "2 - Add");

    putText(5,8, "0 - Back");

    putBaseStatus();

    draw();

    for(int i = 0; i < inpt.size()+44;i++) {
        std::cout << " ";  
    }
    std::cout << "\r"; 
    std::cout << ">> ";  
    std::getline(std::cin, inpt);
    cleanInput = clearInputTextAndSlice(inpt);

    if(cleanInput.size() == 1 && cleanInput[0] == '1') {
       *state = MenuState::CALCULATOR_BASE_CHOOSE;
    } else if(cleanInput.size() == 1 && cleanInput[0] == '2') {
       *state = MenuState::CALCULATOR_ADD;
    }else if(cleanInput.size() == 1 && cleanInput[0] == '0') {
       *state = MenuState::MAIN_MENU;
    }
}

/// @brief Menu wyboru predefiniowanej bazy
/// @param state Stan menu
void calculatorBaseChoose(MenuState* state) {
    clear(1);
    std::string inpt;
    std::vector<char> cleanInput;

    putText(10,2, "Choose base");

    putText(10,4, "1 - base-2 ");
    putText(10,6, "2 - base-8 ");
    putText(10,8, "3 - base-10 ");
    putText(10,10, "4 - base-16 ");
    putText(10,12, "5 - Other");
    putText(10,14, "0 - Back");

    putBaseStatus();

    draw();

    for(int i = 0; i < inpt.size()+44;i++) {
        std::cout << " ";  
    }
    std::cout << "\r"; 
    std::cout << ">> ";  
    std::getline(std::cin, inpt);
    cleanInput = clearInputTextAndSlice(inpt);

    if(cleanInput.size() == 1 && cleanInput[0] == '1') {
        base = 2;
        *state = MenuState::CALCULATOR_MAIN;
    } else if(cleanInput.size() == 1 && cleanInput[0] == '2') {
        base = 8;
        *state = MenuState::CALCULATOR_MAIN;
    }else if(cleanInput.size() == 1 && cleanInput[0] == '3') {
        base = 10;
        *state = MenuState::CALCULATOR_MAIN;
    }else if(cleanInput.size() == 1 && cleanInput[0] == '4') {
        base = 16;
        *state = MenuState::CALCULATOR_MAIN;
    }else if(cleanInput.size() == 1 && cleanInput[0] == '5') {
       *state = MenuState::CALCULATOR_OTHER_BASE;
    }else if(cleanInput.size() == 1 && cleanInput[0] == '0') {
       *state = MenuState::CALCULATOR_MAIN;
    }
}

/// @brief Menu wyboru innej niż predefiniowana baza
/// @param state Stan menu
void calculatorOtherBase(MenuState* state) {
    clear(1);
    std::string inpt;
    std::vector<char> cleanInput;

    putText(10,2, "Choose other 2-36");
    putText(10,4, "0 - Back");

    putBaseStatus();

    draw();

    for(int i = 0; i < inpt.size()+44;i++) {
        std::cout << " ";  
    }
    std::cout << "\r"; 
    std::cout << ">> ";  
    std::getline(std::cin, inpt);
    cleanInput = clearInputTextAndSlice(inpt);

    if(cleanInput.size() == 1 && cleanInput[0] == '0') {
       *state = MenuState::CALCULATOR_BASE_CHOOSE;
    }

    if(cleanInput.size() == 1 && ( cleanInput[0] == '2' || cleanInput[0] == '3' || cleanInput[0] == '4' || cleanInput[0] == '5' || cleanInput[0] == '6' || cleanInput[0] == '7' || cleanInput[0] == '8' || cleanInput[0] == '9')) {
        std::vector<int> out = encode(cleanInput);
        unsigned int result = 0;
        unsigned int last = out.size()-1;
        for(int i = 0; i < out.size(); i++) {
            result += (int)fastPow(10,i) * out[last];
            last--;
        }
        if(result > 0 && result <= maxBase) { base = result; }

        *state = MenuState::CALCULATOR_MAIN;

    }
    if(cleanInput.size() == 2 &&  ((cleanInput[0] == '1' && (cleanInput[1] == '0' || cleanInput[1] == '1' || cleanInput[1] == '2' || cleanInput[1] == '3' || cleanInput[1] == '4' || cleanInput[1] == '5' || cleanInput[1] == '6' || cleanInput[1] == '7' || cleanInput[1] == '8' || cleanInput[1] == '9')) ||
                           (cleanInput[0] == '2' && (cleanInput[1] == '0' || cleanInput[1] == '1' || cleanInput[1] == '2' || cleanInput[1] == '3' || cleanInput[1] == '4' || cleanInput[1] == '5' || cleanInput[1] == '6' || cleanInput[1] == '7' || cleanInput[1] == '8' || cleanInput[1] == '9')) ||
                           (cleanInput[0] == '3' && (cleanInput[1] == '0' || cleanInput[1] == '1' || cleanInput[1] == '2' || cleanInput[1] == '3' || cleanInput[1] == '4' || cleanInput[1] == '5' || cleanInput[1] == '6'))))  {
        std::vector<int> out = encode(cleanInput);
        unsigned int result = 0;
        unsigned int last = out.size()-1;
        for(int i = 0; i < out.size(); i++) {
            result += (int)fastPow(10,i) * out[last];
            last--;
        }
        if(result > 0 && result <= maxBase) { base = result; }
        *state = MenuState::CALCULATOR_MAIN;
        }
}

/// @brief Menu dodawania 2 liczb ze sobą w różnych systemach i pokazująca w base10
/// @param state Stan menu
void calculatorAdd(MenuState* state) {

    static int Mode = 0;
    static std::vector<int> Carry;
    static std::vector<char> First;
    static std::vector<char> Second;
    
    clear(1);
    std::string inpt;
    std::vector<char> cleanInput;

    switch (Mode)
    {
    case 0:
        putText(6,2, "Choose");
        putText(6,4, "1 - set numbers ");
        putText(6,5, "0 - Back");
        break;
    case 1:
        putText(6,2, "Set 1st Number");
        break;
    case 2:
        putText(6,2, "Set 2nd Number");
        break;
    case 3:
        std::vector<int> outpt = add(base,encode(First),encode(Second),&Carry);
        std::vector<int> base10 = changeBase(base,outpt,10);
        std::vector<char> Output = decode(outpt);
        std::vector<char> Outputbase10 = decode(base10);

        putTable(75,6,decode(Carry),First,Second,Output,'+');
        int padding = Outputbase10.size();
        std::string baseString(Outputbase10.begin(), Outputbase10.end());

        if((67-padding) > 0 && (74-padding) > 7) {
            putText(69-padding,12, "Base10: ");
            putText(76-padding,12, baseString);
        } else {
            putText(67,12, "Base10: too big");
        }


        putText(6,2, "Choose");
        putText(6,4, "1 - set numbers ");
        putText(6,5, "0 - Back");

        break;
    }
    putBaseStatus();

    draw();

    for(int i = 0; i < inpt.size()+44;i++) {
        std::cout << " ";  
    }
    std::cout << "\r"; 
    std::cout << ">> ";  
    std::getline(std::cin, inpt);
    cleanInput = clearInputTextAndSlice(inpt);

    if(Mode == 0 || Mode == 3) {
        if(cleanInput.size() == 1 && cleanInput[0] == '1') {
            Mode = 1;
        } else if(cleanInput.size() == 1 && cleanInput[0] == '0') {
           *state = MenuState::CALCULATOR_MAIN;
           Mode = 0;
        }
    } else if(Mode == 1) {
        First = cleanInput;

        if(!(isIllegalSymbol(base,encode(cleanInput)) == true) && cleanInput.size() > 0) {
            Mode=2;
        }

        
    } else if(Mode == 2) {
        Second = cleanInput;
            if(!(isIllegalSymbol(base,encode(cleanInput)) == true) && cleanInput.size() > 0) {
            Mode=3;
        }
    } 
}

int main(int argc, char const *argv[]) {
    bool ShouldStop = false;
    MenuState state = MenuState::MAIN_MENU;

    while(!ShouldStop) {
        switch (state)
        {
        case MenuState::MAIN_MENU:
            mainMenu(&state);
            break;
        case MenuState::CALCULATOR_MAIN:
            calculatorMain(&state);
            break;
        case MenuState::CALCULATOR_BASE_CHOOSE:
            calculatorBaseChoose(&state);
            break;
        case MenuState::CALCULATOR_OTHER_BASE:
            calculatorOtherBase(&state);
            break;
        case MenuState::CALCULATOR_ADD:
            calculatorAdd(&state);
            break;
        case MenuState::EXIT:
            ShouldStop = true;
            break;
        default:
            state = MenuState::EXIT;
            break;
        }
    }

    return 0;
}