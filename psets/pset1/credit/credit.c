#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // get user credit card number
    long long creditcard;
    do
    {
        creditcard = get_long("Number: ");
    }
    while (creditcard < 0);

    // luhn's algorithm
    // intializing vatiables requierd for the calculation
    long long temp_cc = creditcard;
    int evenDigit = 0, oddDigit = 0, total = 0;
    // loop to access two digits at a time of the credit card for computation
    while (temp_cc > 0)
    {
        // calculation on even index digits
        evenDigit = 2 * ((temp_cc % 100) / 10);
        if (evenDigit / 10 == 1)
        {
            total += 1;
            total += evenDigit % 10;
        }
        else
        {
            total += evenDigit;
        }

        // calculation on odd index digits
        oddDigit = (temp_cc % 10);
        total += oddDigit;

        // iteration
        temp_cc /= 100;
    }

    // check if the credit card is valid
    bool valid = false;
    if (total % 10 == 0)
    {
        valid = true;
    }
    else
    {
        // invalid card
        printf("INVALID\n");
    }

    // check type of credit card
    if (valid)
    {
        // storing the credit card no. in temp variable
        temp_cc = creditcard;

        // case for 16-digit cards
        if (temp_cc > 999999999999999)
        {
            temp_cc /= 100000000000000;
            switch (temp_cc)
            {
                case 51:
                case 52:
                case 53:
                case 54:
                case 55:
                    // 16-digit no. starting with the above cases
                    printf("MASTERCARD\n");
                    break;
                default:
                    if (temp_cc / 10 == 4)
                    {
                        // 16-digit no. starting with '4'
                        printf("VISA\n");
                    }
                    else
                    {
                        printf("INVALID\n");
                    }
                    break;
            }
        }

        // case for 15-digit cards
        else if (temp_cc > 99999999999999)
        {
            temp_cc /= 10000000000000;
            switch (temp_cc)
            {
                case 34:
                case 37:
                    // 15-digit no. starting with the above cases
                    printf("AMEX\n");
                    break;
                default:
                    printf("INVALID\n");
                    break;
            }
        }

        // case for 13-digit cards
        else if (temp_cc > 999999999999)
        {
            temp_cc /= 1000000000000;
            if (temp_cc == 4)
            {
                // 13-digit no. starting with '4'
                printf("VISA\n");
            }
        }

        // unaccounted credit nums
        else
        {
            printf("INVALID\n");
        }
    }
}
