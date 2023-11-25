## testing of A*algo
import a_star_algo


# grid map
a= [[0,1,0,0],
    [0,0,1,1],
    [0,1,0,0],
    [0,0,0,0]]

goal=(3,3)

# passing data to algo
b = a_star_algo.shortest_path(a,goal)
