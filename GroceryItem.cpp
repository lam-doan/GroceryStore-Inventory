#include <cmath>                                                      // abs(), pow()
#include <compare>                                                    // weak_ordering
#include <iomanip>                                                    // quoted(), ios::failbit
#include <iostream>                                                   // istream, ostream, ws()
#include <string>
#include <type_traits>                                                // is_floating_point_v, common_type_t
#include <utility>                                                    // move()

#include "GroceryItem.hpp"



/*******************************************************************************
**  Implementation of non-member private types, objects, and functions
*******************************************************************************/
namespace    // unnamed, anonymous namespace
{
  template< typename T,  typename U >   requires std::is_floating_point_v<std::common_type_t<T, U> >
  constexpr bool floating_point_is_equal( T const lhs,  U const rhs,  double const EPSILON = 1e-4 ) noexcept
  {
    return std::abs( lhs - rhs ) < EPSILON;
  }
}    // unnamed, anonymous namespace







/*******************************************************************************
**  Constructors, assignments, and destructor
*******************************************************************************/


GroceryItem::GroceryItem( std::string productName, std::string brandName, std::string upcCode, double price )
  _brandName{std::move(brandName)},
  _productName{std::move(productName)},
  _price{price}
{}




GroceryItem::GroceryItem( GroceryItem const & other )
: _upcCode{other._upcCode},
  _brandName{other._brandName},
  _productName{other._productName},
  _price{other._price}
{}                                                                    // Avoid setting values in constructor's body (when possible)




GroceryItem::GroceryItem( GroceryItem && other ) noexcept
: _upcCode{std::move(other._upcCode)},
  _brandName{std::move(other._brandName)},
  _productName{std::move(other._productName)},
  _price{other._price}
{}



GroceryItem & GroceryItem::operator=( GroceryItem const & rhs ) &
{
  if (this != &rhs) {
    _upcCode = rhs._upcCode;
    _brandName = rhs._brandName;
    _productName = rhs._productName;
    _price       = rhs._price;
  }
  return *this;
}



GroceryItem & GroceryItem::operator=( GroceryItem && rhs ) & noexcept
{
  if (this != &rhs) {
    _upcCode = std::move(rhs._upcCode);
    _brandName = std::move(rhs._brandName);
    _productName = std::move( rhs._productName);
    _price       = rhs._price;
  }
  return *this;
}



// Destructor
GroceryItem::~GroceryItem() noexcept = default;




/*******************************************************************************
**  Accessors
*******************************************************************************/


std::string const & GroceryItem::upcCode() const &
{
  return _upcCode;
}



std::string const & GroceryItem::brandName() const &
{
  return _brandName;
}




std::string const & GroceryItem::productName() const &
{
  return _productName;
}




double GroceryItem::price() const &
{
  return _price;
}




std::string GroceryItem::upcCode() &&
{
  return std::move(_upcCode);
}




std::string GroceryItem::brandName() &&
{
  return std::move(_brandName);
}



std::string GroceryItem::productName() &&
{
  return std::move(_productName);
}








/*******************************************************************************
**  Modifiers
*******************************************************************************/


GroceryItem & GroceryItem::upcCode( std::string newUpcCode ) &
{
  _upcCode = std::move(newUpcCode);
  return *this;
}


GroceryItem & GroceryItem::brandName(std::string newBrandName) &
{
  _brandName = std::move(newBrandName);
  return *this;
}


GroceryItem & GroceryItem::productName( std::string newProductName ) &
{
  _productName = std::move(newProductName);
  return *this;
}


GroceryItem & GroceryItem::price(double newPrice) &
{
  _price = newPrice;
  return *this;
}







/*******************************************************************************
**  Relational Operators
*******************************************************************************/

// operator<=>(...)
std::weak_ordering GroceryItem::operator<=>( const GroceryItem & rhs ) const noexcept
{
  if (auto cmp = _upcCode     <=> rhs._upcCode;      cmp != 0)   return cmp;
  if( auto cmp = _productName <=> rhs._productName;  cmp != 0)   return cmp;
  if (auto cmp = _brandName   <=> rhs._brandName;    cmp != 0)   return cmp;

  if (floating_point_is_equal(_price, rhs._price)) return std::weak_ordering::equivalent;
  if (_price < rhs._price)                         return std::weak_ordering::less;
  return std::weak_ordering::greater;
}




// operator==(...)
bool GroceryItem::operator==( const GroceryItem & rhs ) const noexcept
{
  return  floating_point_is_equal(_price, rhs._price)
          && _upcCode     == rhs._upcCode
          && _brandName   == rhs._brandName
          && _productName == rhs._productName;
}








/*******************************************************************************
**  Insertion and Extraction Operators
*******************************************************************************/

// operator>>(...)
std::istream & operator>>( std::istream & stream, GroceryItem & groceryItem )
{
  char delimiter = '\x{0000}';
  GroceryItem temp;
  if (stream >> std::ws >> std::quoted(temp._upcCode)     && stream >> std::ws >> delimiter && delimiter == ',' &&
      stream >> std::ws >> std::quoted(temp._brandName)   && stream >> std::ws >> delimiter && delimiter == ',' &&
      stream >> std::ws >> std::quoted(temp._productName) && stream >> std::ws >> delimiter && delimiter == ',' &&
      stream >> std::ws >> temp._price) {
    groceryItem = std::move( temp );
  }
  else stream.setstate(std::ios::failbit);
  return stream;
}




// operator<<(...)
std::ostream & operator<<( std::ostream & stream, const GroceryItem & groceryItem )
{
  stream << std::quoted( groceryItem.upcCode() )     << delimiter
         << std::quoted( groceryItem.brandName() )   << delimiter
         << std::quoted( groceryItem.productName() ) << delimiter
         << groceryItem.price();

  return stream;
}
