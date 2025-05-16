#!/bin/bash

# Function to create the book database
create_database() {
    echo "Creating new book database..."
    rm -f books.dat     # Remove old database if exists
    touch books.dat     # Create a new one
    echo "Book database created successfully."
}

# Function to view all books
view_database() {
    echo "Viewing all books..."
    if [ -e books.dat ]; then
        echo -e "ID\tTitle\tAuthor\tYear"
        echo "----------------------------------"
        cat books.dat
    else
        echo "Database not found. Please create it first."
    fi
}

# Function to insert a new book
insert_book() {
    echo "Insert Book"
    if [ -e books.dat ]; then
        echo "Enter Book ID:"
        read id
        echo "Enter Book Title:"
        read title
        echo "Enter Author Name:"
        read author
        echo "Enter Publication Year:"
        read year
        echo -e "$id\t$title\t$author\t$year" >> books.dat
        echo "Book added successfully."
    else
        echo "Database not found. Please create it first."
    fi
}

# Function to modify a book record
modify_book() {
    echo "Modify Book"
    if [ -e books.dat ]; then
        echo "Enter Book ID to modify:"
        read id
        count=$(grep -c "^$id" books.dat)
        if [ "$count" -eq 0 ]; then
            echo "Book ID $id not found."
        else
            echo "Enter new Book ID:"
            read new_id
            echo "Enter new Title:"
            read new_title
            echo "Enter new Author:"
            read new_author
            echo "Enter new Year:"
            read new_year
            sed -i "s/^$id\t.*/$new_id\t$new_title\t$new_author\t$new_year/" books.dat
            echo "Book modified successfully."
        fi
    else
        echo "Database not found."
    fi
}

# Function to delete a book
delete_book() {
    echo "Delete Book"
    if [ -e books.dat ]; then
        echo "Enter Book ID to delete:"
        read id
        count=$(grep -c "^$id" books.dat)
        if [ "$count" -eq 0 ]; then
            echo "Book ID $id not found."
        else
            grep -v "^$id" books.dat > temp.dat
            mv temp.dat books.dat
            echo "Book deleted successfully."
        fi
    else
        echo "Database not found."
    fi
}

# Function to search for a book
search_book() {
    echo "Search Book"
    if [ -e books.dat ]; then
        echo "Enter Book ID to search:"
        read id
        result=$(grep "^$id" books.dat)
        if [ -z "$result" ]; then
            echo "Book ID $id not found."
        else
            echo "------------------------------"
            echo "Book Found:"
            echo "------------------------------"
            echo "ID     : $(echo "$result" | awk '{print $1}')"
            echo "Title  : $(echo "$result" | awk '{print $2}')"
            echo "Author : $(echo "$result" | awk '{print $3}')"
            echo "Year   : $(echo "$result" | awk '{print $4}')"
            echo "------------------------------"
        fi
    else
        echo "Database not found."
    fi
}

# Menu function
menu() {
    while true; do
        echo "=============================="
        echo "      BOOK SYSTEM MENU        "
        echo "=============================="
        echo "1. Create Database"
        echo "2. View All Books"
        echo "3. Insert Book"
        echo "4. Modify Book"
        echo "5. Delete Book"
        echo "6. Search Book"
        echo "7. Exit"
        echo "=============================="
        echo -n "Enter your choice: "
        read choice
        case $choice in
            1) create_database ;;
            2) view_database ;;
            3) insert_book ;;
            4) modify_book ;;
            5) delete_book ;;
            6) search_book ;;
            7) echo "Exiting..."; exit 0 ;;
            *) echo "Invalid choice. Try again." ;;
        esac
    done
}

# Start the menu
menu
