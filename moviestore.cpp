#include "moviestore.h"


MovieStore::MovieStore( )
{
}



void
MovieStore::insert(const Movie & movie)
{
   primaryIndex.insert(movie.getID(), movie);
   SKey keyI(movie);
   secondaryIndex.insert(keyI, &(primaryIndex.find(movie.getID())->data));
   ternaryIndex.insert(keyI, &(primaryIndex.find(movie.getID())->data));
   
}



void
MovieStore::remove(const std::string & id)
{
    
    MSTP::Node* removingObject = primaryIndex.find(id);
    if (removingObject) {
        SKey myKey = removingObject->data;

        secondaryIndex.remove(myKey);
        ternaryIndex.remove(myKey);
        primaryIndex.remove(id);
    }
    
    
}



void
MovieStore::remove(const std::string & title,
                  const std::string & director)
{
    SKey myKey(title, director);
    MSTT::Node* removingObject = ternaryIndex.find(myKey);
    if (removingObject) {
        secondaryIndex.remove(myKey);
        ternaryIndex.remove(myKey);
        primaryIndex.remove(removingObject->data->getID());
    }
}



void
MovieStore::removeAllMoviesWithTitle(const std::string & title)
{
    SKey firstKey(title, "a");
    SKey secondKey(title, "{");
    
    std::list<MSTT::Node* > deletingObjects = ternaryIndex.find(firstKey, secondKey);
    if (!deletingObjects.empty()) {
        auto iter = deletingObjects.begin();
        for (; iter != deletingObjects.end(); iter++) {
            std::string myId = (*iter)->data->getID(); // primaryden silmece.
            MSTP::Node* removingObject = primaryIndex.find(myId);
            if (removingObject) {
                SKey myKey = removingObject->data;
                secondaryIndex.remove(myKey);
                ternaryIndex.remove(myKey);
                primaryIndex.remove(myId);
            }
        }
    }


}



void
MovieStore::makeAvailable(const std::string & id)
{
    MSTP::Node* availableMovie = primaryIndex.find(id);

    if (availableMovie) {
        availableMovie->data.setAvailable();
    }
}



void
MovieStore::makeUnavailable(const std::string & title,
                           const std::string & director)
{
    SKey myKey(title, director);
    MSTS::Node* unavailableMovie = secondaryIndex.find(myKey);
    if (unavailableMovie) {
        std::string myID = unavailableMovie->data->getID();
        MSTP::Node* uMovie = primaryIndex.find(myID);
        if (uMovie) {
            uMovie->data.setUnavailable();
        }

    }
}



void
MovieStore::updateCompany(const std::string & director, 
                           const std::string & Company)
{
    SKey firstKey("a", director);
    SKey secondKey("z", director);

    std::list<MSTS::Node*> updatingCompanies = secondaryIndex.find(firstKey, secondKey);
    auto iter = updatingCompanies.begin();
    if (!updatingCompanies.empty()) {
        for (iter; iter != updatingCompanies.end(); iter++) {
            std::string myID = (*iter)->data->getID();
            ((primaryIndex.find(myID))->data).setCompany(Company);
        }
    }
}



void
MovieStore::printMoviesWithID(const std::string & id1,
                              const std::string & id2,
                              unsigned short since) const
{
    std::list<MSTP::Node* > myMovies = primaryIndex.find(id1, id2);
    auto iter = myMovies.begin();
    if (!myMovies.empty()) {
        for (iter; iter != myMovies.end(); iter++) {
            if ((*iter)->data.getYear() >= since) {
                std::cout << (*iter)->data << std::endl;
            }
        }
    }
}



void
MovieStore::printMoviesOfDirector(const std::string & director,
                                  const std::string & first,
                                  const std::string & last) const
{
    
    SKey firstKey(first, director);

    SKey secondKey(last, director);
    
    std::list<MSTS::Node* > movieRange = secondaryIndex.find(firstKey, secondKey);
    if (!movieRange.empty()) {
        auto iter = movieRange.begin();
        for (; iter != movieRange.end(); iter++) {
            std::cout << *((*iter)->data) << std::endl;
        }

    }
    
    
}



void
MovieStore::printPrimarySorted( ) const
{
  printPrimarySorted(primaryIndex.getRoot());
}


void
MovieStore::printPrimarySorted(MSTP::Node * t) const
{
  if (t == NULL)
  {
    return;
  }

  printPrimarySorted(t->left);
  std::cout << t->data << std::endl;
  printPrimarySorted(t->right);
}


void
MovieStore::printSecondarySorted( ) const
{
  printSecondarySorted(secondaryIndex.getRoot());
}


void
MovieStore::printSecondarySorted(MSTS::Node * t) const
{
  if (t == NULL)
  {
    return;
  }

  printSecondarySorted(t->left);
  std::cout << *(t->data) << std::endl;
  printSecondarySorted(t->right);
}


void
MovieStore::printTernarySorted( ) const
{
  printTernarySorted(ternaryIndex.getRoot());
}


void
MovieStore::printTernarySorted(MSTT::Node * t) const
{
  if (t == NULL)
  {
    return;
  }

  printTernarySorted(t->left);
  std::cout << *(t->data) << std::endl;
  printTernarySorted(t->right);
}
