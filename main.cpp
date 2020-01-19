#include <iostream>
#include <string.h>
#include <vector>
#include <string>
#include <algorithm>
#define DIGITLIMIT 1000
#define ZERO 48

using namespace std;

class BigNumber {
    public:
// Constructors
    BigNumber(long number) : BigNumber() {
        storeNum(number);
    }

    BigNumber(string str) : BigNumber() {
        storeStr(str);
    }

    BigNumber() : decimal(0), negativity(false) {}

// Destructor
    ~BigNumber() {
        vector<char>().swap(storage);
    }
    

// Basic function
    void print() {
        int l = length();
        if (negativity) {
            printf("-");
        }
        for (int i=l-1; i>=0; i--) {
            printf("%c", storage[i]);
            if (((l / 3 * 3 == i) && i != 0) || (i % 3 == 0 && i != 0)) printf(",");
        }
        if (l == 0) printf("0");
    }

    void println() {
        print();
        printf("\n");
    }

    int length() {
        return storage.size();
    }

    bool isNegative() {
        return negativity;
    }

    BigNumber& flipNegativity() {
        negativity = !negativity;
        return *this;
    }

    bool isZero() {
        return storage.size() == 1 && storage.at(0) == '0';
    }

// Operater overloading
    BigNumber& operator=(const BigNumber& origin) {
        negativity = origin.negativity;
        decimal = origin.decimal;
        storage = origin.storage;

        return *this;
    }

    friend bool operator==(const BigNumber& a, const BigNumber& b) {
        return a.storage == b.storage 
            && a.negativity == b.negativity 
            && a.decimal == b.decimal;
    }

    friend bool operator<(const BigNumber& a, const BigNumber& b) {
        
    }

    friend bool operator<(const BigNumber& a, int b) {
        if (b == 0) return a.negativity;
    }

    friend bool operator>(const BigNumber& a, const BigNumber& b) {
        
    }

    friend bool operator>(const BigNumber& a, int b) {
        if (b == 0) return !a.negativity;
    }

    // TODO: overflow 
    friend BigNumber operator+(BigNumber a, BigNumber b) {
        //BigNumber b = c;
        // check negativity
        int negFlag = false;
        if (a < 0) {
            if (b < 0) {
                negFlag = true;
                a.flipNegativity();
                b.flipNegativity();
            }
            else {
                a.flipNegativity();
                return b-a;
            }
        } else {
            if (b < 0) {
                b.flipNegativity();
                return a-b;
            }
        }

        // real thing
        int length1 = a.length();
        int length2 = b.length();
        int length = 0;
        if (length1 > length2) {
            length = length1;
            for (int i=length2; i<length1; i++) b.storage.push_back('0');
        } else {
            length = length2;
            for (int i=length1; i<length2; i++) a.storage.push_back('0');
        }

        int carry = 0;
        for (int i=0; i<length; i++) {
            int d1 = a.storage.at(i) - '0', d2 = b.storage.at(i) - '0';
            int sum = d1 + d2 + carry;
            carry = 0;
            if (sum / 10 == 1) {
                carry = 1;
                sum -= 10;
            }

            a.storage.at(i) = sum + '0';
            // Overflow
        }
        if (carry) a.storage.push_back(carry+'0');

        if (negFlag) a.flipNegativity();

        if (a.isZero()) a.negativity = false;
        return a;
    }
    // TODO: Negative
    friend BigNumber operator-(BigNumber a, BigNumber b) {
        // same num check
        if (a == b) return BigNumber();
        //negativity check
        if (a < 0) {
            if (b < 0) {
                a.flipNegativity();
                b.flipNegativity();
                return b-a;
            }
            else {
                b.flipNegativity();
                return a+b;
            }
        } else {
            if (b < 0) {
                b.flipNegativity();
                return a+b;
            }
        }

        // check which is bigger
        int length1 = a.length();
        int length2 = b.length();
        int length = 0;
        if (length1 > length2) {
            length = length1;
            for (int i=length2; i<length1; i++) b.storage.push_back('0');
        } else {
            length = length2;
            for (int i=length1; i<length2; i++) a.storage.push_back('0');
            //swap(a, b);
            a.negativity = true;
        }

        int carry = 0;
        for (int i=0; i<length; i++) {
            int d1 = a.storage[i] - '0', d2 = b.storage[i] - '0';
            int sum = d1 - d2 - carry;
            carry = 0;
            if (sum < 0) {
                carry = 1;
                sum += 10;
            }

            if (i == length - 1 && sum == 0) a.storage.pop_back();
            else a.storage.at(i) = sum + '0';
            
            
        }

        a.storage.shrink_to_fit();


        if (a.isZero()) a.negativity = false;
        return a;
    }

    BigNumber operator-() {
        BigNumber o = *this;
        return o.flipNegativity();
    }

    private:

// Private Variable
    vector<char> storage;
    int decimal;
    bool negativity;

// Internal Utility
    void storeNum(long num) {
        // no negative
        for (int i=0; i<sizeof(num); i++) {
            storage.push_back(num % 10 + '0');
            num /= 10;
        }
    }

    bool storeStr(string str) {
        if (str.length() > DIGITLIMIT) return false;
        if (str[0] == '-') str = str.substr(1), negativity = true;
        for (int i=0; i<str.length(); i++) {
            storage.push_back(str[str.length()-i-1]);
        }
        return true;
    }


};

int main() {
    BigNumber num1("-1"), num2("-1");
    BigNumber num = num1-num2;
    num.println();
    num1.println();
    return 0;
}