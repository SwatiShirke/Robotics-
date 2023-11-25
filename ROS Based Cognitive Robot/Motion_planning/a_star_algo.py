""" A* algoritm - motion plannig
author : Swati V Shirke
date : 12/6/2015
"""

##remove extra print statements while using !!!!!!!!!!!!!!!!!!!


import numpy

"""class cell(object):
attributes:
1.occupancy     =1 if cell is ocuupied
                =0 if cell is empty
2.g_value :	value/cost/distacne of cell/node  from  initial node. 
                gvalue = -1 : node is not reachable from initial node
                
3.parent_node:  just previous node to this node
4.adjacents :	list of adjacent nodes/cells eg [[0,1],[1,0]]are adjacent of [0,0]
                but it holds just positions not actual object of class cell. Thus from list-adjacents we can not get
                any infromation about adjacent cells such as g_value, parent_node
5.position  :   [row,column] position of node in grid_table
methods/function :
__init__ :      performs initialization
"""
class cell(object):
    def __init__(  self ,occupancy, adjacent, position , parent_node = None , g_value=None  ):
        self.occupancy = occupancy
        self.adjacent = adjacent
        self.position = position        
        self.parent_node = parent_node
        self.g_value = g_value




        
        
"""
function: create_node

parameters:
    grid_table      : matrix which indicates ocuupancy of gridmap
                      eg. if  grid_map [1][5] = 1 means that node is occupied
                      if  grid_map [1][5] = 0 means that node is free
    done_box        : 1 diamensional list of name of nodes which are already occupied
                        eg. [ [0,0] , [1,1] , [2,1] ]
                        meaning these nodes are occupied
    init_cell       : initial position of bot           
returns:
    node       : matrix of objects of class cell for the given grid map
    done_box   : mentioned
note   :         done_box does not hold actual object of class 'cell'. It just hold it's position in grid map eg . [0,0] or [1,1]etc
                 thus from done_box we can not get any infromation about any node such as its g_value, parent_node etc.
"""
def create_node( grid_table , done_box, init_cell ):
    rows = len(grid_table)
    cols = len(grid_table[0])
    adjacent_node = find_adjacent_node( rows , cols )
    node = [ [cell for i in range(cols)] for j in range(rows)]
    for i in range( rows ):
        for j in range( cols ):
            if ( (i,j)==init_cell ):
                node[i][j] = cell( grid_table[i][j], adjacent_node[i][j], (i,j), None, 0)    
            elif( grid_table[i][j]== 1 ):
                node[i][j] = cell( grid_table[i][j], adjacent_node[i][j], (i,j), None, -1)
                done_box.append((i,j))
            else :
                node[i][j] = cell( grid_table[i][j], adjacent_node[i][j], (i,j), None )
    return [node , done_box ]






"""
function: make_check_box
creates list of nodes which are to be tested 'if any of them is a goal node?'
parameters:
    check_box :  holds list of name of nodes which are to be tested 'if any of them is a goal node?'
                 eg. [[1,1] , [2,2]]
                 means node[1][1] is a node (object of class cell) which is to be tested 'if it is goal node?'
                  
    adj_list   : position of nodes to be added in check_box eg [0,1]. does not hold actual object of class cell
returns:
    check_box  : mentioned above



note  :  check_box does not hold actual object of class cell. It just hold it's position in grid map eg . [0,0] or [1,1]etc
         thus from check_box we can not get any infromation about any node such as its g_value, parent_node etc.
"""

def make_check_box( check_box , adj_list):
    #for i in range(len(cell_list)):
    check_box= check_box + adj_list
    return check_box





 
"""
function: get_current_node
finds cell/ node with minimum g_value
parameters:
    check_box :  holds list of name of nodes which are to be tested 'if any of them is a goal node?'
                 eg. [[1,1] , [2,2]]
                 means node[1][1] is a node (object of class cell) which is to be tested 'if it is goal node?'
    node      : matrix of objects of class cell for the given grid map              
    
returns:
    min_node  :  object of type cell with minimum g_value 
note  :  check_box does not hold actual object of class cell. It just hold is position in grid map eg . [0,0] or [1,1]etc
         thus from check_box we can not get any infromation about any node such as its g_value, parent_node etc.    
"""       
    
def get_current_node( check_box , node ):
    min_node = node[check_box[0][0]][check_box[0][1]]
    for i in range(len(check_box)-1):
        if( min_node.g_value <  node[check_box[i+1][0]] [check_box[i+1][1]].g_value):
            min_node = min_node
        else:
            min_node = node[check_box[i+1][0]][check_box[i+1][1]]
    return min_node





""" function : find_adjacent_node()
parameter :
rows : number of rows
columns : number of columns

returns :
adj_mat : returns matrix and  each cell of matrix contains list of adjacent cells of that cell
"""
def find_adjacent_node( rows, columns ):
    adj_mat = [[0 for j in range(columns)] for i in range(rows)]
    Row_max = rows - 1
    Col_max = columns - 1
    for i in range( rows ):
        for j in range( columns ):
            if ( i!=0 and i!=Row_max  and j!=0 and j!= Col_max ):
                adj_mat[i][j] = [ (i+1,j), (i-1,j), (i,j+1), (i,j-1) ]
            elif ( i==0 and j!= 0 and j!= Col_max ):
                adj_mat[i][j]= [ (i,j-1), (i,j+1), (i+1,j) ]
            elif ( i==Row_max and j!=0 and j!= Col_max ):
                adj_mat[i][j] = [ (i,j+1), (i,j-1), (i-1,j)]
            elif ( j==0 and i!=0 and i!=Row_max ):
                adj_mat[i][j] = [ (i+1,j), (i-1,j), (i,j+1) ]
            elif ( j==Col_max and i!=0 and i!= Row_max ):
                adj_mat[i][j] = [ (i-1,j), (i+1,j), (i,j-1) ]
            elif ( i==0 and j==0 ):
                 adj_mat[i][j] = [ (i,j+1), (i+1,j) ]
            elif ( i==0 and j==Col_max ):
                 adj_mat[i][j] = [ (i,j-1), (i+1,j) ]
            elif ( i==Row_max and j==0 ):
                 adj_mat[i][j] = [ (i,j+1), (i-1,j)]
            elif ( i==Row_max and j==Col_max ):
                 adj_mat[i][j] = [ (i,j-1), (i-1,j)]
                                                               
    return adj_mat                               
                                




    

    
"""
Function Name: Shortest_path
Parameters :
    grid_table : matrix showing occupancy of grid_map
    goal       : goal node to be reached
    init_cell  : initial cell from where to start for searching path 
    
return:
None           :if goal is out of reach or if it is outside of the grid map
Or returns shortest path to goal and diastance to goal from init_cell

"""

def shortest_path( grid_table, goal ,init_cell=(0,0) ):
    print "Grid Table",grid_table
    print "Goal Node", goal 
    print "Initial Node",init_cell
    for i in range(len(grid_table)-1):
        if ( len(grid_table[i]) != len(grid_table[i+1]) ):
            print " Error : Grid table is not a matrix "
            return None
        
    if ( ( goal[0] > ( len(grid_table)-1) )  or  ( goal[1] > ( len(grid_table[0])-1) ) ):
        print " Error : goal is present outside the grid map"
        return None
    done_box = []
    check_box = [ init_cell ]
    [ node, done_box ] = create_node( grid_table , done_box, init_cell )
    current_node = get_current_node( check_box , node )
    
    while ( current_node.position != goal ):
        
        done_box.append(current_node.position)
        for i in range( len(current_node.adjacent) ):
            
            if( current_node.adjacent[i] not in done_box ):
                if ( current_node.adjacent[i]  not in check_box ):
                    check_box.append( current_node.adjacent[i] )
                    node[ current_node.adjacent[i][0]][current_node.adjacent[i][1] ].parent_node = current_node.position
                    node[ current_node.adjacent[i][0]][current_node.adjacent[i][1] ].g_value = current_node.g_value + 1
        
        check_box.remove( current_node.position )
        if ( len(check_box) == 0 ):
            print "Goal is not reachable"
            return None
        current_node = get_current_node( check_box , node )
        
        
    goal_node= current_node
    
    
        
    path=[]
    while( current_node.parent_node != None ):
        path.append( current_node.position )
        current_node = node[ current_node.parent_node[0] ] [ current_node.parent_node[1] ]
    path.append( current_node.position )
    path.reverse()
    
    print "Shortest Path",path
    return   path


 
