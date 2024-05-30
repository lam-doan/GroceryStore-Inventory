#include <exception>
#include <iomanip>                                                        // setprecision()
#include <iostream>                                                       // cout, fixed(), showpoint()

#include "GroceryStore.hpp"






int main()
{
  try
  {
    std::cout << std::fixed << std::setprecision( 2 ) << std::showpoint;
    GroceryStore my_store;
    auto customers_cart = my_store.makeShoppingCarts();
    auto today_sale = my_store.ringUpCustomers(customers_cart);
    my_store.inventory();
    my_store.reorderItems(today_sale);
  }

  catch( const std::exception & ex )
  {
    std::cout << "FAILED - Unhandled exception:\n"  << ex.what() << '\n';
  }

  return 0;
}
