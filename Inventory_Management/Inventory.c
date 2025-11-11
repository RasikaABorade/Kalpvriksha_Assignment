#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    int ProductID;
    char ProductName[50];
    float Price;
    int Quantity;
} Product;

void addProduct(Product **inventory, int *totalProducts);
void viewProducts(Product *inventory, int totalProducts);
void updateQuantity(Product *inventory, int totalProducts);
void searchByID(Product *inventory, int totalProducts);
void searchByName(Product *inventory, int totalProducts);
void searchByPriceRange(Product *inventory, int totalProducts);
void deleteProduct(Product **inventory, int *totalProducts);

int main()
{
    int totalProducts;
    do
    {
        printf("Enter the initial Number of Products (1 to 100): ");
        scanf("%d", &totalProducts);

        if (totalProducts < 1 || totalProducts > 100)
        {
            printf("Invalid number! Please enter between 1 and 100.\n");
        }
    } while (totalProducts < 1 || totalProducts > 100);

    Product *inventory = (Product *)calloc(totalProducts, sizeof(Product));

    if (inventory == NULL)
    {
        printf("Failed Memory Allocation\n");
        return 1;
    }

    for (int i = 0; i < totalProducts; i++)
    {
        printf("\nEnter details for product %d:\n", i + 1);

        printf("Enter the Product ID: ");
        scanf("%d", &inventory[i].ProductID);

        printf("Enter Product name: ");
        scanf(" %[^\n]", inventory[i].ProductName);

        printf("Enter Product price: ");
        scanf("%f", &inventory[i].Price);

        printf("Enter the product quantity: ");
        scanf("%d", &inventory[i].Quantity);
    }

    while (getchar() != '\n')
        ;

    int select;

    do
    {
        printf("\n========= INVENTORY MENU =========\n");
        printf("1. Add New Product\n");
        printf("2. View All Products\n");
        printf("3. Update Quantity\n");
        printf("4. Search Product by ID\n");
        printf("5. Search Product by Name\n");
        printf("6. Search Product by Price Range\n");
        printf("7. Delete Product\n");
        printf("8. Exit\n");

        printf("Select your choice: ");
        scanf("%d", &select);

        switch (select)
        {
        case 1:
            addProduct(&inventory, &totalProducts);
            break;

        case 2:
            viewProducts(inventory, totalProducts);
            break;

        case 3:
            updateQuantity(inventory, totalProducts);
            break;

        case 4:
            searchByID(inventory, totalProducts);
            break;

        case 5:
            searchByName(inventory, totalProducts);
            break;

        case 6:
            searchByPriceRange(inventory, totalProducts);
            break;

        case 7:
            deleteProduct(&inventory, &totalProducts);
            break;

        case 8:
            printf("Exiting the Program..\n");
            break;

        default:
            printf("Invalid choice, try again \n");
            break;
        }
    } while (select != 8);

    free(inventory);

    return 0;
}

void addProduct(Product **inventory, int *totalProducts)
{
    int newProductID;
    char newProductName[50];
    float newPrice;
    int newQuantity;

    printf("\nEnter the Product ID: ");
    scanf("%d", &newProductID);

    printf("Enter the Product name: ");
    scanf(" %[^\n]", newProductName);

    printf("Enter the Price of Product: ");
    scanf("%f", &newPrice);

    printf("Enter the Quantity of the Product: ");
    scanf("%d", &newQuantity);

    *inventory = (Product *)realloc(*inventory, (*totalProducts + 1) * sizeof(Product));

    if (*inventory == NULL)
    {
        printf("Failed memory reallocation\n");
        return;
    }

    (*inventory)[*totalProducts].ProductID = newProductID;
    strcpy((*inventory)[*totalProducts].ProductName, newProductName);
    (*inventory)[*totalProducts].Price = newPrice;
    (*inventory)[*totalProducts].Quantity = newQuantity;

    (*totalProducts)++;

    printf("\nProduct added successfully!\n");
}

void viewProducts(Product *inventory, int totalProducts)
{
    if (totalProducts == 0)
    {
        printf("There are no products in inventory\n");
        return;
    }
    printf("\n========= PRODUCT LIST =========\n");

    for (int i = 0; i < totalProducts; i++)
    {
        printf("Product ID: %d | Name: %s | Price: %.2f | Quantity: %d\n", inventory[i].ProductID, inventory[i].ProductName, inventory[i].Price, inventory[i].Quantity);
    }
}

void updateQuantity(Product *inventory, int totalProducts)
{
    int productID;
    int newQuantity;
    int ifFound = 0;

    printf("Enter the product ID to update the quantity: ");
    scanf("%d", &productID);

    for (int i = 0; i < totalProducts; i++)
    {
        if (inventory[i].ProductID == productID)
        {
            printf("Enter the new Quantity: ");
            scanf("%d", &newQuantity);

            inventory[i].Quantity = newQuantity;

            printf("Quantity updated successfully!\n");
            ifFound = 1;
            break;
        }
    }

    if (ifFound == 0)
    {
        printf("Product with ID %d not found \n", productID);
    }
}

void searchByID(Product *inventory, int totalProducts)
{
    int productID;
    int ifFound = 0;

    printf("Enter the Product ID to be searched: ");
    scanf("%d", &productID);

    for (int i = 0; i < totalProducts; i++)
    {
        if (inventory[i].ProductID == productID)
        {
            printf("Product Found: Product ID: %d | Name: %s | Price: %.2f | Quantity: %d \n", inventory[i].ProductID, inventory[i].ProductName, inventory[i].Price, inventory[i].Quantity);

            ifFound = 1;
            break;
        }
    }

    if (ifFound == 0)
    {
        printf("Product with ID %d not found\n", productID);
    }
}

void searchByName(Product *inventory, int totalProducts)
{
    char searchName[50];
    int ifFound = 0;

    printf("Enter the name to be searched: ");
    scanf(" %[^\n]", searchName);

    printf("Products Found:\n");

    for (int i = 0; i < totalProducts; i++)
    {
        if (strstr(inventory[i].ProductName, searchName) != NULL)
        {
            printf("Product ID:%d | Name:%s | Price: %.2f | Quantity: %d", inventory[i].ProductID, inventory[i].ProductName, inventory[i].Price, inventory[i].Quantity);

            ifFound = 1;
        }
    }

    if (ifFound == 0)
    {
        printf("No product with with name %s", searchName);
    }
}

void searchByPriceRange(Product *inventory, int totalProducts)
{
    float minPrice, maxPrice;
    int ifFound = 0;

    printf("Enter the minimum price: ");
    scanf("%f", &minPrice);

    printf("Enter the maximum price: ");
    scanf("%f", &maxPrice);

    printf("PRoducts in the price range:\n");

    for (int i = 0; i < totalProducts; i++)
    {
        if (inventory[i].Price >= minPrice && inventory[i].Price <= maxPrice)
        {
            printf("Product ID:%d | Name:%s | Price: %.2f | Quantity: %d", inventory[i].ProductID, inventory[i].ProductName, inventory[i].Price, inventory[i].Quantity);

            ifFound = 1;
        }
    }

    if (ifFound == 0)
    {
        printf("No prouducts are found in the given price range of %.2f - %.2f", minPrice, maxPrice);
    }
}

void deleteProduct(Product **inventory, int *totalProducts)
{
    int productID;
    int ifFound = -1;

    printf("Enter the Product ID to be deleted: ");
    scanf("%d", &productID);

    for (int i = 0; i < *totalProducts; i++)
    {
        if ((*inventory)[i].ProductID == productID)
        {
            ifFound = i;
            break;
        }
    }
    if (ifFound == -1)
    {
        printf("Product with ID %d not found\n", productID);
        return;
    }

    // to shift after deleting
    for (int i = ifFound; i < *totalProducts - 1; i++)
    {
        (*inventory)[i] = (*inventory)[i + 1];
    }
    (*totalProducts)--;

    if (*totalProducts > 0)
    {
        *inventory = (Product *)realloc(*inventory, (*totalProducts) * sizeof(Product));

        if (*inventory == NULL && *totalProducts > 0)
        {
            printf("Failed memory reallocation\n");
            return;
        }
    }
    else
    {
        free(*inventory);
        *inventory = NULL;
    }
    printf("The product deleted successfully \n");
}