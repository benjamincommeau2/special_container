# special_container
This project is an attempt to design a special container that allows fast sparse matrix multiplication and fast sparse string operator multiplication.

I originally built a special Map.hpp that implements a multi-indexing of list with a map to allow for both linear unordered iteration and individual log(n) loop ups, however I recently discovered that boost provides the multi_index_container class which does this task.

I used boost::multi_index to link a random_access_container with an ordered_container, and I discovered that random_access iterator is slower than vector and deque and gets slower when you add multiple container links. 

Now I am linking several ordered boost::multi_index containers with a seprate std::deque to improve linear iteration performance.
