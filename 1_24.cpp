#include <iostream>
#include <fstream>
#include <stdarg.h>

#define TEMPLATE_VT template< typename value_type >

TEMPLATE_VT
class List
{
private:
    // Simple struct of node for list
    struct Node
    {
        value_type data;
        Node *next;
        Node *prev;
    };

    Node *head;     // Pointer to first element of our list
    Node *tail;     // Pointer to last element of our list
    int count;      // Amount of elements

    // Fill in the list with an unknown number of variables using va_list
    void va_push_back( int, ... );

public:
    // Constructor / destructor
    List();
    List( const List& );
    template< typename ... Arg >
    List( Arg ... );
    ~List();
    
    // Element access
    value_type back();
    value_type front();

    // Modifiers
    void push_back( value_type );
    void push_front( value_type );
    void pop_back();
    void pop_front();

    // Capacity
    int size();
    bool empty();

    // [ TEMP ] I hope this is not forever POG
    void print()
    {
        /**
         * We iterate over the entire list starting from the first
         * element until it ends. At each iteration, we output the value
        */
        Node *temp = head;
        while( temp != nullptr )
        {
            std::cout << temp->data << std::endl;
            temp = temp->next; // Moving on to the next element
        }
    }

};

/**
 * @brief push_back but with va_list
 * @param int count Number of element to receive
 * 
 * Fill in the list with an unknown number of variables using va_list
*/
TEMPLATE_VT
void List< value_type >::va_push_back( int _count, ... )
{
    value_type data;
    va_list argptr;

    va_start( argptr, _count );

    // Until we get all the elements
    for( ; _count; _count-- )
    {
        data = va_arg( argptr, value_type );
        push_back( data );
    }

    va_end( argptr );
}

TEMPLATE_VT
List< value_type >::List()
{
    head = tail = nullptr;
    count = 0;
}

TEMPLATE_VT
List< value_type >::List( const List &L)
{
    head = tail = nullptr;
    count = 0;

    Node *temp = L.head;
    
    while( temp != nullptr ) // Until we go through the whole list
    {
        push_back( temp->data );
        temp = temp->next;
    }
}

TEMPLATE_VT
template< typename ... Arg >
List< value_type >::List( Arg ... args )
{
    head = tail = nullptr;
    count = 0;

    va_push_back( sizeof...( args ), args... );
}

TEMPLATE_VT
List< value_type >::~List()
{
    // nothing here rn...
}

/**
 * Returns a read/write reference to the data at the last
 * element of the list.
*/
TEMPLATE_VT
value_type List< value_type >::back()
{
    value_type temp = 0;

    if( tail != nullptr ) 
        temp = tail->data;

    return temp;
}

/**
 * Returns a read/write reference to the data at the first
 * element of the list.
*/
TEMPLATE_VT
value_type List< value_type >::front()
{
    value_type temp = 0;

    if( head != nullptr ) 
        temp = head->data;

    return temp;
}

/**
 * @brief Adding elements to the end of the list
 * @param _data The value we pass to the list
 *  
 * The function creates an element at the end
 * of the list and assigns the given data to it.
*/
TEMPLATE_VT
void List< value_type >::push_back( value_type _data )
{
    Node *curr = new Node;

    curr->next = nullptr;
    curr->prev = tail;
    curr->data = _data;

    // If we don't have any elements then it's gonna be first
    if( count == 0 )
        head = tail = curr;
    else 
    {
        tail = curr;
        curr->prev->next = curr;
    }

    // We have added 1 more item so need to increase 'count'
    count++;
}

/**
 * @brief Adding elements to the top of the list.
 * @param _value The value we pass to the list.
 * 
 * The function creates an element at the top
 * of the list and assigns the given data to it.
*/
TEMPLATE_VT
void List< value_type >::push_front( value_type _data )
{
    Node *curr = new Node;

    curr->next = head;
    curr->prev = nullptr;
    curr->data = _data;

    // If we don't have any elements then it's gonna be first
    if( count == 0 )
        head = tail = curr;
    else
    {
        head = curr;
        curr->next->prev = curr;
    }

    // We have added 1 more item so need to increase 'count'
    count++;
}

/**
 * @brief Removes last element.
 * 
 * The function removes the list item from the end.
*/
TEMPLATE_VT
void List< value_type >::pop_back()
{
    // We don't have any elements
    if( count <= 0 )
        return;

    // We have 1 element, so after deletion we will have 0
    // i.e. first/last element = null
    if( count == 1 )
    {
        head = tail = nullptr;

        // We don't have the previous one, so let's go
        // straight to decreasing the counter otherwise we got error
        goto decrease;
    }

    tail = tail->prev;
    tail->next = nullptr;

    decrease:
    count--;
}

/**
 * @brief Removes first element.
 *  
 * The function removes the list item from the first.
 */
TEMPLATE_VT
void List< value_type >::pop_front()
{
    // We don't have any elements
    if( count <= 0 )
        return;

    // We have 1 element, so after deletion we will have 0
    // i.e. first/last element = null
    if( count == 1 )
    {
        head = tail = nullptr;

        // We don't have the next element, so let's go
        // straight to decreasing the counter otherwise we got error
        goto decrease;
    }

    head = head->next;
    head->prev = nullptr;

    decrease:
    count--;
}

/**
 * @brief return size of list
 * 
 * The function returns the size (count) of the list.
*/
TEMPLATE_VT
int List< value_type >::size()
{
    return count;
}

/**
 * @brief return true if list is empty and false is not
 * 
 * The function returns true/false. Depending on the number
 * of elements in the list. If count > 0 then the function will
 * print true, else will print false (count < 1).
*/
TEMPLATE_VT
bool List< value_type >::empty()
{
    return count < 1;
}


/**
 * We check all the latest elements ( from 1 to list.size ). If the element is equal to 0
 * then we delete it, counting the number of deleted zeros. If the element
 * is not 0 then we write it at the beginning and then delete it from the end. 
 * Then we add to the end the number of zeros that we removed.
*/
TEMPLATE_VT
void rewriting( List< value_type > &list )
{
    int counter = 0; // For counting of zeros
    int _size = list.size();
    for( int i = 0; i < _size; ++i )
    {
        auto temp = list.back();
        if( list.back() != 0 ) // If element is not 0
        {
            list.push_front( temp ); // Overwrite the element
            list.pop_back();
        }
        else
        {
            counter++; // We removed 1 more zero
            list.pop_back();
        }
    }

    // Add all zeros that we removed to the end
    while( counter-- ) list.push_back( 0 );
}

/**
 * @brief Narrow String to Number Conversions
 * @param str String to process
 * 
 * Function converting a string to a number. 
 * If a letter is found - translates the number up to this letter.
*/
double atof_( char *str )
{
    double integer = 0;
    double fraction = 0;
    double divisor = 1;

    int sign = 1;
    int fraction_flag = 0; // After or before '.'; 1 - after, 0 - before

    char c = *str; // Current char

    // We get the sign of the number
    if ( c == '-' )
    {
        sign = -1;
        str++;
    }
    else if ( c == '+' )
    {
        sign = 1;
        str++;
    }

    // Translating string in double
    while ( ( c = ( *str++ ) ) != '\0' )
    {
        if ( ( c >= '0' ) && ( c <= '9' ) )
        {
            if ( !fraction_flag )
                integer = integer * 10 + ( c -'0' );
            else
            {
                fraction = fraction * 10 + ( c -'0' );
                divisor *= 10;
            }
        }
        else if ( c == '.' )
            fraction_flag = 1;
        else
            break;
    }

    // Example for 6.9: val = 1 * ( 6 + ( 90 / 100 ) ) = 6 + 0.9 = 6.9
    double val = sign *  (integer + ( fraction / divisor ) );

    return val;
}

/**
 * @brief Open the file, get data, write to the list
 * @param _list Link to the list to fill
*/
TEMPLATE_VT
void file_processing( List< value_type > &_list )
{
    // Opening the file
    std::fstream file( "C:/Users/w0resT/Desktop/study/cpp_study/l3_1/file.txt" );

     // If the file did not open
    if( !file.is_open() )
    { 
        std::cerr << "Cannot to open file!" << std::endl; 
        return; 
    }

    // If the file is empty
    if ( file.peek() == EOF ) 
    { 
        std::cerr << "File is empty!" << std::endl; 
        return; 
    }

    int j = 0; // Variable for buf
    char buf[ 256 ]; // Buffer for all text
    char number_buf[ 256 ]; // Buffer for a number
    while ( file.get( buf[ j++ ] ) ); // We get the text

    float curr_number = 0.f;  
    int count_n = 0; // The number of digits that we added to the buffer
    int number_flag = 0; // If current char is number - flag = 1 else flag = 0

    for ( int i = 0; i < 256; ++i )
    {
        char c = buf[ i ]; // Current char

        // Check if it is a number
        if( !number_flag )
        {
            if( c >= '0' || c <= '9' )
                number_flag = 1;
            else
                continue; // This is not a number, we go to the next iteration
        }

        // This is the end of the number
        if( c == ',' )
        {
            number_flag = 0; // Reset flag to zero
                
            curr_number = atof_( number_buf );
            _list.push_back( curr_number ); // Adding new element ( number )

            // Clear the buffer with 0
            for( ; count_n != 0; --count_n ) 
                number_buf[ count_n ] = '0';

            continue;
        }

        // We don't need 'spaces'
        if (c == ' ')
			continue;

        // Write the current digit to the buffer
        number_buf[ count_n++ ] = c;
    }

    file.close();
}

int main( void )
{
    // Creating list
    List < double > list; // = { 0.1, 0., 1.1, 6.9, 4.2, 0., 0.3 };

    // Processing the file and fill in the list
    file_processing( list );

    std::cout << "= Before: " << std::endl;
    list.print();
    std::cout << "================" << std::endl;

    // Rewriting our list
    rewriting( list );

    std::cout << "= After: " << std::endl;
    list.print();

    return EXIT_SUCCESS;
}
