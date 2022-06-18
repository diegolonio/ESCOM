from fractions import Fraction

SUBINDEX = str.maketrans("0123456789", "₀₁₂₃₄₅₆₇₈₉")
GREEK = ['α', 'β', 'γ', 'δ', 'ε', 'ζ', 'η', 'θ', 'ι', 'κ', 'λ',\
        'μ', 'ν', 'ξ', 'ο', 'π', 'ρ', 'σ', 'τ', 'υ', 'φ', 'χ', 'ψ', 'ω']
    
def copy(matrix : list, initial_row : int =False, final_row : int =False, initial_column : int =False, final_column : int =False) -> list:
    """
    Makes a copy of a matrix.

    Args:
        matrix: The matrix to be copied.
        initial_row: The row from where the copy starts.
        final_row: The row to where the copy ends.
        initial_column: The column from where the copy starts.
        final_column: The column to where the copy ends.

    Returns:
        A copy of the matrix passed as parameter.
        If any of the arguments to indicate from where to start or
        to where finish the copy isn't set up, it starts from index 0;
        if they aren't consistent, the function returns False.
    """

    new_matrix = []

    # If initial_row is set up and is less or equal to 0 or greater than
    # the quantity of rows of the matrix
    if initial_row and (initial_row <= 0 or initial_row > len(matrix)):
        return False

    # If final_row is set up and is less or equal to 0 or greater than
    # the quantity of rows of the matrix
    if final_row and (final_row <= 0 or final_row > len(matrix)):
        return False
    
    # If initial_column is set up and is less or equal to 0 or greater than
    # the quantity of columns of the matrix
    if initial_column and (initial_column <= 0 or initial_column > len(matrix[0])):
        return False

    # If final_column is set up and is less or equal to 0 or greater than
    # the quantity of columns of the matrix
    if final_column and (final_column <= 0 or final_column > len(matrix[0])):
        return False

    # If initial_row and final_row are set up and initial_row is greater
    # than final_row
    if initial_row and final_row and initial_row > final_row:
        return False

    # If initial_column and final_column are set up and initial_column is greater
    # than final_column
    if initial_column and final_column and initial_column > final_column:
        return False

    # The copy itself
    for i in range(0 if not initial_row else initial_row-1, len(matrix) if not final_row else final_row):
        new_matrix.append([]) # New row

        for j in range(0 if not initial_column else initial_column-1, len(matrix[i]) if not final_column else final_column):
            new_matrix[i if not initial_row else i-initial_row+1].append(matrix[i][j])

    return new_matrix

def multiply(matrix : list, constant : int, i : int) -> list:
    """
    Multiply a row of the given matrix by a constant.

    Args:
        matrix: The matrix whose row will be multiplied.
        constant: The constant by which the row will be multiplied.
        i: The index of the row.

    Returns:
        A matrix with the specified row multiplied by the given
        constant.
    """

    new_matrix = copy(matrix)

    # If i is between the range of the quantity of rows of the matrix
    if i > 0 and i <= len(new_matrix):
        # Multiplying the specified row by the constant
        for j in range(len(new_matrix[i-1])):
            new_matrix[i-1][j] = new_matrix[i-1][j] * constant

    return new_matrix

def add(matrix : list, i_1 : int, i_2 : int, i_dest : int) -> list:
    """
    Add two rows of a matrix and put the result in a given row index.

    Args:
        matrix: The matrix whose rows will be added.
        i_1: The index of the first row.
        i_2: The index of the second row.
        i_dest: The index to where put the resultant row.

    Returns:
        A copy of the given matrix with the resultant row of the
        addition of two of its rows in the given index.
    """

    new_matrix = copy(matrix)

    # If i_1 and i_2 are between the range of the quantity of rows of the matrix
    if (i_1 > 0 and i_1 <= len(new_matrix)) and (i_2 > 0 and i_2 <= len(new_matrix)):
        # If the index where to put the resultant row is the index
        # of one of the row to be added
        if (i_dest == i_1) or (i_dest == i_2):
            # The addition itself
            for j in range(len(new_matrix[i_dest-1])):
                new_matrix[i_dest-1][j] = new_matrix[i_1-1][j] + new_matrix[i_2-1][j]

    return new_matrix

def swap(matrix : list, i_1 : int, i_2 : int) -> list:
    """
    Swap to rows of a matrix.

    Args:
        matrix: The matrix whose rows will be swaped.
        i_1: The index of the first row.
        i_2: The index of the second row.

    Returns:
        A copy of the given matrix with the specified rows swaped.
    """

    new_matrix = []

    # The swap itself
    for i in range(len(matrix)):
        if i == i_1-1:
            new_matrix.append(matrix[i_2-1])
        elif i == i_2-1:
            new_matrix.append(matrix[i_1-1])
        else:
            new_matrix.append(matrix[i])

    return new_matrix

def theres_a_one(matrix : list, j : int) -> int:
    """
    Obtain the row index of the specified column where there's a
    number 1 (positive or negative).

    Args:
        matrix: The matrix to analyse.
        j: The index of the column to search in.

    Returns:
        The index of the row where the number 1 is.
        If there isn't, the function returns False.
    """

    row_number = False

    for i in range(j-1, len(matrix)):
        if matrix[i][j-1] == 1 or matrix[i][j-1] == -1:
            row_number = i + 1
            break

    return row_number

def theres_elem_diff_zero(matrix : list, j : int) -> int:
    """
    Obtain the index of the row where the first element different
    of zero from top to bottom of the specified column is.

    Args:
        matrix: The matrix to analyse.
        j: The index of the column to search in.

    Return:
        The index of the row where the found element is.
        If all elements in the column are zero, the function
        returns False.
    """

    row_number = False

    for i in range(j-1, len(matrix)):
        if matrix[i][j-1] != 0:
            row_number = i + 1
            break

    return row_number

def row_echelon_form(matrix : list) -> list:
    """
    Turn a matrix in its Row Echelon Form.

    Args:
        matrix: The matrix to work with.

    Returns:
        A copy of the given matrix in its Row Echelon Form.
    """

    new_matrix = copy(matrix)
    aux_matrix = []

    # For each column in the matrix
    for j in range(len(new_matrix[0])):
        # Operate while j is less or equal to the quantity of rows
        if j < len(new_matrix):
            # If the pivot element is zero
            if new_matrix[j][j] == 0:
                # Search for an element different of zero in the current column
                row_without_zero = theres_elem_diff_zero(new_matrix, j+1)
                # Swap the row with index equal to j with the row of the found element 
                new_matrix = swap(matrix, j+1, row_without_zero)

            # If there's a number 1 (positive or negative) in the current column
            if theres_a_one(matrix, j+1):
                # Get the index of the row where the number 1 is
                row_with_a_one = theres_a_one(new_matrix, j+1)
                # Swap the row with index equal to j with the row of the found element
                new_matrix = swap(new_matrix, j+1, row_with_a_one)

            # If the pivot element is different of zero
            if new_matrix[j][j] != 0:
                # Multiply the row with index equal to j by the reciprocal of the pivot element
                new_matrix = multiply(new_matrix, Fraction(1, Fraction(new_matrix[j][j])), j+1)

            # If the pivot element is -1
            if new_matrix[j][j] == -1:
                # Multiply the row with index equal to j by -1
                new_matrix = multiply(new_matrix, -1, j+1)

            # For the elements under the current pivot element
            for i in range(j+1, len(new_matrix)):
                # If the current element is different of zero
                if (new_matrix[i][j] != 0):
                    # Multiply the row of the pivot element by the negative of the current element
                    aux_matrix = multiply(new_matrix, Fraction(new_matrix[i][j]) * -1, j+1)
                    # Add the multiplied row of the pivot with the current row and put the resultant
                    # row in the place of the current row 
                    aux_matrix = add(aux_matrix, j+1, i+1, i+1)
                    # Return the row of the pivot element to its initial state
                    new_matrix = multiply(aux_matrix, Fraction(1, Fraction(new_matrix[i][j])) * -1, j+1)

    return new_matrix

def reduced_row_echelon_form(matrix : list) -> list:
    """
    Turn a matrix in its Row Echelon Form to its Reduced-Row Echelon Form.

    Args:
        matrix: The matrix in its Row Echelon Form.

    Return:
        The Reduced Row Echelon Form of the given matrix.

    Notes:
        This function doesn't verify if the given matrix is in
        its Row Echelon Form.
    """

    new_matrix = copy(matrix)
    aux_matrix = []

    # From the last column to the first one
    for j in range(len(matrix[0])-1, -1, -1):
        # Operate while j is less or equal to the quantity of rows
        if j < len(new_matrix):
            # For the elements above the pivot element
            for i in range(j-1, -1, -1):
                # If the current element is different of zero
                if new_matrix[i][j] != 0:
                    # Multiply the row of the pivot element by the negative of the current element
                    aux_matrix = multiply(new_matrix, Fraction(new_matrix[i][j]) * -1, j+1)
                    # Add the multiplied row of the pivot with the current row and put the resultant
                    # row in the place of the current row 
                    aux_matrix = add(aux_matrix, j+1, i+1, i+1)
                    # Return the row of the pivot element to its initial state
                    new_matrix = multiply(aux_matrix, Fraction(1, Fraction(new_matrix[i][j])) * -1, j+1)

    return new_matrix

def gauss_jordan_elimination(matrix : list) -> list:
    """
    Perform the Gauss-Jordan Elimination on a matrix.

    Args:
        matrix: The matrix to work with.

    Returns:
        The Reduced-Row Echelon Form of the given matrix.
    """

    new_matrix = copy(matrix)
    new_matrix = row_echelon_form(matrix)
    new_matrix = reduced_row_echelon_form(new_matrix)

    return new_matrix

def solve_equation(equation : list, k : int) -> list:
    """
    Solve a factored linear equation for a certain variable.

    Args:
        equation: The coefficients of the equation.
        k: The index of the variable for which will be solved the equation.

    Returns:
        The coefficients of the solved equation.

    Notes:
        1. The coefficients of the given linear equation are assumed to be in the form:
                a₁ * x₁ + a₂ * x₂ + ... + aₙ * xₙ = b  ->  [a₁, a₂, ..., aₙ, b]
        2. The coefficients of the solved linear equation are returned in the form:
                xₖ = (-a₁/aₖ) * x₁ + ... + (-aₙ/aₖ) * xₙ + (b/aₖ)  ->  [1, (-a₁/aₖ), ..., (-aₙ/aₖ), (b/aₖ)]
    """

    solved_equation = []

    for j in range(len(equation)):
        # If the current coefficient isn't of the variable that the equation is being solved for
        if j != k-1:
            # Append the coefficient multiplied by -1 and divided by the coefficient of the variable
            # that the equation is being solved for
            solved_equation.append(Fraction(Fraction(equation[j]), Fraction(equation[k-1])) * -1)
        # If the current coefficient is of the variable that the equation is being solved for
        else:
            # Append the coefficient 1
            solved_equation.insert(0, Fraction(1))

    # Correct the sign of the independent term
    solved_equation[-1] *= -1

    return solved_equation

def is_zeros_row(row : list) -> bool:
    """
    Determine if all of the elements in a row are zero.

    Args:
        row: The row to analyse.

    Returns:
        If all of the elements in the given row are zero, 
        the function returns True; otherwise, it returns False.
    """

    is_zeros_row = True

    for j in range(len(row)):
        if row[j] != 0:
            is_zeros_row = False

    return is_zeros_row

def free_variables(solved_matrix : list) -> int:
    """
    Get the quantity of free variables of a solved system of equations.

    Args:
        solved_matrix: The matrix representation of the solved system of equations.

    Returns:
        The quantity of free variables of the given solved system of equations.
    """

    free_variables = 0

    # From the last row to the first one
    for i in range(len(solved_matrix)-1, -1, -1):
        # If the elements of the current row aren't all zero
        if not is_zeros_row(solved_matrix[i]):
            # From the column before the last one to the i-th column
            for j in range(len(solved_matrix[i])-2, i, -1):
                free_variables += 1
            break

    return free_variables

def formatted_equation(coefficients : list) -> str:
    """
    Construct the string of the general form of a linear equation from
    its coefficients.

    Args:
        coefficients: The coefficients of the linear equation.

    Returns:
        A formatted string containing the general form of the given
        linear equation.

    Example:
        Given the list of coefficients of a linear equation as follows:
                                [3, -4, 1, 13]
        The function returns the string:
                            '3x₁ - 4x₂ + x₃ = 13'

    Notes:
        The coefficients of the given linear equation are assumed to be in the form:
            a₁ * x₁ + a₂ * x₂ + ... + aₙ * xₙ = b  ->  [a₁, a₂, ..., aₙ, b]
    """

    equation = []

    # For each coefficient from left to right
    for j in range(len(coefficients)):
        # If the current coefficient is the first one or the last one
        if (j == 0) or (j == len(coefficients)-1):
            # Append it as it is
            equation.append(str(coefficients[j]))
        # If the current coefficient isn't neither the first one nor the last one
        else:
            if coefficients[j] >= 0:
                equation.append(' + ' + str(coefficients[j]))
            else:
                equation.append(' - ' + str(coefficients[j] * -1))

        # If the current coefficient isn't the independent term
        if j != len(coefficients)-1:
            equation.append('x' + str(j+1).translate(SUBINDEX))

        # If the current coefficient is the one before the independent term
        if j == len(coefficients)-2:
            equation.append(" = ")

    # For the coefficient of each variable
    for j in range(0, len(equation)-2, 2):
        # If the coefficient is 1 (positive or negative)
        if Fraction(equation[j].replace(' ', '')) == 1 or Fraction(equation[j].replace(' ', '')) == -1:
            # Strip the substring '1'
            equation[j] = equation[j].strip('1')

    # Turn the resultant list in a string
    equation = ''.join(equation)

    return equation

def formatted_solved_equation(coefficients : list, free_variables : int, variable_index : int) -> str:
    """
    Construct the string of the general form of a solved linear equation
    from its coefficients.

    Args:
        coefficients: The coefficients of the solved linear equation.
        free_variables: The number of free variables of the solved equation.
        variable_index: The index of the variable that the equation has been solved for.

    Returns:
        A formatted string containing the general form of the given
        solved linear equation.

    Example:
        Given the arguments of the function as follows:
                formatted_solved_equation([1, -4, 1, 13], 2, 3)
        The function returns the string:
                            'x₃ = -4α + β + 13'
    """

    numbers = copy([coefficients])
    numbers = numbers[0][len(coefficients)-free_variables-1:]

    equation = []
    equation.append('x' + str(variable_index).translate(SUBINDEX))
    equation.append(' = ')

    # For the coefficient of each free variable and the independent term
    for j in range(len(numbers)):
        # If the current coefficient is different of zero
        if numbers[j] != 0:
            # If the current coefficient is the first one
            if j == 0:
                # Append it as it is
                equation.append(str(numbers[j]))
            # If the current coefficient isn't the first one
            else:
                if numbers[j] < 0:
                    equation.append(' - ' + str(numbers[j] * -1))
                else:
                    equation.append(' + ' + str(numbers[j]))

            # If the current coefficient isn't the independent term
            if j != len(numbers)-1:
                # Append the i-th variable symbol
                equation.append(GREEK[j])
    
    # For the coefficient of each variable
    for j in range(2, len(equation)-1, 2):
        # If the coefficient is 1 (positive or negative)
        if Fraction(equation[j].replace(' ', '')) == 1 or Fraction(equation[j].replace(' ', '')) == -1:
            # Strip the substring '1'
            equation[j] = equation[j].strip('1')

    if len(equation) == 2:
        equation.append('0')

    # Turn the resultant list in a string
    equation = ''.join(equation)

    return equation

def parameterise(solved_matrix : list) -> list:
    """
    Parameterise the solution-equations of a solved system of linear equations.
    """

    parameterisation = []
    equation = []
    # Get the quantity of free variables of the system
    free_vars = free_variables(solved_matrix)
    # Calculate the quantity of solution-equations of the system
    quantity_equations = len(solved_matrix) + free_vars

    # For each index of the solution-equations
    for i in range(quantity_equations):
        if i < free_vars:
            # Append the solution-equation of the i-th free variable
            parameterisation.append('x' + str(i+1).translate(SUBINDEX) + ' = ' + GREEK[i])
        # If the current equation linearly dependent of the free variables isn't a zeros row
        elif not is_zeros_row(solved_matrix[i-free_vars]):
            # Solve the current equation for the (i-free_vars)-th variable from left to right
            equation = solve_equation(solved_matrix[i-free_vars], i-free_vars+1)
            # Get the formatted string of the current solved equation
            equation = formatted_solved_equation(equation, free_vars, i+1)
            # Append the string to list of parameterised equations
            parameterisation.append(equation)

    return parameterisation

def show(matrix : list):
    """
    Print the elements of a matrix.

    Args:
        matrix: The matrix to print.

    Return:
        None
    """

    for i in range(len(matrix)):
        for j in range(len(matrix[i])):
            print(matrix[i][j], end=' ')

        if i != len(matrix)-1:
            print('\n')

if __name__ == '__main__':
    matrix = []

    rows = int(input("Enter the number of rows of the matrix: "))
    print("\nEnter the coefficients of each row separated by blanks")

    # Get the elements of the matrix row by row
    for i in range(rows):
        row = input("Row {}: ".format(i+1))
        matrix.append(list(map(int, row.split(' '))))

    print('')

    # Print the equations of the system in their general form
    for equation in matrix:
        print(formatted_equation(equation))

    # Get the Row Echelon Form of the input matrix
    solved_matrix = row_echelon_form(matrix)
    
    print('')

    # Determine if the input system has unique solution, infinitely many solutions or it hasn't solution
    # If the system has more variables than quantity of equations
    if len(matrix) < len(matrix[0])-1:
        print("The system has infinitely many solutions.")
    else:
        # From the last equation to the first one
        for i in range(len(matrix)-1, -1, -1):
            # If the coefficient of the variable before the independent term is different of zero
            if solved_matrix[i][len(matrix[i])-2] != 0:
                print("The system has unique solution.")
                break

            # If the coefficient of the variable before the independent term and the independent term are both zero
            if solved_matrix[i][len(matrix[i])-2] == 0 and solved_matrix[i][len(matrix[i])-1] == 0:
                # If the quantity of equations are more than the quantity of variables
                if i+1 > len(matrix[i])-1:
                    # Ignore the current equation
                    continue
                # If the quantity of equations are less or equal to the quantity of variables
                else:
                    print("The system has infinitely many solutions.")
                    break
            # If the coefficient of the variable before the independent term is zero and the independent term is different of zero
            if solved_matrix[i][len(matrix[i])-2] == 0 and solved_matrix[i][len(matrix[i])-1] != 0:
                print("The system hasn't solution.\n")
                exit()
    
    print('')

    # Solve the system
    solved_matrix = reduced_row_echelon_form(solved_matrix)
    # Parameterise the solution-equations of the system
    solved_matrix = parameterise(solved_matrix)

    # Print the parameterised solution-equations of the system
    for equation in solved_matrix:
        print(equation)
