# SudokuSolver
College project in C for solving Sudoku grids

To compile the code, execute the Python code. The algorithm will check if your output matches the precomputed solution given by the professors. 

The solving algorithm is made out of 3 steps: 
1. First, if there is only one candidate in the square, it will set the candidate as the number 
2. Second, it will remove, from the list of candidates of the line, 3x3 square and column, the number if it is already present in the line, 3x3 square or column. 
3. Third, if there is only one number in the list of candidates of all the line, column or 3x3 square, it is the number of the square

As a bonus, I made a bruteforce algorithm using recursion. 
