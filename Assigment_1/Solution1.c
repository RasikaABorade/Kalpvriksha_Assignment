#include <stdio.h>
#include <string.h>
#include <ctype.h>

int isValidExpression(char *input) {
    for (int i = 0; input[i] != '\0'; i++) {
        char ch = input[i];
        if (!((ch >= '0' && ch <= '9') || ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == ' ')) {
            return 0;
        }
    }
    return 1;
}

void removeNewline(char *input) {
    for (int i = 0; input[i] != '\0'; i++) {
        if (input[i] == '\n') {
            input[i] = '\0';
            break;
        }
    }
}

int evaluateExpression(char *input) {
    int numbers[100];
    char operators[100];
    int numCount = 0, opCount = 0;

    int i = 0;
    while (input[i] != '\0') {
        if (input[i] == ' ') {
            i++;
            continue;
        }

        if (isdigit(input[i])) {
            int num = 0;
            while (isdigit(input[i])) {
                num = num * 10 + (input[i] - '0');
                i++;
            }
            numbers[numCount++] = num;
        } else {
            operators[opCount++] = input[i];
            i++;
        }
    }

    for (i = 0; i < opCount; i++) {
        if (operators[i] == '*' || operators[i] == '/') {
            if (operators[i] == '*')
                numbers[i] = numbers[i] * numbers[i + 1];
            else {
                if (numbers[i + 1] == 0) {
                    printf("Error: Division by zero\n");
                    return 0;
                }
                numbers[i] = numbers[i] / numbers[i + 1];
            }

            for (int j = i + 1; j < numCount - 1; j++)
                numbers[j] = numbers[j + 1];
            for (int j = i; j < opCount - 1; j++)
                operators[j] = operators[j + 1];
            numCount--;
            opCount--;
            i--;
        }
    }

    int result = numbers[0];
    for (i = 0; i < opCount; i++) {
        if (operators[i] == '+')
            result += numbers[i + 1];
        else if (operators[i] == '-')
            result -= numbers[i + 1];
    }

    return result;
}

int main() {
    char input[100];

    printf("Enter the input: ");
    fgets(input, sizeof(input), stdin);
    removeNewline(input);

    if (!isValidExpression(input)) {
        printf("Error: Invalid expression\n");
        return 0;
    }

    int result = evaluateExpression(input);
    printf("Result is: %d\n", result);

    return 0;
}
