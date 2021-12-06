## hahahahahaha
So a bit of a story with this problem.
It took far longer than it should have.

Originally I was wrestling with `std::set` cause I was aware that it was a structure where in only one of a value would be present at a given time.
This was to have a concrete list of all the points that had been found so that later on I could check how many occurrences there were by counting them in the entire vector of points.

As it turns out, using `std::count` is very slow (who would have thought).

For one it just wouldn't work properly because it needed a proper `operator <` implementation so that it could have an ordered set and so some points would just be ignored for some reason due to my poor implementation.

As I slammed my head on the desk wondering how to solve this problem I remembered vaguely seeing the term `map` thrown around. I was still wrestling with the `std::set` and counting solution when I started experimenting with `std::map` so I ended up just sort of treating maps like sets. However, my solution suddenly hit me while I was sitting around waiting for the code to run ~~(really who knew that counting took so long)~~.

I have a background of being self-taught in Lua. And as it turns out, perhaps the closest thing to a `table` in Lua a `map`! Working with arrays and vectors so far in C++ has made me fear accidentally indexing outside of their ranges due to the memory issues that could occur. However, out of curiosity I just decided to check in a tiny test program, "What happens if I try to do `map[index] += 1`?" It ended up doing exactly what I hoped. Praise be to maps.

> Upon further reading I discovered that [associative arrays](https://en.wikipedia.org/wiki/Associative_array) (which [tables](https://www.lua.org/pil/2.5.html) in Lua implement) are synonymous with maps, who woulda thought.

 This realization additionally helped me realize that there was no need for some proper `operator <` implementation for points, I could just lazily compare their values as strings and be done with it. This idea arose because all keys in Lua tables are hashed in some way (which is done automatically). I figured that the `operator <` could be approached in a similar way, as I imagined that there was some background work being done within the allocator types that are sorted which require comparisons that could be replaced with string/hash comparisons.

Thus, with the power of `std::map` I simply counted as I found points and  checked which points in the map had a value of at least 2, leading to a cumulative program run time of approximately 8 minutes on my computer to about 7 seconds, give or take 3 seconds.

Praise be to associative arrays.