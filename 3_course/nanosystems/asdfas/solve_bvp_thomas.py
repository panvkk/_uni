def solve_boundary_value_problem(number_of_intervals):
    """Solve the problem and return all values used by the Thomas algorithm."""
    if number_of_intervals < 2:
        raise ValueError("Число отрезков должно быть не меньше 2.")

    step = 1.0 / number_of_intervals
    left_boundary = 0.0
    right_boundary = 1.0
    inner_points = number_of_intervals - 1

    a = [None] * (number_of_intervals + 1)
    c = [None] * (number_of_intervals + 1)
    b = [None] * (number_of_intervals + 1)
    f = [None] * (number_of_intervals + 1)
    alpha = [0.0] * (number_of_intervals + 1)
    beta = [0.0] * (number_of_intervals + 1)
    numerical_solution = [0.0] * (number_of_intervals + 1)
    grid = [index * step for index in range(number_of_intervals + 1)]

    # a_i*y_{i-1} - c_i*y_i + b_i*y_{i+1} = f_i.
    for index in range(1, number_of_intervals):
        x_value = grid[index]
        a[index] = 1.0 - x_value * step / 2.0
        c[index] = 2.0 + step**2
        b[index] = 1.0 + x_value * step / 2.0
        f[index] = step**2 * (2.0 + x_value**2)

    # Прямой ход: y_i = alpha_i*y_{i+1} + beta_i.
    alpha[0] = 0.0
    beta[0] = left_boundary
    for index in range(1, inner_points + 1):
        denominator = c[index] - a[index] * alpha[index - 1]
        alpha[index] = b[index] / denominator
        beta[index] = (
            a[index] * beta[index - 1] - f[index]
        ) / denominator

    # Обратный ход с использованием правого граничного условия.
    numerical_solution[-1] = right_boundary
    for index in range(number_of_intervals - 1, 0, -1):
        numerical_solution[index] = (
            alpha[index] * numerical_solution[index + 1] + beta[index]
        )
    numerical_solution[0] = left_boundary

    return grid, a, c, b, f, alpha, beta, numerical_solution


def print_table(grid, a, c, b, f, alpha, beta, numerical_solution):
    """Print all coefficients and intermediate Thomas algorithm values."""
    print(
        " i      x_i       a_i       c_i       b_i       f_i"
        "     alpha_i   beta_i    y_i      |ошибка|"
    )
    print("-" * 96)
    for index, (x_value, numerical_value) in enumerate(
        zip(grid, numerical_solution)
    ):
        exact_value = x_value**2
        error = abs(numerical_value - exact_value)
        values = (a[index], c[index], b[index], f[index])
        coefficients = " ".join(
            f"{value:9.5f}" if value is not None else f"{'-':>9}"
            for value in values
        )
        print(
            f"{index:2d}  {x_value:8.5f}  {coefficients}  "
            f"{alpha[index]:9.5f}  {beta[index]:9.5f}  "
            f"{numerical_value:9.5f}  {error:10.2e}"
        )


def main():
    try:
        number_of_intervals = int(
            input("Введите число отрезков разбиения N (например, 10): ")
        )
        result = solve_boundary_value_problem(number_of_intervals)
    except ValueError as error:
        print(f"Ошибка: {error}")
        return

    print()
    print_table(*result)
    grid = result[0]
    numerical_solution = result[-1]
    print(f"\nМаксимальная абсолютная ошибка: "
            f"{max(abs(value - x**2) for x, value in zip(grid, numerical_solution)):.4e}")


if __name__ == "__main__":
    main()
