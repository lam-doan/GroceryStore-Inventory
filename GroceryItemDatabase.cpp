#include <cstddef>
#include <filesystem>
#include <fstream>
#include <ios>
#include <string>

#include "GroceryItem.hpp"
#include "GroceryItemDatabase.hpp"


// Return a reference to the one and only instance of the database
GroceryItemDatabase & GroceryItemDatabase::instance()
{
  // Want to probe for persistent database file only the first time called.  By making a (Lambda) function that returns the results
  // of the probe and then calling that when fist construction the instance ensure all this probing stuff happens only the first
  // time instance() is called.
  auto getFileName = []()
  {
    std::string filename;

    // Look for a prioritized list of database files in the current working directory to use
    // Don't forget to #include <filesystem> to get visibility to the exists() function
    if     ( filename = "Grocery_UPC_Database-Full.dat"  ;   std::filesystem::exists( filename ) ) /* intentionally empty*/ ;
    else if( filename = "Grocery_UPC_Database-Large.dat" ;   std::filesystem::exists( filename ) ) /* intentionally empty*/ ;
    else if( filename = "Grocery_UPC_Database-Medium.dat";   std::filesystem::exists( filename ) ) /* intentionally empty*/ ;
    else if( filename = "Grocery_UPC_Database-Small.dat" ;   std::filesystem::exists( filename ) ) /* intentionally empty*/ ;
    else if( filename = "Sample_GroceryItem_Database.dat";   std::filesystem::exists( filename ) ) /* intentionally empty*/ ;
    else     filename.clear();

    return filename;
  };

  static GroceryItemDatabase theInstance( getFileName() );
  return theInstance;
}




// Construction
GroceryItemDatabase::GroceryItemDatabase( const std::string & filename )
{
  std::ifstream fin( filename, std::ios::binary );
  if( !fin.is_open() ) std::cerr << "Warning:  Could not open persistent grocery item database file \"" << filename << "\".  Proceeding with empty database\n\n";
  GroceryItem item;
  while (fin >> item) {
    _data.insert({item.upcCode(), item});
  }
}



GroceryItem * GroceryItemDatabase::find( const std::string & upc )
{
  // std::map<std::string, GroceryItem>::iterator it = _data.begin();
  auto item = _data.find( upc );
  if (item != _data.end()) return &item->second;
  return nullptr;
}

std::size_t GroceryItemDatabase::size() const
{
  return _data.size();
}
