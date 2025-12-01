This C++ project implements a small product-management system for a computer store. It uses object-oriented programming, file handling, search algorithms, and sorting algorithms to manage and display product information.

Key Features:

Product Hierarchy

Base class Product with common attributes like ID, name, brand, price, and quantity.

Two derived classes:

Component – includes specs such as clock speed, cores, and memory.

Peripheral – includes connection type, DPI, and screen size.

Both override a method that calculates a warranty period based on product-specific attributes.

File Input

Loads product data from "products.txt".

Handles formatting issues (e.g., underscores replaced with spaces).

Boyer–Moore String Search

Implements an efficient substring search algorithm.

Allows the user to search for products by keyword in their name.

Sorting

Products are sorted using a hybrid Quicksort + Insertion Sort algorithm based on their total price (price × quantity).

Output

Displays:

Matching search results.

A sorted list of all products with total price and warranty information.

Memory Management

Dynamically allocated objects are properly cleaned up at the end.
