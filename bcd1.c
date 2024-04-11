#include <stdio.h>
#include <stdlib.h>

unsigned long long decimalToBCD(int decimal) {
    unsigned long long bcd = 0;
    int shift = 0;

    while (decimal != 0) {
        int digit = decimal % 10;
        bcd |= (unsigned long long)digit << (shift * 4);
        decimal /= 10;
        shift++;
    }
    return bcd;
}

void displayBCD(unsigned long long bcd) {
    int started = 0;
    int digits = 0;
    unsigned long long temp = bcd;
    while (temp != 0) {
        temp >>= 4;
        digits++;
    }

    if (digits == 0) {
        printf("0000");
    } else {
        for (int i = (digits * 4) - 4; i >= 0; i -= 4) {
            int digit = (bcd >> i) & 0xF; 
            printf("%c%c%c%c",
                   (digit & 0x8) ? '1' : '0',
                   (digit & 0x4) ? '1' : '0',
                   (digit & 0x2) ? '1' : '0',
                   (digit & 0x1) ? '1' : '0');
            if (i > 0) {
                printf(" "); 
            }
        }
    }
    printf("\n"); 
}



unsigned long long addBCD(unsigned long long bcd1, unsigned long long bcd2) {
    unsigned long long result = 0;
    int carry = 0;
    for (int i = 0; i < sizeof(unsigned long long) * 2; i++) {
        int digit1 = (bcd1 >> (i * 4)) & 0xF;
        int digit2 = (bcd2 >> (i * 4)) & 0xF;
        int sum = digit1 + digit2 + carry;
        if (sum >= 10) {
            carry = 1;
            sum -= 10;
        } else {
            carry = 0;
        }
        result |= (unsigned long long)(sum & 0xF) << (i * 4);
    }
    return result;
}

unsigned long long subtractBCD(unsigned long long bcd1, unsigned long long bcd2) {
    unsigned long long result = 0;
    int borrow = 0;
    for (int i = 0; i < sizeof(unsigned long long) * 2; i++) {
        int digit1 = (bcd1 >> (i * 4)) & 0xF;
        int digit2 = (bcd2 >> (i * 4)) & 0xF;
        int diff = digit1 - digit2 - borrow;
        if (diff < 0) {
            borrow = 1;
            diff += 10;
        } else {
            borrow = 0;
        }
        result |= (unsigned long long)(diff & 0xF) << (i * 4);
    }
    return result;
}

int main() {
    int number1, number2;
    char operation;

    int result1 = 0;
    printf("Enter two decimal numbers: ");
    scanf("%d %d", &number1, &number2);

    printf("Choose operation (+/-): ");
    scanf(" %c", &operation);

    long long intermediateResult;  
    unsigned long long result;

    if (operation == '+') {
        result1=number1+number2;
        intermediateResult = (long long)number1 + number2;
    } else if (operation == '-') {
        result1=number1-number2;
        intermediateResult = (long long)number1 - number2;
    } else {
        printf("Invalid operation.\n");
        return -1;
    }

    if (intermediateResult < 0) {
        intermediateResult = -intermediateResult;
    }

    result = decimalToBCD(intermediateResult);

    printf("Result in BCD format: ");

    if (result1 < 0){
        printf("1001 ");
    }
    displayBCD(result);

    printf("\n");

    return 0;
}
