#include <stdio.h>
#include <stdlib.h>

unsigned long long decimalToBCD(long long decimal) {
    unsigned long long bcd = 0;
    int place = 0;
    int isNegative = (decimal < 0);

    decimal = labs(decimal); 

    while (decimal > 0) {
        int digit = decimal % 10;
        bcd |= ((unsigned long long)digit << (place * 4));
        decimal /= 10;
        place++;
    }

    if (isNegative) {
        bcd |= (0b1000ULL << ((place - 1) * 4)); 
    }

    return bcd;
}

long long BCDToDecimal(unsigned long long bcd) {
    long long decimal = 0;
    int place = 0;
    int isNegative = (bcd >> 63) & 1;

    bcd &= ~(1ULL << 63); 

    while (bcd > 0) {
        int digit = (bcd & 0xF);
        decimal += (digit * (1LL << (place * 4)));
        bcd >>= 4;
        place++;
    }

    return (isNegative) ? -decimal : decimal;
}

unsigned long long addBCD(unsigned long long bcd1, unsigned long long bcd2) {
    unsigned long long result = 0;
    int carry = 0;
    int mask = 0xF;

    for (int i = 0; i < 16; i++) { 
        int digit1 = (bcd1 >> (i * 4)) & mask;
        int digit2 = (bcd2 >> (i * 4)) & mask;

        int sum = digit1 + digit2 + carry;

        if (sum > 9) {
            sum -= 10;
            carry = 1;
        } else {
            carry = 0;
        }

        result |= ((unsigned long long)sum << (i * 4));
    }

    return result;
}

unsigned long long subtractBCD(unsigned long long bcd1, unsigned long long bcd2) {
    unsigned long long result = 0;
    int borrow = 0;
    int mask = 0xF;

    for (int i = 0; i < 16; i++) { 
        int digit1 = (bcd1 >> (i * 4)) & mask;
        int digit2 = (bcd2 >> (i * 4)) & mask;

        int diff = digit1 - digit2 - borrow;

        if (diff < 0) {
            diff += 10;
            borrow = 1;
        } else {
            borrow = 0;
        }

        result |= ((unsigned long long)diff << (i * 4));
    }

    return result;
}


void printBCD(unsigned long long bcd) {
    int isFirstDigit = 1; 
    
    for (int i = 15; i >= 0; i--) {
        int digit = (bcd >> (i * 4)) & 0xF;

        if (digit != 0 || i == 0) {
            if (!isFirstDigit) {
                printf(" ");
            }

            printf("%d%d%d%d", (digit >> 3) & 1, (digit >> 2) & 1, (digit >> 1) & 1, digit & 1);

            isFirstDigit = 0; 
        }
    }

    printf("\n");
}

int main() {
    long long num1, num2;
    char operation;

    printf("Enter the first integer: ");
    if (scanf("%lld", &num1) != 1) {
        printf("Invalid input. Please enter a valid integer.\n");
        return 1;
    }

    printf("Enter the second integer: ");
    if (scanf("%lld", &num2) != 1) {
        printf("Invalid input. Please enter a valid integer.\n");
        return 1;
    }

    printf("Enter the operation (+ or -): ");
    scanf(" %c", &operation);

    unsigned long long bcd1 = decimalToBCD(num1);
    unsigned long long bcd2 = decimalToBCD(num2);

    switch (operation) {
        case '+':
            {
                unsigned long long bcdSum = addBCD(bcd1, bcd2);
                printf("Sum in BCD: ");
                printBCD(bcdSum);
                printf("\n");
                break;
            }
        case '-':
            {
                unsigned long long bcdDiff = subtractBCD(bcd1, bcd2);
                printf("Difference in BCD: ");
                printBCD(bcdDiff);
                printf("\n");
                break;
            }
        default:
            printf("Invalid operation. Please enter '+' or '-'.\n");
            break;
    }

    return 0;
}

