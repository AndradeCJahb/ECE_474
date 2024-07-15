/**
 * @file mathlib.h
 * @brief A basic math library
 * @details This file includes function prototypes for a basic math library.
 * It includes basic arithmetic operations for integers and complex numbers.
 *
 * The file includes the following modules:
 * - @subpage basicmath
 * - @subpage complexmath
 */

#ifndef MATHLIB_H
#define MATHLIB_H

/** 
 * @brief Struct that defines a complex number
 * 
 * @ingroup complexmath
 */
typedef struct {
    double real; /**< The real part of the complex number */
    double imag; /**< The imaginary part of the complex number */
} complex_t;

/** 
 * @brief Enum that defines possible operations for the math library 
 * 
 * @ingroup basicmath
 * @ingroup complexmath
 */
typedef enum {
    ADD, /**< Addition operation */
    SUB, /**< Subtraction operation */
    MUL, /**< Multiplication operation */
    DIV  /**< Division operation */
} operation_t;

/**
 * @addtogroup basicmath Basic Math Operations
 * @{
 */

/**
 * @brief Perform the specified operation on two integers
 * 
 * @param a The first integer
 * @param b The second integer
 * @param op The operation to perform
 * @return The result of the operation
 * @see perform_complex_operation
 * @note Division by zero will return 0.
 */
int perform_operation(int a, int b, operation_t op);

/** @} */ // end of basicmath

/**
 * @addtogroup complexmath Complex Math Operations
 * @{
 */

/**
 * @brief Perform the specified operation on two complex numbers
 * 
 * @param a The first complex number
 * @param b The second complex number
 * @param op The operation to perform
 * @return The result of the operation
 * @see perform_operation
 * @details This function performs an operation specified by the operation_t enum on two complex numbers.
 *          If the operation is not recognized, it returns a complex number with NaN for both real and imaginary parts.
 *
 * Here is a usage example: 
 * @code{.c}
 * complex_t a = {1, 2};
 * complex_t b = {3, 4};
 * complex_t result = perform_complex_operation(a, b, ADD);
 * @endcode
 */
complex_t perform_complex_operation(complex_t a, complex_t b, operation_t op);

/** @} */ // end of complexmath

#endif // MATHLIB_H
